#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/poly/compression.hpp"
#include <cstddef>
#include <cstdint>

namespace {

// Fuzzer for polynomial compression/decompression property.
// Ensures that decompress(compress(x)) is "close enough" to x.
// Specifically, |decompress(compress(x)) - x| mod Q should be <= round(Q / 2^{d+1}).
template<size_t d>
void
fuzz_compression_property(const uint8_t* data, size_t size)
{
  if (size < 2) {
    return;
  }

  const uint16_t x_val = static_cast<uint16_t>(static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8));
  const auto x = ml_kem_field::zq_t::from_non_reduced(x_val);

  auto compressed = ml_kem_utils::compress<d>(x);
  auto decompressed = ml_kem_utils::decompress<d>(compressed);

  // Property check: distance modulo Q
  int32_t diff = static_cast<int32_t>(x.raw()) - static_cast<int32_t>(decompressed.raw());
  if (diff < 0) {
    diff += static_cast<int32_t>(ml_kem_field::Q);
  }
  if (diff > static_cast<int32_t>(ml_kem_field::Q / 2)) {
    diff = static_cast<int32_t>(ml_kem_field::Q) - diff;
  }

  // The maximum error is Q / 2^{d+1} rounded.
  // d=1: Q/4 = 3329/4 = 832.25 -> 832
  // d=4: Q/32 = 104.03 -> 104
  // d=5: Q/64 = 52.01 -> 52
  // d=10: Q/2048 = 1.62 -> 2
  // d=11: Q/4096 = 0.81 -> 1
  constexpr int32_t max_err = (static_cast<int32_t>(ml_kem_field::Q) + (1 << static_cast<int32_t>(d))) >> (static_cast<int32_t>(d) + 1);

  if (diff > max_err) {
    __builtin_trap();
  }
}

}

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  if (size < 3) {
    return -1;
  }

  // Use first byte to select d
  const uint8_t d_choice = data[0] % 5;
  const uint8_t* f_data = data + 1;
  const size_t f_size = size - 1;

  switch (d_choice) {
    case 0:
      fuzz_compression_property<1>(f_data, f_size);
      break;
    case 1:
      fuzz_compression_property<4>(f_data, f_size);
      break;
    case 2:
      fuzz_compression_property<5>(f_data, f_size);
      break;
    case 3:
      fuzz_compression_property<10>(f_data, f_size);
      break;
    case 4:
      fuzz_compression_property<11>(f_data, f_size);
      break;
    default:
      break;
  }

  return 0;
}
