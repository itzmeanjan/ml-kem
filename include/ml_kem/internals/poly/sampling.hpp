#pragma once
#include "ml_kem/internals/poly/sampling/constexpr.hpp"

#if defined(__AVX2__)
#include "ml_kem/internals/poly/sampling/avx2.hpp"
#include <type_traits>
#endif

namespace ml_kem_utils {

// Generate public matrix A ( consists of degree-255 polynomials ) in NTT domain, by sampling from a XOF ( read SHAKE128 ),
// which is seeded with 32 -bytes key and two nonces ( each of 1 -byte ).
//
// See step (3-7) of algorithm 13 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t k, bool transpose>
constexpr void
generate_matrix(std::span<ml_kem_field::zq_t, k * k * ml_kem_ntt::N> mat, std::span<const uint8_t, 32> rho)
  requires(ml_kem_params::check_k(k))
{
#if defined(__AVX2__)
  if (!std::is_constant_evaluated()) {
    generate_matrix_avx2<k, transpose>(mat, rho);
    return;
  }
#endif

  generate_matrix_constexpr<k, transpose>(mat, rho);
}

// Sample a polynomial vector from Bη, following step (8-11) of algorithm 13 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
template<size_t k, size_t eta>
constexpr void
generate_vector(std::span<ml_kem_field::zq_t, k * ml_kem_ntt::N> vec, std::span<const uint8_t, 32> sigma, const uint8_t nonce)
  requires((k == 1) || ml_kem_params::check_k(k))
{
#if defined(__AVX2__)
  if (!std::is_constant_evaluated()) {
    generate_vector_avx2<k, eta>(vec, sigma, nonce);
    return;
  }
#endif

  generate_vector_constexpr<k, eta>(vec, sigma, nonce);
}

}
