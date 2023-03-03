#pragma once
#include "field.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Test functional correctness of Kyber prime field operations, by running
// through multiple rounds of execution of field operations on randomly
// sampled field elements
inline void
test_field_ops()
{
  constexpr size_t itr_cnt = 1ul << 10;
  prng::prng_t prng;

  for (size_t i = 0; i < itr_cnt; i++) {
    const auto a = ff::ff_t::random(prng);
    const auto b = ff::ff_t::random(prng);

    // addition, subtraction, negation
    const auto c = a - b;
    const auto d = -b;
    const auto e = a + d;

    assert(c == e);

    // multiplication, division, inversion
    const auto f = a * b;
    const auto g = f / b;

    if (b == ff::ff_t::zero()) {
      assert(g == ff::ff_t::zero());
    } else {
      assert(g == a);
    }

    // exponentiation, multiplication
    size_t exp = 0;
    prng.read(reinterpret_cast<uint8_t*>(&exp), sizeof(exp));
    exp >>= 48;

    const auto h = a ^ exp;

    auto res = ff::ff_t::one();
    for (size_t j = 0; j < exp; j++) {
      res = res * a;
    }

    assert(res == h);
  }
}

}
