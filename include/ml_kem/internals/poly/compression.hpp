#pragma once
#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/ntt.hpp"
#include "ml_kem/internals/utility/force_inline.hpp"
#include "ml_kem/internals/utility/params.hpp"
#include <span>

namespace ml_kem_utils {

// Given an element x ∈ Z_q | q = 3329, this routine compresses it by discarding some low-order bits, computing y ∈ [0, 2^d) | d < round(log2(q)).
//
// See formula 4.7 on page 21 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
// Following implementation collects inspiration from https://github.com/FiloSottile/mlkem768/blob/cffbfb96/mlkem768.go#L395-L425.
template<size_t d>
forceinline constexpr ml_kem_field::zq_t
compress(const ml_kem_field::zq_t x)
  requires(ml_kem_params::check_d(d))
{
  constexpr uint16_t mask = (1u << d) - 1;

  const auto dividend = x.raw() << d;
  const auto quotient0 = static_cast<uint32_t>((static_cast<uint64_t>(dividend) * ml_kem_field::R) >> (ml_kem_field::Q_BIT_WIDTH * 2));
  const auto remainder = dividend - quotient0 * ml_kem_field::Q;

  const auto quotient1 = quotient0 + ((((ml_kem_field::Q / 2) - remainder) >> 31) & 1);
  const auto quotient2 = quotient1 + (((ml_kem_field::Q + (ml_kem_field::Q / 2) - remainder) >> 31) & 1);

  return ml_kem_field::zq_t(static_cast<uint16_t>(quotient2) & mask);
}

// Given an element x ∈ [0, 2^d) | d < round(log2(q)), this routine decompresses it back to y ∈ Z_q | q = 3329.
//
// See formula 4.8 on page 21 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t d>
forceinline constexpr ml_kem_field::zq_t
decompress(const ml_kem_field::zq_t x)
  requires(ml_kem_params::check_d(d))
{
  constexpr uint32_t t0 = 1u << d;
  constexpr uint32_t t1 = t0 >> 1;

  const uint32_t t2 = ml_kem_field::Q * x.raw();
  const uint32_t t3 = t2 + t1;
  const uint16_t t4 = static_cast<uint16_t>(t3 >> d);

  return ml_kem_field::zq_t(t4);
}

// Utility function to compress each of 256 coefficients of a degree-255 polynomial while mutating the input.
template<size_t d>
constexpr void
poly_compress(std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly)
  requires(ml_kem_params::check_d(d))
{
  for (size_t i = 0; i < poly.size(); i++) {
    poly[i] = compress<d>(poly[i]);
  }
}

// Utility function to decompress each of 256 coefficients of a degree-255 polynomial while mutating the input.
template<size_t d>
constexpr void
poly_decompress(std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly)
  requires(ml_kem_params::check_d(d))
{
  for (size_t i = 0; i < poly.size(); i++) {
    poly[i] = decompress<d>(poly[i]);
  }
}

}
