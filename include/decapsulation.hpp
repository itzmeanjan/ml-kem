#pragma once
#include "decryption.hpp"
#include "encryption.hpp"
#include "params.hpp"
#include "sha3_256.hpp"
#include "sha3_512.hpp"
#include "subtle.hpp"

// IND-CCA2-secure Key Encapsulation Mechanism
namespace ccakem {

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
static inline shake256::shake256<false>
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

  cpapke::decrypt<k, du, dv>(seckey, cipher, g_in);
  std::memcpy(g_in + 32, h, 32);
  sha3_512::hash(g_in, sizeof(g_in), g_out);

  cpapke::encrypt<k, eta1, eta2, du, dv>(pubkey, g_in, g_out + 32, c_prime);

  // line 7-11 of algorithm 9, in constant-time
  uint32_t flg = -1u;
  for (size_t i = 0; i < ctlen; i++) {
    flg &= subtle::ct_eq<uint8_t, uint32_t>(cipher[i], c_prime[i]);
  }

  for (size_t i = 0; i < 32; i++) {
    kdf_in[i] = subtle::ct_select(flg, g_out[i], z[i]);
  }

  sha3_256::hash(cipher, ctlen, kdf_in + 32);

  shake256::shake256 hasher;
  hasher.hash(kdf_in, sizeof(kdf_in));
  return hasher;
}

}
