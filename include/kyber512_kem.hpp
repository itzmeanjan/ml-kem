#pragma once
#include "kyber_kem.hpp"
#include "utils.hpp"

// Kyber Key Encapsulation Mechanism (KEM) instantiated with Kyber512 parameters
//
// See table 1 of specification @
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
namespace kyber512_kem {

// Compile-time compute Kyber512 KEM public key byte length
constexpr size_t
pub_key_len()
{
  return kyber_utils::get_ccakem_public_key_len<2>();
}

// Compile-time compute Kyber512 KEM secret key byte length
constexpr size_t
sec_key_len()
{
  return kyber_utils::get_ccakem_secret_key_len<2>();
}

// Compile-time compute Kyber512 KEM cipher text byte length
constexpr size_t
cipher_text_len()
{
  return kyber_utils::get_ccakem_cipher_len<2, 10, 4>();
}

// Computes a new Kyber512 KEM keypair s.t. public key is 800 -bytes and secret
// key is 1632 -bytes, given a pseudo random number generator.
inline void
keygen(prng::prng_t& prng,
       uint8_t* const __restrict pubkey,
       uint8_t* const __restrict seckey)
{
  kyber_kem::keygen<2, 3>(prng, pubkey, seckey);
}

// Given a Kyber512 KEM public key ( of 800 -bytes ) and a pseudo random number
// generator, this routine computes a SHAKE256 XOF backed KDF (key derivation
// function) and 768 -bytes of cipher text, which can only be decrypted by
// corresponding Kyber512 KEM secret key, for arriving at same SHAKE256 XOF
// backed KDF.
//
// Returned KDF can be used for deriving shared key of arbitrary bytes length.
inline shake256::shake256<false>
encapsulate(prng::prng_t& prng,
            const uint8_t* const __restrict pubkey,
            uint8_t* const __restrict cipher)
{
  return kyber_kem::encapsulate<2, 3, 2, 10, 4>(prng, pubkey, cipher);
}

// Given a Kyber512 KEM secret key ( of 1632 -bytes ) and a cipher text of 768
// -bytes, which holds encrypted ( using corresponding Kyber512 KEM public key )
// 32 -bytes seed, this routine computes a SHAKE256 XOF backed KDF (key
// derivation function).
//
// Returned KDF can be used for deriving shared key of arbitrary bytes length.
inline shake256::shake256<false>
decapsulate(const uint8_t* const __restrict seckey,
            const uint8_t* const __restrict cipher)
{
  return kyber_kem::decapsulate<2, 3, 2, 10, 4>(seckey, cipher);
}

}
