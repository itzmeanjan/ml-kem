#pragma once
#include "ff.hpp"
#include "shake128.hpp"
#include <cstdint>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Uniform sampling in R_q | q = 3329
//
// Given a byte stream, this routine *deterministically* samples a degree 255
// polynomial in NTT representation. If the byte stream is statistically close
// to uniform random byte stream, produced polynomial coefficients are also
// statiscally close to randomly sampled elements of R_q.
//
// See algorithm 1, defined in Kyber specification, present in NIST PQC final
// round submission package
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
static void
parse(shake128::shake128* const __restrict hasher, // Squeezes arbitrary bytes
      ff::ff_t* const __restrict poly              // Degree 255 polynomial
)
{
  constexpr size_t n = 256;
  constexpr uint16_t q = ((1 << 8) * 13) + 1;

  size_t i = 0;
  uint8_t buf[3]{};

  while (i < n) {
    hasher->read(buf, sizeof(buf));

    const uint16_t d1 = (static_cast<uint16_t>(buf[1] & 15) << 8) ^
                        (static_cast<uint16_t>(buf[0]) << 0);
    const uint16_t d2 = (static_cast<uint16_t>(buf[1] >> 4) << 0) ^
                        (static_cast<uint16_t>(buf[2]) << 4);

    const bool flg0 = d1 < q;
    poly[i] = ff::ff_t{ static_cast<uint16_t>(d1 * flg0) };
    i = i + 1 * flg0;

    const bool flg1 = (d2 < q) & (i < n);
    poly[i] = ff::ff_t{ static_cast<uint16_t>(d2 * flg1) };
    i = i + 1 * flg1;
  }
}

// Compile time check to ensure that η ( read eta ) is either 2 or 3, as defined
// in Kyber specification
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
inline static constexpr bool
check_eta(const size_t eta)
{
  return (eta == 2) || (eta == 3);
}

// Centered Binomial Distribution
//
// A degree 255 polynomial deterministically sampled from 64 * eta -bytes output
// of a pseudorandom function ( PRF )
//
// See algorithm 2, defined in Kyber specification, present in NIST PQC final
// round submission package
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t eta>
static void
cbd(const uint8_t* const __restrict prf, // Byte array of length 64 * eta
    ff::ff_t* const __restrict poly      // Degree 255 polynomial
    ) requires(check_eta(eta))
{
  constexpr size_t n = 256;
  constexpr uint16_t q = ((1 << 8) * 13) + 1;

  for (size_t i = 0; i < n; i++) {
    uint16_t a = 0;
    for (size_t j = 0; j < eta; j++) {
      const size_t off = 2 * i * eta + j;

      const size_t byte_off = off >> 3;
      const size_t bit_off = off & 7ul;

      a += (prf[byte_off] >> bit_off) & 0b1;
    }

    uint16_t b = 0;
    for (size_t j = 0; j < eta; j++) {
      const size_t off = 2 * i * eta + eta + j;

      const size_t byte_off = off >> 3;
      const size_t bit_off = off & 7ul;

      b += (prf[byte_off] >> bit_off) & 0b1;
    }

    const uint16_t coeff = a > b ? a - b : q - (b - a);
    poly[i] = ff::ff_t{ coeff };
  }
}

}
