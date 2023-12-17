#pragma once
#include "params.hpp"
#include <array>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <span>
#include <sstream>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
inline const std::string
to_hex(std::span<const uint8_t> bytes)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < bytes.size(); i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Given a hex encoded string of length 2*L, this routine can be used for
// parsing it as a byte array of length L.
template<size_t L>
inline std::array<uint8_t, L>
from_hex(std::string_view bytes)
{
  const size_t blen = bytes.length();

  assert(blen % 2 == 0);
  assert(blen / 2 == L);

  std::array<uint8_t, L> res{};

  for (size_t i = 0; i < L; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = bytes.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
}

// Compile-time compute IND-CCA-secure Kyber KEM public key length ( in bytes )
static inline constexpr size_t
get_kem_public_key_len(const size_t k)
{
  return k * 12 * 32 + 32;
}

// Compile-time compute IND-CCA-secure Kyber KEM secret key length ( in bytes )
static inline constexpr size_t
get_kem_secret_key_len(const size_t k)
{
  const size_t t0 = k * 12 * 32;
  const size_t t1 = get_kem_public_key_len(k);

  return t0 + t1 + 32 + 32;
}

// Compile-time compute IND-CCA-secure Kyber KEM cipher text length ( in bytes )
static inline constexpr size_t
get_kem_cipher_len(size_t k, size_t du, size_t dv)
{
  return k * du * 32 + dv * 32;
}

}
