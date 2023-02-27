#pragma once
#include "ff.hpp"
#include "ntt.hpp"
#include "params.hpp"
#include <cstring>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Given a degree-255 polynomial, where significant portion of each ( total 256
// of them ) coefficient ∈ [0, 2^l), this routine serializes the polynomial to a
// byte array of length 32 * l -bytes
//
// See algorithm 3 described in section 1.1 ( page 7 ) of Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t l>
static inline void
encode(const ff::ff_t* const __restrict poly, // degree 255 polynomial
       uint8_t* const __restrict arr // byte array of length 32*l -bytes
       )
  requires(kyber_params::check_l(l))
{
  constexpr size_t len = 32 * l;
  std::memset(arr, 0, len);

  if constexpr (l == 1) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint16_t one = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 3;
      const uint8_t byte = (static_cast<uint8_t>(poly[off + 7].v & one) << 7) |
                           (static_cast<uint8_t>(poly[off + 6].v & one) << 6) |
                           (static_cast<uint8_t>(poly[off + 5].v & one) << 5) |
                           (static_cast<uint8_t>(poly[off + 4].v & one) << 4) |
                           (static_cast<uint8_t>(poly[off + 3].v & one) << 3) |
                           (static_cast<uint8_t>(poly[off + 2].v & one) << 2) |
                           (static_cast<uint8_t>(poly[off + 1].v & one) << 1) |
                           (static_cast<uint8_t>(poly[off + 0].v & one) << 0);

      arr[i] = byte;
    }
  } else if constexpr (l == 4) {
    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint16_t mask = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 1;
      const uint8_t byte = (static_cast<uint8_t>(poly[off + 1].v & mask) << 4) |
                           (static_cast<uint8_t>(poly[off + 0].v & mask) << 0);

      arr[i] = byte;
    }
  } else if constexpr (l == 5) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint16_t mask5 = 0b11111;
    constexpr uint16_t mask4 = 0b1111;
    constexpr uint16_t mask3 = 0b111;
    constexpr uint16_t mask2 = 0b11;
    constexpr uint16_t mask1 = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 5;

      arr[boff + 0] = (static_cast<uint8_t>(poly[poff + 1].v & mask3) << 5) |
                      (static_cast<uint8_t>(poly[poff + 0].v & mask5) << 0);
      arr[boff + 1] = (static_cast<uint8_t>(poly[poff + 3].v & mask1) << 7) |
                      (static_cast<uint8_t>(poly[poff + 2].v & mask5) << 2) |
                      static_cast<uint8_t>((poly[poff + 1].v >> 3) & mask2);
      arr[boff + 2] = (static_cast<uint8_t>(poly[poff + 4].v & mask4) << 4) |
                      static_cast<uint8_t>((poly[poff + 3].v >> 1) & mask4);
      arr[boff + 3] = (static_cast<uint8_t>(poly[poff + 6].v & mask2) << 6) |
                      (static_cast<uint8_t>(poly[poff + 5].v & mask5) << 1) |
                      static_cast<uint8_t>((poly[poff + 4].v >> 4) & mask1);
      arr[boff + 4] = (static_cast<uint8_t>(poly[poff + 7].v & mask5) << 3) |
                      static_cast<uint8_t>((poly[poff + 6].v >> 2) & mask3);
    }
  } else if constexpr (l == 10) {
    constexpr size_t itr_cnt = ntt::N >> 2;
    constexpr uint16_t mask6 = 0b111111;
    constexpr uint16_t mask4 = 0b1111;
    constexpr uint16_t mask2 = 0b11;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 2;
      const size_t boff = i * 5;

      arr[boff + 0] = static_cast<uint8_t>(poly[poff + 0].v);
      arr[boff + 1] = static_cast<uint8_t>((poly[poff + 1].v & mask6) << 2) |
                      static_cast<uint8_t>((poly[poff + 0].v >> 8) & mask2);
      arr[boff + 2] = static_cast<uint8_t>((poly[poff + 2].v & mask4) << 4) |
                      static_cast<uint8_t>((poly[poff + 1].v >> 6) & mask4);
      arr[boff + 3] = static_cast<uint8_t>((poly[poff + 3].v & mask2) << 6) |
                      static_cast<uint8_t>((poly[poff + 2].v >> 4) & mask6);
      arr[boff + 4] = static_cast<uint8_t>(poly[poff + 3].v >> 2);
    }
  } else if constexpr (l == 11) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint16_t mask8 = 0b11111111;
    constexpr uint16_t mask7 = 0b1111111;
    constexpr uint16_t mask6 = 0b111111;
    constexpr uint16_t mask5 = 0b11111;
    constexpr uint16_t mask4 = 0b1111;
    constexpr uint16_t mask3 = 0b111;
    constexpr uint16_t mask2 = 0b11;
    constexpr uint16_t mask1 = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 11;

      arr[boff + 0] = static_cast<uint8_t>(poly[poff + 0].v & mask8);
      arr[boff + 1] = static_cast<uint8_t>((poly[poff + 1].v & mask5) << 3) |
                      static_cast<uint8_t>((poly[poff + 0].v >> 8) & mask3);
      arr[boff + 2] = static_cast<uint8_t>((poly[poff + 2].v & mask2) << 6) |
                      static_cast<uint8_t>((poly[poff + 1].v >> 5) & mask6);
      arr[boff + 3] = static_cast<uint8_t>((poly[poff + 2].v >> 2) & mask8);
      arr[boff + 4] = static_cast<uint8_t>((poly[poff + 3].v & mask7) << 1) |
                      static_cast<uint8_t>((poly[poff + 2].v >> 10) & mask1);
      arr[boff + 5] = static_cast<uint8_t>((poly[poff + 4].v & mask4) << 4) |
                      static_cast<uint8_t>((poly[poff + 3].v >> 7) & mask4);
      arr[boff + 6] = static_cast<uint8_t>((poly[poff + 5].v & mask1) << 7) |
                      static_cast<uint8_t>((poly[poff + 4].v >> 4) & mask7);
      arr[boff + 7] = static_cast<uint8_t>((poly[poff + 5].v >> 1) & mask8);
      arr[boff + 8] = static_cast<uint8_t>((poly[poff + 6].v & mask6) << 2) |
                      static_cast<uint8_t>((poly[poff + 5].v >> 9) & mask2);
      arr[boff + 9] = static_cast<uint8_t>((poly[poff + 7].v & mask3) << 5) |
                      static_cast<uint8_t>((poly[poff + 6].v >> 6) & mask5);
      arr[boff + 10] = static_cast<uint8_t>((poly[poff + 7].v >> 3) & mask8);
    }
  } else {
    static_assert(l == 12, "l must be equal to 12 !");

    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint16_t mask4 = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 1;
      const size_t boff = i * 3;

      arr[boff + 0] = static_cast<uint8_t>(poly[poff + 0].v);
      arr[boff + 1] = static_cast<uint8_t>((poly[poff + 1].v & mask4) << 4) |
                      static_cast<uint8_t>((poly[poff + 0].v >> 8) & mask4);
      arr[boff + 2] = static_cast<uint8_t>(poly[poff + 1].v >> 4);
    }
  }
}

// Given a byte array of length 32 * l -bytes this routine deserializes it to a
// polynomial of degree 255 s.t. significant portion of each ( total 256 of them
// ) coefficient ∈ [0, 2^l)
//
// See algorithm 3 described in section 1.1 ( page 7 ) of Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t l>
static inline void
decode(const uint8_t* const __restrict arr, // byte array of length 32*l -bytes
       ff::ff_t* const __restrict poly      // degree 255 polynomial
       )
  requires(kyber_params::check_l(l))
{
  std::memset(poly, 0, ntt::N * sizeof(ff::ff_t));

  if constexpr (l == 1) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint8_t one = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 3;
      const uint8_t byte = arr[i];

      poly[off + 0].v = static_cast<uint16_t>((byte >> 0) & one);
      poly[off + 1].v = static_cast<uint16_t>((byte >> 1) & one);
      poly[off + 2].v = static_cast<uint16_t>((byte >> 2) & one);
      poly[off + 3].v = static_cast<uint16_t>((byte >> 3) & one);
      poly[off + 4].v = static_cast<uint16_t>((byte >> 4) & one);
      poly[off + 5].v = static_cast<uint16_t>((byte >> 5) & one);
      poly[off + 6].v = static_cast<uint16_t>((byte >> 6) & one);
      poly[off + 7].v = static_cast<uint16_t>((byte >> 7) & one);
    }
  } else if constexpr (l == 4) {
    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint8_t mask = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 1;
      const uint8_t byte = arr[i];

      poly[off + 0].v = static_cast<uint16_t>((byte >> 0) & mask);
      poly[off + 1].v = static_cast<uint16_t>((byte >> 4) & mask);
    }
  } else if constexpr (l == 5) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint8_t mask5 = 0b11111;
    constexpr uint8_t mask4 = 0b1111;
    constexpr uint8_t mask3 = 0b111;
    constexpr uint8_t mask2 = 0b11;
    constexpr uint8_t mask1 = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 5;

      poly[poff + 0].v = static_cast<uint16_t>(arr[boff + 0] & mask5);
      poly[poff + 1].v = static_cast<uint16_t>((arr[boff + 1] & mask2) << 3) |
                         static_cast<uint16_t>((arr[boff + 0] >> 5) & mask3);
      poly[poff + 2].v = static_cast<uint16_t>((arr[boff + 1] >> 2) & mask5);
      poly[poff + 3].v = static_cast<uint16_t>((arr[boff + 2] & mask4) << 1) |
                         static_cast<uint16_t>((arr[boff + 1] >> 7) & mask1);
      poly[poff + 4].v = static_cast<uint16_t>((arr[boff + 3] & mask1) << 4) |
                         static_cast<uint16_t>((arr[boff + 2] >> 4) & mask4);
      poly[poff + 5].v = static_cast<uint16_t>((arr[boff + 3] >> 1) & mask5);
      poly[poff + 6].v = static_cast<uint16_t>((arr[boff + 4] & mask3) << 2) |
                         static_cast<uint16_t>((arr[boff + 3] >> 6) & mask2);
      poly[poff + 7].v = static_cast<uint16_t>((arr[boff + 4] >> 3) & mask5);
    }
  } else if constexpr (l == 10) {
    constexpr size_t itr_cnt = ntt::N >> 2;
    constexpr uint8_t mask6 = 0b111111;
    constexpr uint8_t mask4 = 0b1111;
    constexpr uint8_t mask2 = 0b11;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 2;
      const size_t boff = i * 5;

      poly[poff + 0].v = (static_cast<uint16_t>(arr[boff + 1] & mask2) << 8) |
                         static_cast<uint16_t>(arr[boff + 0]);
      poly[poff + 1].v = (static_cast<uint16_t>(arr[boff + 2] & mask4) << 6) |
                         static_cast<uint16_t>(arr[boff + 1] >> 2);
      poly[poff + 2].v = (static_cast<uint16_t>(arr[boff + 3] & mask6) << 4) |
                         static_cast<uint16_t>(arr[boff + 2] >> 4);
      poly[poff + 3].v = (static_cast<uint16_t>(arr[boff + 4]) << 2) |
                         static_cast<uint16_t>(arr[boff + 3] >> 6);
    }
  } else if constexpr (l == 11) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint8_t mask7 = 0b1111111;
    constexpr uint8_t mask6 = 0b111111;
    constexpr uint8_t mask5 = 0b11111;
    constexpr uint8_t mask4 = 0b1111;
    constexpr uint8_t mask3 = 0b111;
    constexpr uint8_t mask2 = 0b11;
    constexpr uint8_t mask1 = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 11;

      poly[poff + 0].v = (static_cast<uint16_t>(arr[boff + 1] & mask3) << 8) |
                         static_cast<uint16_t>(arr[boff + 0]);
      poly[poff + 1].v = (static_cast<uint16_t>(arr[boff + 2] & mask6) << 5) |
                         static_cast<uint16_t>(arr[boff + 1] >> 3);
      poly[poff + 2].v = (static_cast<uint16_t>(arr[boff + 4] & mask1) << 10) |
                         (static_cast<uint16_t>(arr[boff + 3]) << 2) |
                         static_cast<uint16_t>(arr[boff + 2] >> 6);
      poly[poff + 3].v = (static_cast<uint16_t>(arr[boff + 5] & mask4) << 7) |
                         static_cast<uint16_t>(arr[boff + 4] >> 1);
      poly[poff + 4].v = (static_cast<uint16_t>(arr[boff + 6] & mask7) << 4) |
                         static_cast<uint16_t>(arr[boff + 5] >> 4);
      poly[poff + 5].v = (static_cast<uint16_t>(arr[boff + 8] & mask2) << 9) |
                         (static_cast<uint16_t>(arr[boff + 7]) << 1) |
                         static_cast<uint16_t>(arr[boff + 6] >> 7);
      poly[poff + 6].v = (static_cast<uint16_t>(arr[boff + 9] & mask5) << 6) |
                         static_cast<uint16_t>(arr[boff + 8] >> 2);
      poly[poff + 7].v = (static_cast<uint16_t>(arr[boff + 10]) << 3) |
                         static_cast<uint16_t>(arr[boff + 9] >> 5);
    }
  } else {
    static_assert(l == 12, "l must be equal to 12 !");

    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint8_t mask4 = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 1;
      const size_t boff = i * 3;

      poly[poff + 0].v = (static_cast<uint16_t>(arr[boff + 1] & mask4) << 8) |
                         static_cast<uint16_t>(arr[boff + 0]);
      poly[poff + 1].v = (static_cast<uint16_t>(arr[boff + 2]) << 4) |
                         static_cast<uint16_t>(arr[boff + 1] >> 4);
    }
  }
}

}
