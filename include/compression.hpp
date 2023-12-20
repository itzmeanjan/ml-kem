#pragma once
#include "field.hpp"
#include "ntt.hpp"
#include "params.hpp"
#include <span>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given an element x ∈ Z_q | q = 3329, this routine compresses it by discarding
// some low-order bits, computing y ∈ [0, 2^d) | d < round(log2(q))
//
// See top of page 5 of Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
//
// Following implementation collects inspiration from https://github.com/FiloSottile/mlkem768/blob/cffbfb96c407b3cfc9f6e1749475b673794402c1/mlkem768.go#L395-L425.
template<size_t d>
static inline constexpr field::zq_t
compress(const field::zq_t x)
  requires(kyber_params::check_d(d))
{
  constexpr uint16_t mask = (1u << d) - 1;

  const auto dividend = x.raw() << d;
  const auto quotient0 = static_cast<uint32_t>((static_cast<uint64_t>(dividend) * field::R) >> (field::RADIX_BIT_WIDTH * 2));
  const auto remainder = dividend - quotient0 * field::Q;

  const auto quotient1 = quotient0 + ((((field::Q / 2) - remainder) >> 31) & 1);
  const auto quotient2 = quotient1 + (((field::Q + (field::Q / 2) - remainder) >> 31) & 1);

  return field::zq_t(static_cast<uint16_t>(quotient2) & mask);
}

// Given an element x ∈ [0, 2^d) | d < round(log2(q)), this routine decompresses
// it back to y ∈ Z_q | q = 3329
//
// This routine recovers the compressed element with error probability as
// defined in eq. 2 of Kyber specification.
//
// See top of page 5 of Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t d>
static inline constexpr field::zq_t
decompress(const field::zq_t x)
  requires(kyber_params::check_d(d))
{
  constexpr uint32_t t0 = 1u << d;
  constexpr uint32_t t1 = t0 >> 1;

  const uint32_t t2 = field::Q * x.raw();
  const uint32_t t3 = t2 + t1;
  const uint16_t t4 = static_cast<uint16_t>(t3 >> d);

  return field::zq_t(t4);
}

// Utility function to compress each of 256 coefficients of a degree-255
// polynomial s.t. input polynomial is mutated.
template<size_t d>
static inline constexpr void
poly_compress(std::span<field::zq_t, ntt::N> poly)
  requires(kyber_params::check_d(d))
{
  for (size_t i = 0; i < poly.size(); i++) {
    poly[i] = compress<d>(poly[i]);
  }
}

// Utility function to decompress each of 256 coefficients of a degree-255
// polynomial s.t. input polynomial is mutated.
template<size_t d>
static inline constexpr void
poly_decompress(std::span<field::zq_t, ntt::N> poly)
  requires(kyber_params::check_d(d))
{
  for (size_t i = 0; i < poly.size(); i++) {
    poly[i] = decompress<d>(poly[i]);
  }
}

}
