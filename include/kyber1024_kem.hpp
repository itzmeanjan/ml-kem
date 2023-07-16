#pragma once
#include "kem.hpp"
#include "utils.hpp"

// Kyber Key Encapsulation Mechanism (KEM) instantiated with Kyber1024
// parameters
namespace kyber1024_kem {

// See row 3 of table 1 of specification @
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf

constexpr size_t k = 4;
constexpr size_t η1 = 2;
constexpr size_t η2 = 2;
constexpr size_t du = 11;
constexpr size_t dv = 5;

// = 1568 -bytes Kyber1024 public key
constexpr size_t PKEY_LEN = kyber_utils::get_kem_public_key_len<k>();

// = 3168 -bytes Kyber1024 secret key
constexpr size_t SKEY_LEN = kyber_utils::get_kem_secret_key_len<k>();

// = 1568 -bytes Kyber1024 cipher text length
constexpr size_t CIPHER_LEN = kyber_utils::get_kem_cipher_len<k, du, dv>();

// Computes a new Kyber1024 KEM keypair s.t. public key is 1568 -bytes and
// secret key is 3168 -bytes, given 32 -bytes seed d ( used in CPA-PKE ) and 32
// -bytes seed z ( used in CCA-KEM ).
inline void
keygen(const uint8_t* const __restrict d,
       const uint8_t* const __restrict z,
       uint8_t* const __restrict pubkey,
       uint8_t* const __restrict seckey)
{
  kem::keygen<k, η1>(d, z, pubkey, seckey);
}

// Given 32 -bytes seed m ( which is used during encapsulation ) and a Kyber1024
// KEM public key ( of 1568 -bytes ), this routine computes a SHAKE256 XOF
// backed KDF (key derivation function) and 1568 -bytes of cipher text, which
// can only be decrypted by corresponding Kyber1024 KEM secret key, for arriving
// at same SHAKE256 XOF backed KDF.
//
// Returned KDF can be used for deriving shared key of arbitrary bytes length.
inline shake256::shake256
encapsulate(const uint8_t* const __restrict m,
            const uint8_t* const __restrict pubkey,
            uint8_t* const __restrict cipher)
{
  return kem::encapsulate<k, η1, η2, du, dv>(m, pubkey, cipher);
}

// Given a Kyber1024 KEM secret key ( of 3168 -bytes ) and a cipher text of 1568
// -bytes, which holds encrypted ( using corresponding Kyber1024 KEM public key
// ) 32 -bytes seed, this routine computes a SHAKE256 XOF backed KDF (key
// derivation function).
//
// Returned KDF can be used for deriving shared key of arbitrary bytes length.
inline shake256::shake256
decapsulate(const uint8_t* const __restrict seckey,
            const uint8_t* const __restrict cipher)
{
  return kem::decapsulate<k, η1, η2, du, dv>(seckey, cipher);
}

}
