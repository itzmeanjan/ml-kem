#pragma once
#include "ntt.hpp"
#include <cassert>

// Test functional correctness of (inverse) NTT implementation
namespace test_ntt {

// Ensure functional correctness of (inverse) NTT implementation for degree-255
// polynomial over F_q | q = 3329, using following rule
//
// f -> random polynomial
// f' = ntt(f)
// f'' = intt(f')
//
// assert(f == f'')
static void
test_ntt_intt()
{
  constexpr size_t poly_len = sizeof(ff::ff_t) * ntt::N;

  ff::ff_t* poly_a = static_cast<ff::ff_t*>(std::malloc(poly_len));
  ff::ff_t* poly_b = static_cast<ff::ff_t*>(std::malloc(poly_len));
  ff::ff_t* poly_c = static_cast<ff::ff_t*>(std::malloc(poly_len));

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = ff::ff_t::random();
  }

  ntt::ntt(poly_a, poly_b);
  ntt::intt(poly_b, poly_c);

  for (size_t i = 0; i < ntt::N; i++) {
    assert(poly_a[i] == poly_c[i]);
  }

  std::free(poly_a);
  std::free(poly_b);
  std::free(poly_c);
}

}
