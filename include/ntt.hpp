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

// Precomputed constants ( powers of ζ ), used when multiplying two degree-255
// polynomials in NTT domain
//
// [ ζ ^ ((br<7>(i) << 1) ^ 1) for i in range(128) ]
//
// s.t. ζ = 17 and br<7>(i) is bit reversal of `i`, while only considering lower
// 7 -bits of `i`.
constexpr ff::ff_t POLY_MUL_ζ_EXP[]{
  17,   3312, 2761, 568,  583,  2746, 2649, 680,  1637, 1692, 723,  2606, 2288,
  1041, 1100, 2229, 1409, 1920, 2662, 667,  3281, 48,   233,  3096, 756,  2573,
  2156, 1173, 3015, 314,  3050, 279,  1703, 1626, 1651, 1678, 2789, 540,  1789,
  1540, 1847, 1482, 952,  2377, 1461, 1868, 2687, 642,  939,  2390, 2308, 1021,
  2437, 892,  2388, 941,  733,  2596, 2337, 992,  268,  3061, 641,  2688, 1584,
  1745, 2298, 1031, 2037, 1292, 3220, 109,  375,  2954, 2549, 780,  2090, 1239,
  1645, 1684, 1063, 2266, 319,  3010, 2773, 556,  757,  2572, 2099, 1230, 561,
  2768, 2466, 863,  2594, 735,  2804, 525,  1092, 2237, 403,  2926, 1026, 2303,
  1143, 2186, 2150, 1179, 2775, 554,  886,  2443, 1722, 1607, 1212, 2117, 1874,
  1455, 1029, 2300, 2110, 1219, 2935, 394,  885,  2444, 2154, 1175
};

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

  for (size_t l = LOG2N - 1; l >= 1; l--) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = N >> (l + 1);

    for (size_t start = 0; start < N; start += lenx2) {
      const size_t k_now = k_beg ^ (start >> (l + 1));
      const auto ζ_exp = ζ ^ ntt::bit_rev<LOG2N - 1>(k_now);

      for (size_t i = start; i < start + len; i++) {
        const auto a = dst[i];
        const auto b = dst[i + len];

        const auto tmp = ζ_exp * b;

        dst[i] = a + tmp;
        dst[i + len] = a - tmp;
      }
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

  for (size_t l = 1; l < LOG2N; l++) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = (N >> l) - 1;

    for (size_t start = 0; start < N; start += lenx2) {
      const size_t k_now = k_beg - (start >> (l + 1));
      const ff::ff_t neg_ζ_exp = -(ζ ^ ntt::bit_rev<LOG2N - 1>(k_now));

      for (size_t i = start; i < start + len; i++) {
        const auto tmp = dst[i];

        dst[i] = tmp + dst[i + len];
        dst[i + len] = (tmp - dst[i + len]) * neg_ζ_exp;
      }
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
    basemul(f + off, g + off, h + off, POLY_MUL_ζ_EXP[i]);
  }
}

}
