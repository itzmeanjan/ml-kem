#pragma once
#include "params.hpp"
#include "subtle.hpp"
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

// Given two byte arrays of equal length, this routine can be used for comparing them in constant-time,
// producing truth value (0xffffffff) in case of equality, otherwise it returns false value (0x00000000).
template<size_t n>
static inline uint32_t
ct_memcmp(std::span<const uint8_t, n> bytes0, std::span<const uint8_t, n> bytes1)
{
  uint32_t flag = -1u;
  for (size_t i = 0; i < n; i++) {
    flag &= subtle::ct_eq<uint8_t, uint32_t>(bytes0[i], bytes1[i]);
  }

  return flag;
}

// Given a branch value, taking either 0x00000000 (false value) or 0xffffffff (truth value), this routine can be used for conditionally
// copying bytes from either `source0` byte array (in case branch holds truth value) or `source1` byte array (if branch holds false value)
// to `sink` byte array, all in constant-time. Note, all these byte arrays are of equal length.
template<size_t n>
static inline void
ct_cond_memcpy(const uint32_t cond, std::span<uint8_t, n> sink, std::span<const uint8_t, n> source0, std::span<const uint8_t, n> source1)
{
  for (size_t i = 0; i < n; i++) {
    sink[i] = subtle::ct_select(cond, source0[i], source1[i]);
  }
}

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
