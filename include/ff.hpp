#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <ostream>
#include <random>

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

// Primitive Element of Prime field
//
// $ python3
// >>> import galois as gl
// >>> gf = gl.GF(3329)
// >>> gf.primitive_element
// GF(3, order=3329)
constexpr uint16_t GENERATOR = 3;

// Two Adicity of Prime Field
//
// $ python3
// >>> assert is_odd((Q - 1) >> k) | k = 8
constexpr uint16_t TWO_ADICITY = 8;

// Two adic root of unity
//
// $ python3
// >>> import galois as gl
// >>> gf = gl.GF(3329)
// >>> k = (gf.order - 1) >> 8
// >>> gf.primitive_element ** k
// GF(3061, order=3329)
constexpr uint16_t TWO_ADIC_ROOT_OF_UNITY = 3061;

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

  inline constexpr ff_t(const uint16_t a = 0) { v = a % Q; }

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

  // Generate a random prime field element a | a ∈ [0, Q)
  static inline ff_t random()
  {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint16_t> dis{ 0, Q - 1 };

    return ff_t{ dis(gen) };
  }

  // Writes field element into output stream, used for debugging purposes
  friend inline std::ostream& operator<<(std::ostream& os, const ff_t& elm);
};

inline std::ostream&
operator<<(std::ostream& os, const ff_t& elm)
{
  return os << "ff_q(" << elm.v << ", " << Q << ")";
}

}
