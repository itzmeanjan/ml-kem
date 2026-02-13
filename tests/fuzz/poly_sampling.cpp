#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/sampling.hpp"
#include "sha3/shake128.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

// Fuzzer for polynomial sampling (sample_ntt and sample_poly_cbd).
// Verifies that they can handle arbitrary byte streams (from fuzz data or a hasher)
// without crashing or infinite loops.
extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  if (size < 64) {
    return -1;
  }

  // Use first byte for choosing sampling type
  const uint8_t choice = data[0] % 3;
  const uint8_t* f_data = data + 1;
  const size_t f_size = size - 1;

  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> poly{};

  if (choice == 0) {
    // 1. Stress-test sample_ntt
    // We need a shake128_t. We'll absorb the fuzz data into it.
    shake128::shake128_t hasher;
    hasher.absorb(std::span(f_data, f_size));
    hasher.finalize();

    ml_kem_utils::sample_ntt(hasher, poly);
  } else if (choice == 1) {
    // 2. Stress-test sample_poly_cbd<2>
    // Needs exactly 64 * 2 = 128 bytes.
    if (f_size < 128) {
      return -1;
    }
    ml_kem_utils::sample_poly_cbd<2>(std::span<const uint8_t, 128>(f_data, 128), poly);
  } else if (choice == 2) {
    // 3. Stress-test sample_poly_cbd<3>
    // Needs exactly 64 * 3 = 192 bytes.
    if (f_size < 192) {
      return -1;
    }
    ml_kem_utils::sample_poly_cbd<3>(std::span<const uint8_t, 192>(f_data, 192), poly);
  }

  // Property check: Resulting coefficients must be in [0, Q)
  for (size_t i = 0; i < ml_kem_ntt::N; ++i) {
    if (poly[i].raw() >= ml_kem_field::Q) {
      __builtin_trap();
    }
  }

  return 0;
}
