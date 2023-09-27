#include "serialize.hpp"
#include <gtest/gtest.h>
#include <vector>

// Ensure that degree-255 polynomial serialization to byte array ( of length
// 32*l -bytes ) and deserialization of that byte array to degree-255 polynomial
// works as expected for parameterizable values of l | l ∈ [1, 12].
//
// l denotes significant bit width ( from LSB side ) for each coefficient of
// polynomial.
template<size_t l>
void
test_serialize_deserialize()
{
  constexpr size_t blen = (ntt::N * l) / 8;
  constexpr uint32_t mask = (1u << l) - 1u;

  std::vector<field::zq_t> src(ntt::N);
  std::vector<field::zq_t> dst(ntt::N);
  std::vector<uint8_t> bytes(blen);

  prng::prng_t prng;

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = field::zq_t::random(prng);
  }

  kyber_utils::encode<l>(src, bytes);
  kyber_utils::decode<l>(bytes, dst);

  for (size_t i = 0; i < ntt::N; i++) {
    EXPECT_EQ((src[i].to_canonical() & mask), (dst[i].to_canonical() & mask));
  }
}

TEST(KyberKEM, PolynomialSerialization)
{
  test_serialize_deserialize<12>();
  test_serialize_deserialize<11>();
  test_serialize_deserialize<10>();
  test_serialize_deserialize<5>();
  test_serialize_deserialize<4>();
  test_serialize_deserialize<1>();
}
