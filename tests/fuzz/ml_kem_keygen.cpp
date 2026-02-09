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
  constexpr size_t seed_d_len = mk::SEED_D_BYTE_LEN;
  constexpr size_t seed_z_len = mk::SEED_Z_BYTE_LEN;

  if (size != seed_d_len + seed_z_len) {
    return -1;
  }

  std::array<uint8_t, seed_d_len> d;
  std::array<uint8_t, seed_z_len> z;
  std::array<uint8_t, mk::PKEY_BYTE_LEN> pk;
  std::array<uint8_t, mk::SKEY_BYTE_LEN> sk;

  std::copy(data, data + seed_d_len, d.begin());
  std::copy(data + seed_d_len, data + seed_d_len + seed_z_len, z.begin());

  mk::keygen(d, z, pk, sk);
  return 0;
}
