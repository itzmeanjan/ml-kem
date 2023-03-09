#pragma once
#include "field.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Test functional correctness of Kyber prime field operations ( using
// Montgomery Arithmetic ), by running through multiple rounds of execution of
// field operations on randomly sampled field elements
inline void
test_field_ops()
{
  constexpr size_t itr_cnt = 1ul << 10;
  prng::prng_t prng;

  for (size_t i = 0; i < itr_cnt; i++) {
    const auto a = field::zq_t::random(prng);
    const auto b = field::zq_t::random(prng);

    // Addition, Subtraction and Negation
    const auto c = a + b;
    const auto d = c - b;
    const auto e = c - a;

    assert(d == a);
    assert(e == b);

    // Multiplication, Exponentiation, Inversion and Division

    const auto f = a * b;
    const auto g = f / b;
    const auto h = f / a;

    if (b != field::zq_t()) {
      assert(g == a);
    } else {
      assert(g == field::zq_t());
    }

    if (a != field::zq_t()) {
      assert(h == b);
    } else {
      assert(h == field::zq_t());
    }
  }
}

}
