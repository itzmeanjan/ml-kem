#pragma once
#include "kyber_kem.hpp"
#include "utils.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Given k, η1, η2, du, dv - Kyber parameters, this routine checks whether
//
// - A new key pair can be generated for key establishment over insecure channel
// - Key pair is for receiving party, its public key will be used by sender.
// - Sender can produce a cipher text and a key derivation function ( KDF )
// - Sender uses receiver's public key.
// - Cipher text is sent over insecure channel to receiver
// - Receiver can decrypt message ( using secret key ) and arrives at same KDF
// - Both parties use KDF ( SHAKE256 hasher object ) to generate arbitrary
//   length shared secret key.
// - This shared secret key can now be used with any symmetric key primitive.
//
// works as expected.
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv,
         const size_t klen>
void
test_kyber_cca_kem()
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_ccakem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_ccakem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_ccakem_cipher_len<k, du, dv>();

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* m = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));
  uint8_t* receiver_key = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);
  std::memset(cipher, 0, ctlen);

  prng::prng_t prng;
  prng.read(d, slen);
  prng.read(z, slen);
  prng.read(m, slen);

  ccakem::keygen<k, eta1>(d, z, pkey, skey);
  auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
  auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(skey, cipher);

  skdf.read(sender_key, klen);
  rkdf.read(receiver_key, klen);

  bool flg = false;
  for (size_t i = 0; i < klen; i++) {
    flg |= static_cast<bool>(sender_key[i] ^ receiver_key[i]);
  }

  std::free(d);
  std::free(z);
  std::free(m);
  std::free(pkey);
  std::free(skey);
  std::free(cipher);
  std::free(sender_key);
  std::free(receiver_key);

  assert(!flg);
}

}
