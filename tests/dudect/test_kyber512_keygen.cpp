#include "kyber512_kem.hpp"
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
  constexpr size_t doff2 = doff1 + slen;
  constexpr size_t doff3 = doff2 + kyber512_kem::PKEY_LEN;

  auto _d = std::span<uint8_t, slen>(data + doff0, slen);
  auto _z = std::span<uint8_t, slen>(data + doff1, slen);
  auto _pkey = std::span<uint8_t, kyber512_kem::PKEY_LEN>(data + doff2, kyber512_kem::PKEY_LEN);
  auto _skey = std::span<uint8_t, kyber512_kem::SKEY_LEN>(data + doff3, kyber512_kem::SKEY_LEN);

  kem::keygen<kyber512_kem::k, kyber512_kem::η1>(_d, _z, _pkey, _skey);
  return 0;
}

void
prepare_inputs(dudect_config_t* const c, uint8_t* const input_data, uint8_t* const classes)
{
  randombytes(input_data, c->number_measurements * c->chunk_size);

  for (size_t i = 0; i < c->number_measurements; i++) {
    classes[i] = randombit();
    if (classes[i] == 0) {
      uint8_t byte = 0;
      randombytes(&byte, sizeof(byte));
      std::memset(input_data + i * c->chunk_size, byte, c->chunk_size);
    }
  }
}

dudect_state_t
test_kyber512_keygen()
{
  constexpr size_t chunk_size = 32 + // byte length of seed `d`
                                32 + // byte length of seed `z`
                                kyber512_kem::PKEY_LEN + kyber512_kem::SKEY_LEN;
  using namespace std::chrono_literals;
  const auto max_test_duration = 5min;

  dudect_config_t config = {
    .chunk_size = chunk_size,
    .number_measurements = 1ul << 14,
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
