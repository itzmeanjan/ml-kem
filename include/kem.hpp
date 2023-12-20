#pragma once
#include "pke.hpp"
#include "sha3_256.hpp"
#include "sha3_512.hpp"
#include "shake256.hpp"
#include "utils.hpp"
#include <array>
#include <cstdint>

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
template<size_t k, size_t eta1>
static inline void
keygen(std::span<const uint8_t, 32> d, // used in CPA-PKE
       std::span<const uint8_t, 32> z, // used in CCA-KEM
       std::span<uint8_t, kyber_utils::get_kem_public_key_len(k)> pubkey,
       std::span<uint8_t, kyber_utils::get_kem_secret_key_len(k)> seckey)
  requires(kyber_params::check_keygen_params(k, eta1))
{
  constexpr size_t skoff0 = k * 12 * 32;
  constexpr size_t skoff1 = skoff0 + pubkey.size();
  constexpr size_t skoff2 = skoff1 + 32;

  auto _seckey0 = seckey.template subspan<0, skoff0>();
  auto _seckey1 = seckey.template subspan<skoff0, skoff1 - skoff0>();
  auto _seckey2 = seckey.template subspan<skoff1, skoff2 - skoff1>();
  auto _seckey3 = seckey.template subspan<skoff2, seckey.size() - skoff2>();

  pke::keygen<k, eta1>(d, pubkey, _seckey0); // CPAPKE key generation
  std::copy(pubkey.begin(), pubkey.end(), _seckey1.begin());
  std::copy(z.begin(), z.end(), _seckey3.begin());

  // hash public key
  sha3_256::sha3_256_t hasher;
  hasher.absorb(pubkey);
  hasher.finalize();
  hasher.digest(_seckey2);
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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
static inline shake256::shake256_t
encapsulate(std::span<const uint8_t, 32> m,
            std::span<const uint8_t, kyber_utils::get_kem_public_key_len(k)> pubkey,
            std::span<uint8_t, kyber_utils::get_kem_cipher_len(k, du, dv)> cipher)
  requires(kyber_params::check_encap_params(k, eta1, eta2, du, dv))
{
  std::array<uint8_t, 64> g_in{};
  std::array<uint8_t, 64> g_out{};
  std::array<uint8_t, 64> kdf_in{};

  auto _g_in = std::span(g_in);
  auto _g_out = std::span(g_out);
  auto _kdf_in = std::span(kdf_in);

  auto _g_in0 = _g_in.template subspan<0, 32>();
  auto _g_in1 = _g_in.template subspan<_g_in0.size(), 32>();

  auto _g_out0 = _g_out.template subspan<0, 32>();
  auto _g_out1 = _g_out.template subspan<_g_out0.size(), 32>();

  auto _kdf_in0 = _kdf_in.template subspan<0, 32>();
  auto _kdf_in1 = _kdf_in.template subspan<_kdf_in0.size(), 32>();

  sha3_256::sha3_256_t h256;

  h256.absorb(m);
  h256.finalize();
  h256.digest(_g_in0);
  h256.reset();

  h256.absorb(pubkey);
  h256.finalize();
  h256.digest(_g_in1);
  h256.reset();

  sha3_512::sha3_512_t h512;

  h512.absorb(_g_in);
  h512.finalize();
  h512.digest(_g_out);

  pke::encrypt<k, eta1, eta2, du, dv>(pubkey, _g_in0, _g_out1, cipher);
  std::copy(_g_out0.begin(), _g_out0.end(), _kdf_in0.begin());

  h256.absorb(cipher);
  h256.finalize();
  h256.digest(_kdf_in1);

  shake256::shake256_t xof256;
  xof256.absorb(_kdf_in);
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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
static inline shake256::shake256_t
decapsulate(std::span<const uint8_t, kyber_utils::get_kem_secret_key_len(k)> seckey, std::span<const uint8_t, kyber_utils::get_kem_cipher_len(k, du, dv)> cipher)
  requires(kyber_params::check_decap_params(k, eta1, eta2, du, dv))
{
  constexpr size_t sklen = k * 12 * 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t ctlen = cipher.size();

  constexpr size_t skoff0 = sklen;
  constexpr size_t skoff1 = skoff0 + pklen;
  constexpr size_t skoff2 = skoff1 + 32;

  auto pke_sk = seckey.template subspan<0, skoff0>();
  auto pubkey = seckey.template subspan<skoff0, skoff1 - skoff0>();
  auto h = seckey.template subspan<skoff1, skoff2 - skoff1>();
  auto z = seckey.template subspan<skoff2, seckey.size() - skoff2>();

  std::array<uint8_t, 64> g_in{};
  std::array<uint8_t, 64> g_out{};
  std::array<uint8_t, cipher.size()> c_prime{};
  std::array<uint8_t, 64> kdf_in{};

  auto _g_in = std::span(g_in);
  auto _g_out = std::span(g_out);
  auto _kdf_in = std::span(kdf_in);

  auto _g_in0 = _g_in.template subspan<0, 32>();
  auto _g_in1 = _g_in.template subspan<_g_in0.size(), 32>();

  auto _g_out0 = _g_out.template subspan<0, 32>();
  auto _g_out1 = _g_out.template subspan<_g_out0.size(), 32>();

  auto _kdf_in0 = _kdf_in.template subspan<0, 32>();
  auto _kdf_in1 = _kdf_in.template subspan<_kdf_in0.size(), 32>();

  pke::decrypt<k, du, dv>(pke_sk, cipher, _g_in0);
  std::copy(h.begin(), h.end(), _g_in1.begin());

  sha3_512::sha3_512_t h512;
  h512.absorb(_g_in);
  h512.finalize();
  h512.digest(_g_out);

  pke::encrypt<k, eta1, eta2, du, dv>(pubkey, _g_in0, _g_out1, c_prime);

  // line 7-11 of algorithm 9, in constant-time
  using kdf_t = std::span<const uint8_t, 32>;
  const uint32_t cond = kyber_utils::ct_memcmp(cipher, std::span<const uint8_t, ctlen>(c_prime));
  kyber_utils::ct_cond_memcpy(cond, _kdf_in0, kdf_t(_g_out0), kdf_t(z));

  sha3_256::sha3_256_t h256;
  h256.absorb(cipher);
  h256.finalize();
  h256.digest(_kdf_in1);

  shake256::shake256_t xof256;
  xof256.absorb(_kdf_in);
  xof256.finalize();
  return xof256;
}

}
