#pragma once
#include "ff.hpp"
#include <cstring>

// IND-CPA-secure Public Key Encryption Scheme
namespace indcpa {

// Given a degree-255 polynomial, where significant portion of each ( total 256
// of them ) coefficient ∈ [0, 2^l), this routine serializes the polynomial to a
// byte array of length 32 * l -bytes
//
// See algorithm 3 described in section 1.1 ( page 7 ) of Kyber specification,
// as submitted to NIST PQC final round call
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
template<const size_t l>
static void
encode(const ff::ff_t* const __restrict poly, // degree 255 polynomial
       uint8_t* const __restrict arr // byte array of length 32*l -bytes
)
{
  constexpr size_t len = 32 * l;
  constexpr size_t blen = len << 3;

  std::memset(arr, 0, len);

  for (size_t i = 0; i < blen; i++) {
    const size_t pidx = i / l;
    const size_t poff = i % l;

    const size_t aidx = i >> 3;
    const size_t aoff = i & 7ul;

    const uint8_t bit = static_cast<uint8_t>((poly[pidx].v >> poff) & 0b1);
    arr[aidx] = a[aidx] ^ (bit << aoff);
  }
}

}
