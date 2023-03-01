#pragma once
#include "decapsulation.hpp"
#include "encapsulation.hpp"
#include "kem_keygen.hpp"
#include "prng.hpp"

// Kyber Key Encapsulation Mechanism (KEM)
namespace kyber_kem {

// Kyber IND-CCA2-secure KEM key generation algorithm, which takes two
// parameters `k` & `η1` ( read eta1 ) along with a pseudo random number
// generator and generates byte serialized public key and secret key of
// following length
//
// Possible values of parameters like `k` & `η1`, can be found from table 1 of
// specification ( linked below ).
//
// public key: (k * 12 * 32 + 32) -bytes wide
// secret key: (k * 24 * 32 + 96) -bytes wide [ includes public key ]
//
// See algorithm 7 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const size_t eta1>
static inline void
keygen(prng::prng_t& prng,
       uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // (k * 24 * 32 + 96) -bytes secret key
)
{
  uint8_t d[32];
  uint8_t z[32];

  prng.read(d, sizeof(d));
  prng.read(z, sizeof(z));

  ccakem::keygen<k, eta1>(d, z, pubkey, seckey);
}

// Given (k * 12 * 32 + 32) -bytes public key and a pseudo random number
// generator, this routine computes cipher text of length (k * du * 32 + dv *
// 32) -bytes which can be shared with recipient party ( having respective
// secret key ) over insecure channel, so that both of these communicating
// parties reach to same shared secret key ( derived from a KDF, which is seed
// with same 32 -bytes, that's encrypted using public key cryptography i.e.
// Kyber PKE, in this context ).
//
// Returned SHAKE256 object acts as a KDF ( key derivation function ), used for
// generating arbitrary length shared secret key, to be used for symmetric key
// encryption between these two participating entities.
//
// Other side of communication should also be able to generate same arbitrary
// length key stream ( using KDF ), after successful decryption of cipher text.
//
// Possible values for parameters like `k`, `η1`, `η2`, `du` & `dv`, can be
// found from table 1 of specification ( linked below ).
//
// See algorithm 8 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static inline shake256::shake256<false>
encapsulate(prng::prng_t& prng,
            const uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes
            uint8_t* const __restrict cipher // (k * du * 32 + dv * 32) -bytes
)
{
  uint8_t m[32];
  prng.read(m, sizeof(m));

  return ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pubkey, cipher);
}

// Given (k * 24 * 32 + 96) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( i.e. cipher ) text, this routine recovers 32 -bytes plain text
// which was encrypted by sender, using respective public key, associated with
// this secret key.

// Recovered 32 -bytes plain text is used for deriving same key stream ( using
// SHAKE256 key derivation function ), which is the shared secret key between
// two communicating parties, over insecure channel. Using returned KDF (
// SHAKE256 object ) both parties can reach to same shared secret key ( of
// arbitrary length ), which will be used for encrypting traffic using symmetric
// key primitives.
//
// Find possible values for parameters like `k`, `η1`, `η2`, `du` & `dv` in
// table 1 of specification ( linked below ).
//
// See algorithm 9 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static inline shake256::shake256<false>
decapsulate(
  const uint8_t* const __restrict seckey, // (k * 24 * 32 + 96) -bytes
  const uint8_t* const __restrict cipher  // (k * du * 32 + dv * 32) -bytes
)
{
  return ccakem::decapsulate<k, eta1, eta2, du, dv>(seckey, cipher);
}

}
