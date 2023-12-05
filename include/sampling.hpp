#pragma once
#include "field.hpp"
#include "ntt.hpp"
#include "params.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
#include <array>
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
inline void
parse(shake128::shake128_t& hasher, std::span<field::zq_t, ntt::N> poly)
{
  constexpr size_t n = poly.size();

  size_t coeff_idx = 0;
  std::array<uint8_t, shake128::RATE / 8> buf{};

  while (coeff_idx < n) {
    hasher.squeeze(buf);

    for (size_t off = 0; (off < buf.size()) && (coeff_idx < n); off += 3) {
      const uint16_t d1 = (static_cast<uint16_t>(buf[off + 1] & 0x0f) << 8) | static_cast<uint16_t>(buf[off + 0]);
      const uint16_t d2 = (static_cast<uint16_t>(buf[off + 2]) << 4) | (static_cast<uint16_t>(buf[off + 1] >> 4));

      if (d1 < field::Q) {
        poly[coeff_idx] = field::zq_t(d1);
        coeff_idx++;
      }

      if ((d2 < field::Q) && (coeff_idx < n)) {
        poly[coeff_idx] = field::zq_t(d2);
        coeff_idx++;
      }
    }
  }
}

// Generate public matrix A ( consists of degree-255 polynomials ) in NTT
// domain, by sampling from a XOF ( read SHAKE128 ), which is seeded with 32
// -bytes key and two nonces ( each of 1 -byte )
//
// See step (4-8) of algorithm 4/ 5, defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t k, bool transpose>
static inline void
generate_matrix(std::span<field::zq_t, k * k * ntt::N> mat, std::span<const uint8_t, 32> rho)
  requires(kyber_params::check_k(k))
{
  std::array<uint8_t, rho.size() + 2> xof_in{};
  std::copy(rho.begin(), rho.end(), xof_in.begin());

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

      shake128::shake128_t hasher;
      hasher.absorb(xof_in);
      hasher.finalize();

      using poly_t = std::span<field::zq_t, mat.size() / (k * k)>;
      parse(hasher, poly_t(mat.subspan(off, ntt::N)));
    }
  }
}

// Centered Binomial Distribution
//
// A degree 255 polynomial deterministically sampled from 64 * eta -bytes output
// of a pseudorandom function ( PRF )
//
// See algorithm 2, defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t eta>
static inline void
cbd(std::span<const uint8_t, 64 * eta> prf, std::span<field::zq_t, ntt::N> poly)
  requires(kyber_params::check_eta(eta))
{
  if constexpr (eta == 2) {
    static_assert(eta == 2, "η must be 2 !");

    constexpr size_t till = 64 * eta;
    constexpr uint8_t mask8 = 0b01010101;
    constexpr uint8_t mask2 = 0b11;

    for (size_t i = 0; i < till; i++) {
      const size_t poff = i << 1;
      const uint8_t word = prf[i];

      const uint8_t t0 = (word >> 0) & mask8;
      const uint8_t t1 = (word >> 1) & mask8;
      const uint8_t t2 = t0 + t1;

      poly[poff + 0] = field::zq_t((t2 >> 0) & mask2) - field::zq_t((t2 >> 2) & mask2);
      poly[poff + 1] = field::zq_t((t2 >> 4) & mask2) - field::zq_t((t2 >> 6) & mask2);
    }
  } else {
    static_assert(eta == 3, "η must be 3 !");

    constexpr size_t till = 64;
    constexpr uint32_t mask24 = 0b001001001001001001001001u;
    constexpr uint32_t mask3 = 0b111u;

    for (size_t i = 0; i < till; i++) {
      const size_t boff = i * 3;
      const size_t poff = i << 2;

      const uint32_t word = (static_cast<uint32_t>(prf[boff + 2]) << 16) | (static_cast<uint32_t>(prf[boff + 1]) << 8) | static_cast<uint32_t>(prf[boff + 0]);

      const uint32_t t0 = (word >> 0) & mask24;
      const uint32_t t1 = (word >> 1) & mask24;
      const uint32_t t2 = (word >> 2) & mask24;
      const uint32_t t3 = t0 + t1 + t2;

      poly[poff + 0] = field::zq_t((t3 >> 0) & mask3) - field::zq_t((t3 >> 3) & mask3);
      poly[poff + 1] = field::zq_t((t3 >> 6) & mask3) - field::zq_t((t3 >> 9) & mask3);
      poly[poff + 2] = field::zq_t((t3 >> 12) & mask3) - field::zq_t((t3 >> 15) & mask3);
      poly[poff + 3] = field::zq_t((t3 >> 18) & mask3) - field::zq_t((t3 >> 21) & mask3);
    }
  }
}

// Sample a polynomial vector from Bη, following step (9-12) of algorithm 4,
// defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t k, size_t eta>
static inline void
generate_vector(std::span<field::zq_t, k * ntt::N> vec, std::span<const uint8_t, 32> sigma, const uint8_t nonce)
  requires((k == 1) || kyber_params::check_k(k))
{
  std::array<uint8_t, 64 * eta> prf_out{};
  std::array<uint8_t, sigma.size() + 1> prf_in{};
  std::copy(sigma.begin(), sigma.end(), prf_in.begin());

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    prf_in[32] = nonce + static_cast<uint8_t>(i);

    shake256::shake256_t hasher;
    hasher.absorb(prf_in);
    hasher.finalize();
    hasher.squeeze(prf_out);

    using poly_t = std::span<field::zq_t, vec.size() / k>;
    kyber_utils::cbd<eta>(prf_out, poly_t(vec.subspan(off, ntt::N)));
  }
}

}
