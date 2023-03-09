#pragma once
#include "prng.hpp"
#include <bit>
#include <cstdint>

// Prime field arithmetic over Zq, for Kyber PQC Algorithm s.t. q = 3329
//
// For Montgomery Arithmetic it collects motivation from https://ia.cr/2017/1057
// and
// https://github.com/itzmeanjan/secp256k1/blob/37b339db/field/base_field.py
namespace field {

// Kyber Prime Field Modulus ( = 3329 )
constexpr uint32_t Q = (1u << 8) * 13 + 1;

// Bit width of Kyber Prime Field Modulus ( = 12 )
constexpr size_t RADIX_BIT_WIDTH = std::bit_width(Q);

// = 2 ** RADIX_BIT_WIDTH = 2 ** 12 = 4096
constexpr uint32_t RADIX = 1u << RADIX_BIT_WIDTH;

// = 0x0FFF = 4095
constexpr uint32_t MASK = RADIX - 1u;

// Montgomery form of prime field element 1 ( = 767 )
constexpr uint32_t R = RADIX % Q;

// Required for converting an integer to its Montgomery Form ( = 2385 )
constexpr uint32_t R2 = (R * R) % Q;

// Compile-time computable constant μ, required for division less Montgomery
// Multiplication ( with modulo reduction ), follows algo 3 of ia.cr/2017/1057
constexpr uint32_t
compute_mu()
{
  uint32_t y = 1u;

  for (size_t i = 2; i < RADIX_BIT_WIDTH + 1; i++) {
    const uint32_t t0 = Q * y;
    const uint32_t t1 = t0 & ((1u << i) - 1u);
    if (t1 != 1u) {
      y = y + (1u << (i - 1));
    }
  }

  return RADIX - y;
}

// Montgomery Constant μ ( = 3327 )
constexpr uint32_t MU = compute_mu();

// Prime field Zq | q = 3329, with arithmetic operations defined over it
//
// Note, this implementation makes use of Montgomery technique for modulo
// arithmetic.
struct zq_t
{
public:
  // Given an integer in Montgomery form, constructs a Zq object.
  //
  // Note, argument value must be <= RADIX !
  //
  // If you want to get a Zq object from an integer in canonical form, consider
  // using `from_canonical()` routine.
  inline constexpr zq_t(const uint32_t a = 0u) { v = a; }

  // Given an integer in canonical form, converts it to Montgormery form.
  static inline constexpr zq_t from_canonical(const uint16_t a)
  {
    return zq_t(mont_mul(a, R2));
  }

  // Given an integer in Montgomery Form, converts it to canonical form.
  inline constexpr uint32_t to_canonical() const
  {
    const uint32_t t0 = mont_mul(v, 1u);
    const uint32_t mask = -static_cast<uint32_t>(t0 >= Q);
    return t0 - (Q & mask);
  }

  // Makes underlying value ( in Montgomery Form ) available.
  inline constexpr uint32_t to_montgomery() const { return v; }

  // Returns 1 as zq_t ( in Montgomery Form )
  static inline constexpr zq_t one() { return zq_t::from_canonical(1u); }

  // Modulo addition of two Zq elements ( in Montgomery Form )
  inline constexpr zq_t operator+(const zq_t& rhs) const
  {
    constexpr uint32_t ONE = R; // = field::zq_t::one().to_montgomery()

    uint32_t r = v + rhs.v;

    r = (r >> RADIX_BIT_WIDTH) * ONE + (r & MASK);
    r = (r >> RADIX_BIT_WIDTH) * ONE + (r & MASK);

    return zq_t(r);
  }

  // Compound modulo addition of two Zq elements ( in Montgomery Form )
  inline constexpr void operator+=(const zq_t& rhs) { *this = *this + rhs; }

  // Modulo negation of Zq element ( in Montgomery Form )
  inline constexpr zq_t operator-() const
  {
    constexpr uint32_t ONE = R; // = field::zq_t::one().to_montgomery()

    uint32_t r = Q - v;
    r = (r & MASK) - ((r >> RADIX_BIT_WIDTH) & ONE);

    return zq_t(r);
  }

  // Modulo subtraction of two Zq elements ( in Montgomery Form )
  inline constexpr zq_t operator-(const zq_t& rhs) const
  {
    return *this + (-rhs);
  }

  // Compound modulo subtraction of two Zq elements ( in Montgomery Form )
  inline constexpr void operator-=(const zq_t& rhs) { *this = *this - rhs; }

  // Modulo multiplication of two Zq elements ( in Montgomery Form )
  inline constexpr zq_t operator*(const zq_t& rhs) const
  {
    return zq_t(mont_mul(v, rhs.v));
  }

  // Compound modulo multiplication of two Zq elements ( in Montgomery Form )
  inline constexpr void operator*=(const zq_t& rhs) { *this = *this * rhs; }

  // Modulo exponentiation of Zq element ( in Montgomery Form )
  inline constexpr zq_t operator^(const size_t exp) const
  {
    auto res = zq_t::one();
    const size_t bw = std::bit_width(exp);

    for (int64_t i = bw - 1; i >= 0; i--) {
      res = res * res;
      if ((exp >> i) & 1ul) {
        res = res * *this;
      }
    }

    return res;
  }

  // Multiplicative inverse of Zq element ( in Montgomery Form ). Note, if Zq
  // element is 0, we can't compute multiplicative inverse, so 0 is returned.
  inline constexpr zq_t inv() const
  {
    return *this ^ static_cast<size_t>((Q - 2));
  }

  // Modulo division of two Zq elements ( in Montgomery Form ). Note, if
  // denominator is 0, returned result is 0 too, becaue we can't compute
  // multiplicative inverse of it.
  inline constexpr zq_t operator/(const zq_t& rhs) const
  {
    return *this * rhs.inv();
  }

  // Compare two Zq elements, returning truth value in case they are same
  inline constexpr bool operator==(const zq_t& rhs) const
  {
    return this->to_canonical() == rhs.to_canonical();
  }

  // Samples a random Zq element, using pseudo random number generator.
  static inline zq_t random(prng::prng_t& prng)
  {
    uint16_t res = 0;
    prng.read(reinterpret_cast<uint8_t*>(&res), sizeof(res));
    return zq_t::from_canonical(res);
  }

private:
  // Underlying value held in this type
  uint32_t v = 0;

  // Given two integers ( in Montgomery Form ) performs Montgomery
  // multiplication ( with modulo reduction ).
  //
  // This routine can be used for converting an integer ( in canonical form ) to
  // its Montgomery form and also for getting it back to canonical form.
  //
  // Ensure atleast one of a, b is < 2**12, while other operand must be < 2**16
  // Returned value will be < 2**12
  static inline constexpr uint32_t mont_mul(const uint32_t a, const uint32_t b)
  {
    constexpr uint32_t ONE = R; // = field::zq_t::one().to_montgomery()
    const uint32_t c = a * b;

    const uint64_t t = static_cast<uint64_t>(MU) * static_cast<uint64_t>(c);
    const uint32_t q = static_cast<uint32_t>(t & static_cast<uint64_t>(MASK));

    uint32_t d = (c + (q * Q)) >> 12;

    d = (d >> RADIX_BIT_WIDTH) * ONE + (d & MASK);
    d = (d >> RADIX_BIT_WIDTH) * ONE + (d & MASK);
    d = (d >> RADIX_BIT_WIDTH) * ONE + (d & MASK);

    // now d must be <= RADIX now !
    return d;
  }
};

}
