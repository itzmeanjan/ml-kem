#pragma once
#include "pke.hpp"
#include "sha3_256.hpp"
#include "sha3_512.hpp"
#include "shake256.hpp"
#include "subtle.hpp"

// IND-CCA2-secure Key Encapsulation Mechanism
namespace kem {

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
  pke::keygen<k, eta1>(d, pubkey, seckey);     // CPAPKE key generation
  std::memcpy(seckey + skoff0, pubkey, pklen); // copy public key

  // hash public key
  sha3_256::sha3_256 hasher;
  hasher.absorb(pubkey, pklen);
  hasher.finalize();
  hasher.digest(seckey + skoff1);
}

// Given (k * 12 * 32 + 32) -bytes public key and 32 -bytes seed ( used for
// deriving 32 -bytes message & 32 -bytes random coin ), this routine computes
// cipher text of length (k * du * 32 + dv * 32) -bytes which can be shared with
// recipient party ( having respective secret key ) over insecure channel.
//
// It also returns a SHAKE256 object which acts as a KDF ( key derivation
// function ), used for generating arbitrary length shared secret key, to be
// used for symmetric key encryption between these two participating entities.
//
// Other side of communication should also be able to generate same arbitrary
// length key stream ( using KDF ), after successful decryption of cipher text.
//
// See algorithm 8 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
//
// Note, this routine allows you to pass 32 -bytes seed ( see first parameter ),
// which is designed this way for ease of writing test cases against known
// answer tests, obtained from Kyber reference implementation
// https://github.com/pq-crystals/kyber.git. It also helps in properly
// benchmarking underlying KEM's encapsulation implementation.
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static inline shake256::shake256
encapsulate(
  const uint8_t* const __restrict m,      // 32 -bytes seed for encapsulation
  const uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes
  uint8_t* const __restrict cipher        // (k * du * 32 + dv * 32) -bytes
  )
  requires(kyber_params::check_encap_params(k, eta1, eta2, du, dv))
{
  constexpr size_t mlen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;

  uint8_t g_in[64]{};
  uint8_t g_out[64]{};
  uint8_t kdf_in[64]{};

  sha3_256::sha3_256 h256;

  h256.absorb(m, mlen);
  h256.finalize();
  h256.digest(g_in);
  h256.reset();

  h256.absorb(pubkey, pklen);
  h256.finalize();
  h256.digest(g_in + 32);
  h256.reset();

  sha3_512::sha3_512 h512;

  h512.absorb(g_in, sizeof(g_in));
  h512.finalize();
  h512.digest(g_out);
  h512.reset();

  pke::encrypt<k, eta1, eta2, du, dv>(pubkey, g_in, g_out + 32, cipher);

  std::memcpy(kdf_in, g_out, 32);

  h256.absorb(cipher, ctlen);
  h256.finalize();
  h256.digest(kdf_in + 32);
  h256.reset();

  shake256::shake256 xof256;
  xof256.absorb(kdf_in, sizeof(kdf_in));
  xof256.finalize();
  return xof256;
}

// Given (k * 24 * 32 + 96) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( cipher ) text, this routine recovers 32 -bytes plain text which
// was encrypted by sender, using respective public key, associated with this
// secret key.

// Recovered 32 -bytes plain text is used for deriving same key stream ( using
// SHAKE256 key derivation function ), which is the shared secret key between
// two communicating parties, over insecure channel. Using returned KDF (
// SHAKE256 object ) both parties can reach to same shared secret key ( of
// arbitrary length ), which will be used for encrypting traffic using symmetric
// key primitives.
//
// See algorithm 9 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static inline shake256::shake256
decapsulate(
  const uint8_t* const __restrict seckey, // (k * 24 * 32 + 96) -bytes
  const uint8_t* const __restrict cipher  // (k * du * 32 + dv * 32) -bytes
  )
  requires(kyber_params::check_decap_params(k, eta1, eta2, du, dv))
{
  constexpr size_t sklen = k * 12 * 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;

  constexpr size_t skoff0 = sklen;
  constexpr size_t skoff1 = skoff0 + pklen;
  constexpr size_t skoff2 = skoff1 + 32;

  const uint8_t* const pubkey = seckey + skoff0;
  const uint8_t* const h = seckey + skoff1;
  const uint8_t* const z = seckey + skoff2;

  uint8_t g_in[64]{};
  uint8_t g_out[64]{};
  uint8_t c_prime[ctlen]{};
  uint8_t kdf_in[64]{};

  pke::decrypt<k, du, dv>(seckey, cipher, g_in);
  std::memcpy(g_in + 32, h, 32);

  sha3_512::sha3_512 h512;
  h512.absorb(g_in, sizeof(g_in));
  h512.finalize();
  h512.digest(g_out);
  h512.reset();

  pke::encrypt<k, eta1, eta2, du, dv>(pubkey, g_in, g_out + 32, c_prime);

  // line 7-11 of algorithm 9, in constant-time
  uint32_t flg = -1u;
  for (size_t i = 0; i < ctlen; i++) {
    flg &= subtle::ct_eq<uint8_t, uint32_t>(cipher[i], c_prime[i]);
  }

  for (size_t i = 0; i < 32; i++) {
    kdf_in[i] = subtle::ct_select(flg, g_out[i], z[i]);
  }

  sha3_256::sha3_256 h256;
  h256.absorb(cipher, ctlen);
  h256.finalize();
  h256.digest(kdf_in + 32);

  shake256::shake256 xof256;
  xof256.absorb(kdf_in, sizeof(kdf_in));
  xof256.finalize();
  return xof256;
}

}
