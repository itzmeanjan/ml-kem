#pragma once
#include "ntt.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Ensure functional correctness of (inverse) NTT implementation for degree-255
// polynomial over F_q | q = 3329, using following rule
//
// f -> random polynomial
// f' = ntt(f)
// f'' = intt(f')
//
// assert(f == f'')
inline void
test_ntt_intt()
{
  constexpr size_t poly_len = sizeof(field::zq_t) * ntt::N;

  field::zq_t* poly_a = static_cast<field::zq_t*>(std::malloc(poly_len));
  field::zq_t* poly_b = static_cast<field::zq_t*>(std::malloc(poly_len));

  prng::prng_t prng;

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = field::zq_t::random(prng);
  }

  std::memcpy(poly_b, poly_a, poly_len);

  ntt::ntt(poly_b);
  ntt::intt(poly_b);

  for (size_t i = 0; i < ntt::N; i++) {
    assert(poly_a[i] == poly_b[i]);
  }

  std::free(poly_a);
  std::free(poly_b);
}

}
