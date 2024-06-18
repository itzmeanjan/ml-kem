#include "ml_kem/ml_kem_512.hpp"
#include <gtest/gtest.h>

// For ML-KEM-512
//
// - A new key pair can be generated for key establishment over insecure channel.
// - Secret key is for receiving party, its public key will be used by sender.
// - Sender can produce a cipher text and a shared secret.
// - Sender uses receiver's public key.
// - Cipher text is sent over insecure channel to receiver.
// - Receiver can decrypt message ( using secret key ) and produce same shared secret.
//
// works as expected.
TEST(ML_KEM, ML_KEM_512_KeygenEncapsDecaps)
{
  std::array<uint8_t, ml_kem_512::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_512::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_512::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> shared_secret_sender{};
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> shared_secret_receiver{};

  ml_kem_prng::prng_t<128> prng{};
  prng.read(seed_d);
  prng.read(seed_z);
  prng.read(seed_m);

  ml_kem_512::keygen(seed_d, seed_z, pubkey, seckey);
  const auto is_encapsulated = ml_kem_512::encapsulate(seed_m, pubkey, cipher, shared_secret_sender);
  ml_kem_512::decapsulate(seckey, cipher, shared_secret_receiver);

  EXPECT_TRUE(is_encapsulated);
  EXPECT_EQ(shared_secret_sender, shared_secret_receiver);
}
