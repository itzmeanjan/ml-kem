#pragma once
#include "compression.hpp"
#include <cassert>

// Test functional correctness of Kyber PQC suite implementation
namespace test_kyber {

// Test functional correctness of compression/ decompression logic s.t. given an
// element x ∈ Z_q following is satisfied
//
// x' = decompress(compress(x, d), d)
//
// |(x' - x) mod q| <= round(q / 2 ^ (d + 1))
//
// This test is executed a few times on some random Z_q elements, for some
// specified `d`.
template<const size_t d>
void
test_compression()
{
  constexpr size_t cnt = 1024;
  prng::prng_t prng;

  for (size_t i = 0; i < cnt; i++) {
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

    assert(err <= terr);
  }
}

}
