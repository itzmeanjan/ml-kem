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
void
test_serialization()
{
  constexpr size_t plen = sizeof(field::zq_t) * ntt::N;
  constexpr size_t blen = 32 * l;
  constexpr uint32_t mask = (1u << l) - 1u;

  field::zq_t* src = static_cast<field::zq_t*>(std::malloc(plen));
  uint8_t* arr = static_cast<uint8_t*>(std::malloc(blen));
  field::zq_t* dst = static_cast<field::zq_t*>(std::malloc(plen));

  prng::prng_t prng;

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = field::zq_t::random(prng);
  }

  kyber_utils::encode<l>(src, arr);
  kyber_utils::decode<l>(arr, dst);

  for (size_t i = 0; i < ntt::N; i++) {
    assert((src[i].to_canonical() & mask) == (dst[i].to_canonical() & mask));
  }

  std::free(src);
  std::free(arr);
  std::free(dst);
}

}
