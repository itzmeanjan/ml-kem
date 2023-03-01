#pragma once
#include "pke_keygen.hpp"
#include "sha3_256.hpp"

// IND-CCA2-secure Key Encapsulation Mechanism
namespace ccakem {

// Kyber CCAKEM key generation algorithm, which takes two parameters `k` & `η1`
// ( read eta1 ) and generates byte serialized public key and secret key of
// following length
//
// public key: (k * 12 * 32 + 32) -bytes wide
// secret key: (k * 24 * 32 + 96) -bytes wide [ includes public key ]
//
// See algorithm 7 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
//
// Note, this routine allows you to pass two 32 -bytes seeds ( see first &
// second parameter ), which is designed this way for ease of writing test cases
// against known answer tests, obtained from Kyber reference implementation
// https://github.com/pq-crystals/kyber.git. It also helps in properly
// benchmarking underlying KEM's key generation implementation.
template<const size_t k, const size_t eta1>
static inline void
keygen(const uint8_t* const __restrict d, // 32 -bytes seed ( used in CPA-PKE )
       const uint8_t* const __restrict z, // 32 -bytes seed ( used in CCA-KEM )
       uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // (k * 24 * 32 + 96) -bytes secret key
       )
  requires(kyber_params::check_keygen_params(k, eta1))
{
  constexpr size_t zlen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;

  constexpr size_t skoff0 = k * 12 * 32;
  constexpr size_t skoff1 = skoff0 + pklen;
  constexpr size_t skoff2 = skoff1 + 32;

  std::memcpy(seckey + skoff2, z, zlen);
  cpapke::keygen<k, eta1>(d, pubkey, seckey);     // CPAPKE key generation
  std::memcpy(seckey + skoff0, pubkey, pklen);    // copy public key
  sha3_256::hash(pubkey, pklen, seckey + skoff1); // hash public key
}

}
