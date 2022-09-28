#pragma once
#include "compression.hpp"
#include "ntt.hpp"
#include "serialize.hpp"

// IND-CPA-secure Public Key Encryption Scheme
namespace cpapke {

// Given (k * 12 * 32) -bytes secret key and (k * du * 32 + dv * 32) -bytes
// encrypted ( cipher ) text, this routine recovers 32 -bytes plain text which
// was encrypted using respective public key, which is associated with this
// secret key.
//
// See algorithm 6 defined in Kyber specification, as submitted to NIST PQC
// final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t k, const size_t du, const size_t dv>
static void
decrypt(
  const uint8_t* const __restrict seckey, // (k * 12 * 32) -bytes secret key
  const uint8_t* const __restrict enc,    // (k * du * 32 + dv * 32) -bytes
  uint8_t* const __restrict dec           // 32 -bytes plain text
)
{
  // step 1
  ff::ff_t u[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    const size_t encoff = i * du * 32;

    kyber_utils::decode<du>(enc + encoff, u + uoff);

    for (size_t l = 0; l < ntt::N; l++) {
      u[uoff + l] = kyber_utils::decompress<du>(u[uoff + l]);
    }
  }

  // step 2
  ff::ff_t v[ntt::N]{};

  constexpr size_t encoff = k * du * 32;
  kyber_utils::decode<dv>(enc + encoff, v);

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] = kyber_utils::decompress<dv>(v[i]);
  }

  // step 3
  ff::ff_t s_prime[k * ntt::N]{};

  for (size_t i = 0; i < k; i++) {
    const size_t soff = i * ntt::N;
    const size_t skoff = i * 12 * 32;

    kyber_utils::decode<12>(seckey + skoff, s_prime + soff);
  }

  // step 4
  for (size_t i = 0; i < k; i++) {
    const size_t uoff = i * ntt::N;
    ntt::ntt(u + uoff);
  }

  ff::ff_t t[ntt::N]{};
  ff::ff_t tmp[ntt::N]{};

  std::memset(t, 0, sizeof(t));

  for (size_t i = 0; i < k; i++) {
    const size_t off = i * ntt::N;

    ntt::polymul(s_prime + off, u + off, tmp);

    for (size_t l = 0; l < ntt::N; l++) {
      t[l] += tmp[l];
    }
  }

  ntt::intt(t);

  for (size_t i = 0; i < ntt::N; i++) {
    v[i] = kyber_utils::compress<1>(v[i] - t[i]);
  }

  kyber_utils::encode<1>(v, dec);
}

}
