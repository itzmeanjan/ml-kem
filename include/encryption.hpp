#pragma once
#include "compression.hpp"
#include "ntt.hpp"
#include "sampling.hpp"
#include "serialize.hpp"
#include "shake256.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace indcpa {

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

    decode<12>(pubkey + pkoff, t_prime + toff);
  }

  // step 3
  constexpr size_t pkoff = k * 12 * 32;
  const uint8_t* const rho = pubkey + pkoff;

  // step 4, 5, 6, 7, 8
  uint8_t xof_in[34]{};
  std::memcpy(xof_in, rho, sizeof(xof_in) - 2);

  ff::ff_t A_prime[k * k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < k; j++) {
      const size_t off = (i * k + j) * ntt::N;

      xof_in[32] = static_cast<uint8_t>(i);
      xof_in[33] = static_cast<uint8_t>(j);

      shake128::shake128 hasher{};
      hasher.hash(xof_in, sizeof(xof_in));

      indcpa::parse(&hasher, A_prime + off);
    }
  }

  // step 1
  uint8_t N = 0;

  // step 9, 10, 11, 12
  uint8_t prf_in[33]{};
  std::memcpy(prf_in, rcoin, sizeof(prf_in) - 1);

  uint8_t prf_out_eta1[64 * eta1]{};
  uint8_t prf_out_eta2[64 * eta2]{};

  ff::ff_t r[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    prf_in[32] = N;

    shake256::shake256 hasher{};
    hasher.hash(prf_in, sizeof(prf_in));
    hasher.read(prf_out_eta1, sizeof(prf_out_eta1));

    cbd<eta1>(prf_out_eta1, r + off);

    N += 1;
  }

  // step 13, 14, 15, 16
  ff::ff_t e1[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    prf_in[32] = N;

    shake256::shake256 hasher{};
    hasher.hash(prf_in, sizeof(prf_in));
    hasher.read(prf_out_eta2, sizeof(prf_out_eta2));

    cbd<eta2>(prf_out_eta2, e1 + off);

    N += 1;
  }

  // step 17
  ff::ff_t e2[ntt::N]{};

  prf_in[32] = N;

  shake256::shake256 hasher{};
  hasher.hash(prf_in, sizeof(prf_in));
  hasher.read(prf_out_eta2, sizeof(prf_out_eta2));

  cbd<eta2>(prf_out_eta2, e2);

  // step 18
  ff::ff_t r_prime[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;
    ntt::ntt(r + off, r_prime + off);
  }

  // step 19
  ff::ff_t u[k * ntt::N]{};
  std::memset(u, 0, sizeof(u));

  ff::ff_t tmp0[ntt::N]{};
  ff::ff_t tmp1[ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    const size_t e1off = i * ntt::N;

    for (size_t j = 0; j < k; j++) {
      const size_t aoff = (i * k + j) * ntt::N;
      const size_t roff = j * ntt::N;

      ntt::polymul(A_prime + aoff, r_prime + roff, tmp0);

      for (size_t l = 0; l < ntt::N; l++) {
        u[uoff + l] += tmp0[l];
      }
    }

    ntt::intt(u + uoff, tmp1);
    std::memcpy(u + uoff, tmp1, sizeof(tmp1));

    for (size_t l = 0; l < ntt::N; l++) {
      u[uoff + l] += e1[e1off + l];
    }
  }

  // step 20
  ff::ff_t v[ntt::N]{};
  std::memset(v, 0, sizeof(v));

  for (size_t i = 0; i < k; i++) {
    const size_t toff = i * ntt::N;
    const size_t roff = i * ntt::N;

    ntt::polymul(t_prime + toff, r_prime + roff, tmp0);

    for (size_t l = 0; l < ntt::N; l++) {
      v[l] += tmp0[l];
    }
  }

  ntt::intt(v, tmp1);
  std::memcpy(v, tmp1, sizeof(tmp1));

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] += e2[i];
  }

  ff::ff_t m[ntt::N]{};
  indcpa::decode<1>(msg, m);

  for (size_t i = 0; i < ntt::N; i++) {
    m[i] = indcpa::decompress<1>(m[i]);
  }

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] += m[i];
  }

  // step 21
  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    const size_t encoff = i * du * 32;

    for (size_t l = 0; l < ntt ::N; l++) {
      u[uoff + l] = indcpa::compress<du>(u[uoff + l]);
    }

    encode<du>(u + uoff, enc + encoff);
  }

  // step 22
  for (size_t i = 0; i < ntt ::N; i++) {
    v[i] = indcpa::compress<dv>(v[i]);
  }

  constexpr size_t encoff = k * du * 32;
  encode<dv>(v, enc + encoff);
}

}
