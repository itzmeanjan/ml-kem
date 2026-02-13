#include "ml_kem/internals/math/field.hpp"
#include <cstddef>
#include <cstdint>

namespace {

uint16_t
bytes_to_u16(const uint8_t* data)
{
  return static_cast<uint16_t>(static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8));
}

}

// Fuzzer for Zq field arithmetic
extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  // We need at least 4 bytes to form two 16-bit inputs (treated as uint32_t for zq_t)
  // Actually zq_t constructor takes uint32_t, but the field modulus is small (3329).
  // We'll interpret input bytes as coefficients.

  if (size < 4) {
    return -1;
  }

  // Consume 4 bytes
  const uint16_t a_raw = bytes_to_u16(data);
  const uint16_t b_raw = bytes_to_u16(data + 2);

  // Construct field elements
  auto a = ml_kem_field::zq_t::from_non_reduced(a_raw);
  auto b = ml_kem_field::zq_t::from_non_reduced(b_raw);

  // 1. Addition
  auto c_add = a + b;
  // Property: result must be < Q
  if (c_add.raw() >= ml_kem_field::Q) {
    __builtin_trap();
  }

  // 2. Subtraction
  auto c_sub = a - b;
  if (c_sub.raw() >= ml_kem_field::Q) {
    __builtin_trap();
  }

  // 3. Multiplication
  auto c_mul = a * b;
  if (c_mul.raw() >= ml_kem_field::Q) {
    __builtin_trap();
  }

  // 4. Inversion (only if b != 0)
  if (b.raw() != 0) {
    auto b_inv = b.inv();
    auto check = b * b_inv;

    // Property: b * b^-1 == 1 (mod Q)
    if (check.raw() != 1) {
      __builtin_trap();
    }
  }

  // 5. Self-consistency check: (a + b) - b == a
  auto check_identity = (a + b) - b;
  if (check_identity.raw() != a.raw()) {
    __builtin_trap();
  }

  return 0;
}
