#pragma once
#include "ff.hpp"
#include <cstring>

// (inverse) Number Theoretic Transform for degree 255 polynomial, over Kyber
// Prime Field F_q | q = 3329
namespace ntt {

constexpr size_t LOG2N = 8;
constexpr size_t N = 1 << LOG2N;

// First primitive 256 -th root of unity modulo q | q = 3329
//
// Meaning, 17 ** 256 == 1 mod q
constexpr ff::ff_t ζ{ 17 };

// Multiplicative inverse of N/ 2 over F_q | q = 3329 and N = 256
constexpr auto INV_N = ff::ff_t{ N >> 1 }.inv();

// Given a 64 -bit unsigned integer, this routine extracts specified many
// contiguous bits from ( least significant bits ) LSB side & reverses their bit
// order, returning bit reversed `mbw` -bit wide number
//
// See
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L30-L38
// for source of inspiration
template<const size_t mbw>
size_t
bit_rev(const size_t v)
{
  size_t v_rev = 0ul;

  for (size_t i = 0; i < mbw; i++) {
    const size_t bit = (v >> i) & 0b1;
    v_rev ^= bit << (mbw - 1ul - i);
  }

  return v_rev;
}

// Given a polynomial f with 256 coefficients over F_q | q = 3329, this routine
// computes number theoretic transform using cooley-tukey algorithm, producing
// polynomial f' s.t. its coefficients are placed in bit-reversed order
//
// Implementation inspired from
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L69-L144
static void
ntt(const ff::ff_t* const __restrict src, // polynomial f with 256 coefficients
    ff::ff_t* const __restrict dst        // polynomial f' with 256 coefficients
)
{
  std::memcpy(dst, src, N * sizeof(ff::ff_t));

  size_t k = 1;

  for (size_t len = (N >> 1); len >= 2; len >>= 1) {

    for (size_t start = 0; start < N; start += (len << 1)) {
      const auto ζ_exp = ζ ^ ntt::bit_rev<LOG2N - 1>(k);

      for (size_t i = start; i < start + len; i++) {
        const auto a = dst[i];
        const auto b = dst[i + len];

        const auto tmp = ζ_exp * b;

        dst[i] = a + tmp;
        dst[i + len] = a - tmp;
      }

      k += 1;
    }
  }
}

// Given a polynomial f with 256 coefficients over F_q | q = 3329, s.t. its
// coefficients are placed in bit-reversed order, this routine computes inverse
// number theoretic transform using cooley-tukey algorithm, producing polynomial
// f' s.t. its coefficients are placed in standard order
//
// Implementation inspired from
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L146-L224
static void
intt(const ff::ff_t* const __restrict src, // polynomial f with 256 coefficients
     ff::ff_t* const __restrict dst // polynomial f' with 256 coefficients
)
{
  std::memcpy(dst, src, N * sizeof(ff::ff_t));

  size_t k = 127;

  for (size_t len = 2; len <= (N >> 1); len <<= 1) {

    for (size_t start = 0; start < N; start += (len << 1)) {
      const ff::ff_t neg_ζ_exp = -(ζ ^ ntt::bit_rev<LOG2N - 1>(k));

      for (size_t i = start; i < start + len; i++) {
        const auto tmp = dst[i];

        dst[i] = tmp + dst[i + len];
        dst[i + len] = (tmp - dst[i + len]) * neg_ζ_exp;
      }

      k -= 1;
    }
  }

  for (size_t i = 0; i < N; i++) {
    dst[i] = dst[i] * INV_N;
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
// See page 6 of Kyber specification as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
static void
basemul(const ff::ff_t* const __restrict f, // degree-1 polynomial
        const ff::ff_t* const __restrict g, // degree-1 polynomial
        ff::ff_t* const __restrict h,       // degree-1 polynomial
        const ff::ff_t ζ                    // zeta
)
{
  h[0] = f[1] * g[1] * ζ + f[0] * g[0];
  h[1] = f[0] * g[1] + f[1] * g[0];
}

// Given two degree-255 polynomials in NTT form, this routine performs 128
// basecase multiplications for 128 pairs of degree-1 polynomials s.t.
//
// f = (f0ˆ + f1ˆX, f2ˆ + f3ˆX, ..., f254ˆ + f255ˆX)
// g = (g0ˆ + g1ˆX, g2ˆ + g3ˆX, ..., g254ˆ + g255ˆX)
//
// h = f ◦ g
static void
polymul(const ff::ff_t* const __restrict f, // degree-255 polynomial
        const ff::ff_t* const __restrict g, // degree-255 polynomial
        ff::ff_t* const __restrict h        // degree-255 polynomial
)
{
  constexpr size_t cnt = N >> 1;

  for (size_t i = 0; i < cnt; i++) {
    const size_t off = i << 1;
    const ff::ff_t ζ_exp = ζ ^ ((bit_rev<LOG2N - 1>(i) << 1) ^ 1);

    basemul(f + off, g + off, h + off, ζ_exp);
  }
}

}
