#pragma once
#include "ff.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
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

// Generate public matrix A ( consists of polynomials ) in NTT domain, by
// sampling from a XOF ( read SHAKE128 ), which is seeded with 32 -bytes key and
// two nonces ( each of 1 -byte )
//
// See step (4-8) of algorithm 4/ 5, defined in Kyber specification, as
// submitted to NIST final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k, const bool transpose>
static void
generate_matrix(ff::ff_t* const __restrict mat,
                uint8_t* const __restrict xof_in)
{
  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < k; j++) {
      const size_t off = (i * k + j) * ntt::N;

      if constexpr (transpose) {
        xof_in[32] = static_cast<uint8_t>(i);
        xof_in[33] = static_cast<uint8_t>(j);
      } else {
        xof_in[32] = static_cast<uint8_t>(j);
        xof_in[33] = static_cast<uint8_t>(i);
      }

      shake128::shake128 hasher{};
      hasher.hash(xof_in, sizeof(xof_in));

      parse(&hasher, mat + off);
    }
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

// Sample a polynomial vector from Bη, following step (9-12) of algorithm 4,
// defined in Kyber specification, as submitted to NIST final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k, const size_t eta>
static void
generate_vector(ff::ff_t* const __restrict vec,
                uint8_t* const __restrict prf_in,
                const uint8_t nonce)
{
  uint8_t prf_out[64 * eta]{};

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    prf_in[32] = nonce + i;

    shake256::shake256 hasher{};
    hasher.hash(prf_in, sizeof(prf_in));
    hasher.read(prf_out, sizeof(prf_out));

    kyber_utils::cbd<eta>(prf_out, vec + off);
  }
}

}
