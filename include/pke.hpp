#pragma once
#include "field.hpp"
#include "params.hpp"
#include "poly_vec.hpp"
#include "sampling.hpp"
#include "sha3_512.hpp"
#include "utils.hpp"
#include <array>
#include <span>

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
template<size_t k, size_t eta1>
static inline void
keygen(std::span<const uint8_t, 32> d, std::span<uint8_t, k * 12 * 32 + 32> pubkey, std::span<uint8_t, k * 12 * 32> seckey)
  requires(kyber_params::check_keygen_params(k, eta1))
{
  // step 2
  std::array<uint8_t, 64> g_out{};
  auto _g_out = std::span(g_out);

  sha3_512::sha3_512_t h512;
  h512.absorb(d);
  h512.finalize();
  h512.digest(_g_out);

  const auto rho = _g_out.template subspan<0, 32>();
  const auto sigma = _g_out.template subspan<rho.size(), 32>();

  // step 4, 5, 6, 7, 8
  std::array<field::zq_t, k * k * ntt::N> A_prime{};
  kyber_utils::generate_matrix<k, false>(A_prime, rho);

  // step 3
  uint8_t N = 0;

  // step 9, 10, 11, 12
  std::array<field::zq_t, k * ntt::N> s{};
  kyber_utils::generate_vector<k, eta1>(s, sigma, N);
  N += k;

  // step 13, 14, 15, 16
  std::array<field::zq_t, k * ntt::N> e{};
  kyber_utils::generate_vector<k, eta1>(e, sigma, N);
  N += k;

  // step 17, 18
  kyber_utils::poly_vec_ntt<k>(s);
  kyber_utils::poly_vec_ntt<k>(e);

  // step 19
  std::array<field::zq_t, k * ntt::N> t_prime{};

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, s, t_prime);
  kyber_utils::poly_vec_add_to<k>(e, t_prime);

  // step 20, 21, 22
  constexpr size_t pkoff = k * 12 * 32;
  auto _pubkey0 = pubkey.template subspan<0, pkoff>();
  auto _pubkey1 = pubkey.template subspan<pkoff, 32>();

  kyber_utils::poly_vec_encode<k, 12>(t_prime, _pubkey0);
  std::copy(rho.begin(), rho.end(), _pubkey1.begin());
  kyber_utils::poly_vec_encode<k, 12>(s, seckey);
}

// Given (k * 12 * 32 + 32) -bytes public key, 32 -bytes message ( to be
// encrypted ) and 32 -bytes random coin ( from where all randomness is
// deterministically sampled ), this routine encrypts message using
// INDCPA-secure Kyber encryption algorithm, computing compressed cipher text of
// (k * du * 32 + dv * 32) -bytes.
//
// See algorithm 5 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
static inline void
encrypt(std::span<const uint8_t, k * 12 * 32 + 32> pubkey,
        std::span<const uint8_t, 32> msg,
        std::span<const uint8_t, 32> rcoin,
        std::span<uint8_t, k * du * 32 + dv * 32> enc)
  requires(kyber_params::check_encrypt_params(k, eta1, eta2, du, dv))
{
  // step 2, 3
  constexpr size_t pkoff = k * 12 * 32;
  auto _pubkey0 = pubkey.template subspan<0, pkoff>();
  auto rho = pubkey.template subspan<pkoff, 32>();

  std::array<field::zq_t, k * ntt::N> t_prime{};
  kyber_utils::poly_vec_decode<k, 12>(_pubkey0, t_prime);

  // step 4, 5, 6, 7, 8
  std::array<field::zq_t, k * k * ntt::N> A_prime{};
  kyber_utils::generate_matrix<k, true>(A_prime, rho);

  // step 1
  uint8_t N = 0;

  // step 9, 10, 11, 12
  std::array<field::zq_t, k * ntt::N> r{};
  kyber_utils::generate_vector<k, eta1>(r, rcoin, N);
  N += k;

  // step 13, 14, 15, 16
  std::array<field::zq_t, k * ntt::N> e1{};
  kyber_utils::generate_vector<k, eta2>(e1, rcoin, N);
  N += k;

  // step 17
  std::array<field::zq_t, ntt::N> e2{};
  kyber_utils::generate_vector<1, eta2>(e2, rcoin, N);

  // step 18
  kyber_utils::poly_vec_ntt<k>(r);

  // step 19
  std::array<field::zq_t, k * ntt::N> u{};

  kyber_utils::matrix_multiply<k, k, k, 1>(A_prime, r, u);
  kyber_utils::poly_vec_intt<k>(u);
  kyber_utils::poly_vec_add_to<k>(e1, u);

  // step 20
  std::array<field::zq_t, ntt::N> v{};

  kyber_utils::matrix_multiply<1, k, k, 1>(t_prime, r, v);
  kyber_utils::poly_vec_intt<1>(v);
  kyber_utils::poly_vec_add_to<1>(e2, v);

  std::array<field::zq_t, ntt::N> m{};
  kyber_utils::decode<1>(msg, m);
  kyber_utils::poly_decompress<1>(m);
  kyber_utils::poly_vec_add_to<1>(m, v);

  constexpr size_t encoff = k * du * 32;
  auto _enc0 = enc.template subspan<0, encoff>();
  auto _enc1 = enc.template subspan<encoff, dv * 32>();

  // step 21
  kyber_utils::poly_vec_compress<k, du>(u);
  kyber_utils::poly_vec_encode<k, du>(u, _enc0);

  // step 22
  kyber_utils::poly_compress<dv>(v);
  kyber_utils::encode<dv>(v, _enc1);
}

// Given (k * 12 * 32) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( cipher ) text, this routine recovers 32 -bytes plain text which
// was encrypted using respective public key, which is associated with this
// secret key.
//
// See algorithm 6 defined in Kyber specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
template<size_t k, size_t du, size_t dv>
static inline void
decrypt(std::span<const uint8_t, k * 12 * 32> seckey, std::span<const uint8_t, k * du * 32 + dv * 32> enc, std::span<uint8_t, 32> dec)
  requires(kyber_params::check_decrypt_params(k, du, dv))
{
  constexpr size_t encoff = k * du * 32;
  auto _enc0 = enc.template subspan<0, encoff>();
  auto _enc1 = enc.template subspan<encoff, dv * 32>();

  // step 1
  std::array<field::zq_t, k * ntt::N> u{};

  kyber_utils::poly_vec_decode<k, du>(_enc0, u);
  kyber_utils::poly_vec_decompress<k, du>(u);

  // step 2
  std::array<field::zq_t, ntt::N> v{};

  kyber_utils::decode<dv>(_enc1, v);
  kyber_utils::poly_decompress<dv>(v);

  // step 3
  std::array<field::zq_t, k * ntt::N> s_prime{};
  kyber_utils::poly_vec_decode<k, 12>(seckey, s_prime);

  // step 4
  kyber_utils::poly_vec_ntt<k>(u);

  std::array<field::zq_t, ntt::N> t{};

  kyber_utils::matrix_multiply<1, k, k, 1>(s_prime, u, t);
  kyber_utils::poly_vec_intt<1>(t);
  kyber_utils::poly_vec_sub_from<1>(t, v);

  kyber_utils::poly_compress<1>(v);
  kyber_utils::encode<1>(v, dec);
}

}
