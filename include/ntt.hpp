#pragma once
#include "field.hpp"
#include <array>
#include <cstring>

// (inverse) Number Theoretic Transform for degree-255 polynomial, over Kyber
// Prime Field Zq | q = 3329
namespace ntt {

constexpr size_t LOG2N = 8;
constexpr size_t N = 1 << LOG2N;

// First primitive 256 -th root of unity modulo q | q = 3329
//
// Meaning, 17 ** 256 == 1 mod q
constexpr auto ζ = field::zq_t(17);

// Multiplicative inverse of N/ 2 over Z_q | q = 3329 and N = 256
//
// Meaning (N/ 2) * 3303 = 1 mod q
constexpr auto INV_N = field::zq_t(N / 2).inv();

// Given a 64 -bit unsigned integer, this routine extracts specified many
// contiguous bits from ( least significant bits ) LSB side & reverses their bit
// order, returning bit reversed `mbw` -bit wide number
//
// See
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L30-L38
// for source of inspiration
template<size_t mbw>
static inline constexpr size_t
bit_rev(const size_t v)
{
  size_t v_rev = 0ul;

  for (size_t i = 0; i < mbw; i++) {
    const size_t bit = (v >> i) & 0b1;
    v_rev ^= bit << (mbw - 1ul - i);
  }

  return v_rev;
}

// Compile-time compute powers of ζ, used for polynomial evaluation ( NTT )
consteval std::array<field::zq_t, N / 2>
compute_ntt_ζ()
{
  std::array<field::zq_t, N / 2> res;

  for (size_t i = 0; i < N / 2; i++) {
    res[i] = ζ ^ bit_rev<LOG2N - 1>(i);
  }

  return res;
}

// Precomputed constants ( powers of ζ ), used for computing NTT form of
// degree-255 polynomial
constexpr std::array<field::zq_t, N / 2> NTT_ζ_EXP = compute_ntt_ζ();

// Compile-time compute negated powers of ζ, used for polynomial interpolation (
// iNTT )
consteval std::array<field::zq_t, N / 2>
compute_intt_ζ()
{
  std::array<field::zq_t, N / 2> res;

  for (size_t i = 0; i < N / 2; i++) {
    res[i] = -NTT_ζ_EXP[i];
  }

  return res;
}

// Precomputed constants ( negated powers of ζ ), used for computing coefficient
// form of degree-255 polynomial using inverse NTT
constexpr std::array<field::zq_t, N / 2> INTT_ζ_EXP = compute_intt_ζ();

// Compile-time compute powers of ζ, used for multiplication of two degree-255
// polynomials in NTT representation.
consteval std::array<field::zq_t, N / 2>
compute_mul_ζ()
{
  std::array<field::zq_t, N / 2> res;

  for (size_t i = 0; i < N / 2; i++) {
    res[i] = ζ ^ ((bit_rev<LOG2N - 1>(i) << 1) ^ 1);
  }

  return res;
}

// Precomputed constants ( powers of ζ ), used when multiplying two degree-255
// polynomials in NTT domain.
constexpr std::array<field::zq_t, N / 2> POLY_MUL_ζ_EXP = compute_mul_ζ();

// Given a polynomial f with 256 coefficients over F_q | q = 3329, this routine
// computes number theoretic transform using cooley-tukey algorithm, producing
// polynomial f' s.t. its coefficients are placed in bit-reversed order
//
// Note, this routine mutates input i.e. it's an in-place NTT implementation.
//
// Implementation inspired from
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L69-L144
static inline constexpr void
ntt(std::span<field::zq_t, N> poly)
{
  for (size_t l = LOG2N - 1; l >= 1; l--) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = N >> (l + 1);

    for (size_t start = 0; start < poly.size(); start += lenx2) {
      const size_t k_now = k_beg + (start >> (l + 1));
      // Looking up precomputed constant, though it can be computed using
      //
      // ζ ^ bit_rev<LOG2N - 1>(k_now)
      //
      // This is how these constants are generated !
      const field::zq_t ζ_exp = NTT_ζ_EXP[k_now];

      for (size_t i = start; i < start + len; i++) {
        auto tmp = ζ_exp;
        tmp *= poly[i + len];

        poly[i + len] = poly[i] - tmp;
        poly[i] += tmp;
      }
    }
  }
}

// Given a polynomial f with 256 coefficients over F_q | q = 3329, s.t. its
// coefficients are placed in bit-reversed order, this routine computes inverse
// number theoretic transform using cooley-tukey algorithm, producing polynomial
// f' s.t. its coefficients are placed in standard order
//
// Note, this routine mutates input i.e. it's an in-place iNTT implementation.
//
// Implementation inspired from
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L146-L224
static inline constexpr void
intt(std::span<field::zq_t, N> poly)
{
  for (size_t l = 1; l < LOG2N; l++) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = (N >> l) - 1;

    for (size_t start = 0; start < poly.size(); start += lenx2) {
      const size_t k_now = k_beg - (start >> (l + 1));
      // Looking up precomputed constant, though it can be computed using
      //
      // -(ζ ^ bit_rev<LOG2N - 1>(k_now))
      //
      // Or simpler
      //
      // -NTT_ζ_EXP[k_now]
      const field::zq_t neg_ζ_exp = INTT_ζ_EXP[k_now];

      for (size_t i = start; i < start + len; i++) {
        const auto tmp = poly[i];

        poly[i] += poly[i + len];
        poly[i + len] = tmp - poly[i + len];
        poly[i + len] *= neg_ζ_exp;
      }
    }
  }

  for (size_t i = 0; i < poly.size(); i++) {
    poly[i] *= INV_N;
  }
}

// Given two degree-1 polynomials s.t.
//
// f = f_2i + f_(2i + 1) * X
// g = g_2i + g_(2i + 1) * X
//
// this routine computes resulting degree-1 polynomial h s.t.
//
// h = f * g mod X ^ 2 − ζ ^ (2 * br<7>(i) + 1) | i ∈ [0, 128)
//
// See page 6 of Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
static inline constexpr void
basemul(std::span<const field::zq_t, 2> f, // degree-1 polynomial
        std::span<const field::zq_t, 2> g, // degree-1 polynomial
        std::span<field::zq_t, 2> h,       // degree-1 polynomial
        const field::zq_t ζ                // zeta
)
{
  field::zq_t f0 = f[0];
  field::zq_t f1 = f[1];

  f0 *= g[0];
  f1 *= g[1];
  f1 *= ζ;
  f1 += f0;

  h[0] = f1;

  field::zq_t g0 = g[0];
  field::zq_t g1 = g[1];

  g1 *= f[0];
  g0 *= f[1];
  g1 += g0;

  h[1] = g1;
}

// Given two degree-255 polynomials in NTT form, this routine performs 128
// basecase multiplications for 128 pairs of degree-1 polynomials s.t.
//
// f = (f0ˆ + f1ˆX, f2ˆ + f3ˆX, ..., f254ˆ + f255ˆX)
// g = (g0ˆ + g1ˆX, g2ˆ + g3ˆX, ..., g254ˆ + g255ˆX)
//
// h = f ◦ g
static inline constexpr void
polymul(std::span<const field::zq_t, N> f, // degree-255 polynomial
        std::span<const field::zq_t, N> g, // degree-255 polynomial
        std::span<field::zq_t, N> h        // degree-255 polynomial
)
{
  constexpr size_t cnt = f.size() >> 1;

  using poly_t = std::span<const field::zq_t, 2>;
  using mut_poly_t = std::span<field::zq_t, 2>;

  for (size_t i = 0; i < cnt; i++) {
    const size_t off = i << 1;
    basemul(poly_t(f.subspan(off, 2)), poly_t(g.subspan(off, 2)), mut_poly_t(h.subspan(off, 2)), POLY_MUL_ζ_EXP[i]);
  }
}

}
