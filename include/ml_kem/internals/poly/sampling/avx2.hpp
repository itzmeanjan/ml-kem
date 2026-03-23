#pragma once

#if defined(__AVX2__)

#include "ml_kem/internals/poly/sampling/constexpr.hpp"
#include "sha3/shake128_x4.hpp"
#include "sha3/shake256_x4.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

namespace ml_kem_utils {

// Rejection-sample one lane: extract two 12-bit candidates from 3 bytes and accept if < Q.
static forceinline void
reject_sample_lane(std::span<const uint8_t, 3> bytes, std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly, size_t& ci)
{
  constexpr size_t n = ml_kem_ntt::N;

  if (ci >= n) {
    return;
  }

  const uint16_t d1 = static_cast<uint16_t>((static_cast<uint16_t>(bytes[1] & 0x0f) << 8) | static_cast<uint16_t>(bytes[0]));
  const uint16_t d2 = static_cast<uint16_t>((static_cast<uint16_t>(bytes[2]) << 4) | (static_cast<uint16_t>(bytes[1] >> 4)));

  if (d1 < ml_kem_field::Q) {
    poly[ci++] = ml_kem_field::zq_t(d1);
  }

  if ((d2 < ml_kem_field::Q) && (ci < n)) {
    poly[ci++] = ml_kem_field::zq_t(d2);
  }
}

// 4-way parallel sample_ntt: runs 4 independent SHAKE128 instances simultaneously,
// performing rejection sampling on all 4 lanes in lockstep.
static forceinline void
sample_ntt_x4(shake128_x4::shake128_x4_t& hasher,
              std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly0,
              std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly1,
              std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly2,
              std::span<ml_kem_field::zq_t, ml_kem_ntt::N> poly3)
{
  constexpr size_t n = ml_kem_ntt::N;
  constexpr size_t buf_len = shake128::RATE / std::numeric_limits<uint8_t>::digits;

  size_t ci0 = 0;
  size_t ci1 = 0;
  size_t ci2 = 0;
  size_t ci3 = 0;

  std::array<uint8_t, buf_len> b0{};
  std::array<uint8_t, buf_len> b1{};
  std::array<uint8_t, buf_len> b2{};
  std::array<uint8_t, buf_len> b3{};

  const std::span<uint8_t, buf_len> s0{ b0 };
  const std::span<uint8_t, buf_len> s1{ b1 };
  const std::span<uint8_t, buf_len> s2{ b2 };
  const std::span<uint8_t, buf_len> s3{ b3 };

  while ((ci0 < n) || (ci1 < n) || (ci2 < n) || (ci3 < n)) {
    hasher.squeeze(s0, s1, s2, s3);

    for (size_t off = 0; off < buf_len; off += 3) {
      reject_sample_lane(s0.subspan(off).first<3>(), poly0, ci0);
      reject_sample_lane(s1.subspan(off).first<3>(), poly1, ci1);
      reject_sample_lane(s2.subspan(off).first<3>(), poly2, ci2);
      reject_sample_lane(s3.subspan(off).first<3>(), poly3, ci3);
    }
  }
}

// 4-way parallel matrix generation using AVX2 SHAKE128 x4.
template<size_t k, bool transpose>
void
generate_matrix_avx2(std::span<ml_kem_field::zq_t, k * k * ml_kem_ntt::N> mat, std::span<const uint8_t, 32> rho)
  requires(ml_kem_params::check_k(k))
{
  constexpr size_t total = k * k;
  constexpr size_t batch4 = (total / 4) * 4;

  using poly_t = std::span<ml_kem_field::zq_t, ml_kem_ntt::N>;

  for (size_t idx = 0; idx < batch4; idx += 4) {
    std::array<uint8_t, rho.size() + 2> xof_in0{};
    std::array<uint8_t, rho.size() + 2> xof_in1{};
    std::array<uint8_t, rho.size() + 2> xof_in2{};
    std::array<uint8_t, rho.size() + 2> xof_in3{};

    std::copy(rho.begin(), rho.end(), xof_in0.begin());
    std::copy(rho.begin(), rho.end(), xof_in1.begin());
    std::copy(rho.begin(), rho.end(), xof_in2.begin());
    std::copy(rho.begin(), rho.end(), xof_in3.begin());

    auto set_nonce = [&](std::array<uint8_t, 34>& buf, size_t flat_idx) {
      const auto i = static_cast<uint8_t>(flat_idx / k);
      const auto j = static_cast<uint8_t>(flat_idx % k);
      if constexpr (transpose) {
        buf[32] = i;
        buf[33] = j;
      } else {
        buf[32] = j;
        buf[33] = i;
      }
    };

    set_nonce(xof_in0, idx + 0);
    set_nonce(xof_in1, idx + 1);
    set_nonce(xof_in2, idx + 2);
    set_nonce(xof_in3, idx + 3);

    shake128_x4::shake128_x4_t hasher;
    hasher.absorb(xof_in0, xof_in1, xof_in2, xof_in3);
    hasher.finalize();

    sample_ntt_x4(hasher,
                  poly_t(mat.subspan((idx + 0) * ml_kem_ntt::N, ml_kem_ntt::N)),
                  poly_t(mat.subspan((idx + 1) * ml_kem_ntt::N, ml_kem_ntt::N)),
                  poly_t(mat.subspan((idx + 2) * ml_kem_ntt::N, ml_kem_ntt::N)),
                  poly_t(mat.subspan((idx + 3) * ml_kem_ntt::N, ml_kem_ntt::N)));
  }

  // Scalar remainder
  for (size_t idx = batch4; idx < total; idx++) {
    const auto i = idx / k;
    const auto j = idx % k;

    std::array<uint8_t, 34> xof_in{};
    std::copy(rho.begin(), rho.end(), xof_in.begin());

    if constexpr (transpose) {
      xof_in[32] = static_cast<uint8_t>(i);
      xof_in[33] = static_cast<uint8_t>(j);
    } else {
      xof_in[32] = static_cast<uint8_t>(j);
      xof_in[33] = static_cast<uint8_t>(i);
    }

    shake128::shake128_t hasher;
    hasher.absorb(xof_in);
    hasher.finalize();
    sample_ntt(hasher, poly_t(mat.subspan(idx * ml_kem_ntt::N, ml_kem_ntt::N)));
  }
}

// 4-way parallel vector generation using AVX2 SHAKE256 x4.
template<size_t k, size_t eta>
void
generate_vector_avx2(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec, std::span<const uint8_t, 32> sigma, const uint8_t nonce)
  requires((k == 1) || ml_kem_params::check_k(k))
{
  constexpr size_t prf_len = 64 * eta;
  constexpr size_t batch4 = (k / 4) * 4;

  using poly_t = std::span<ml_kem_field::zq_t, vec.size() / k>;

  for (size_t i = 0; i < batch4; i += 4) {
    std::array<uint8_t, sigma.size() + 1> pin0{};
    std::array<uint8_t, sigma.size() + 1> pin1{};
    std::array<uint8_t, sigma.size() + 1> pin2{};
    std::array<uint8_t, sigma.size() + 1> pin3{};

    std::copy(sigma.begin(), sigma.end(), pin0.begin());
    std::copy(sigma.begin(), sigma.end(), pin1.begin());
    std::copy(sigma.begin(), sigma.end(), pin2.begin());
    std::copy(sigma.begin(), sigma.end(), pin3.begin());

    pin0[32] = nonce + static_cast<uint8_t>(i + 0);
    pin1[32] = nonce + static_cast<uint8_t>(i + 1);
    pin2[32] = nonce + static_cast<uint8_t>(i + 2);
    pin3[32] = nonce + static_cast<uint8_t>(i + 3);

    shake256_x4::shake256_x4_t hasher;
    hasher.absorb(pin0, pin1, pin2, pin3);
    hasher.finalize();

    std::array<uint8_t, prf_len> out0{};
    std::array<uint8_t, prf_len> out1{};
    std::array<uint8_t, prf_len> out2{};
    std::array<uint8_t, prf_len> out3{};

    hasher.squeeze(out0, out1, out2, out3);

    ml_kem_utils::sample_poly_cbd<eta>(out0, poly_t(vec.subspan((i + 0) * ml_kem_ntt::N, ml_kem_ntt::N)));
    ml_kem_utils::sample_poly_cbd<eta>(out1, poly_t(vec.subspan((i + 1) * ml_kem_ntt::N, ml_kem_ntt::N)));
    ml_kem_utils::sample_poly_cbd<eta>(out2, poly_t(vec.subspan((i + 2) * ml_kem_ntt::N, ml_kem_ntt::N)));
    ml_kem_utils::sample_poly_cbd<eta>(out3, poly_t(vec.subspan((i + 3) * ml_kem_ntt::N, ml_kem_ntt::N)));
  }

  // Scalar remainder
  std::array<uint8_t, prf_len> prf_out{};
  std::array<uint8_t, sigma.size() + 1> prf_in{};
  std::copy(sigma.begin(), sigma.end(), prf_in.begin());

  for (size_t i = batch4; i < k; i++) {
    prf_in[32] = nonce + static_cast<uint8_t>(i);

    shake256::shake256_t hasher;
    hasher.absorb(prf_in);
    hasher.finalize();
    hasher.squeeze(prf_out);

    ml_kem_utils::sample_poly_cbd<eta>(prf_out, poly_t(vec.subspan(i * ml_kem_ntt::N, ml_kem_ntt::N)));
  }
}

}

#endif
