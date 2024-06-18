#pragma once
#include "kyber/internals/ml_kem.hpp"

namespace kyber768_kem {

// ML-KEM Key Encapsulation Mechanism instantiated with ML-KEM-768 parameters
// See row 2 of table 2 of ML-KEM specification @ https://doi.org/10.6028/NIST.FIPS.203.ipd

static constexpr size_t k = 3;
static constexpr size_t η1 = 2;
static constexpr size_t η2 = 2;
static constexpr size_t du = 10;
static constexpr size_t dv = 4;

// 32 -bytes seed `d`, used in underlying K-PKE key generation
static constexpr size_t SEED_D_BYTE_LEN = 32;

// 32 -bytes seed `z`, used in ML-KEM key generation
static constexpr size_t SEED_Z_BYTE_LEN = 32;

// 1184 -bytes ML-KEM-768 public key
static constexpr size_t PKEY_BYTE_LEN = kyber_utils::get_kem_public_key_len(k);

// 2400 -bytes ML-KEM-768 secret key
static constexpr size_t SKEY_BYTE_LEN = kyber_utils::get_kem_secret_key_len(k);

// 32 -bytes seed `m`, used in ML-KEM encapsulation
static constexpr size_t SEED_M_BYTE_LEN = 32;

// 1088 -bytes ML-KEM-768 cipher text
static constexpr size_t CIPHER_TEXT_BYTE_LEN = kyber_utils::get_kem_cipher_text_len(k, du, dv);

// 32 -bytes ML-KEM-768 shared secret
static constexpr size_t SHARED_SECRET_BYTE_LEN = 32;

// Computes a new ML-KEM-768 keypair, given seed `d` and `z`.
inline void
keygen(std::span<const uint8_t, SEED_D_BYTE_LEN> d,
       std::span<const uint8_t, SEED_Z_BYTE_LEN> z,
       std::span<uint8_t, PKEY_BYTE_LEN> pubkey,
       std::span<uint8_t, SKEY_BYTE_LEN> seckey)
{
  ml_kem::keygen<k, η1>(d, z, pubkey, seckey);
}

// Given seed `m` and a ML-KEM-768 public key, this routine computes a ML-KEM-768 cipher text and a fixed size shared secret.
// If, input ML-KEM-768 public key is malformed, encapsulation will fail, returning false.
[[nodiscard("If public key is malformed, encapsulation fails")]] inline bool
encapsulate(std::span<const uint8_t, SEED_M_BYTE_LEN> m,
            std::span<const uint8_t, PKEY_BYTE_LEN> pubkey,
            std::span<uint8_t, CIPHER_TEXT_BYTE_LEN> cipher,
            std::span<uint8_t, SHARED_SECRET_BYTE_LEN> shared_secret)
{
  return ml_kem::encapsulate<k, η1, η2, du, dv>(m, pubkey, cipher, shared_secret);
}

// Given a ML-KEM-768 secret key and a cipher text, this routine computes a fixed size shared secret.
inline void
decapsulate(std::span<const uint8_t, SKEY_BYTE_LEN> seckey, std::span<const uint8_t, CIPHER_TEXT_BYTE_LEN> cipher, std::span<uint8_t, SHARED_SECRET_BYTE_LEN> shared_secret)
{
  ml_kem::decapsulate<k, η1, η2, du, dv>(seckey, cipher, shared_secret);
}

}
