#pragma once
#include "subtle.hpp"
#include <span>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given two byte arrays of equal length, this routine can be used for comparing them in constant-time,
// producing truth value (0xffffffff) in case of equality, otherwise it returns false value (0x00000000).
template<size_t n>
static inline constexpr uint32_t
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
static inline constexpr void
ct_cond_memcpy(const uint32_t cond, std::span<uint8_t, n> sink, std::span<const uint8_t, n> source0, std::span<const uint8_t, n> source1)
{
  for (size_t i = 0; i < n; i++) {
    sink[i] = subtle::ct_select(cond, source0[i], source1[i]);
  }
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
get_kem_cipher_text_len(size_t k, size_t du, size_t dv)
{
  return k * du * 32 + dv * 32;
}

}
