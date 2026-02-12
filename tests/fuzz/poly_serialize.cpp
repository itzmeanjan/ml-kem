#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/serialize.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace {

// Fuzzer for polynomial serialization (encode/decode) round-trip consistency.
// Ensures that decode(encode(poly)) == poly for all bit-widths l in {1, 4, 5, 10, 11, 12}.
template<size_t l>
void
fuzz_serialize_round_trip(const uint8_t* data, size_t size)
{
  // encode(poly) produces 32 * l bytes.
  // We need 256 coefficients, we'll extract them as uint16_t from data.
  if (size < 512) {
    return;
  }

  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> poly_orig{};
  std::array<ml_kem_field::zq_t, ml_kem_ntt::N> poly_work{};
  std::array<uint8_t, 32 * l> serialized{};

  const uint16_t mask = (1U << l) - 1;

  for (size_t i = 0; i < ml_kem_ntt::N; ++i) {
    const uint16_t val = static_cast<uint16_t>(static_cast<uint16_t>(data[(i * 2)]) | (static_cast<uint16_t>(data[((i * 2)) + 1]) << 8));

    // Coefficients must be in [0, 2^l). For l=12, we must also ensure they are reduced to [0, Q).
    poly_orig[i] = ml_kem_field::zq_t::from_non_reduced(val & mask);
    poly_work[i] = ml_kem_field::zq_t::zero();
  }

  // Round-trip: Encode followed by Decode
  ml_kem_utils::encode<l>(poly_orig, serialized);
  ml_kem_utils::decode<l>(serialized, poly_work);

  // Property: Result must be identical to original
  for (size_t i = 0; i < ml_kem_ntt::N; ++i) {
    if (poly_work[i].raw() != poly_orig[i].raw()) {
      __builtin_trap();
    }
  }
}

}

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  if (size < 513) {
    return -1;
  }

  // Use first byte to select bit-width l
  const uint8_t l_choice = data[0] % 6;
  const uint8_t* f_data = data + 1;
  const size_t f_size = size - 1;

  switch (l_choice) {
    case 0:
      fuzz_serialize_round_trip<1>(f_data, f_size);
      break;
    case 1:
      fuzz_serialize_round_trip<4>(f_data, f_size);
      break;
    case 2:
      fuzz_serialize_round_trip<5>(f_data, f_size);
      break;
    case 3:
      fuzz_serialize_round_trip<10>(f_data, f_size);
      break;
    case 4:
      fuzz_serialize_round_trip<11>(f_data, f_size);
      break;
    case 5:
      fuzz_serialize_round_trip<12>(f_data, f_size);
      break;
    default:
      break;
  }

  return 0;
}
