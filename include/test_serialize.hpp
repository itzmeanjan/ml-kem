#pragma once
#include "serialize.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Ensure that degree-255 polynomial serialization to byte array ( of length
// 32*l -bytes ) and deserialization of that byte array to degree-255 polynomial
// works as expected for parameterizable values of l | l ∈ [1, 12]
//
// l denotes significant bit width ( from LSB side ) for each coefficient of
// polynomial.
template<const size_t l>
static void
test_serialization()
{
  constexpr size_t plen = sizeof(ff::ff_t) * ntt::N;
  constexpr size_t blen = 32 * l;
  constexpr uint16_t mask = (1u << l) - 1u;

  ff::ff_t* src = static_cast<ff::ff_t*>(std::malloc(plen));
  uint8_t* arr = static_cast<uint8_t*>(std::malloc(blen));
  ff::ff_t* dst = static_cast<ff::ff_t*>(std::malloc(plen));

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = ff::ff_t::random();
  }

  indcpa::encode<l>(src, arr);
  indcpa::decode<l>(arr, dst);

  for (size_t i = 0; i < ntt::N; i++) {
    assert((src[i].v & mask) == (dst[i].v & mask));
  }

  std::free(src);
  std::free(arr);
  std::free(dst);
}

}
