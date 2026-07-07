#pragma once
#include "ml_kem/internals/utility/force_inline.hpp"
#include <cstddef>

namespace ml_kem_utils {

// Returns compile-time computable K-PKE public key byte length.
forceinline constexpr size_t
get_pke_public_key_len(const size_t k)
{
  return (k * 12 * 32) + 32;
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
