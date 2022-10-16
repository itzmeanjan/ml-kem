#pragma once
#include "compression.hpp"
#include "ntt.hpp"
#include "poly_vec.hpp"
#include "sampling.hpp"
#include "serialize.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace cpapke {

// Given (k * 12 * 32 + 32) -bytes public key, 32 -bytes message ( to be
// encrypted ) and 32 -bytes random coin ( from where all randomness is
// deterministically sampled ), this routine encrypts message using
// INDCPA-secure Kyber encryption algorithm, computing compressed cipher text of
// (k * du * 32 + dv * 32) -bytes.
//
// See algorithm 5 defined in Kyber specification, as submitted to NIST PQC
// final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
static void
encrypt(const uint8_t* const __restrict pubkey, // (k * 12 * 32 + 32) -bytes
        const uint8_t* const __restrict msg,    // 32 -bytes message
        const uint8_t* const __restrict rcoin,  // 32 -bytes random coin
        uint8_t* const __restrict enc           // k * du * 32 + dv * 32 -bytes
)
{
  // step 2
  ff::ff_t t_prime[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t toff = i * ntt::N;
    const size_t pkoff = i * 12 * 32;

    kyber_utils::decode<12>(pubkey + pkoff, t_prime + toff);
  }

  // step 3
  constexpr size_t pkoff = k * 12 * 32;
  const uint8_t* const rho = pubkey + pkoff;

  // step 4, 5, 6, 7, 8
  uint8_t xof_in[34]{};
  std::memcpy(xof_in, rho, sizeof(xof_in) - 2);

  ff::ff_t A_prime[k * k * ntt::N]{};
  kyber_utils::generate_matrix<k, true>(A_prime, xof_in);

  // step 1
  uint8_t N = 0;

  // step 9, 10, 11, 12
  uint8_t prf_in[33]{};
  std::memcpy(prf_in, rcoin, sizeof(prf_in) - 1);

  ff::ff_t r[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta1>(r, prf_in, N);
  N += k;

  // step 13, 14, 15, 16
  ff::ff_t e1[k * ntt::N]{};
  kyber_utils::generate_vector<k, eta2>(e1, prf_in, N);
  N += k;

  // step 17
  ff::ff_t e2[ntt::N]{};
  kyber_utils::generate_vector<1, eta2>(e2, prf_in, N);

  // step 18
  kyber_utils::poly_vec_ntt<k>(r);

  // step 19
  ff::ff_t u[k * ntt::N]{};
  std::memset(u, 0, sizeof(u));

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, r, u);
  kyber_utils::poly_vec_intt<k>(u);

  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    const size_t e1off = i * ntt::N;

    for (size_t l = 0; l < ntt::N; l++) {
      u[uoff + l] += e1[e1off + l];
    }
  }

  // step 20
  ff::ff_t v[ntt::N]{};
  std::memset(v, 0, sizeof(v));

  kyber_utils::matrix_multiply<1, k, k, 1>(t_prime, r, v);
  kyber_utils::poly_vec_intt<1>(v);

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] += e2[i];
  }

  ff::ff_t m[ntt::N]{};
  kyber_utils::decode<1>(msg, m);
  kyber_utils::poly_decompress<1>(m);

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] += m[i];
  }

  // step 21
  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    const size_t encoff = i * du * 32;

    kyber_utils::poly_compress<du>(u + uoff);
    kyber_utils::encode<du>(u + uoff, enc + encoff);
  }

  // step 22
  constexpr size_t encoff = k * du * 32;

  kyber_utils::poly_compress<dv>(v);
  kyber_utils::encode<dv>(v, enc + encoff);
}

}
