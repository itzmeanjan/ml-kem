#pragma once
#include "prng.hpp"
#include <bit>
#include <cstdint>

// Prime field arithmetic over Zq, for Kyber PQC Algorithm s.t. q = 3329
namespace field {

// Kyber Prime Field Modulus ( = 3329 )
constexpr uint32_t Q = (1u << 8) * 13 + 1;

// Bit width of Kyber Prime Field Modulus ( = 12 )
constexpr size_t RADIX_BIT_WIDTH = std::bit_width(Q);

// Precomputed Barrett Reduction Constant
//
// Note,
//
// k = ceil(log2(Q)) = 12,
// r = floor((1 << 2k) / Q) = 5039
//
// See https://www.nayuki.io/page/barrett-reduction-algorithm.
constexpr uint32_t R = (1u << (2 * RADIX_BIT_WIDTH)) / Q;

// Prime field Zq | q = 3329, with arithmetic operations defined over it.
//
// This implementation collects inspiration from
// https://github.com/itzmeanjan/dilithium/blob/3fe6ab61d2d70c1a0b71fc6ed4449f64da08b020/include/field.hpp.
struct zq_t
{
public:
  // Returns prime field element 0.
  inline constexpr zq_t() = default;

  // Constructs field element s.t. input is already reduced by prime modulo Q.
  inline constexpr zq_t(const uint16_t a) { this->v = a; }

  // Returns canonical value held under Zq type. Returned value must ∈ [0, Q).
  inline constexpr uint32_t raw() const { return this->v; }

  // Returns prime field element 0.
  static inline constexpr zq_t zero() { return zq_t(); }

  // Returns prime field element 1.
  static inline constexpr zq_t one() { return zq_t(1u); }

  // Modulo addition of two Zq elements.
  inline constexpr zq_t operator+(const zq_t rhs) const
  {
    const uint32_t t = this->v + rhs.v;
    return zq_t(reduce_once(t));
  }

  // Compound modulo addition of two Zq elements.
  inline constexpr void operator+=(const zq_t rhs) { *this = *this + rhs; }

  // Modulo negation of a Zq element.
  inline constexpr zq_t operator-() const { return zq_t(Q - this->v); }

  // Modulo subtraction of one Zq element from another one.
  inline constexpr zq_t operator-(const zq_t rhs) const { return *this + (-rhs); }

  // Compound modulo subtraction of two Zq elements.
  inline constexpr void operator-=(const zq_t rhs) { *this = *this - rhs; }

  // Modulo multiplication of two Zq elements.
  inline constexpr zq_t operator*(const zq_t rhs) const
  {
    auto res = zq_t();
    res.v = barrett_reduce(this->v * rhs.v);
    return res;
  }

  // Compound modulo multiplication of two Zq elements.
  inline constexpr void operator*=(const zq_t rhs) { *this = *this * rhs; }

  // Modulo exponentiation of Zq element.
  //
  // Taken from
  // https://github.com/itzmeanjan/dilithium/blob/3fe6ab61d2d70c1a0b71fc6ed4449f64da08b020/include/field.hpp#L144-L167.
  inline constexpr zq_t operator^(const size_t n) const
  {
    zq_t base = *this;

    const zq_t br[]{ zq_t(1), base };
    zq_t res = br[n & 0b1ul];

    const size_t zeros = std::countl_zero(n);
    const size_t till = 64ul - zeros;

    for (size_t i = 1; i < till; i++) {
      base = base * base;

      const zq_t br[]{ zq_t(1), base };
      res = res * br[(n >> i) & 0b1ul];
    }

    return res;
  }

  // Multiplicative inverse of Zq element.
  //
  // Note, if Zq element is 0, we can't compute multiplicative inverse and 0 is
  // returned.
  inline constexpr zq_t inv() const { return *this ^ static_cast<size_t>((Q - 2)); }

  // Modulo division of two Zq elements.
  //
  // Note, if denominator is 0, returned result is 0 too, becaue we can't
  // compute multiplicative inverse of 0.
  inline constexpr zq_t operator/(const zq_t rhs) const { return *this * rhs.inv(); }

  // Compare two Zq elements, returning truth value, in case they are same,
  // otherwise returns false value.
  inline constexpr bool operator==(const zq_t rhs) const { return this->v == rhs.v; }

  // Samples a random Zq element, using pseudo random number generator.
  static inline zq_t random(prng::prng_t& prng)
  {
    uint16_t res = 0;
    prng.read(std::span(reinterpret_cast<uint8_t*>(&res), sizeof(res)));

    return zq_t(barrett_reduce(static_cast<uint32_t>(res)));
  }

private:
  // Underlying value held in this type.
  //
  // Note, v is always kept in its canonical form i.e. v ∈ [0, Q).
  uint32_t v = 0u;

  // Given a 32 -bit unsigned integer `v` such that `v` ∈ [0, Q*Q), this routine can be invoked for reducing `v` modulo Q, using
  // barrett reduction technique, following algorithm description @ https://www.nayuki.io/page/barrett-reduction-algorithm.
  static inline constexpr uint32_t barrett_reduce(const uint32_t v)
  {
    const uint64_t t0 = static_cast<uint64_t>(v) * static_cast<uint64_t>(R);
    const uint32_t t1 = static_cast<uint32_t>(t0 >> (2 * RADIX_BIT_WIDTH));
    const uint32_t t2 = t1 * Q;
    const uint32_t t = v - t2;

    return reduce_once(t);
  }

  // Given a 32 -bit unsigned integer `v` such that `v` ∈ [0, 2*Q), this routine can be invoked for reducing `v` modulo prime Q.
  static inline constexpr uint32_t reduce_once(const uint32_t v)
  {
    const uint32_t t0 = v - Q;
    const uint32_t t1 = -(t0 >> 31);
    const uint32_t t2 = Q & t1;
    const uint32_t t3 = t0 + t2;

    return t3;
  }
};

}
