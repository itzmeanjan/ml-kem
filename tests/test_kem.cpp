#include "kem.hpp"
#include "utils.hpp"
#include <gtest/gtest.h>

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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv, size_t klen>
void
test_kyber_kem()
  requires(klen > 0)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_kem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_kem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_kem_cipher_len<k, du, dv>();

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(klen);
  std::vector<uint8_t> receiver_key(klen);

  auto _d = std::span<uint8_t, slen>(d);
  auto _z = std::span<uint8_t, slen>(z);
  auto _m = std::span<uint8_t, slen>(m);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _cipher = std::span<uint8_t, ctlen>(cipher);

  prng::prng_t prng;
  prng.read(d);
  prng.read(z);
  prng.read(m);

  kem::keygen<k, eta1>(_d, _z, _pkey, _skey);
  auto skdf = kem::encapsulate<k, eta1, eta2, du, dv>(_m, _pkey, _cipher);
  auto rkdf = kem::decapsulate<k, eta1, eta2, du, dv>(_skey, _cipher);

  skdf.squeeze(sender_key);
  rkdf.squeeze(receiver_key);

  EXPECT_EQ(sender_key, receiver_key);
}

TEST(KyberKEM, Kyber512KeygenEncapsDecaps)
{
  test_kyber_kem<2, 3, 2, 10, 4, 32>();
}

TEST(KyberKEM, Kyber768KeygenEncapsDecaps)
{
  test_kyber_kem<3, 2, 2, 10, 4, 32>();
}

TEST(KyberKEM, Kyber1024KeygenEncapsDecaps)
{
  test_kyber_kem<4, 2, 2, 11, 5, 32>();
}
