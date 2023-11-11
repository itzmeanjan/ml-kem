#pragma once
#include "compression.hpp"
#include "field.hpp"
#include "ntt.hpp"
#include "params.hpp"
#include "serialize.hpp"
#include <array>
#include <cstdint>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given two matrices ( in NTT domain ) of compatible dimension, where each
// matrix element is a degree-255 polynomial over Z_q | q = 3329, this routine
// attempts to multiply and compute resulting matrix
template<size_t a_rows, size_t a_cols, size_t b_rows, size_t b_cols>
static inline constexpr void
matrix_multiply(std::span<const field::zq_t, a_rows * a_cols * ntt::N> a,
                std::span<const field::zq_t, b_rows * b_cols * ntt::N> b,
                std::span<field::zq_t, a_rows * b_cols * ntt::N> c)
  requires(kyber_params::check_matrix_dim(a_cols, b_rows))
{
  using poly_t = std::span<const field::zq_t, ntt::N>;

  std::array<field::zq_t, ntt::N> tmp{};
  auto _tmp = std::span(tmp);

  for (size_t i = 0; i < a_rows; i++) {
    for (size_t j = 0; j < b_cols; j++) {
      const size_t coff = (i * b_cols + j) * ntt::N;

      for (size_t k = 0; k < a_cols; k++) {
        const size_t aoff = (i * a_cols + k) * ntt::N;
        const size_t boff = (k * b_cols + j) * ntt::N;

        ntt::polymul(poly_t(a.subspan(aoff, ntt::N)), poly_t(b.subspan(boff, ntt::N)), _tmp);

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
template<size_t k>
static inline constexpr void
poly_vec_ntt(std::span<field::zq_t, k * ntt::N> vec)
  requires((k == 1) || kyber_params::check_k(k))
{
  using poly_t = std::span<field::zq_t, ntt::N>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    ntt::ntt(poly_t(vec.subspan(off, ntt::N)));
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials ( where
// polynomial coefficients are in NTT form i.e. they are placed in bit-reversed
// order ), this routine applies in-place polynomial iNTT over those k
// polynomials
template<size_t k>
static inline constexpr void
poly_vec_intt(std::span<field::zq_t, k * ntt::N> vec)
  requires((k == 1) || kyber_params::check_k(k))
{
  using poly_t = std::span<field::zq_t, ntt::N>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    ntt::intt(poly_t(vec.subspan(off, ntt::N)));
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, this
// routine adds it to another polynomial vector of same dimension
template<size_t k>
static inline constexpr void
poly_vec_add_to(std::span<const field::zq_t, k * ntt::N> src, std::span<field::zq_t, k * ntt::N> dst)
  requires((k == 1) || kyber_params::check_k(k))
{
  constexpr size_t cnt = k * ntt::N;

  for (size_t i = 0; i < cnt; i++) {
    dst[i] += src[i];
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, this
// routine subtracts it to another polynomial vector of same dimension
template<size_t k>
static inline constexpr void
poly_vec_sub_from(std::span<const field::zq_t, k * ntt::N> src, std::span<field::zq_t, k * ntt::N> dst)
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
template<size_t k, size_t l>
static inline void
poly_vec_encode(std::span<const field::zq_t, k * ntt::N> src, std::span<uint8_t, k * 32 * l> dst)
  requires(kyber_params::check_k(k))
{
  using poly_t = std::span<const field::zq_t, src.size() / k>;
  using serialized_t = std::span<uint8_t, dst.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * ntt::N;
    const size_t off1 = i * l * 32;

    kyber_utils::encode<l>(poly_t(src.subspan(off0, ntt::N)), serialized_t(dst.subspan(off1, 32 * l)));
  }
}

// Given a byte array of length (k x 32 x l) -bytes, this routine decodes them
// into k degree-255 polynomials, writing them to a column vector of dimension
// k x 1
template<size_t k, size_t l>
static inline void
poly_vec_decode(std::span<const uint8_t, k * 32 * l> src, std::span<field::zq_t, k * ntt::N> dst)
  requires(kyber_params::check_k(k))
{
  using serialized_t = std::span<const uint8_t, src.size() / k>;
  using poly_t = std::span<field::zq_t, dst.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * l * 32;
    const size_t off1 = i * ntt::N;

    kyber_utils::decode<l>(serialized_t(src.subspan(off0, 32 * l)), poly_t(dst.subspan(off1, ntt::N)));
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, each of
// k * 256 coefficients are compressed, while mutating input.
template<size_t k, size_t d>
static inline constexpr void
poly_vec_compress(std::span<field::zq_t, k * ntt::N> vec)
  requires(kyber_params::check_k(k))
{
  using poly_t = std::span<field::zq_t, vec.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    kyber_utils::poly_compress<d>(poly_t(vec.subspan(off, ntt::N)));
  }
}

// Given a vector ( of dimension k x 1 ) of degree-255 polynomials, each of
// k * 256 coefficients are decompressed, while mutating input.
template<size_t k, size_t d>
static inline constexpr void
poly_vec_decompress(std::span<field::zq_t, k * ntt::N> vec)
  requires(kyber_params::check_k(k))
{
  using poly_t = std::span<field::zq_t, vec.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    kyber_utils::poly_decompress<d>(poly_t(vec.subspan(off, ntt::N)));
  }
}

}
