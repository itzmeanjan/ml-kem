#pragma once
#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/compression.hpp"
#include "ml_kem/internals/poly/ntt.hpp"
#include "ml_kem/internals/poly/serialize.hpp"
#include "ml_kem/internals/utility/params.hpp"

namespace ml_kem_utils {

// Given two matrices ( in NTT domain ) of compatible dimension, where each matrix element is a degree-255 polynomial over Z_q | q = 3329,
// this routine multiplies them, computing a resulting matrix.
template<size_t a_rows, size_t a_cols, size_t b_rows, size_t b_cols>
constexpr void
matrix_multiply(std::span<const ml_kem_field::zq_t, a_rows * a_cols * ml_kem_ntt::N> a,
                std::span<const ml_kem_field::zq_t, b_rows * b_cols * ml_kem_ntt::N> b,
                std::span<ml_kem_field::zq_t, a_rows * b_cols * ml_kem_ntt::N> c)
  requires(ml_kem_params::check_matrix_dim(a_cols, b_rows))
{
  using poly_t = std::span<const ml_kem_field::zq_t, ml_kem_ntt::N>;

  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> tmp{};
  auto tmp_span = std::span(tmp);

  for (size_t i = 0; i < a_rows; i++) {
    for (size_t j = 0; j < b_cols; j++) {
      const size_t coff = (i * b_cols + j) * ml_kem_ntt::N;

      for (size_t k = 0; k < a_cols; k++) {
        const size_t aoff = (i * a_cols + k) * ml_kem_ntt::N;
        const size_t boff = (k * b_cols + j) * ml_kem_ntt::N;

        ml_kem_ntt::polymul(poly_t(a.subspan(aoff, ml_kem_ntt::N)), poly_t(b.subspan(boff, ml_kem_ntt::N)), tmp_span);

        for (size_t l = 0; l < ml_kem_ntt::N; l++) {
          c[coff + l] += tmp[l];
        }
      }
    }
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials ( where polynomial coefficients are in non-NTT form ),
// this routine applies in-place polynomial NTT over `k` polynomials.
template<size_t k>
constexpr void
poly_vec_ntt(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec)
  requires((k == 1) || ml_kem_params::check_k(k))
{
  using poly_t = std::span<ml_kem_field::zq_t, ml_kem_ntt::N>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ml_kem_ntt::N;
    ml_kem_ntt::ntt(poly_t(vec.subspan(off, ml_kem_ntt::N)));
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials ( where polynomial coefficients are in NTT form i.e.
// they are placed in bit-reversed order ), this routine applies in-place polynomial iNTT over those `k` polynomials.
template<size_t k>
constexpr void
poly_vec_intt(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec)
  requires((k == 1) || ml_kem_params::check_k(k))
{
  using poly_t = std::span<ml_kem_field::zq_t, ml_kem_ntt::N>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ml_kem_ntt::N;
    ml_kem_ntt::intt(poly_t(vec.subspan(off, ml_kem_ntt::N)));
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials, this routine adds it to another polynomial vector of same dimension.
template<size_t k>
constexpr void
poly_vec_add_to(std::span<const ml_kem_field::zq_t, k * ml_kem_ntt::N> src, std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> dst)
  requires((k == 1) || ml_kem_params::check_k(k))
{
  constexpr size_t cnt = k * ml_kem_ntt::N;

  for (size_t i = 0; i < cnt; i++) {
    dst[i] += src[i];
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials, this routine subtracts it to another polynomial vector of same dimension.
template<size_t k>
constexpr void
poly_vec_sub_from(std::span<const ml_kem_field::zq_t, k * ml_kem_ntt::N> src, std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> dst)
  requires((k == 1) || ml_kem_params::check_k(k))
{
  constexpr size_t cnt = k * ml_kem_ntt::N;

  for (size_t i = 0; i < cnt; i++) {
    dst[i] -= src[i];
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials, this routine encodes each of those polynomials into 32 x l -bytes,
// writing to a (k x 32 x l) -bytes destination array.
template<size_t k, size_t l>
constexpr void
poly_vec_encode(std::span<const ml_kem_field::zq_t, k * ml_kem_ntt::N> src, std::span<uint8_t, k * 32 * l> dst)
  requires(ml_kem_params::check_k(k))
{
  using poly_t = std::span<const ml_kem_field::zq_t, src.size() / k>;
  using serialized_t = std::span<uint8_t, dst.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * ml_kem_ntt::N;
    const size_t off1 = i * l * 32;

    ml_kem_utils::encode<l>(poly_t(src.subspan(off0, ml_kem_ntt::N)), serialized_t(dst.subspan(off1, 32 * l)));
  }
}

// Given a byte array of length (k x 32 x l) -bytes, this routine decodes them into k degree-255 polynomials, writing them to a
// column vector of dimension `k x 1`.
template<size_t k, size_t l>
constexpr void
poly_vec_decode(std::span<const uint8_t, k * 32 * l> src, std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> dst)
  requires(ml_kem_params::check_k(k))
{
  using serialized_t = std::span<const uint8_t, src.size() / k>;
  using poly_t = std::span<ml_kem_field::zq_t, dst.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off0 = i * l * 32;
    const size_t off1 = i * ml_kem_ntt::N;

    ml_kem_utils::decode<l>(serialized_t(src.subspan(off0, 32 * l)), poly_t(dst.subspan(off1, ml_kem_ntt::N)));
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials, each of k * 256 coefficients are compressed, while mutating input.
template<size_t k, size_t d>
constexpr void
poly_vec_compress(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec)
  requires(ml_kem_params::check_k(k))
{
  using poly_t = std::span<ml_kem_field::zq_t, vec.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ml_kem_ntt::N;
    ml_kem_utils::poly_compress<d>(poly_t(vec.subspan(off, ml_kem_ntt::N)));
  }
}

// Given a vector ( of dimension `k x 1` ) of degree-255 polynomials, each of k * 256 coefficients are decompressed, while mutating input.
template<size_t k, size_t d>
constexpr void
poly_vec_decompress(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec)
  requires(ml_kem_params::check_k(k))
{
  using poly_t = std::span<ml_kem_field::zq_t, vec.size() / k>;

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ml_kem_ntt::N;
    ml_kem_utils::poly_decompress<d>(poly_t(vec.subspan(off, ml_kem_ntt::N)));
  }
}

}
