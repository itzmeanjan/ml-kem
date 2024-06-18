#pragma once
#include "ml_kem/internals/rng/prng.hpp"
#include <bit>
#include <cstdint>

namespace ml_kem_field {

// Ml_kem Prime Field Modulus ( = 3329 )
static constexpr uint32_t Q = (1u << 8) * 13 + 1;

// Bit width of Ml_kem Prime Field Modulus ( = 12 )
static constexpr size_t Q_BIT_WIDTH = std::bit_width(Q);

// Precomputed Barrett Reduction Constant
//
// Note,
//
// k = ceil(log2(Q)) = 12,
// r = floor((1 << 2k) / Q) = 5039
//
// See https://www.nayuki.io/page/barrett-reduction-algorithm.
static constexpr uint32_t R = (1u << (2 * Q_BIT_WIDTH)) / Q;

// Prime field Zq | q = 3329, with arithmetic operations defined over it.
//
// Collects inspiration from https://github.com/itzmeanjan/dilithium/blob/3fe6ab61/include/field.hpp.
struct zq_t
{
private:
  // Underlying value held in this type.
  //
  // Note, v is always kept in its canonical form i.e. v ∈ [0, Q).
  uint32_t v = 0u;

  // Given a 32 -bit unsigned integer `v` such that `v` ∈ [0, 2*Q), this routine can be invoked for reducing `v` modulo prime Q.
  static inline constexpr uint32_t reduce_once(const uint32_t v)
  {
    const uint32_t t0 = v - Q;
    const uint32_t t1 = -(t0 >> 31);
    const uint32_t t2 = Q & t1;
    const uint32_t t3 = t0 + t2;

    return t3;
  }

  // Given a 32 -bit unsigned integer `v` such that `v` ∈ [0, Q*Q), this routine can be invoked for reducing `v` modulo Q, using
  // barrett reduction technique, following algorithm description @ https://www.nayuki.io/page/barrett-reduction-algorithm.
  static inline constexpr uint32_t barrett_reduce(const uint32_t v)
  {
    const uint64_t t0 = static_cast<uint64_t>(v) * static_cast<uint64_t>(R);
    const uint32_t t1 = static_cast<uint32_t>(t0 >> (2 * Q_BIT_WIDTH));
    const uint32_t t2 = t1 * Q;
    const uint32_t t = v - t2;

    return reduce_once(t);
  }

public:
  // Constructor(s)
  inline constexpr zq_t() = default;
  inline constexpr zq_t(const uint16_t a /* Expects a ∈ [0, Q) */) { this->v = a; }
  static inline constexpr zq_t from_non_reduced(const uint16_t a /* Doesn't expect that a ∈ [0, Q) */) { return barrett_reduce(a); }

  // Returns canonical value held under Zq type. Returned value must ∈ [0, Q).
  inline constexpr uint32_t raw() const { return this->v; }

  static inline constexpr zq_t zero() { return zq_t(0u); }
  static inline constexpr zq_t one() { return zq_t(1u); }

  // Modulo addition of two Zq elements.
  inline constexpr zq_t operator+(const zq_t& rhs) const { return reduce_once(this->v + rhs.v); }
  inline constexpr void operator+=(const zq_t& rhs) { *this = *this + rhs; }

  // Modulo negation of a Zq element.
  inline constexpr zq_t operator-() const { return zq_t(Q - this->v); }

  // Modulo subtraction of one Zq element from another one.
  inline constexpr zq_t operator-(const zq_t& rhs) const { return *this + (-rhs); }
  inline constexpr void operator-=(const zq_t& rhs) { *this = *this - rhs; }

  // Modulo multiplication of two Zq elements.
  inline constexpr zq_t operator*(const zq_t& rhs) const { return barrett_reduce(this->v * rhs.v); }
  inline constexpr void operator*=(const zq_t& rhs) { *this = *this * rhs; }

  // Modulo exponentiation of Zq element.
  //
  // Taken from https://github.com/itzmeanjan/dilithium/blob/3fe6ab61/include/field.hpp#L144-L167.
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

  // Multiplicative inverse of Zq element. Also division of one Zq element by another one.
  //
  // Note, if Zq element is 0, we can't compute multiplicative inverse and 0 is returned.
  inline constexpr zq_t inv() const { return *this ^ static_cast<size_t>((Q - 2)); }
  inline constexpr zq_t operator/(const zq_t& rhs) const { return *this * rhs.inv(); }

  // Comparison operators, see https://en.cppreference.com/w/cpp/language/default_comparisons
  inline constexpr auto operator<=>(const zq_t&) const = default;

  // Samples a random Zq element, using pseudo random number generator.
  template<size_t bit_security_level>
  static inline zq_t random(ml_kem_prng::prng_t<bit_security_level>& prng)
  {
    uint16_t res = 0;
    prng.read(std::span(reinterpret_cast<uint8_t*>(&res), sizeof(res)));

    return zq_t::from_non_reduced(static_cast<uint32_t>(res));
  }
};

}
