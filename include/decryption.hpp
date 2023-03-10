#pragma once
#include "params.hpp"
#include "poly_vec.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace cpapke {

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
