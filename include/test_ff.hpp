#pragma once
#include "ff.hpp"
#include <cassert>

// Test functional correctness of Kyber prime field operations
namespace test_ff {

// Rounds of testing iterations to be performed, using random field elements
constexpr size_t ITR = 4096;

// Test functional correctness of Kyber prime field operations, by running
// through multiple rounds ( = ITR ) of execution of field operations on
// randomly generated field element
static void
test_field_ops()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<size_t> dis{ 0, 1ul << 16 };

  for (size_t i = 0; i < ITR; i++) {
    const auto a = ff::ff_t::random();
    const auto b = ff::ff_t::random();

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
    const size_t exp = dis(gen);
    const auto h = a ^ exp;

    auto res = ff::ff_t::one();
    for (size_t j = 0; j < exp; j++) {
      res = res * a;
    }

    assert(res == h);
  }
}

}
