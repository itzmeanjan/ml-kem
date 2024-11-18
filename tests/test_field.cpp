#include "ml_kem/internals/math/field.hpp"
#include "randomshake/randomshake.hpp"
#include <gtest/gtest.h>

// Test functional correctness of ML-KEM prime field operations, by running through multiple rounds
// of execution of field operations on randomly sampled field elements.
TEST(ML_KEM, ArithmeticOverZq)
{
  constexpr size_t ITERATION_COUNT = 1ul << 20;

  randomshake::randomshake_t<128> csprng{};

  for (size_t i = 0; i < ITERATION_COUNT; i++) {
    const auto a = ml_kem_field::zq_t::random(csprng);
    const auto b = ml_kem_field::zq_t::random(csprng);

    // Addition, Subtraction and Negation
    const auto c = a + b;
    const auto d = c - b;
    const auto e = c - a;

    EXPECT_EQ(d, a);
    EXPECT_EQ(e, b);

    // Multiplication, Exponentiation, Inversion and Division
    const auto f = a * b;
    const auto g = f / b;
    const auto h = f / a;

    if (b != ml_kem_field::zq_t::zero()) {
      EXPECT_EQ(g, a);
    } else {
      EXPECT_EQ(g, ml_kem_field::zq_t());
    }

    if (a != ml_kem_field::zq_t::zero()) {
      EXPECT_EQ(h, b);
    } else {
      EXPECT_EQ(h, ml_kem_field::zq_t());
    }
  }
}
