#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/ntt.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

// Fuzzer for NTT/INTT round-trip consistency.
// Ensures that INTT(NTT(poly)) == poly for any arbitrary polynomial.
extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  // We need 256 coefficients, each 2 bytes (uint16_t) = 512 bytes.
  if (size < 512) {
    return -1;
  }

  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> poly_orig{};
  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> poly_work{};

  for (size_t i = 0; i < ml_kem_ntt::N; ++i) {
    const uint16_t val = static_cast<uint16_t>(static_cast<uint16_t>(data[(i * 2)]) | (static_cast<uint16_t>(data[((i * 2)) + 1]) << 8));

    poly_orig[i] = ml_kem_field::zq_t::from_non_reduced(val);
    poly_work[i] = poly_orig[i];
  }

  // Round-trip: NTT followed by INTT
  ml_kem_ntt::ntt(poly_work);
  ml_kem_ntt::intt(poly_work);

  // Property: Result must be identical to original
  for (size_t i = 0; i < ml_kem_ntt::N; ++i) {
    if (poly_work[i].raw() != poly_orig[i].raw()) {
      __builtin_trap();
    }
  }

  return 0;
}
