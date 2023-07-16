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
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv,
         const size_t klen>
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

  prng::prng_t prng;
  prng.read(d.data(), d.size());
  prng.read(z.data(), z.size());
  prng.read(m.data(), m.size());

  kem::keygen<k, eta1>(d.data(), z.data(), pkey.data(), skey.data());
  auto skdf = kem::encapsulate<k, eta1, eta2, du, dv>(
    m.data(), pkey.data(), cipher.data());
  auto rkdf =
    kem::decapsulate<k, eta1, eta2, du, dv>(skey.data(), cipher.data());

  skdf.squeeze(sender_key.data(), sender_key.size());
  rkdf.squeeze(receiver_key.data(), receiver_key.size());

  ASSERT_EQ(sender_key, receiver_key);
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
