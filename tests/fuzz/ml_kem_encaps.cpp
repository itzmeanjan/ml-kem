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
  constexpr size_t seed_m_len = mk::SEED_M_BYTE_LEN;
  constexpr size_t pk_len = mk::PKEY_BYTE_LEN;

  if (size != seed_m_len + pk_len) {
    return -1;
  }

  std::array<uint8_t, seed_m_len> m;
  std::array<uint8_t, pk_len> pk;
  std::array<uint8_t, mk::CIPHER_TEXT_BYTE_LEN> ct;
  std::array<uint8_t, mk::SHARED_SECRET_BYTE_LEN> ss;

  std::copy(data, data + seed_m_len, m.begin());
  std::copy(data + seed_m_len, data + seed_m_len + pk_len, pk.begin());

  (void)mk::encapsulate(m, pk, ct, ss);
  return 0;
}
