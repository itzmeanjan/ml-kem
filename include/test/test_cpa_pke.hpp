#pragma once
#include "kyber_pke.hpp"
#include "utils.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Given k, η1, η2, du, dv - Kyber parameters, this routine checks whether
//
// - A random key pair can be generated
// - Public key can be used for encrypting 32 -bytes random message
// - Secret key can decrypt (k * du * 32 + dv * 32) -bytes cipher text to
//   32 -bytes plain text
//
// works as expected.
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
void
test_kyber_cpa_pke()
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_cpapke_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_cpapke_secret_key_len<k>();
  constexpr size_t mlen = 32;
  constexpr size_t enclen = kyber_utils::get_cpapke_cipher_len<k, du, dv>();

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* txt = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));
  uint8_t* dec = static_cast<uint8_t*>(std::malloc(mlen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);
  std::memset(enc, 0, enclen);
  std::memset(dec, 0, mlen);

  prng::prng_t prng;
  prng.read(seed, slen);
  prng.read(txt, mlen);
  prng.read(rcoin, mlen);

  cpapke::keygen<k, eta1>(seed, pkey, skey);
  cpapke::encrypt<k, eta1, eta2, du, dv>(pkey, txt, rcoin, enc);
  cpapke::decrypt<k, du, dv>(skey, enc, dec);

  bool flg = false;
  for (size_t i = 0; i < mlen; i++) {
    flg |= static_cast<bool>(txt[i] ^ dec[i]);
  }

  std::free(seed);
  std::free(pkey);
  std::free(skey);
  std::free(rcoin);
  std::free(txt);
  std::free(enc);
  std::free(dec);

  assert(!flg);
}

}
