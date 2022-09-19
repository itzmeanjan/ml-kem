#pragma once
#include <cstdint>
#include <ostream>

// Prime field arithmetic over F_q, for Kyber PQC Algorithm s.t. q = 3329
namespace ff {

constexpr uint16_t Q = (1 << 8) * 13 + 1;

// Element of prime field F_q | q = 3329, with arithmetic operations
struct ff_t
{
  // Value of field element | v ∈ [0, Q)
  uint16_t v = 0;

  ff_t(const uint16_t a) { v = a % Q; }

  // Computes canonical form of prime field addition
  ff_t operator+(const ff_t& rhs)
  {
    const uint16_t tmp = (this->v + rhs.v) % Q;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field subtraction
  ff_t operator-(const ff_t& rhs)
  {
    const uint16_t tmp = (Q + this->v - rhs.v) % Q;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field negation
  ff_t operator-()
  {
    const uint16_t tmp = Q - this->v;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field multiplication
  ff_t operator*(const ff_t& rhs)
  {
    const uint16_t tmp = (this->v * rhs.v) % Q;
    return ff_t{ tmp };
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
