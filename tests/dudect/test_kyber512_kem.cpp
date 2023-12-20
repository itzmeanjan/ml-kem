#include "kyber512_kem.hpp"
#include <array>
#include <chrono>
#include <cstdlib>

#define DUDECT_IMPLEMENTATION
#define DUDECT_VISIBLITY_STATIC
#include "dudect.h"

constexpr size_t SEED_LEN = 32; // Byte length of seed(s)

uint8_t
do_one_computation(uint8_t* const data)
{
  constexpr size_t doff0 = 0;
  constexpr size_t doff1 = doff0 + SEED_LEN;
  constexpr size_t doff2 = doff1 + 1;
  constexpr size_t doff3 = doff2 + kyber512_kem::CIPHER_LEN;
  constexpr size_t doff4 = doff3 + kyber512_kem::CIPHER_LEN;
  constexpr size_t doff5 = doff4 + SEED_LEN;
  constexpr size_t doff6 = doff5 + SEED_LEN;

  std::array<field::zq_t, kyber512_kem::k * ntt::N> poly_vec{};
  std::array<uint8_t, kyber512_kem::k * 32 * kyber512_kem::du> byte_arr{};

  auto sigma = std::span<const uint8_t, SEED_LEN>(data + doff0, doff1 - doff0);
  const auto nonce = data[doff1];

  kyber_utils::generate_vector<kyber512_kem::k, kyber512_kem::η1>(poly_vec, sigma, nonce);
  kyber_utils::poly_vec_ntt<kyber512_kem::k>(poly_vec);
  kyber_utils::poly_vec_intt<kyber512_kem::k>(poly_vec);
  kyber_utils::poly_vec_compress<kyber512_kem::k, kyber512_kem::du>(poly_vec);
  kyber_utils::poly_vec_encode<kyber512_kem::k, kyber512_kem::du>(poly_vec, byte_arr);
  kyber_utils::poly_vec_decode<kyber512_kem::k, kyber512_kem::du>(byte_arr, poly_vec);
  kyber_utils::poly_vec_decompress<kyber512_kem::k, kyber512_kem::du>(poly_vec);

  std::array<uint8_t, SEED_LEN> sink{};
  auto _sink = std::span(sink);

  using ctxt_t = std::span<const uint8_t, kyber512_kem::CIPHER_LEN>;
  using seed_t = std::span<const uint8_t, SEED_LEN>;

  const uint32_t cond = kyber_utils::ct_memcmp(ctxt_t(data + doff2, doff3 - doff2), ctxt_t(data + doff3, doff4 - doff3));
  kyber_utils::ct_cond_memcpy(cond, _sink, seed_t(data + doff4, doff5 - doff4), seed_t(data + doff5, doff6 - doff5));

  // Just so that optimizer doesn't remove above function calls !
  return static_cast<uint8_t>(poly_vec[0].raw() ^ poly_vec[poly_vec.size() - 1].raw()) & // result of generating vector of polynomials
         (byte_arr[0] ^ byte_arr[byte_arr.size() - 1]) &                                 // result of serializing vector of polynomials
         (_sink[0] ^ _sink[_sink.size() - 1]) &                                          // result of conditional memcpy
         static_cast<uint8_t>(cond);                                                     // result of constant-time memcmp
}

void
prepare_inputs(dudect_config_t* const c, uint8_t* const input_data, uint8_t* const classes)
{
  randombytes(input_data, c->number_measurements * c->chunk_size);

  for (size_t i = 0; i < c->number_measurements; i++) {
    classes[i] = randombit();
    if (classes[i] == 0) {
      std::memset(input_data + i * c->chunk_size, 0x00, c->chunk_size);
    }
  }
}

dudect_state_t
test_kyber512_kem()
{
  using namespace std::chrono_literals;

  constexpr size_t chunk_size = SEED_LEN +                 // bytes holding seed `sigma`
                                1 +                        // single byte nonce
                                kyber512_kem::CIPHER_LEN + // bytes holding received cipher text
                                kyber512_kem::CIPHER_LEN + // bytes for locally computed cipher text
                                SEED_LEN +                 // bytes for first source buffer to copy from
                                SEED_LEN;                  // bytes for second source buffer to copy from

  constexpr size_t number_measurements = 1ul << 20;
  const auto max_test_duration = 5min;

  dudect_config_t config = {
    chunk_size,
    number_measurements,
  };
  dudect_ctx_t ctx;
  dudect_init(&ctx, &config);

  const auto begin_tp = std::chrono::steady_clock::now();
  dudect_state_t state = DUDECT_NO_LEAKAGE_EVIDENCE_YET;

  while (state == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    state = dudect_main(&ctx);

    const auto cur_tp = std::chrono::steady_clock::now();
    const auto time_spent = std::chrono::duration_cast<std::chrono::minutes>(cur_tp - begin_tp);
    if (time_spent >= max_test_duration) {
      break;
    }
  }

  dudect_free(&ctx);
  return state;
}

int
main()
{
  if (test_kyber512_kem() != DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
