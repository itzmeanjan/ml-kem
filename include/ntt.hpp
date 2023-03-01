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

// Multiplicative inverse of N/ 2 over Z_q | q = 3329 and N = 256
constexpr auto INV_N = ff::ff_t{ N >> 1 }.inv();

// Precomputed constants ( powers of ζ ), used for computing NTT form of
// degree-255 polynomial
constexpr ff::ff_t NTT_ζ_EXP[]{
  1,    1729, 2580, 3289, 2642, 630,  1897, 848,  1062, 1919, 193,  797,  2786,
  3260, 569,  1746, 296,  2447, 1339, 1476, 3046, 56,   2240, 1333, 1426, 2094,
  535,  2882, 2393, 2879, 1974, 821,  289,  331,  3253, 1756, 1197, 2304, 2277,
  2055, 650,  1977, 2513, 632,  2865, 33,   1320, 1915, 2319, 1435, 807,  452,
  1438, 2868, 1534, 2402, 2647, 2617, 1481, 648,  2474, 3110, 1227, 910,  17,
  2761, 583,  2649, 1637, 723,  2288, 1100, 1409, 2662, 3281, 233,  756,  2156,
  3015, 3050, 1703, 1651, 2789, 1789, 1847, 952,  1461, 2687, 939,  2308, 2437,
  2388, 733,  2337, 268,  641,  1584, 2298, 2037, 3220, 375,  2549, 2090, 1645,
  1063, 319,  2773, 757,  2099, 561,  2466, 2594, 2804, 1092, 403,  1026, 1143,
  2150, 2775, 886,  1722, 1212, 1874, 1029, 2110, 2935, 885,  2154
};

// Precomputed constants ( negated powers of ζ ), used for computing coefficient
// form of degree-255 polynomial using inverse NTT
//
// [-NTT_ζ_EXP[i] for i in range(128)]
//
// That's how these constants were computed !
constexpr ff::ff_t INTT_ζ_EXP[]{
  3328, 1600, 749,  40,   687,  2699, 1432, 2481, 2267, 1410, 3136, 2532, 543,
  69,   2760, 1583, 3033, 882,  1990, 1853, 283,  3273, 1089, 1996, 1903, 1235,
  2794, 447,  936,  450,  1355, 2508, 3040, 2998, 76,   1573, 2132, 1025, 1052,
  1274, 2679, 1352, 816,  2697, 464,  3296, 2009, 1414, 1010, 1894, 2522, 2877,
  1891, 461,  1795, 927,  682,  712,  1848, 2681, 855,  219,  2102, 2419, 3312,
  568,  2746, 680,  1692, 2606, 1041, 2229, 1920, 667,  48,   3096, 2573, 1173,
  314,  279,  1626, 1678, 540,  1540, 1482, 2377, 1868, 642,  2390, 1021, 892,
  941,  2596, 992,  3061, 2688, 1745, 1031, 1292, 109,  2954, 780,  1239, 1684,
  2266, 3010, 556,  2572, 1230, 2768, 863,  735,  525,  2237, 2926, 2303, 2186,
  1179, 554,  2443, 1607, 2117, 1455, 2300, 1219, 394,  2444, 1175
};

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

// Given a polynomial f with 256 coefficients over F_q | q = 3329, this routine
// computes number theoretic transform using cooley-tukey algorithm, producing
// polynomial f' s.t. its coefficients are placed in bit-reversed order
//
// Note, this routine mutates input i.e. it's an in-place NTT implementation.
//
// Implementation inspired from
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L69-L144
static inline void
ntt(ff::ff_t* const poly)
{
  for (size_t l = LOG2N - 1; l >= 1; l--) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = N >> (l + 1);

    for (size_t start = 0; start < N; start += lenx2) {
      const size_t k_now = k_beg + (start >> (l + 1));
      // Looking up precomputed constant, though it can be computed using
      //
      // ζ ^ bit_rev<LOG2N - 1>(k_now)
      //
      // This is how these constants are generated !
      const ff::ff_t ζ_exp = NTT_ζ_EXP[k_now];

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
static inline void
intt(ff::ff_t* const poly)
{
  for (size_t l = 1; l < LOG2N; l++) {
    const size_t len = 1ul << l;
    const size_t lenx2 = len << 1;
    const size_t k_beg = (N >> l) - 1;

    for (size_t start = 0; start < N; start += lenx2) {
      const size_t k_now = k_beg - (start >> (l + 1));
      // Looking up precomputed constant, though it can be computed using
      //
      // -(ζ ^ bit_rev<LOG2N - 1>(k_now))
      //
      // Or simpler
      //
      // -NTT_ζ_EXP[k_now]
      const ff::ff_t neg_ζ_exp = INTT_ζ_EXP[k_now];

      for (size_t i = start; i < start + len; i++) {
        const auto tmp = poly[i];

        poly[i] += poly[i + len];
        poly[i + len] = tmp - poly[i + len];
        poly[i + len] *= neg_ζ_exp;
      }
    }
  }

  for (size_t i = 0; i < N; i++) {
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
static inline void
basemul(const ff::ff_t* const __restrict f, // degree-1 polynomial
        const ff::ff_t* const __restrict g, // degree-1 polynomial
        ff::ff_t* const __restrict h,       // degree-1 polynomial
        const ff::ff_t ζ                    // zeta
)
{
  ff::ff_t f0 = f[0];
  ff::ff_t f1 = f[1];

  f0 *= g[0];
  f1 *= g[1];
  f1 *= ζ;
  f1 += f0;

  h[0] = f1;

  ff::ff_t g0 = g[0];
  ff::ff_t g1 = g[1];

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
static inline void
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
