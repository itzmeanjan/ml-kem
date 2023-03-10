#pragma once
#include "params.hpp"
#include "poly_vec.hpp"
#include "sampling.hpp"
#include "sha3_512.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace cpapke {

// Kyber CPAPKE key generation algorithm, which takes two parameters `k` & `η1`
// ( read eta1 ) and generates byte serialized public key and secret key of
// following length
//
// public key: (k * 12 * 32 + 32) -bytes wide
// secret key: (k * 12 * 32) -bytes wide
//
// See algorithm 4 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
//
// Note, this routine allows you to pass 32 -bytes seed ( see first parameter ),
// which is designed this way for ease of writing test cases against known
// answer tests, obtained from Kyber reference implementation
// https://github.com/pq-crystals/kyber.git. It also helps in properly
// benchmarking underlying PKE's key generation implementation.
template<const size_t k, const size_t eta1>
static inline void
keygen(const uint8_t* const __restrict d, // 32 -bytes seed
       uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // k * 12 * 32 -bytes secret key
       )
  requires(kyber_params::check_keygen_params(k, eta1))
{
  constexpr size_t dlen = 32;

  // step 2
  uint8_t g_out[64]{};
  sha3_512::hash(d, dlen, g_out);

  const uint8_t* rho = g_out + 0;
  const uint8_t* sigma = g_out + 32;

  // step 4, 5, 6, 7, 8
  field::zq_t A_prime[k * k * ntt::N]{};
  kyber_utils::generate_matrix<k, false>(A_prime, rho);

  // step 3
  uint8_t N = 0;

  // step 9, 10, 11, 12
  field::zq_t s[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(s, sigma, N);
  N += k;

  // step 13, 14, 15, 16
  field::zq_t e[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(e, sigma, N);
  N += k;

  // step 17, 18
  kyber_utils::poly_vec_ntt<k>(s);
  kyber_utils::poly_vec_ntt<k>(e);

  // step 19
  field::zq_t t_prime[k * ntt::N]{};

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, s, t_prime);
  kyber_utils::poly_vec_add_to<k>(e, t_prime);

  // step 20, 21, 22
  kyber_utils::poly_vec_encode<k, 12>(t_prime, pubkey);
  kyber_utils::poly_vec_encode<k, 12>(s, seckey);

  constexpr size_t pkoff = k * 12 * 32;
  std::memcpy(pubkey + pkoff, rho, 32);
}

}
