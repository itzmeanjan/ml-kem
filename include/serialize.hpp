#pragma once
#include "field.hpp"
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
encode(const field::zq_t* const __restrict poly, // degree 255 polynomial
       uint8_t* const __restrict arr // byte array of length 32*l -bytes
       )
  requires(kyber_params::check_l(l))
{
  constexpr size_t len = 32 * l;
  std::memset(arr, 0, len);

  if constexpr (l == 1) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint32_t one = 0b1u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 3;
      arr[i] = (static_cast<uint8_t>(poly[off + 7].to_canonical() & one) << 7) |
               (static_cast<uint8_t>(poly[off + 6].to_canonical() & one) << 6) |
               (static_cast<uint8_t>(poly[off + 5].to_canonical() & one) << 5) |
               (static_cast<uint8_t>(poly[off + 4].to_canonical() & one) << 4) |
               (static_cast<uint8_t>(poly[off + 3].to_canonical() & one) << 3) |
               (static_cast<uint8_t>(poly[off + 2].to_canonical() & one) << 2) |
               (static_cast<uint8_t>(poly[off + 1].to_canonical() & one) << 1) |
               (static_cast<uint8_t>(poly[off + 0].to_canonical() & one) << 0);
    }
  } else if constexpr (l == 4) {
    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint32_t msk = 0b1111u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 1;
      arr[i] = (static_cast<uint8_t>(poly[off + 1].to_canonical() & msk) << 4) |
               (static_cast<uint8_t>(poly[off + 0].to_canonical() & msk) << 0);
    }
  } else if constexpr (l == 5) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint32_t mask5 = 0b11111u;
    constexpr uint32_t mask4 = 0b1111u;
    constexpr uint32_t mask3 = 0b111u;
    constexpr uint32_t mask2 = 0b11u;
    constexpr uint32_t mask1 = 0b1u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 5;

      const auto t0 = poly[poff + 0].to_canonical();
      const auto t1 = poly[poff + 1].to_canonical();
      const auto t2 = poly[poff + 2].to_canonical();
      const auto t3 = poly[poff + 3].to_canonical();
      const auto t4 = poly[poff + 4].to_canonical();
      const auto t5 = poly[poff + 5].to_canonical();
      const auto t6 = poly[poff + 6].to_canonical();
      const auto t7 = poly[poff + 7].to_canonical();

      arr[boff + 0] = (static_cast<uint8_t>(t1 & mask3) << 5) |
                      (static_cast<uint8_t>(t0 & mask5) << 0);
      arr[boff + 1] = (static_cast<uint8_t>(t3 & mask1) << 7) |
                      (static_cast<uint8_t>(t2 & mask5) << 2) |
                      static_cast<uint8_t>((t1 >> 3) & mask2);
      arr[boff + 2] = (static_cast<uint8_t>(t4 & mask4) << 4) |
                      static_cast<uint8_t>((t3 >> 1) & mask4);
      arr[boff + 3] = (static_cast<uint8_t>(t6 & mask2) << 6) |
                      (static_cast<uint8_t>(t5 & mask5) << 1) |
                      static_cast<uint8_t>((t4 >> 4) & mask1);
      arr[boff + 4] = (static_cast<uint8_t>(t7 & mask5) << 3) |
                      static_cast<uint8_t>((t6 >> 2) & mask3);
    }
  } else if constexpr (l == 10) {
    constexpr size_t itr_cnt = ntt::N >> 2;
    constexpr uint32_t mask6 = 0b111111u;
    constexpr uint32_t mask4 = 0b1111u;
    constexpr uint32_t mask2 = 0b11u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 2;
      const size_t boff = i * 5;

      const auto t0 = poly[poff + 0].to_canonical();
      const auto t1 = poly[poff + 1].to_canonical();
      const auto t2 = poly[poff + 2].to_canonical();
      const auto t3 = poly[poff + 3].to_canonical();

      arr[boff + 0] = static_cast<uint8_t>(t0);
      arr[boff + 1] = static_cast<uint8_t>((t1 & mask6) << 2) |
                      static_cast<uint8_t>((t0 >> 8) & mask2);
      arr[boff + 2] = static_cast<uint8_t>((t2 & mask4) << 4) |
                      static_cast<uint8_t>((t1 >> 6) & mask4);
      arr[boff + 3] = static_cast<uint8_t>((t3 & mask2) << 6) |
                      static_cast<uint8_t>((t2 >> 4) & mask6);
      arr[boff + 4] = static_cast<uint8_t>(t3 >> 2);
    }
  } else if constexpr (l == 11) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint32_t mask8 = 0b11111111u;
    constexpr uint32_t mask7 = 0b1111111u;
    constexpr uint32_t mask6 = 0b111111u;
    constexpr uint32_t mask5 = 0b11111u;
    constexpr uint32_t mask4 = 0b1111u;
    constexpr uint32_t mask3 = 0b111u;
    constexpr uint32_t mask2 = 0b11u;
    constexpr uint32_t mask1 = 0b1u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 3;
      const size_t boff = i * 11;

      const auto t0 = poly[poff + 0].to_canonical();
      const auto t1 = poly[poff + 1].to_canonical();
      const auto t2 = poly[poff + 2].to_canonical();
      const auto t3 = poly[poff + 3].to_canonical();
      const auto t4 = poly[poff + 4].to_canonical();
      const auto t5 = poly[poff + 5].to_canonical();
      const auto t6 = poly[poff + 6].to_canonical();
      const auto t7 = poly[poff + 7].to_canonical();

      arr[boff + 0] = static_cast<uint8_t>(t0 & mask8);
      arr[boff + 1] = static_cast<uint8_t>((t1 & mask5) << 3) |
                      static_cast<uint8_t>((t0 >> 8) & mask3);
      arr[boff + 2] = static_cast<uint8_t>((t2 & mask2) << 6) |
                      static_cast<uint8_t>((t1 >> 5) & mask6);
      arr[boff + 3] = static_cast<uint8_t>((t2 >> 2) & mask8);
      arr[boff + 4] = static_cast<uint8_t>((t3 & mask7) << 1) |
                      static_cast<uint8_t>((t2 >> 10) & mask1);
      arr[boff + 5] = static_cast<uint8_t>((t4 & mask4) << 4) |
                      static_cast<uint8_t>((t3 >> 7) & mask4);
      arr[boff + 6] = static_cast<uint8_t>((t5 & mask1) << 7) |
                      static_cast<uint8_t>((t4 >> 4) & mask7);
      arr[boff + 7] = static_cast<uint8_t>((t5 >> 1) & mask8);
      arr[boff + 8] = static_cast<uint8_t>((t6 & mask6) << 2) |
                      static_cast<uint8_t>((t5 >> 9) & mask2);
      arr[boff + 9] = static_cast<uint8_t>((t7 & mask3) << 5) |
                      static_cast<uint8_t>((t6 >> 6) & mask5);
      arr[boff + 10] = static_cast<uint8_t>((t7 >> 3) & mask8);
    }
  } else {
    static_assert(l == 12, "l must be equal to 12 !");

    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint32_t mask4 = 0b1111u;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 1;
      const size_t boff = i * 3;

      const auto t0 = poly[poff + 0].to_canonical();
      const auto t1 = poly[poff + 1].to_canonical();

      arr[boff + 0] = static_cast<uint8_t>(t0);
      arr[boff + 1] = static_cast<uint8_t>((t1 & mask4) << 4) |
                      static_cast<uint8_t>((t0 >> 8) & mask4);
      arr[boff + 2] = static_cast<uint8_t>(t1 >> 4);
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
       field::zq_t* const __restrict poly   // degree 255 polynomial
       )
  requires(kyber_params::check_l(l))
{
  if constexpr (l == 1) {
    constexpr size_t itr_cnt = ntt::N >> 3;
    constexpr uint8_t one = 0b1;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 3;
      const uint8_t byte = arr[i];

      poly[off + 0] = field::zq_t::from_canonical((byte >> 0) & one);
      poly[off + 1] = field::zq_t::from_canonical((byte >> 1) & one);
      poly[off + 2] = field::zq_t::from_canonical((byte >> 2) & one);
      poly[off + 3] = field::zq_t::from_canonical((byte >> 3) & one);
      poly[off + 4] = field::zq_t::from_canonical((byte >> 4) & one);
      poly[off + 5] = field::zq_t::from_canonical((byte >> 5) & one);
      poly[off + 6] = field::zq_t::from_canonical((byte >> 6) & one);
      poly[off + 7] = field::zq_t::from_canonical((byte >> 7) & one);
    }
  } else if constexpr (l == 4) {
    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint8_t mask = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t off = i << 1;
      const uint8_t byte = arr[i];

      poly[off + 0] = field::zq_t::from_canonical((byte >> 0) & mask);
      poly[off + 1] = field::zq_t::from_canonical((byte >> 4) & mask);
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

      const auto t0 = static_cast<uint16_t>(arr[boff + 0] & mask5);
      const auto t1 = static_cast<uint16_t>((arr[boff + 1] & mask2) << 3) |
                      static_cast<uint16_t>((arr[boff + 0] >> 5) & mask3);
      const auto t2 = static_cast<uint16_t>((arr[boff + 1] >> 2) & mask5);
      const auto t3 = static_cast<uint16_t>((arr[boff + 2] & mask4) << 1) |
                      static_cast<uint16_t>((arr[boff + 1] >> 7) & mask1);
      const auto t4 = static_cast<uint16_t>((arr[boff + 3] & mask1) << 4) |
                      static_cast<uint16_t>((arr[boff + 2] >> 4) & mask4);
      const auto t5 = static_cast<uint16_t>((arr[boff + 3] >> 1) & mask5);
      const auto t6 = static_cast<uint16_t>((arr[boff + 4] & mask3) << 2) |
                      static_cast<uint16_t>((arr[boff + 3] >> 6) & mask2);
      const auto t7 = static_cast<uint16_t>((arr[boff + 4] >> 3) & mask5);

      poly[poff + 0] = field::zq_t::from_canonical(t0);
      poly[poff + 1] = field::zq_t::from_canonical(t1);
      poly[poff + 2] = field::zq_t::from_canonical(t2);
      poly[poff + 3] = field::zq_t::from_canonical(t3);
      poly[poff + 4] = field::zq_t::from_canonical(t4);
      poly[poff + 5] = field::zq_t::from_canonical(t5);
      poly[poff + 6] = field::zq_t::from_canonical(t6);
      poly[poff + 7] = field::zq_t::from_canonical(t7);
    }
  } else if constexpr (l == 10) {
    constexpr size_t itr_cnt = ntt::N >> 2;
    constexpr uint8_t mask6 = 0b111111;
    constexpr uint8_t mask4 = 0b1111;
    constexpr uint8_t mask2 = 0b11;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 2;
      const size_t boff = i * 5;

      const auto t0 = (static_cast<uint16_t>(arr[boff + 1] & mask2) << 8) |
                      static_cast<uint16_t>(arr[boff + 0]);
      const auto t1 = (static_cast<uint16_t>(arr[boff + 2] & mask4) << 6) |
                      static_cast<uint16_t>(arr[boff + 1] >> 2);
      const auto t2 = (static_cast<uint16_t>(arr[boff + 3] & mask6) << 4) |
                      static_cast<uint16_t>(arr[boff + 2] >> 4);
      const auto t3 = (static_cast<uint16_t>(arr[boff + 4]) << 2) |
                      static_cast<uint16_t>(arr[boff + 3] >> 6);

      poly[poff + 0] = field::zq_t::from_canonical(t0);
      poly[poff + 1] = field::zq_t::from_canonical(t1);
      poly[poff + 2] = field::zq_t::from_canonical(t2);
      poly[poff + 3] = field::zq_t::from_canonical(t3);
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

      const auto t0 = (static_cast<uint16_t>(arr[boff + 1] & mask3) << 8) |
                      static_cast<uint16_t>(arr[boff + 0]);
      const auto t1 = (static_cast<uint16_t>(arr[boff + 2] & mask6) << 5) |
                      static_cast<uint16_t>(arr[boff + 1] >> 3);
      const auto t2 = (static_cast<uint16_t>(arr[boff + 4] & mask1) << 10) |
                      (static_cast<uint16_t>(arr[boff + 3]) << 2) |
                      static_cast<uint16_t>(arr[boff + 2] >> 6);
      const auto t3 = (static_cast<uint16_t>(arr[boff + 5] & mask4) << 7) |
                      static_cast<uint16_t>(arr[boff + 4] >> 1);
      const auto t4 = (static_cast<uint16_t>(arr[boff + 6] & mask7) << 4) |
                      static_cast<uint16_t>(arr[boff + 5] >> 4);
      const auto t5 = (static_cast<uint16_t>(arr[boff + 8] & mask2) << 9) |
                      (static_cast<uint16_t>(arr[boff + 7]) << 1) |
                      static_cast<uint16_t>(arr[boff + 6] >> 7);
      const auto t6 = (static_cast<uint16_t>(arr[boff + 9] & mask5) << 6) |
                      static_cast<uint16_t>(arr[boff + 8] >> 2);
      const auto t7 = (static_cast<uint16_t>(arr[boff + 10]) << 3) |
                      static_cast<uint16_t>(arr[boff + 9] >> 5);

      poly[poff + 0] = field::zq_t::from_canonical(t0);
      poly[poff + 1] = field::zq_t::from_canonical(t1);
      poly[poff + 2] = field::zq_t::from_canonical(t2);
      poly[poff + 3] = field::zq_t::from_canonical(t3);
      poly[poff + 4] = field::zq_t::from_canonical(t4);
      poly[poff + 5] = field::zq_t::from_canonical(t5);
      poly[poff + 6] = field::zq_t::from_canonical(t6);
      poly[poff + 7] = field::zq_t::from_canonical(t7);
    }
  } else {
    static_assert(l == 12, "l must be equal to 12 !");

    constexpr size_t itr_cnt = ntt::N >> 1;
    constexpr uint8_t mask4 = 0b1111;

    for (size_t i = 0; i < itr_cnt; i++) {
      const size_t poff = i << 1;
      const size_t boff = i * 3;

      const auto t0 = (static_cast<uint16_t>(arr[boff + 1] & mask4) << 8) |
                      static_cast<uint16_t>(arr[boff + 0]);
      const auto t1 = (static_cast<uint16_t>(arr[boff + 2]) << 4) |
                      static_cast<uint16_t>(arr[boff + 1] >> 4);

      poly[poff + 0] = field::zq_t::from_canonical(t0);
      poly[poff + 1] = field::zq_t::from_canonical(t1);
    }
  }
}

}
