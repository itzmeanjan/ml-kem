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

}
