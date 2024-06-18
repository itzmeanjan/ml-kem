#pragma once
#include "kyber/internals/ml_kem.hpp"

namespace kyber1024_kem {

// ML-KEM Key Encapsulation Mechanism instantiated with ML-KEM-1024 parameters
// See row 3 of table 2 of ML-KEM specification @ https://doi.org/10.6028/NIST.FIPS.203.ipd

static constexpr size_t k = 4;
static constexpr size_t η1 = 2;
static constexpr size_t η2 = 2;
static constexpr size_t du = 11;
static constexpr size_t dv = 5;

// 32 -bytes seed `d`, used in underlying K-PKE key generation
static constexpr size_t SEED_D_BYTE_LEN = 32;

// 32 -bytes seed `z`, used in ML-KEM key generation
static constexpr size_t SEED_Z_BYTE_LEN = 32;

// 1568 -bytes ML-KEM-1024 public key
static constexpr size_t PKEY_BYTE_LEN = kyber_utils::get_kem_public_key_len(k);

// 3168 -bytes ML-KEM-1024 secret key
static constexpr size_t SKEY_BYTE_LEN = kyber_utils::get_kem_secret_key_len(k);

// 32 -bytes seed `m`, used in ML-KEM encapsulation
static constexpr size_t SEED_M_BYTE_LEN = 32;

// 1568 -bytes ML-KEM-1024 cipher text
static constexpr size_t CIPHER_TEXT_BYTE_LEN = kyber_utils::get_kem_cipher_text_len(k, du, dv);

// 32 -bytes ML-KEM-1024 shared secret
static constexpr size_t SHARED_SECRET_BYTE_LEN = 32;

// Computes a new ML-KEM-1024 keypair, given seed `d` and `z`.
inline void
keygen(std::span<const uint8_t, SEED_D_BYTE_LEN> d,
       std::span<const uint8_t, SEED_Z_BYTE_LEN> z,
       std::span<uint8_t, PKEY_BYTE_LEN> pubkey,
       std::span<uint8_t, SKEY_BYTE_LEN> seckey)
{
  ml_kem::keygen<k, η1>(d, z, pubkey, seckey);
}

// Given seed `m` and a ML-KEM-1024 public key, this routine computes a ML-KEM-1024 cipher text and a fixed size shared secret.
// If, input ML-KEM-1024 public key is malformed, encapsulation will fail, returning false.
[[nodiscard("If public key is malformed, encapsulation fails")]] inline bool
encapsulate(std::span<const uint8_t, SEED_M_BYTE_LEN> m,
            std::span<const uint8_t, PKEY_BYTE_LEN> pubkey,
            std::span<uint8_t, CIPHER_TEXT_BYTE_LEN> cipher,
            std::span<uint8_t, SHARED_SECRET_BYTE_LEN> shared_secret)
{
  return ml_kem::encapsulate<k, η1, η2, du, dv>(m, pubkey, cipher, shared_secret);
}

// Given a ML-KEM-1024 secret key and a cipher text, this routine computes a fixed size shared secret.
inline void
decapsulate(std::span<const uint8_t, SKEY_BYTE_LEN> seckey, std::span<const uint8_t, CIPHER_TEXT_BYTE_LEN> cipher, std::span<uint8_t, SHARED_SECRET_BYTE_LEN> shared_secret)
{
  ml_kem::decapsulate<k, η1, η2, du, dv>(seckey, cipher, shared_secret);
}

}
