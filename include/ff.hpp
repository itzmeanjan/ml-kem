#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <ostream>

// Prime field arithmetic over F_q, for Kyber PQC Algorithm s.t. q = 3329
namespace ff {

// Kyber Prime Field Modulus
constexpr uint16_t Q = (1 << 8) * 13 + 1;

// Extended GCD algorithm for computing inverse of prime ( = Q ) field element
//
// Taken from
// https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L40-L67
static std::array<int16_t, 3>
xgcd(const uint16_t x, const uint16_t y)
{
  int16_t old_r = static_cast<int16_t>(x), r = static_cast<int16_t>(y);
  int16_t old_s = 1, s = 0;
  int16_t old_t = 0, t = 1;

  while (r != 0) {
    int16_t quotient = old_r / r;
    int16_t tmp = 0;

    tmp = old_r;
    old_r = r;
    r = tmp - quotient * r;

    tmp = old_s;
    old_s = s;
    s = tmp - quotient * s;

    tmp = old_t;
    old_t = t;
    t = tmp - quotient * t;
  }

  return {
    old_s, // a
    old_t, // b
    old_r  // g
  };       // s.t. `ax + by = g`
}

// Element of prime field F_q | q = 3329, with arithmetic operations
struct ff_t
{
  // Value of field element | v ∈ [0, Q)
  uint16_t v = 0;

  ff_t(const uint16_t a) { v = a % Q; }

  // Computes canonical form of prime field addition
  ff_t operator+(const ff_t& rhs) const
  {
    const uint16_t tmp = (this->v + rhs.v) % Q;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field subtraction
  ff_t operator-(const ff_t& rhs) const
  {
    const uint16_t tmp = (Q + this->v - rhs.v) % Q;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field negation
  ff_t operator-() const
  {
    const uint16_t tmp = Q - this->v;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field multiplication
  ff_t operator*(const ff_t& rhs) const
  {
    const uint16_t tmp = (this->v * rhs.v) % Q;
    return ff_t{ tmp };
  }

  // Computes canonical form of multiplicative inverse of prime field element,
  // where a ∈ F_q | q = 3329; ensure a ∈ [1, q)
  //
  // Say return value of this function is b, then
  //
  // assert (a * b) % q == 1
  //
  // Taken from
  // https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L69-L94
  ff_t inv() const
  {
    // Can't compute multiplicative inverse of 0 in prime field
    if (this->v == 0) {
      return ff_t{ 0 };
    }

    auto res = xgcd(this->v, Q);

    if (res[0] < 0) {
      return ff_t{ static_cast<uint16_t>(Q + res[0]) };
    }

    return ff_t{ static_cast<uint16_t>(res[0] % Q) };
  }

  // Computes canonical form of prime field division
  ff_t operator/(const ff_t& rhs) const { return (*this) * rhs.inv(); }

  // Raises field element to N -th power ( result kept in canonical form ),
  // using exponentiation by repeated squaring rule
  //
  // Taken from
  // https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L153-L181
  ff_t operator^(const size_t n) const
  {
    if (n == 0) {
      return ff_t{ 1 };
    }
    if (n == 1) {
      return *this;
    }
    if (this->v == 0) {
      return ff_t{ 0 };
    }

    auto base = *this;
    auto r = n & 0b1 ? base : ff_t{ 1 };

    const size_t zeros = std::countl_zero(n);

    for (size_t i = 1; i < 64 - zeros; i++) {
      base = base * base;
      if ((n >> i) & 0b1) {
        r = r * base;
      }
    }

    return r;
  }

  // Writes field element into output stream, used for debugging purposes
  friend std::ostream& operator<<(std::ostream& os, const ff_t& elm);
};

std::ostream&
operator<<(std::ostream& os, const ff_t& elm)
{
  return os << "ff_q(" << elm.v << ", " << Q << ")";
}

}
