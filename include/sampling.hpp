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
// See algorithm 1, defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
static void
parse(shake128::shake128* const __restrict hasher, // Squeezes arbitrary bytes
      ff::ff_t* const __restrict poly              // Degree 255 polynomial
)
{
  size_t i = 0;
  uint8_t buf[3]{};

  while (i < ntt::N) {
    hasher->read(buf, 3);

    const uint16_t d1 = (static_cast<uint16_t>(buf[1] & 0b1111) << 8) |
                        (static_cast<uint16_t>(buf[0]) << 0);
    const uint16_t d2 = (static_cast<uint16_t>(buf[2]) << 4) |
                        (static_cast<uint16_t>(buf[1] >> 4));

    const bool flg0 = d1 < ff::Q;
    const ff::ff_t br0[]{ poly[i], ff::ff_t{ d1 } };
    poly[i] = br0[flg0];
    i += 1 * flg0;

    const bool flg1 = (d2 < ff::Q) & (i < ntt::N);
    const ff::ff_t br1[]{ poly[i], ff::ff_t{ d2 } };
    poly[i] = br1[flg1];
    i += 1 * flg1;
  }
}

// Generate public matrix A ( consists of polynomials ) in NTT domain, by
// sampling from a XOF ( read SHAKE128 ), which is seeded with 32 -bytes key and
// two nonces ( each of 1 -byte )
//
// See step (4-8) of algorithm 4/ 5, defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const bool transpose>
static void
generate_matrix(ff::ff_t* const __restrict mat,
                const uint8_t* const __restrict rho)
{
  uint8_t xof_in[32 + 2]{};
  std::memcpy(xof_in, rho, 32);

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
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
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
// See algorithm 2, defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t eta>
static void
cbd(const uint8_t* const __restrict prf, // Byte array of length 64 * eta
    ff::ff_t* const __restrict poly      // Degree 255 polynomial
    )
  requires(check_eta(eta))
{
  if constexpr (eta == 2) {
    constexpr size_t till = 64 * eta;
    constexpr uint8_t mask8 = 0b01010101;
    constexpr uint8_t mask2 = 0b11;

    for (size_t i = 0; i < till; i++) {
      const size_t poff = i << 1;
      const uint8_t word = prf[i];

      const uint8_t t0 = (word >> 0) & mask8;
      const uint8_t t1 = (word >> 1) & mask8;
      const uint8_t t2 = t0 + t1;

      poly[poff + 0] = ff::ff_t{ static_cast<uint16_t>((t2 >> 0) & mask2) } -
                       ff::ff_t{ static_cast<uint16_t>((t2 >> 2) & mask2) };
      poly[poff + 1] = ff::ff_t{ static_cast<uint16_t>((t2 >> 4) & mask2) } -
                       ff::ff_t{ static_cast<uint16_t>((t2 >> 6) & mask2) };
    }
  } else if constexpr (eta == 3) {
    constexpr size_t till = 64;
    constexpr uint32_t mask24 = 0b001001001001001001001001u;
    constexpr uint32_t mask3 = 0b111u;

    for (size_t i = 0; i < till; i++) {
      const size_t boff = i * 3;
      const size_t poff = i << 2;

      const uint32_t word = (static_cast<uint32_t>(prf[boff + 2]) << 16) |
                            (static_cast<uint32_t>(prf[boff + 1]) << 8) |
                            (static_cast<uint32_t>(prf[boff + 0]) << 0);

      const uint32_t t0 = (word >> 0) & mask24;
      const uint32_t t1 = (word >> 1) & mask24;
      const uint32_t t2 = (word >> 2) & mask24;
      const uint32_t t3 = t0 + t1 + t2;

      poly[poff + 0] = ff::ff_t{ static_cast<uint16_t>((t3 >> 0) & mask3) } -
                       ff::ff_t{ static_cast<uint16_t>((t3 >> 3) & mask3) };
      poly[poff + 1] = ff::ff_t{ static_cast<uint16_t>((t3 >> 6) & mask3) } -
                       ff::ff_t{ static_cast<uint16_t>((t3 >> 9) & mask3) };
      poly[poff + 2] = ff::ff_t{ static_cast<uint16_t>((t3 >> 12) & mask3) } -
                       ff::ff_t{ static_cast<uint16_t>((t3 >> 15) & mask3) };
      poly[poff + 3] = ff::ff_t{ static_cast<uint16_t>((t3 >> 18) & mask3) } -
                       ff::ff_t{ static_cast<uint16_t>((t3 >> 21) & mask3) };
    }
  } else {
    for (size_t i = 0; i < ntt::N; i++) {
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

      const ff::ff_t coeff = ff::ff_t{ a } - ff::ff_t{ b };
      poly[i] = coeff;
    }
  }
}

// Sample a polynomial vector from Bη, following step (9-12) of algorithm 4,
// defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const size_t eta>
static void
generate_vector(ff::ff_t* const __restrict vec,
                const uint8_t* const __restrict sigma,
                const uint8_t nonce)
{
  uint8_t prf_out[64 * eta]{};
  uint8_t prf_in[32 + 1]{};
  std::memcpy(prf_in, sigma, 32);

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    prf_in[32] = nonce + static_cast<uint8_t>(i);

    shake256::shake256 hasher{};
    hasher.hash(prf_in, sizeof(prf_in));
    hasher.read(prf_out, sizeof(prf_out));

    kyber_utils::cbd<eta>(prf_out, vec + off);
  }
}

}
