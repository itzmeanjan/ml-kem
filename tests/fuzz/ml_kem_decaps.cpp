#include "ml_kem/ml_kem_1024.hpp"
#include "ml_kem/ml_kem_512.hpp"
#include "ml_kem/ml_kem_768.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  // We need at least one byte for the discriminator
  if (size < 1) {
    return 0;
  }

  const uint8_t level_idx = data[0] % 3;
  const uint8_t* payload = data + 1;
  const size_t payload_size = size - 1;

  if (level_idx == 0) {
    // ML-KEM-512
    constexpr size_t sk_len = ml_kem_512::SKEY_BYTE_LEN;
    constexpr size_t ct_len = ml_kem_512::CIPHER_TEXT_BYTE_LEN;
    constexpr size_t ss_len = ml_kem_512::SHARED_SECRET_BYTE_LEN;

    if (payload_size < sk_len + ct_len) {
      return 0;
    }

    std::array<uint8_t, sk_len> seckey;
    std::array<uint8_t, ct_len> cipher;
    std::array<uint8_t, ss_len> shared_secret;

    std::copy(payload, payload + sk_len, seckey.begin());
    std::copy(payload + sk_len, payload + sk_len + ct_len, cipher.begin());

    ml_kem_512::decapsulate(seckey, cipher, shared_secret);
  } else if (level_idx == 1) {
    // ML-KEM-768
    constexpr size_t sk_len = ml_kem_768::SKEY_BYTE_LEN;
    constexpr size_t ct_len = ml_kem_768::CIPHER_TEXT_BYTE_LEN;
    constexpr size_t ss_len = ml_kem_768::SHARED_SECRET_BYTE_LEN;

    if (payload_size < sk_len + ct_len) {
      return 0;
    }

    std::array<uint8_t, sk_len> seckey;
    std::array<uint8_t, ct_len> cipher;
    std::array<uint8_t, ss_len> shared_secret;

    std::copy(payload, payload + sk_len, seckey.begin());
    std::copy(payload + sk_len, payload + sk_len + ct_len, cipher.begin());

    ml_kem_768::decapsulate(seckey, cipher, shared_secret);
  } else if (level_idx == 2) {
    // ML-KEM-1024
    constexpr size_t sk_len = ml_kem_1024::SKEY_BYTE_LEN;
    constexpr size_t ct_len = ml_kem_1024::CIPHER_TEXT_BYTE_LEN;
    constexpr size_t ss_len = ml_kem_1024::SHARED_SECRET_BYTE_LEN;

    if (payload_size < sk_len + ct_len) {
      return 0;
    }

    std::array<uint8_t, sk_len> seckey;
    std::array<uint8_t, ct_len> cipher;
    std::array<uint8_t, ss_len> shared_secret;

    std::copy(payload, payload + sk_len, seckey.begin());
    std::copy(payload + sk_len, payload + sk_len + ct_len, cipher.begin());

    ml_kem_1024::decapsulate(seckey, cipher, shared_secret);
  }

  return 0;
}
