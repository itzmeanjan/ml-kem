#pragma once
#include "ff.hpp"
#include <cmath>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Compile-time check to ensure that number of bits ( read `d` ) to consider
// during polynomial coefficient compression/ decompression is within tolerable
// bounds.
//
// See page 5 of Kyber specification, as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
constexpr bool
check_d(const size_t d)
{
  // $ python3
  // >>> import math
  // >>> a = math.log2(3329) # == 11.700873155140263
  // >>> math.round(a) # == 12
  constexpr size_t log2d = 12ul;
  return d < log2d;
}

// Given an element x ∈ Z_q | q = 3329, this routine compresses it by discarding
// some low-order bits, computing y ∈ [0, 2^d) | d < round(log2(q))
//
// See top of page 5 of Kyber specification, as submitted to NIST PQC final
// round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t d>
static ff::ff_t
compress(const ff::ff_t x) requires(check_d(d))
{
  constexpr size_t t0 = 1ul << d;

  const size_t t1 = static_cast<size_t>(x.v) << d;
  const double t2 = static_cast<double>(t1) / static_cast<double>(ff::Q);
  const size_t t3 = static_cast<size_t>(std::round(t2));
  const uint16_t t4 = static_cast<uint16_t>(t3 & (t0 - 1));

  return ff::ff_t{ t4 };
}

// Given an element x ∈ [0, 2^d) | d < round(log2(q)), this routine decompresses
// it back to y ∈ Z_q | q = 3329
//
// This routine recovers the compressed element with error probability as
// defined in eq. 2 of Kyber specification.
//
// See top of page 5 of Kyber specification, as submitted to NIST PQC final
// round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t d>
static ff::ff_t
decompress(const ff::ff_t x) requires(check_d(d))
{
  constexpr size_t t0 = 1ul << d;

  const size_t t1 = static_cast<size_t>(ff::Q * x.v);
  const double t2 = static_cast<double>(t1) / static_cast<double>(t0);
  const uint16_t t3 = static_cast<uint16_t>(std::round(t2));

  return ff::ff_t{ t3 };
}

// Decompression error that can happen for some given `d` s.t.
//
// x' = decompress(compress(x, d), d)
//
// |(x' - x) mod q| <= round(q / 2 ^ (d + 1))
//
// See eq. 2 of Kyber specification, as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t d>
inline static size_t
compute_error()
{
  constexpr double t0 = static_cast<double>(ff::Q);
  constexpr double t1 = static_cast<double>(1ul << (d + 1));

  const size_t t2 = static_cast<size_t>(std::round(t0 / t1));
  return t2;
}

}
