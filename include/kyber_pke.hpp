#pragma once
#include "decryption.hpp"
#include "encryption.hpp"
#include "pke_keygen.hpp"
#include "utils.hpp"

// Kyber Public Key Encryption (PKE)
namespace kyber_pke {

// Kyber IND-CPA-secure PKE key generation algorithm, which takes two parameters
// `k` & `η1` and generates byte serialized public key and secret key of
// following length
//
// public key: (k * 12 * 32 + 32) -bytes wide
// secret key: (k * 12 * 32) -bytes wide
//
// Possible values of parameters like `k` & `η1`, can be found from table 1 of
// specification ( linked below ).
//
// Note, required randomness of 32 -bytes ( i.e. seed ) is sampled from system
// non-deterministic randomness ( if available, so do read
// https://en.cppreference.com/w/cpp/numeric/random/random_device ) source.
//
// See algorithm 4 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const size_t eta1>
inline static void
keygen(uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // k * 12 * 32 -bytes secret key
)
{
  uint8_t d[32]{};
  kyber_utils::random_data<uint8_t>(d, sizeof(d));

  cpapke::keygen<k, eta1>(d, pubkey, seckey);
}

// Given (k * 12 * 32 + 32) -bytes public key, 32 -bytes message ( to be
// encrypted ) and 32 -bytes random coin ( from where all randomness is
// deterministically sampled ), this routine encrypts message using
// IND-CPA-secure Kyber encryption algorithm, computing compressed cipher text
// of (k * du * 32 + dv * 32) -bytes.
//
// Other than that, parameters like `k`, `η1`, `η2`, `du` & `dv`, needs to be
// specified, whose possible values can be found in table 1 of specification (
// linked below ).
//
// See algorithm 5 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
inline static void
encrypt(const uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes
        const uint8_t* const __restrict msg,    // 32 -bytes message
        const uint8_t* const __restrict rcoin,  // 32 -bytes random coin
        uint8_t* const __restrict enc           // k * du * 32 + dv * 32 -bytes
)
{
  cpapke::encrypt<k, eta1, eta2, du, dv>(pubkey, msg, rcoin, enc);
}

// Given (k * 12 * 32) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( i.e. cipher ) text, this routine recovers 32 -bytes plain text
// which was encrypted using respective public key, which is associated with
// this secret key.
//
// Other than that, one needs to specify parameters like `k`, `du` & `dv`, whose
// possible values can be found from table 1 of specification ( linked below ).
//
// See algorithm 6 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const size_t du, const size_t dv>
inline static void
decrypt(
  const uint8_t* const __restrict seckey, // (k * 12 * 32) -bytes secret key
  const uint8_t* const __restrict enc,    // (k * du * 32 + dv * 32) -bytes
  uint8_t* const __restrict dec           // 32 -bytes plain text
)
{
  cpapke::decrypt<k, du, dv>(seckey, enc, dec);
}

}
