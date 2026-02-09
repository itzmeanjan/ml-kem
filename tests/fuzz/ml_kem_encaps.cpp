#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#if defined(ML_KEM_512)
#include "ml_kem/ml_kem_512.hpp"
namespace mk = ml_kem_512;
#elif defined(ML_KEM_768)
#include "ml_kem/ml_kem_768.hpp"
namespace mk = ml_kem_768;
#elif defined(ML_KEM_1024)
#include "ml_kem/ml_kem_1024.hpp"
namespace mk = ml_kem_1024;
#else
#error "ML-KEM security level not defined"
#endif

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  constexpr size_t OFF_DISCRIMINATOR = 0;

  // Malformed Input Mode (Mode A)
  constexpr size_t OFF_MALFORM_SEED_M = 0;
  constexpr size_t OFF_MALFORM_PKEY = OFF_MALFORM_SEED_M + mk::SEED_M_BYTE_LEN;
  constexpr size_t OFF_MALFORM_END = OFF_MALFORM_PKEY + mk::PKEY_BYTE_LEN;

  // Logic Discovery Mode (Mode B)
  constexpr size_t OFF_LOGIC_SEED_D = OFF_DISCRIMINATOR + 1;
  constexpr size_t OFF_LOGIC_SEED_Z = OFF_LOGIC_SEED_D + mk::SEED_D_BYTE_LEN;
  constexpr size_t OFF_LOGIC_SEED_M = OFF_LOGIC_SEED_Z + mk::SEED_Z_BYTE_LEN;
  constexpr size_t OFF_LOGIC_END = OFF_LOGIC_SEED_M + mk::SEED_M_BYTE_LEN;

  const size_t required_min = (data[OFF_DISCRIMINATOR] % 2 == 1) ? OFF_MALFORM_END : OFF_LOGIC_END;
  if (size < required_min) {
    return -1;
  }

  std::array<uint8_t, mk::SEED_M_BYTE_LEN> m;
  std::array<uint8_t, mk::PKEY_BYTE_LEN> pk;
  std::array<uint8_t, mk::CIPHER_TEXT_BYTE_LEN> ct;
  std::array<uint8_t, mk::SHARED_SECRET_BYTE_LEN> ss;

  if (data[OFF_DISCRIMINATOR] % 2 == 1) {
    // Mode A: Possibly malformed input mode
    std::copy(data + OFF_MALFORM_SEED_M, data + OFF_MALFORM_PKEY, m.begin());
    std::copy(data + OFF_MALFORM_PKEY, data + OFF_MALFORM_END, pk.begin());
  } else {
    // Mode B: Valid input mode
    std::array<uint8_t, mk::SEED_D_BYTE_LEN> d;
    std::array<uint8_t, mk::SEED_Z_BYTE_LEN> z;
    std::array<uint8_t, mk::SKEY_BYTE_LEN> sk;

    std::copy(data + OFF_LOGIC_SEED_D, data + OFF_LOGIC_SEED_Z, d.begin());
    std::copy(data + OFF_LOGIC_SEED_Z, data + OFF_LOGIC_SEED_M, z.begin());
    std::copy(data + OFF_LOGIC_SEED_M, data + OFF_LOGIC_END, m.begin());

    mk::keygen(d, z, pk, sk);
  }

  (void)mk::encapsulate(m, pk, ct, ss);
  return 0;
}
