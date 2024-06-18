#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/serialize.hpp"
#include <cstdint>
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

  prng::prng_t<256> prng{};

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = field::zq_t::random(prng);
  }

  using poly_t = std::span<field::zq_t, ntt::N>;
  using serialized_t = std::span<uint8_t, blen>;

  ml_kem_utils::encode<l>(poly_t(src), serialized_t(bytes));
  ml_kem_utils::decode<l>(serialized_t(bytes), poly_t(dst));

  for (size_t i = 0; i < ntt::N; i++) {
    EXPECT_EQ((src[i].raw() & mask), (dst[i].raw() & mask));
  }
}

TEST(Ml_kemKEM, PolynomialSerialization)
{
  test_serialize_deserialize<12>();
  test_serialize_deserialize<11>();
  test_serialize_deserialize<10>();
  test_serialize_deserialize<5>();
  test_serialize_deserialize<4>();
  test_serialize_deserialize<1>();
}
