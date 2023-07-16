#pragma once
#include "params.hpp"
#include "poly_vec.hpp"
#include "sampling.hpp"
#include "sha3_512.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace pke {

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

  sha3_512::sha3_512 h512;
  h512.absorb(d, dlen);
  h512.finalize();
  h512.digest(g_out);
  h512.reset();

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

// Given (k * 12 * 32 + 32) -bytes public key, 32 -bytes message ( to be
// encrypted ) and 32 -bytes random coin ( from where all randomness is
// deterministically sampled ), this routine encrypts message using
// INDCPA-secure Kyber encryption algorithm, computing compressed cipher text of
// (k * du * 32 + dv * 32) -bytes.
//
// See algorithm 5 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static inline void
encrypt(const uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes
        const uint8_t* const __restrict msg,    // 32 -bytes message
        const uint8_t* const __restrict rcoin,  // 32 -bytes random coin
        uint8_t* const __restrict enc           // k * du * 32 + dv * 32 -bytes
        )
  requires(kyber_params::check_encrypt_params(k, eta1, eta2, du, dv))
{
  // step 2
  field::zq_t t_prime[k * ntt::N]{};
  kyber_utils::poly_vec_decode<k, 12>(pubkey, t_prime);

  // step 3
  constexpr size_t pkoff = k * 12 * 32;
  const uint8_t* const rho = pubkey + pkoff;

  // step 4, 5, 6, 7, 8
  field::zq_t A_prime[k * k * ntt::N]{};
  kyber_utils::generate_matrix<k, true>(A_prime, rho);

  // step 1
  uint8_t N = 0;

  // step 9, 10, 11, 12
  field::zq_t r[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(r, rcoin, N);
  N += k;

  // step 13, 14, 15, 16
  field::zq_t e1[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta2>(e1, rcoin, N);
  N += k;

  // step 17
  field::zq_t e2[ntt::N]{};
  kyber_utils::generate_vector<1, eta2>(e2, rcoin, N);

  // step 18
  kyber_utils::poly_vec_ntt<k>(r);

  // step 19
  field::zq_t u[k * ntt::N]{};

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, r, u);
  kyber_utils::poly_vec_intt<k>(u);
  kyber_utils::poly_vec_add_to<k>(e1, u);

  // step 20
  field::zq_t v[ntt::N]{};

  kyber_utils::matrix_multiply<1, k, k, 1>(t_prime, r, v);
  kyber_utils::poly_vec_intt<1>(v);
  kyber_utils::poly_vec_add_to<1>(e2, v);

  field::zq_t m[ntt::N]{};
  kyber_utils::decode<1>(msg, m);
  kyber_utils::poly_decompress<1>(m);
  kyber_utils::poly_vec_add_to<1>(m, v);

  // step 21
  kyber_utils::poly_vec_compress<k, du>(u);
  kyber_utils::poly_vec_encode<k, du>(u, enc);

  // step 22
  constexpr size_t encoff = k * du * 32;
  kyber_utils::poly_compress<dv>(v);
  kyber_utils::encode<dv>(v, enc + encoff);
}

// Given (k * 12 * 32) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( cipher ) text, this routine recovers 32 -bytes plain text which
// was encrypted using respective public key, which is associated with this
// secret key.
//
// See algorithm 6 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<const size_t k, const size_t du, const size_t dv>
static inline void
decrypt(
  const uint8_t* const __restrict seckey, // (k * 12 * 32) -bytes secret key
  const uint8_t* const __restrict enc,    // (k * du * 32 + dv * 32) -bytes
  uint8_t* const __restrict dec           // 32 -bytes plain text
  )
  requires(kyber_params::check_decrypt_params(k, du, dv))
{
  // step 1
  field::zq_t u[k * ntt::N]{};

  kyber_utils::poly_vec_decode<k, du>(enc, u);
  kyber_utils::poly_vec_decompress<k, du>(u);

  // step 2
  field::zq_t v[ntt::N]{};

  constexpr size_t encoff = k * du * 32;
  kyber_utils::decode<dv>(enc + encoff, v);
  kyber_utils::poly_decompress<dv>(v);

  // step 3
  field::zq_t s_prime[k * ntt::N]{};
  kyber_utils::poly_vec_decode<k, 12>(seckey, s_prime);

  // step 4
  kyber_utils::poly_vec_ntt<k>(u);

  field::zq_t t[ntt::N]{};

  kyber_utils::matrix_multiply<1, k, k, 1>(s_prime, u, t);
  kyber_utils::poly_vec_intt<1>(t);
  kyber_utils::poly_vec_sub_from<1>(t, v);

  kyber_utils::poly_compress<1>(v);
  kyber_utils::encode<1>(v, dec);
}

}
