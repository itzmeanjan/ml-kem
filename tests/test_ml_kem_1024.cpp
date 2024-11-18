#include "ml_kem/ml_kem_1024.hpp"
#include "randomshake/randomshake.hpp"
#include "test_helper.hpp"
#include <gtest/gtest.h>

// For ML-KEM-1024
//
// - A new key pair can be generated for key establishment over insecure channel.
// - Secret key is for receiving party, its public key will be used by sender.
// - Sender can produce a cipher text and a shared secret.
// - Sender uses receiver's public key.
// - Cipher text is sent over insecure channel to receiver.
// - Receiver can decrypt message ( using secret key ) and produce same shared secret.
//
// works as expected.
TEST(ML_KEM, ML_KEM_1024_KeygenEncapsDecaps)
{
  std::array<uint8_t, ml_kem_1024::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_1024::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_1024::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_1024::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> shared_secret_sender{};
  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> shared_secret_receiver{};

  randomshake::randomshake_t<256> csprng{};
  csprng.generate(seed_d);
  csprng.generate(seed_z);
  csprng.generate(seed_m);

  ml_kem_1024::keygen(seed_d, seed_z, pubkey, seckey);
  const auto is_encapsulated = ml_kem_1024::encapsulate(seed_m, pubkey, cipher, shared_secret_sender);
  ml_kem_1024::decapsulate(seckey, cipher, shared_secret_receiver);

  EXPECT_TRUE(is_encapsulated);
  EXPECT_EQ(shared_secret_sender, shared_secret_receiver);
}

// For ML-KEM-1024
//
// - Generate a valid keypair.
// - Malform public key s.t. last coefficient of polynomial vector is not properly reduced.
// - Attempt to encapsulate using malformed public key. It must fail.
TEST(ML_KEM, ML_KEM_1024_EncapsFailureDueToNonReducedPubKey)
{
  std::array<uint8_t, ml_kem_1024::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_1024::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_1024::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_1024::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> shared_secret{};

  randomshake::randomshake_t<256> csprng{};
  csprng.generate(seed_d);
  csprng.generate(seed_z);
  csprng.generate(seed_m);

  ml_kem_1024::keygen(seed_d, seed_z, pubkey, seckey);

  make_malformed_pubkey<pubkey.size()>(pubkey);
  const auto is_encapsulated = ml_kem_1024::encapsulate(seed_m, pubkey, cipher, shared_secret);

  EXPECT_FALSE(is_encapsulated);
}

// For ML-KEM-1024
//
// - Generate a valid keypair.
// - Encapsulate using public key, generate shared secret, at sender's side.
// - Cause a random bitflip in cipher text, at receiver's side.
// - Attempt to decapsulate bit-flipped cipher text, using valid secret key. Must fail *implicitly*.
// - Shared secret of sender and receiver must not match.
// - Shared secret at receiver's end must match `seed_z`, which is last 32 -bytes of secret key.
TEST(ML_KEM, ML_KEM_1024_DecapsFailureDueToBitFlippedCipherText)
{
  std::array<uint8_t, ml_kem_1024::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_1024::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_1024::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_1024::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> shared_secret_sender{};
  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> shared_secret_receiver{};

  randomshake::randomshake_t<256> csprng{};
  csprng.generate(seed_d);
  csprng.generate(seed_z);
  csprng.generate(seed_m);

  ml_kem_1024::keygen(seed_d, seed_z, pubkey, seckey);
  const auto is_encapsulated = ml_kem_1024::encapsulate(seed_m, pubkey, cipher, shared_secret_sender);

  random_bitflip_in_cipher_text<cipher.size()>(cipher, csprng);
  ml_kem_1024::decapsulate(seckey, cipher, shared_secret_receiver);

  EXPECT_TRUE(is_encapsulated);
  EXPECT_NE(shared_secret_sender, shared_secret_receiver);
  EXPECT_EQ(shared_secret_receiver, seed_z);
  EXPECT_TRUE(std::equal(shared_secret_receiver.begin(), shared_secret_receiver.end(), std::span(seckey).last<32>().begin()));
}
