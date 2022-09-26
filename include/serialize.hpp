#pragma once
#include "ff.hpp"
#include <cstring>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Compile-time check to ensure that at min 1 -bit and at max 12 -bits ( from
// LSB side ) of polynomial coefficients are considered to be significant during
//
// - serialization to byte array
// - deserialization to degree-255 polynomial
//
// Note, bit-width of Kyber prime ( = 3329 ) is 12
//
// $ python3
// >>> (3329).bit_length()
static constexpr bool
check_l(const size_t l)
{
  return (l > 0) && (l <= 12);
}

// Given a degree-255 polynomial, where significant portion of each ( total 256
// of them ) coefficient ∈ [0, 2^l), this routine serializes the polynomial to a
// byte array of length 32 * l -bytes
//
// See algorithm 3 described in section 1.1 ( page 7 ) of Kyber specification,
// as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t l>
static void
encode(const ff::ff_t* const __restrict poly, // degree 255 polynomial
       uint8_t* const __restrict arr // byte array of length 32*l -bytes
       ) requires(check_l(l))
{
  constexpr size_t len = 32 * l;
  constexpr size_t blen = len << 3;

  std::memset(arr, 0, len);

  for (size_t i = 0; i < blen; i++) {
    const size_t pidx = i / l;
    const size_t poff = i % l;

    const size_t aidx = i >> 3;
    const size_t aoff = i & 7ul;

    const uint8_t bit = static_cast<uint8_t>((poly[pidx].v >> poff) & 0b1);
    arr[aidx] = arr[aidx] ^ (bit << aoff);
  }
}

// Given a byte array of length 32 * l -bytes this routine deserializes it to a
// polynomial of degree 255 s.t. significant portion of each ( total 256 of them
// ) coefficient ∈ [0, 2^l)
//
// See algorithm 3 described in section 1.1 ( page 7 ) of Kyber specification,
// as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t l>
static void
decode(const uint8_t* const __restrict arr, // byte array of length 32*l -bytes
       ff::ff_t* const __restrict poly      // degree 255 polynomial
       ) requires(check_l(l))
{
  constexpr size_t n = 256;
  constexpr size_t len = 32 * l;
  constexpr size_t blen = len << 3;

  std::memset(poly, 0, n * sizeof(ff::ff_t));

  for (size_t i = 0; i < blen; i++) {
    const size_t aidx = i >> 3;
    const size_t aoff = i & 7ul;

    const size_t pidx = i / l;
    const size_t poff = i % l;

    const uint8_t bit = (arr[aidx] >> aoff) & 0b1;
    poly[pidx].v = poly[pidx].v ^ static_cast<uint16_t>(bit) << poff;
  }
}

}
