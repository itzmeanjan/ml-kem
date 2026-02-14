#include "ml_kem/ml_kem_768.hpp"
#include "randomshake/randomshake.hpp" // IWYU pragma: keep
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Given a bytearray of length N, this function converts it to human readable hex formatted string of length 2*N | N >= 0.
inline std::string
to_hex(std::span<const uint8_t> bytes)
{
  std::stringstream ss;
  ss << std::hex;

  for (const auto byte : bytes) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(byte);
  }

  return ss.str();
}

}

// Build with CMake, from this directory:
//   cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
// Or from the repository root:
//   cmake -B build -DML_KEM_BUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
int
main()
{
  // Seeds required for keypair generation
  std::vector<uint8_t> d(ml_kem_768::SEED_D_BYTE_LEN, 0);
  std::vector<uint8_t> z(ml_kem_768::SEED_Z_BYTE_LEN, 0);

  auto d_span = std::span<uint8_t, ml_kem_768::SEED_D_BYTE_LEN>(d);
  auto z_span = std::span<uint8_t, ml_kem_768::SEED_Z_BYTE_LEN>(z);

  // Public/ private keypair
  std::vector<uint8_t> pkey(ml_kem_768::PKEY_BYTE_LEN, 0);
  std::vector<uint8_t> skey(ml_kem_768::SKEY_BYTE_LEN, 0);

  auto pkey_span = std::span<uint8_t, ml_kem_768::PKEY_BYTE_LEN>(pkey);
  auto skey_span = std::span<uint8_t, ml_kem_768::SKEY_BYTE_LEN>(skey);

  // Seed required for key encapsulation
  std::vector<uint8_t> m(ml_kem_768::SEED_M_BYTE_LEN, 0);
  std::vector<uint8_t> cipher(ml_kem_768::CIPHER_TEXT_BYTE_LEN, 0);

  auto m_span = std::span<uint8_t, ml_kem_768::SEED_M_BYTE_LEN>(m);
  auto cipher_span = std::span<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN>(cipher);

  // Shared secret that sender/ receiver arrives at
  std::vector<uint8_t> sender_key(ml_kem_768::SHARED_SECRET_BYTE_LEN, 0);
  std::vector<uint8_t> receiver_key(ml_kem_768::SHARED_SECRET_BYTE_LEN, 0);

  auto sender_key_span = std::span<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN>(sender_key);
  auto receiver_key_span = std::span<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN>(receiver_key);

  // Pseudo-randomness source
  randomshake::randomshake_t csprng{};

  // Fill up seeds using PRNG
  csprng.generate(d_span);
  csprng.generate(z_span);

  // Generate a keypair
  ml_kem_768::keygen(d_span, z_span, pkey_span, skey_span);

  // Fill up seed required for key encapsulation, using PRNG
  csprng.generate(m_span);

  // Encapsulate key, compute cipher text and obtain KDF
  const bool is_encapsulated = ml_kem_768::encapsulate(m_span, pkey_span, cipher_span, sender_key_span);
  // Decapsulate cipher text and obtain KDF
  ml_kem_768::decapsulate(skey_span, cipher_span, receiver_key_span);

  // Check that both of the communicating parties arrived at same shared secret key
  assert(std::ranges::equal(sender_key_span, receiver_key_span));

  std::cout << "ML-KEM-768\n";
  std::cout << "Pubkey         : " << to_hex(pkey_span) << "\n";
  std::cout << "Seckey         : " << to_hex(skey_span) << "\n";
  std::cout << "Encapsulated ? : " << std::boolalpha << is_encapsulated << "\n";
  std::cout << "Cipher         : " << to_hex(cipher_span) << "\n";
  std::cout << "Shared secret  : " << to_hex(sender_key_span) << "\n";

  return EXIT_SUCCESS;
}
