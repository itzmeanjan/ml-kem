#pragma once
#include "k_pke.hpp"
#include "ml_kem/internals/utility/utils.hpp"
#include "sha3_256.hpp"
#include "sha3_512.hpp"
#include "shake256.hpp"
#include <algorithm>

// Key Encapsulation Mechanism
namespace ml_kem {

// ML-KEM key generation algorithm, generating byte serialized public key and secret key, given 32 -bytes seed `d` and `z`.
// See algorithm 16 defined in ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t k, size_t eta1>
constexpr void
keygen(std::span<const uint8_t, 32> d, // used in CPA-PKE
       std::span<const uint8_t, 32> z, // used in CCA-KEM
       std::span<uint8_t, ml_kem_utils::get_kem_public_key_len(k)> pubkey,
       std::span<uint8_t, ml_kem_utils::get_kem_secret_key_len(k)> seckey)
  requires(ml_kem_params::check_keygen_params(k, eta1))
{
  constexpr size_t skoff0 = k * 12 * 32;
  constexpr size_t skoff1 = skoff0 + pubkey.size();
  constexpr size_t skoff2 = skoff1 + 32;

  auto _seckey0 = seckey.template subspan<0, skoff0>();
  auto _seckey1 = seckey.template subspan<skoff0, skoff1 - skoff0>();
  auto _seckey2 = seckey.template subspan<skoff1, skoff2 - skoff1>();
  auto _seckey3 = seckey.template subspan<skoff2, seckey.size() - skoff2>();

  k_pke::keygen<k, eta1>(d, pubkey, _seckey0);
  std::copy(pubkey.begin(), pubkey.end(), _seckey1.begin());
  std::copy(z.begin(), z.end(), _seckey3.begin());

  sha3_256::sha3_256_t hasher{};
  hasher.absorb(pubkey);
  hasher.finalize();
  hasher.digest(_seckey2);
  hasher.reset();
}

// Given ML-KEM public key and 32 -bytes seed ( used for deriving 32 -bytes message & 32 -bytes random coin ), this routine computes
// ML-KEM cipher text which can be shared with recipient party ( owning corresponding secret key ) over insecure channel.
//
// It also computes a fixed length 32 -bytes shared secret, which can be used for fast symmetric key encryption between these
// two participating entities. Alternatively they might choose to derive longer keys from this shared secret. Other side of
// communication should also be able to generate same 32 -byte shared secret, after successful decryption of cipher text.
//
// If invalid ML-KEM public key is input, this function execution will fail, returning false.
//
// See algorithm 17 defined in ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
[[nodiscard("Use result, it might fail because of malformed input public key")]] constexpr bool
encapsulate(std::span<const uint8_t, 32> m,
            std::span<const uint8_t, ml_kem_utils::get_kem_public_key_len(k)> pubkey,
            std::span<uint8_t, ml_kem_utils::get_kem_cipher_text_len(k, du, dv)> cipher,
            std::span<uint8_t, 32> shared_secret)
  requires(ml_kem_params::check_encap_params(k, eta1, eta2, du, dv))
{
  std::array<uint8_t, m.size() + sha3_256::DIGEST_LEN> g_in{};
  std::array<uint8_t, sha3_512::DIGEST_LEN> g_out{};

  auto g_in_span = std::span(g_in);
  auto g_in_span0 = g_in_span.template first<m.size()>();
  auto g_in_span1 = g_in_span.template last<sha3_256::DIGEST_LEN>();

  auto g_out_span = std::span(g_out);
  auto g_out_span0 = g_out_span.template first<shared_secret.size()>();
  auto g_out_span1 = g_out_span.template last<g_out_span.size() - g_out_span0.size()>();

  std::copy(m.begin(), m.end(), g_in_span0.begin());

  sha3_256::sha3_256_t h256{};
  h256.absorb(pubkey);
  h256.finalize();
  h256.digest(g_in_span1);

  sha3_512::sha3_512_t h512{};
  h512.absorb(g_in_span);
  h512.finalize();
  h512.digest(g_out_span);

  const auto has_mod_check_passed = k_pke::encrypt<k, eta1, eta2, du, dv>(pubkey, m, g_out_span1, cipher);
  if (!has_mod_check_passed) {
    // Got an invalid public key
    return has_mod_check_passed;
  }

  std::copy(g_out_span0.begin(), g_out_span0.end(), shared_secret.begin());
  return true;
}

// Given ML-KEM secret key and cipher text, this routine recovers 32 -bytes plain text which was encrypted by sender,
// using ML-KEM public key, associated with this secret key.
//
// Recovered 32 -bytes plain text is used for deriving a 32 -bytes shared secret key, which can now be
// used for encrypting communication between two participating parties, using fast symmetric key algorithms.
//
// See algorithm 18 defined in ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
constexpr void
decapsulate(std::span<const uint8_t, ml_kem_utils::get_kem_secret_key_len(k)> seckey,
            std::span<const uint8_t, ml_kem_utils::get_kem_cipher_text_len(k, du, dv)> cipher,
            std::span<uint8_t, 32> shared_secret)
  requires(ml_kem_params::check_decap_params(k, eta1, eta2, du, dv))
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

  std::array<uint8_t, 32 + h.size()> g_in{};
  std::array<uint8_t, shared_secret.size() + 32> g_out{};
  std::array<uint8_t, shared_secret.size()> j_out{};
  std::array<uint8_t, cipher.size()> c_prime{};

  auto g_in_span = std::span(g_in);
  auto g_in_span0 = g_in_span.template first<32>();
  auto g_in_span1 = g_in_span.template last<h.size()>();

  auto g_out_span = std::span(g_out);
  auto g_out_span0 = g_out_span.template first<shared_secret.size()>();
  auto g_out_span1 = g_out_span.template last<32>();

  k_pke::decrypt<k, du, dv>(pke_sk, cipher, g_in_span0);
  std::copy(h.begin(), h.end(), g_in_span1.begin());

  sha3_512::sha3_512_t h512{};
  h512.absorb(g_in_span);
  h512.finalize();
  h512.digest(g_out_span);

  shake256::shake256_t xof256{};
  xof256.absorb(z);
  xof256.absorb(cipher);
  xof256.finalize();
  xof256.squeeze(j_out);

  // Explicitly ignore return value, because public key, held as part of secret key is *assumed* to be valid.
  (void)k_pke::encrypt<k, eta1, eta2, du, dv>(pubkey, g_in_span0, g_out_span1, c_prime);

  // line 9-12 of algorithm 17, in constant-time
  using kdf_t = std::span<const uint8_t, shared_secret.size()>;
  const uint32_t cond = ml_kem_utils::ct_memcmp(cipher, std::span<const uint8_t, ctlen>(c_prime));
  ml_kem_utils::ct_cond_memcpy(cond, shared_secret, kdf_t(g_out_span0), kdf_t(z));
}

}
