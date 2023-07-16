#include "ntt.hpp"
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

  prng::prng_t prng;

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = field::zq_t::random(prng);
    poly_b[i] = poly_a[i];
  }

  ntt::ntt(poly_b.data());
  ntt::intt(poly_b.data());

  ASSERT_EQ(poly_a, poly_b);
}
