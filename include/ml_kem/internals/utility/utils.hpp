#pragma once
#include "ml_kem/internals/utility/force_inline.hpp"
#include "subtle.hpp"
#include <span>

namespace ml_kem_utils {

// Given two byte arrays of equal length, this routine can be used for comparing them in constant-time,
// producing truth value (0xffffffff) in case of equality, otherwise it returns false value (0x00000000).
template<size_t n>
forceinline constexpr uint32_t
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
// to `sink` byte array, all in constant-time.
//
// In simple words, `sink = cond ? source0 ? source1`
template<size_t n>
forceinline constexpr void
ct_cond_memcpy(const uint32_t cond, std::span<uint8_t, n> sink, std::span<const uint8_t, n> source0, std::span<const uint8_t, n> source1)
{
  for (size_t i = 0; i < n; i++) {
    sink[i] = subtle::ct_select(cond, source0[i], source1[i]);
  }
}

// Returns compile-time computable K-PKE public key byte length.
forceinline constexpr size_t
get_pke_public_key_len(const size_t k)
{
  return k * 12 * 32 + 32;
}

// Returns compile-time computable K-PKE secret key byte length.
forceinline constexpr size_t
get_pke_secret_key_len(const size_t k)
{
  return k * 12 * 32;
}

// Returns compile-time computable K-PKE cipher text byte length.
forceinline constexpr size_t
get_pke_cipher_text_len(size_t k, size_t du, size_t dv)
{
  return 32 * (k * du + dv);
}

// Returns compile-time computable ML-KEM public key byte length.
forceinline constexpr size_t
get_kem_public_key_len(const size_t k)
{
  return get_pke_public_key_len(k);
}

// Returns compile-time computable ML-KEM secret key byte length.
forceinline constexpr size_t
get_kem_secret_key_len(const size_t k)
{
  return get_pke_secret_key_len(k) + get_pke_public_key_len(k) + 32 + 32;
}

// Returns compile-time computable ML-KEM cipher text byte length.
forceinline constexpr size_t
get_kem_cipher_text_len(size_t k, size_t du, size_t dv)
{
  return get_pke_cipher_text_len(k, du, dv);
}

}
