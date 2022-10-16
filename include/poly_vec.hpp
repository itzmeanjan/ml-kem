#pragma once
#include "ntt.hpp"

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Compile-time check to ensure that operand matrices are having compatible
// dimension for matrix multiplication
static inline constexpr bool
check_matrix_dim(const size_t a_cols, const size_t b_rows)
{
  return !static_cast<bool>(a_cols ^ b_rows);
}

// Given two matrices ( in NTT domain ) of compatible dimension, where each
// matrix element is a degree-255 polynomial over Z_q | q = 3329, this routine
// attempts to multiply and compute resulting matrix
template<const size_t a_rows,
         const size_t a_cols,
         const size_t b_rows,
         const size_t b_cols>
static void
matrix_multiply(const ff::ff_t* const __restrict a,
                const ff::ff_t* const __restrict b,
                ff::ff_t* const __restrict c) requires(check_matrix_dim(a_cols,
                                                                        b_rows))
{
  ff::ff_t tmp[ntt::N]{};

  for (size_t i = 0; i < a_rows; i++) {
    for (size_t j = 0; j < b_cols; j++) {
      const size_t coff = (i * b_cols + j) * ntt::N;

      for (size_t k = 0; k < a_cols; k++) {
        const size_t aoff = (i * a_cols + k) * ntt::N;
        const size_t boff = (k * b_cols + j) * ntt::N;

        ntt::polymul(a + aoff, b + boff, tmp);

        for (size_t l = 0; l < ntt::N; l++) {
          c[coff + l] += tmp[l];
        }
      }
    }
  }
}

}
