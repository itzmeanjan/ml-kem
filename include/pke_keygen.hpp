#pragma once
#include "poly_vec.hpp"
#include "sampling.hpp"
#include "sha3_512.hpp"
#include "utils.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace cpapke {

// Kyber CPAPKE key generation algorithm, which takes two parameters `k` & `η1`
// ( read eta1 ) and generates byte serialized public key and secret key of
// following length
//
// public key: (k * 12 * 32 + 32) -bytes wide
// secret key: (k * 12 * 32) -bytes wide
//
// Note, initial 32 -bytes random sampling is done pretty naively ( see
// ./utils.hpp ), it uses C++ standard library's `<random>` functionality. Also
// see https://en.cppreference.com/w/cpp/header/random for more information.
//
// See algorithm 4 defined in Kyber specification, as submitted to NIST PQC
// final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k, const size_t eta1>
static void
keygen(uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes public key
       uint8_t* const __restrict seckey  // k * 12 * 32 -bytes secret key
)
{
  // step 1
  uint8_t d[32]{};
  kyber_utils::random_data<uint8_t>(d, sizeof(d));

  // step 2
  uint8_t g_out[64]{};
  sha3_512::hash(d, sizeof(d), g_out);

  const uint8_t* const rho = g_out + 0;
  const uint8_t* const sigma = g_out + 32;

  // step 4, 5, 6, 7, 8
  uint8_t xof_in[34]{};
  std::memcpy(xof_in, rho, sizeof(g_out) >> 1);

  ff::ff_t A_prime[k * k * ntt::N]{};
  kyber_utils::generate_matrix<k, false>(A_prime, xof_in);

  // step 3
  uint8_t N = 0;

  // step 9, 10, 11, 12
  uint8_t prf_in[33]{};
  std::memcpy(prf_in, sigma, sizeof(g_out) >> 1);

  ff::ff_t s[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(s, prf_in, N);
  N += k;

  // step 13, 14, 15, 16
  ff::ff_t e[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(e, prf_in, N);
  N += k;

  // step 17, 18
  kyber_utils::poly_vec_ntt<k>(s);
  kyber_utils::poly_vec_ntt<k>(e);

  // step 19
  ff::ff_t t_prime[k * ntt::N]{};
  std::memset(t_prime, 0, sizeof(t_prime));

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, s, t_prime);
  kyber_utils::poly_vec_add_to<k>(e, t_prime);

  // step 20, 21, 22
  kyber_utils::poly_vec_encode<k, 12>(t_prime, pubkey);
  kyber_utils::poly_vec_encode<k, 12>(s, seckey);

  constexpr size_t pkoff = k * 12 * 32;
  std::memcpy(pubkey + pkoff, rho, sizeof(g_out) >> 1);
}

}
