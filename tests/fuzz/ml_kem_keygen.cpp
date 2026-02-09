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
  constexpr size_t OFF_SEED_D = 0;
  constexpr size_t OFF_SEED_Z = OFF_SEED_D + mk::SEED_D_BYTE_LEN;
  constexpr size_t TOTAL_REQUIRED_NUM_BYTES = OFF_SEED_Z + mk::SEED_Z_BYTE_LEN;

  if (size < TOTAL_REQUIRED_NUM_BYTES) {
    return -1;
  }

  std::array<uint8_t, mk::SEED_D_BYTE_LEN> d;
  std::array<uint8_t, mk::SEED_Z_BYTE_LEN> z;
  std::array<uint8_t, mk::PKEY_BYTE_LEN> pk;
  std::array<uint8_t, mk::SKEY_BYTE_LEN> sk;

  std::copy(data + OFF_SEED_D, data + OFF_SEED_Z, d.begin());
  std::copy(data + OFF_SEED_Z, data + TOTAL_REQUIRED_NUM_BYTES, z.begin());

  mk::keygen(d, z, pk, sk);
  return 0;
}
