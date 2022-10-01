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
// Note, 32 -bytes random sampling is done pretty naively ( see
// ./utils.hpp ), it uses C++ standard library's `<random>` functionality. Also
// see https://en.cppreference.com/w/cpp/header/random for more information.
//
// See algorithm 7 defined in Kyber specification, as submitted to NIST PQC
// final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k, const size_t eta1>
static void
keygen(uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // (k * 24 * 32 + 96) -bytes secret key
)
{
  constexpr size_t zlen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;

  constexpr size_t skoff0 = k * 12 * 32;
  constexpr size_t skoff1 = skoff0 + pklen;
  constexpr size_t skoff2 = skoff1 + 32;

  kyber_utils::random_data<uint8_t>(seckey + skoff2, zlen); // sample 32 -bytes
  cpapke::keygen<k, eta1>(pubkey, seckey);        // CPAPKE key generation
  std::memcpy(seckey + skoff0, pubkey, pklen);    // copy public key
  sha3_256::hash(pubkey, pklen, seckey + skoff1); // hash public key
}

}
