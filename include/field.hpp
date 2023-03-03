#pragma once
#include "prng.hpp"
#include <array>
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
  // Default constructor s.t. value held is 0
  inline constexpr zq_t() = default;

  // Given a 32 -bit unsigned integer, converts it to Montgomery Form
  inline constexpr zq_t(const uint32_t a) { v = mont_mul(a, R2); }

  // Given integer in Montgomery Form, converts it to canonical form.
  inline constexpr uint32_t to_canonical() const { return mont_mul(v, 1u); }

  // Makes underlying value ( in Montgomery Form ) available.
  inline constexpr uint32_t in_mont_form() const { return v; }

  // Returns 1 as zq_t ( in Montgomery Form )
  static inline constexpr zq_t one() { return zq_t(1u); }

  // Modulo Addition of two Zq elements ( in Montgomery Form )
  inline constexpr zq_t operator+(const zq_t& rhs) const
  {
    const uint32_t r = v + rhs.v;
    constexpr uint32_t ONE = R; // = field::zq_t::one().in_mont_form()

    const uint32_t carry = r >> 12;
    uint32_t c = r & MASK;
    c = carry * ONE + c;

    auto ret = zq_t();
    ret.v = c;

    return ret;
  }

private:
  // Underlying value held in this type
  uint32_t v = 0;

  // Given two 32 -bit unsigned integers ( in Montgomery Form ) this routine
  // performs Montgomery Multiplication ( with modulo reduction ).
  //
  // This routine can be used for converting an integer ( in canonical form ) to
  // its Montgomery form and also for getting it back to canonical form.
  static inline constexpr uint32_t mont_mul(const uint32_t a, const uint32_t b)
  {
    const uint32_t c = a * b;

    const uint64_t t = static_cast<uint64_t>(MU) * static_cast<uint64_t>(c);
    const uint32_t q = static_cast<uint32_t>(t & static_cast<uint64_t>(MASK));

    const uint32_t d = (c + (q * Q)) >> 12;
    constexpr uint32_t ONE = R; // = field::zq_t::one().in_mont_form()

    uint32_t carry = d >> 12;
    uint32_t e = d & MASK;

    while (carry > 0) {
      e = carry * ONE + e;

      carry = e >> 12;
      e = e & MASK;
    }

    return e;
  }
};

}

// Prime field arithmetic over F_q, for Kyber PQC Algorithm s.t. q = 3329
namespace ff {

// Kyber Prime Field Modulus
constexpr uint16_t Q = (1 << 8) * 13 + 1;

// Precomputed Barrett Reduction Constant
//
// Note,
//
// k = ceil(log2(Q)) = 12
// r = floor((1 << 2k) / Q) = 5039
//
// See https://www.nayuki.io/page/barrett-reduction-algorithm for more.
constexpr uint16_t R = 5039;

// Extended GCD algorithm for computing inverse of prime ( = Q ) field element
//
// Taken from
// https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L40-L67
static inline constexpr std::array<int16_t, 3>
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

// Element of prime field F_q | q = 3329, with arithmetic operations defined
// over it
struct ff_t
{
  // Value of field element | v ∈ [0, Q)
  uint16_t v = 0;

  inline constexpr ff_t() = default;

  inline constexpr ff_t(const uint16_t a) { v = a % Q; }

  // Generate field element having canonical value 0
  static inline ff_t zero() { return ff_t{ 0 }; }

  // Generate field element having canonical value 1
  static inline ff_t one() { return ff_t{ 1 }; }

  // Computes canonical form of prime field addition
  inline constexpr ff_t operator+(const ff_t& rhs) const
  {
    const uint16_t t0 = this->v + rhs.v;
    const bool flg = t0 >= Q;
    const uint16_t t1 = t0 - flg * Q;

    return ff_t{ t1 };
  }

  // Computes canonical form of prime field compound addition
  inline constexpr void operator+=(const ff_t& rhs)
  {
    const uint16_t t0 = this->v + rhs.v;
    const bool flg = t0 >= Q;
    const uint16_t t1 = t0 - flg * Q;

    this->v = t1;
  }

  // Computes canonical form of prime field subtraction
  inline constexpr ff_t operator-(const ff_t& rhs) const
  {
    const uint16_t t0 = (Q + this->v) - rhs.v;
    const bool flg = t0 >= Q;
    const uint16_t t1 = t0 - flg * Q;

    return ff_t{ t1 };
  }

  // Computes canonical form of prime field compound subtraction
  inline constexpr void operator-=(const ff_t& rhs)
  {
    const uint16_t t0 = (Q + this->v) - rhs.v;
    const bool flg = t0 >= Q;
    const uint16_t t1 = t0 - flg * Q;

    this->v = t1;
  }

  // Computes canonical form of prime field negation
  inline constexpr ff_t operator-() const
  {
    const uint16_t tmp = Q - this->v;
    return ff_t{ tmp };
  }

  // Computes canonical form of prime field multiplication
  //
  // Note, after multiplying two 12 -bit numbers resulting 24 -bit number is
  // reduced to Z_q using Barrett reduction algorithm, which avoid division by
  // any value which is not power of 2 | q = 3329.
  //
  // See https://www.nayuki.io/page/barrett-reduction-algorithm for Barrett
  // reduction algorithm
  inline constexpr ff_t operator*(const ff_t& rhs) const
  {
    const uint32_t t0 = static_cast<uint32_t>(this->v);
    const uint32_t t1 = static_cast<uint32_t>(rhs.v);
    const uint32_t t2 = t0 * t1;

    const uint64_t t3 = static_cast<uint64_t>(t2) * static_cast<uint64_t>(R);
    const uint32_t t4 = static_cast<uint32_t>(t3 >> 24);
    const uint32_t t5 = t4 * static_cast<uint32_t>(Q);
    const uint16_t t6 = static_cast<uint16_t>(t2 - t5);

    const bool flg = t6 >= Q;
    const uint16_t t7 = t6 - flg * Q;

    return ff_t{ t7 };
  }

  // Computes canonical form of compound modular multiplication
  // over Z_q | q = 3329
  inline constexpr void operator*=(const ff_t& rhs)
  {
    const uint32_t t0 = static_cast<uint32_t>(this->v);
    const uint32_t t1 = static_cast<uint32_t>(rhs.v);
    const uint32_t t2 = t0 * t1;

    const uint64_t t3 = static_cast<uint64_t>(t2) * static_cast<uint64_t>(R);
    const uint32_t t4 = static_cast<uint32_t>(t3 >> 24);
    const uint32_t t5 = t4 * static_cast<uint32_t>(Q);
    const uint16_t t6 = static_cast<uint16_t>(t2 - t5);

    const bool flg = t6 >= Q;
    const uint16_t t7 = t6 - flg * Q;

    this->v = t7;
  }

  // Computes canonical form of multiplicative inverse of prime field element,
  // where a ∈ F_q | q = 3329; ensure a ∈ [1, q)
  //
  // Say return value of this function is b, then
  //
  // assert (a * b) % q == 1
  //
  // Note, when operand is zero value, multiplicative inverse can't be computed
  // -- so zero value is returned.
  //
  // Taken from
  // https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L69-L94
  inline constexpr ff_t inv() const
  {
    const bool flg0 = this->v == 0;
    const uint16_t t0 = this->v + flg0 * 1;

    auto res = xgcd(t0, Q);

    const bool flg1 = res[0] < 0;
    const uint16_t t1 = static_cast<uint16_t>(flg1 * Q + res[0]);
    const bool flg2 = t1 >= Q;
    const uint16_t t2 = t1 - flg2 * Q;
    const uint16_t t3 = t2 - flg0 * 1;

    return ff_t{ t3 };
  }

  // Computes canonical form of prime field division
  inline constexpr ff_t operator/(const ff_t& rhs) const
  {
    return (*this) * rhs.inv();
  }

  // Raises field element to N -th power ( result kept in canonical form ),
  // using exponentiation by repeated squaring rule
  //
  // Taken from
  // https://github.com/itzmeanjan/falcon/blob/45b0593215c3f2ec550860128299b123885b3a42/include/ff.hpp#L153-L181
  inline constexpr ff_t operator^(const size_t n) const
  {
    ff_t base = *this;

    const ff_t br[]{ ff_t{ 1 }, base };
    ff_t r = br[n & 0b1];

    const size_t zeros = std::countl_zero(n);

    for (size_t i = 1; i < 64 - zeros; i++) {
      base = base * base;

      const ff_t br[]{ ff_t{ 1 }, base };
      r = r * br[(n >> i) & 0b1];
    }

    return r;
  }

  // Checks whether two prime field elements are holding same canonical value,
  // returning boolean result
  inline constexpr bool operator==(const ff_t& rhs) const
  {
    return !static_cast<bool>(this->v ^ rhs.v);
  }

  // Generate a random field element ∈ Z_q
  static inline ff_t random(prng::prng_t& prng)
  {
    uint16_t res = 0;

    for (size_t i = 0; i < (1ul << 10); i++) {
      uint16_t v = 0;
      prng.read(reinterpret_cast<uint8_t*>(&v), sizeof(res));

      if (v < ff::Q) {
        res = v;
        break;
      }
    }

    return ff_t{ res };
  }
};

}
