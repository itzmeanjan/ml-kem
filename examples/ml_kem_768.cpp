#include "ml_kem/ml_kem_768.hpp"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

// Given a bytearray of length N, this function converts it to human readable hex formatted string of length 2*N | N >= 0.
static inline std::string
to_hex(std::span<const uint8_t> bytes)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < bytes.size(); i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Compile it with
//
// g++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I ./include -I ./sha3/include -I ./subtle/include/ examples/ml_kem_768.cpp
int
main()
{
  constexpr size_t SEED_LEN = 32;
  constexpr size_t KEY_LEN = 32;

  // Seeds required for keypair generation
  std::vector<uint8_t> d(SEED_LEN, 0);
  std::vector<uint8_t> z(SEED_LEN, 0);

  auto _d = std::span<uint8_t, SEED_LEN>(d);
  auto _z = std::span<uint8_t, SEED_LEN>(z);

  // Public/ private keypair
  std::vector<uint8_t> pkey(ml_kem_768::PKEY_BYTE_LEN, 0);
  std::vector<uint8_t> skey(ml_kem_768::SKEY_BYTE_LEN, 0);

  auto _pkey = std::span<uint8_t, ml_kem_768::PKEY_BYTE_LEN>(pkey);
  auto _skey = std::span<uint8_t, ml_kem_768::SKEY_BYTE_LEN>(skey);

  // Seed required for key encapsulation
  std::vector<uint8_t> m(SEED_LEN, 0);
  std::vector<uint8_t> cipher(ml_kem_768::CIPHER_TEXT_BYTE_LEN, 0);

  auto _m = std::span<uint8_t, SEED_LEN>(m);
  auto _cipher = std::span<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN>(cipher);

  // Shared secret that sender/ receiver arrives at
  std::vector<uint8_t> shrd_key0(KEY_LEN, 0);
  std::vector<uint8_t> shrd_key1(KEY_LEN, 0);

  auto _shrd_key0 = std::span<uint8_t, KEY_LEN>(shrd_key0);
  auto _shrd_key1 = std::span<uint8_t, KEY_LEN>(shrd_key1);

  // Pseudo-randomness source
  prng::prng_t<128> prng{};

  // Fill up seeds using PRNG
  prng.read(_d);
  prng.read(_z);

  // Generate a keypair
  ml_kem_768::keygen(_d, _z, _pkey, _skey);

  // Fill up seed required for key encapsulation, using PRNG
  prng.read(_m);

  // Encapsulate key, compute cipher text and obtain KDF
  const bool is_encapsulated = ml_kem_768::encapsulate(_m, _pkey, _cipher, _shrd_key0);
  // Decapsulate cipher text and obtain KDF
  ml_kem_768::decapsulate(_skey, _cipher, _shrd_key1);

  // Check that both of the communicating parties arrived at same shared secret key
  assert(std::ranges::equal(_shrd_key0, _shrd_key1));

  std::cout << "ML-KEM-768\n";
  std::cout << "Pubkey         : " << to_hex(_pkey) << "\n";
  std::cout << "Seckey         : " << to_hex(_skey) << "\n";
  std::cout << "Encapsulated ? : " << std::boolalpha << is_encapsulated << "\n";
  std::cout << "Cipher         : " << to_hex(_cipher) << "\n";
  std::cout << "Shared secret  : " << to_hex(_shrd_key0) << "\n";

  return EXIT_SUCCESS;
}
