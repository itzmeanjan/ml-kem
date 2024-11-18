#include "ml_kem/internals/poly/compression.hpp"
#include "ml_kem/internals/utility/force_inline.hpp"
#include "randomshake/randomshake.hpp"
#include <gtest/gtest.h>

// Decompression error that can happen for some given `d` s.t.
//
// x' = decompress(compress(x, d), d)
//
// |(x' - x) mod q| <= round(q / 2 ^ (d + 1))
//
// See eq. 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t d>
forceinline constexpr size_t
compute_error()
{
  constexpr double t0 = static_cast<double>(ml_kem_field::Q);
  constexpr double t1 = static_cast<double>(1ul << (d + 1));

  const size_t t2 = static_cast<size_t>(std::round(t0 / t1));
  return t2;
}

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
template<size_t d, size_t itr_cnt>
bool
test_zq_compression()
  requires(itr_cnt > 0)
{
  bool res = true;
  randomshake::randomshake_t<256> csprng{};

  for (size_t i = 0; i < itr_cnt; i++) {
    const auto a = ml_kem_field::zq_t::random(csprng);

    const auto b = ml_kem_utils::compress<d>(a);
    const auto c = ml_kem_utils::decompress<d>(b);

    const auto a_canon = a.raw();
    const auto c_canon = c.raw();

    const uint32_t br0[]{ static_cast<uint16_t>(ml_kem_field::Q - c_canon), c_canon };
    const bool flg0 = c_canon <= (ml_kem_field::Q >> 1);
    const auto c_prime = static_cast<int32_t>(br0[flg0]);

    const uint32_t br1[]{ static_cast<uint16_t>(ml_kem_field::Q - a_canon), a_canon };
    const bool flg1 = a_canon <= (ml_kem_field::Q >> 1);
    const auto a_prime = static_cast<int32_t>(br1[flg1]);

    const size_t err = static_cast<size_t>(std::abs(c_prime - a_prime));
    const size_t terr = compute_error<d>();

    res &= (err <= terr);
  }

  return res;
}

TEST(ML_KEM, CompressDecompressZq)
{
  EXPECT_TRUE((test_zq_compression<11, 1ul << 20>()));
  EXPECT_TRUE((test_zq_compression<10, 1ul << 20>()));
  EXPECT_TRUE((test_zq_compression<5, 1ul << 20>()));
  EXPECT_TRUE((test_zq_compression<4, 1ul << 20>()));
  EXPECT_TRUE((test_zq_compression<1, 1ul << 20>()));
}
