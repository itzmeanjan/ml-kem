#include "ml_kem/internals/ml_kem.hpp"
#include <gtest/gtest.h>

// Given k, η1, η2, du, dv - ML-KEM parameters, this routine checks whether
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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv, size_t bit_security_level>
void
test_ml_kem_kem()
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = ml_kem_utils::get_kem_public_key_len(k);
  constexpr size_t sklen = ml_kem_utils::get_kem_secret_key_len(k);
  constexpr size_t ctlen = ml_kem_utils::get_kem_cipher_text_len(k, du, dv);
  constexpr size_t sslen = 32;

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(sslen);
  std::vector<uint8_t> receiver_key(sslen);

  auto _d = std::span<uint8_t, slen>(d);
  auto _z = std::span<uint8_t, slen>(z);
  auto _m = std::span<uint8_t, slen>(m);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _cipher = std::span<uint8_t, ctlen>(cipher);
  auto _sender_key = std::span<uint8_t, sslen>(sender_key);
  auto _receiver_key = std::span<uint8_t, sslen>(receiver_key);

  prng::prng_t<bit_security_level> prng{};
  prng.read(d);
  prng.read(z);
  prng.read(m);

  ml_kem::keygen<k, eta1>(_d, _z, _pkey, _skey);
  (void)ml_kem::encapsulate<k, eta1, eta2, du, dv>(_m, _pkey, _cipher, _sender_key);
  ml_kem::decapsulate<k, eta1, eta2, du, dv>(_skey, _cipher, _receiver_key);

  EXPECT_EQ(sender_key, receiver_key);
}

TEST(Ml_kemKEM, Ml_kem512KeygenEncapsDecaps)
{
  test_ml_kem_kem<2, 3, 2, 10, 4, 128>();
}

TEST(Ml_kemKEM, Ml_kem768KeygenEncapsDecaps)
{
  test_ml_kem_kem<3, 2, 2, 10, 4, 192>();
}

TEST(Ml_kemKEM, Ml_kem1024KeygenEncapsDecaps)
{
  test_ml_kem_kem<4, 2, 2, 11, 5, 256>();
}
