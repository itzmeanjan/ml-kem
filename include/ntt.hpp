#pragma once
#include "ff.hpp"
#include <cstring>

// (inverse) Number Theoretic Transform for degree 255 polynomial, over Kyber
// Prime Field F_q | q = 3329
namespace ntt {

constexpr size_t LOG2N = 8;
constexpr size_t N = 1 << LOG2N;

const auto INV_N = ff::ff_t{ N >> 1 }.inv();

// Given a 64 -bit unsigned integer, this routine extracts specified many
// contiguous bits from ( least significant bits ) LSB side & reverses their bit
// order, returning bit reversed `mbw` -bit wide number
//
// See
// https://github.com/itzmeanjan/falcon/blob/45b0593/include/ntt.hpp#L30-L38
// for source of inspiration
template<const size_t mbw>
const size_t
bit_rev(const size_t v)
{
  size_t v_rev = 0ul;

  for (size_t i = 0; i < mbw; i++) {
    const size_t extracted = (v >> i) & 0b1;
    v_rev ^= extracted << (mbw - 1ul - i);
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

  const auto ω = ff::nth_root_of_unity(LOG2N - 1);

  for (size_t i = LOG2N - 1; i >= 1; i--) {
    const size_t p = 1ul << i;
    const size_t q = N >> i;

    for (size_t k = 0; k < N; k++) {
      const size_t k_rev = bit_rev<LOG2N>(k) % q;
      const auto ω_exp = ω ^ (p * k_rev);

      if (k < (k ^ p)) {
        const auto a = dst[k];
        const auto b = dst[k ^ p];

        const auto bxω = b * ω_exp;

        dst[k] = a + bxω;
        dst[k ^ p] = a - bxω;
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

  const auto ω = ff::nth_root_of_unity(LOG2N - 1).inv();

  for (size_t i = 1; i < LOG2N; i++) {
    const size_t p = 1ul << i;
    const size_t q = N >> i;

    for (size_t k = 0; k < N; k++) {
      const size_t k_rev = bit_rev<LOG2N>(k) % q;
      const auto ω_exp = ω ^ (p * k_rev);

      if (k < (k ^ p)) {
        const auto a = dst[k];
        const auto b = dst[k ^ p];

        dst[k] = a + b;
        dst[k ^ p] = (a - b) * ω_exp;
      }
    }
  }

  for (size_t i = 0; i < N; i++) {
    dst[i] = dst[i] * INV_N;
  }
}

}
