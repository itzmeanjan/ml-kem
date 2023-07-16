#include "compression.hpp"
#include <gtest/gtest.h>

// Test functional correctness of compression/ decompression logic s.t. given an
// element x ∈ Z_q following is satisfied
//
// x' = decompress(compress(x, d), d)
//
// |(x' - x) mod q| <= round(q / 2 ^ (d + 1))
//
// Returned boolean accumulates result of all compression/ decompression
// execution iterations. It must hold truth value for function caller to believe
// that compression/ decompression logic is implemented correctly.
template<const size_t d, const size_t itr_cnt>
bool
test_zq_compression()
  requires(itr_cnt > 0)
{
  bool res = true;
  prng::prng_t prng;

  for (size_t i = 0; i < itr_cnt; i++) {
    const auto a = field::zq_t::random(prng);

    const auto b = kyber_utils::compress<d>(a);
    const auto c = kyber_utils::decompress<d>(b);

    const auto a_canon = a.to_canonical();
    const auto c_canon = c.to_canonical();

    const uint32_t br0[]{ static_cast<uint16_t>(field::Q - c_canon), c_canon };
    const bool flg0 = c_canon <= (field::Q >> 1);
    const auto c_prime = static_cast<int32_t>(br0[flg0]);

    const uint32_t br1[]{ static_cast<uint16_t>(field::Q - a_canon), a_canon };
    const bool flg1 = a_canon <= (field::Q >> 1);
    const auto a_prime = static_cast<int32_t>(br1[flg1]);

    const size_t err = static_cast<size_t>(std::abs(c_prime - a_prime));
    const size_t terr = kyber_utils::compute_error<d>();

    res &= (err <= terr);
  }

  return res;
}

TEST(KyberKEM, CompressDecompressZq)
{
  ASSERT_TRUE((test_zq_compression<11, 1ul << 20>()));
  ASSERT_TRUE((test_zq_compression<10, 1ul << 20>()));
  ASSERT_TRUE((test_zq_compression<5, 1ul << 20>()));
  ASSERT_TRUE((test_zq_compression<4, 1ul << 20>()));
  ASSERT_TRUE((test_zq_compression<1, 1ul << 20>()));
}
