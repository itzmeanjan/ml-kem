#include "ml_kem/ml_kem_1024.hpp"
#include "ml_kem/ml_kem_512.hpp"
#include "ml_kem/ml_kem_768.hpp"
#include "test_helper.hpp"
#include <gtest/gtest.h>

namespace {

// Seeds from NIST official KAT vector no. 1.
constexpr auto SEED_D = from_hex<32>("7c9935a0b07694aa0c6d10e4db6b1add2fd81a25ccb148032dcd739936737f2d");
constexpr auto SEED_Z = from_hex<32>("b505d7cfad1b497499323c8686325e4792f267aafa3f87ca60d01cb54f29202a");
constexpr auto SEED_M = from_hex<32>("eb4a7c66ef4eba2ddb38c88d8bc706b1d639002198172a7b1942eca8f6c001ba");

// Compile-time evaluation of ML-KEM-512 keygen, encapsulation, and decapsulation.
// If this file compiles, the static_assert has already proven correctness at compile-time.
constexpr bool
eval_ml_kem_512()
{
  std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> cipher{};
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> sender_key{};
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> receiver_key{};

  ml_kem_512::keygen(SEED_D, SEED_Z, pubkey, seckey);
  const auto encaps_ok = ml_kem_512::encapsulate(SEED_M, pubkey, cipher, sender_key);
  ml_kem_512::decapsulate(seckey, cipher, receiver_key);

  return encaps_ok && (sender_key == receiver_key);
}

// Compile-time evaluation of ML-KEM-768 keygen, encapsulation, and decapsulation.
constexpr bool
eval_ml_kem_768()
{
  std::array<uint8_t, ml_kem_768::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_768::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN> cipher{};
  std::array<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN> sender_key{};
  std::array<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN> receiver_key{};

  ml_kem_768::keygen(SEED_D, SEED_Z, pubkey, seckey);
  const auto encaps_ok = ml_kem_768::encapsulate(SEED_M, pubkey, cipher, sender_key);
  ml_kem_768::decapsulate(seckey, cipher, receiver_key);

  return encaps_ok && (sender_key == receiver_key);
}

// Compile-time evaluation of ML-KEM-1024 keygen, encapsulation, and decapsulation.
constexpr bool
eval_ml_kem_1024()
{
  std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_1024::CIPHER_TEXT_BYTE_LEN> cipher{};
  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> sender_key{};
  std::array<uint8_t, ml_kem_1024::SHARED_SECRET_BYTE_LEN> receiver_key{};

  ml_kem_1024::keygen(SEED_D, SEED_Z, pubkey, seckey);
  const auto encaps_ok = ml_kem_1024::encapsulate(SEED_M, pubkey, cipher, sender_key);
  ml_kem_1024::decapsulate(seckey, cipher, receiver_key);

  return encaps_ok && (sender_key == receiver_key);
}

} // anonymous namespace

// The static_asserts below are the real tests. They fire at compile-time.
// If this translation unit compiles, all three ML-KEM variants are proven correct at program compile-time itself.
// The GTest bodies exist only so that ctest reports them as passed.

TEST(ML_KEM, CompileTimeEval_ML_KEM_512)
{
  static_assert(eval_ml_kem_512(), "ML-KEM-512 keygen/encaps/decaps must be correct at compile-time");
  SUCCEED();
}

TEST(ML_KEM, CompileTimeEval_ML_KEM_768)
{
  static_assert(eval_ml_kem_768(), "ML-KEM-768 keygen/encaps/decaps must be correct at compile-time");
  SUCCEED();
}

TEST(ML_KEM, CompileTimeEval_ML_KEM_1024)
{
  static_assert(eval_ml_kem_1024(), "ML-KEM-1024 keygen/encaps/decaps must be correct at compile-time");
  SUCCEED();
}
