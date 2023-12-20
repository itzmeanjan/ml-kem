#include "field.hpp"
#include "kyber512_kem.hpp"
#include "sampling.hpp"
#include <array>
#include <chrono>
#include <cstring>

#define DUDECT_IMPLEMENTATION
#define DUDECT_VISIBLITY_STATIC
#include "dudect.h"

uint8_t
do_one_computation(uint8_t* const data)
{
  constexpr size_t slen = 32;

  constexpr size_t doff0 = 0;
  constexpr size_t doff1 = doff0 + slen;

  std::array<field::zq_t, kyber512_kem::k * ntt::N> poly_vec{};
  auto sigma = std::span<const uint8_t, slen>(data + doff0, doff1 - doff0);
  const auto nonce = data[doff1];

  kyber_utils::generate_vector<kyber512_kem::k, kyber512_kem::η1>(poly_vec, sigma, nonce);
  // Just so that optimizer doesn't remove above function call !
  return static_cast<uint8_t>(poly_vec[0].raw() ^ poly_vec[poly_vec.size() - 1].raw());
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
test_kyber512_generate_vector()
{
  using namespace std::chrono_literals;

  constexpr size_t chunk_size = 32 + // byte length of seed `sigma`
                                1    // single byte nonce
    ;

  constexpr size_t number_measurements = 1ul << 20;
  const auto max_test_duration = 2min;

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
