#pragma once
#include "compression.hpp"
#include "params.hpp"
#include "serialize.hpp"

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given two matrices ( in NTT domain ) of compatible dimension, where each
// matrix element is a degree-255 polynomial over Z_q | q = 3329, this routine
// attempts to multiply and compute resulting matrix
template<const size_t a_rows,
         const size_t a_cols,
         const size_t b_rows,
         const size_t b_cols>
static inline void
matrix_multiply(const field::zq_t* const __restrict a,
                const field::zq_t* const __restrict b,
                field::zq_t* const __restrict c)
  requires(kyber_params::check_matrix_dim(a_cols, b_rows))
{
  field::zq_t tmp[ntt::N]{};

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

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials ( where
// polynomial coefficients are in non-NTT form ), this routine applies in-place
// polynomial NTT over k polynomials
template<const size_t k>
static inline void
poly_vec_ntt(field::zq_t* const __restrict vec)
  requires((k == 1) || kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    ntt::ntt(vec + off);
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials ( where
// polynomial coefficients are in NTT form i.e. they are placed in bit-reversed
// order ), this routine applies in-place polynomial iNTT over those k
// polynomials
template<const size_t k>
static inline void
poly_vec_intt(field::zq_t* const __restrict vec)
  requires((k == 1) || kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    ntt::intt(vec + off);
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, this
// routine adds it to another polynomial vector of same dimension
template<const size_t k>
static inline void
poly_vec_add_to(const field::zq_t* const __restrict src,
                field::zq_t* const __restrict dst)
  requires((k == 1) || kyber_params::check_k(k))
{
  constexpr size_t cnt = k * ntt::N;

  for (size_t i = 0; i < cnt; i++) {
    dst[i] += src[i];
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, this
// routine subtracts it to another polynomial vector of same dimension
template<const size_t k>
static inline void
poly_vec_sub_from(const field::zq_t* const __restrict src,
                  field::zq_t* const __restrict dst)
  requires((k == 1) || kyber_params::check_k(k))
{
  constexpr size_t cnt = k * ntt::N;

  for (size_t i = 0; i < cnt; i++) {
    dst[i] -= src[i];
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, this routine
// encodes each of those polynomials into 32 x l -bytes, writing to a
// (k x 32 x l) -bytes destination array
template<const size_t k, const size_t l>
static inline void
poly_vec_encode(const field::zq_t* const __restrict src,
                uint8_t* const __restrict dst)
  requires(kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * ntt::N;
    const size_t off1 = i * l * 32;

    kyber_utils::encode<l>(src + off0, dst + off1);
  }
}

// Given a byte array of length (k x 32 x l) -bytes, this routine decodes them
// into k degree-255 polynomials, writing them to a column vector of dimension
// k x 1
template<const size_t k, const size_t l>
static inline void
poly_vec_decode(const uint8_t* const __restrict src,
                field::zq_t* const __restrict dst)
  requires(kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * l * 32;
    const size_t off1 = i * ntt::N;

    kyber_utils::decode<l>(src + off0, dst + off1);
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, each of
// k * 256 coefficients are compressed, while mutating input
template<const size_t k, const size_t d>
static inline void
poly_vec_compress(field::zq_t* const __restrict vec)
  requires(kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    kyber_utils::poly_compress<d>(vec + off);
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, each of
// k * 256 coefficients are decompressed, while mutating input
template<const size_t k, const size_t d>
static inline void
poly_vec_decompress(field::zq_t* const __restrict vec)
  requires(kyber_params::check_k(k))
{
  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    kyber_utils::poly_decompress<d>(vec + off);
  }
}

}
