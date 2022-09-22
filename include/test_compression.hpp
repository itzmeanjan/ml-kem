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
static void
test_compression()
{
  constexpr size_t cnt = 1024;

  for (size_t i = 0; i < cnt; i++) {
    const auto a = ff::ff_t::random();

    const auto b = indcpa::compress<d>(a);
    const auto c = indcpa::decompress<d>(b);

    const uint16_t br0[]{ static_cast<uint16_t>(ff::Q - c.v), c.v };
    const bool flg0 = c.v <= (ff::Q >> 1);
    const auto c_prime = static_cast<int32_t>(br0[flg0]);

    const uint16_t br1[]{ static_cast<uint16_t>(ff::Q - a.v), a.v };
    const bool flg1 = a.v <= (ff::Q >> 1);
    const auto a_prime = static_cast<int32_t>(br1[flg1]);

    const size_t err = static_cast<size_t>(std::abs(c_prime - a_prime));
    const size_t terr = indcpa::compute_error<d>();

    assert(err <= terr);
  }
}

}
