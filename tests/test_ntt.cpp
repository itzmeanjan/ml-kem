#include "kyber/internals/math/field.hpp"
#include "kyber/internals/poly/ntt.hpp"
#include <gtest/gtest.h>
#include <vector>

// Ensure functional correctness of (inverse) NTT implementation for degree-255
// polynomial over F_q | q = 3329, using following rule
//
// f <- random polynomial
// f' <- ntt(f)
// f'' <- intt(f')
//
// assert(f == f'')
TEST(KyberKEM, NumberTheoreticTransform)
{
  std::vector<field::zq_t> poly_a(ntt::N);
  std::vector<field::zq_t> poly_b(ntt::N);

  auto _poly_a = std::span<field::zq_t, ntt::N>(poly_a);
  auto _poly_b = std::span<field::zq_t, ntt::N>(poly_b);

  prng::prng_t prng;

  for (size_t i = 0; i < ntt::N; i++) {
    _poly_a[i] = field::zq_t::random(prng);
  }
  std::copy(_poly_a.begin(), _poly_a.end(), _poly_b.begin());

  ntt::ntt(_poly_b);
  ntt::intt(_poly_b);

  EXPECT_EQ(poly_a, poly_b);
}
