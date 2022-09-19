#pragma once
#include "shake128.hpp"
#include <cstdint>

// IND-CPA-secure Public Key Encryption Scheme
namespace indcpa {

// Uniform sampling in R_q | q = 3329
//
// Given a byte stream, this routine *deterministically* samples a degree 255
// polynomial in NTT representation. If the byte stream is statistically close
// to uniform random byte stream, produced polynomial coefficients are also
// statiscally close to randomly sampled elements of R_q.
//
// See algorithm 1, defined in Kyber specification, present in NIST PQC final
// round submission package
// https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
static void
parse(shake128::shake128* const __restrict hasher, // Squeezes arbitrary bytes
      uint16_t* const __restrict poly              // Degree 255 polynomial
)
{
  constexpr size_t n = 256;
  constexpr uint16_t q = ((1 << 8) * 13) + 1;

  size_t i = 0;
  uint8_t buf[3]{};

  while (i < n) {
    hasher->read(buf, sizeof(buf));

    const uint16_t d1 = (static_cast<uint16_t>(buf[1] & 15) << 8) ^
                        (static_cast<uint16_t>(buf[0]) << 0);
    const uint16_t d2 = (static_cast<uint16_t>(buf[1] >> 4) << 0) ^
                        (static_cast<uint16_t>(buf[2]) << 4);

    if (d1 < q) {
      poly[i] = d1;
      i++;
    }

    if ((d2 < q) && (i < n)) {
      poly[i] = d2;
      i++;
    }
  }
}

}
