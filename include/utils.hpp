#pragma once
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>
#include <type_traits>

// IND-CPA-secure Public Key Encryption Scheme Utilities
namespace kyber_utils {

// Generates N -many random values of type T | N >= 0
template<typename T>
static inline void
random_data(T* const data, const size_t len)
  requires(std::is_unsigned_v<T>)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
static inline const std::string
to_hex(const uint8_t* const bytes, const size_t len)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < len; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Compile-time compute IND-CPA-secure Kyber PKE public key length ( in bytes )
template<const size_t k>
static inline constexpr size_t
get_cpapke_public_key_len()
{
  return k * 12 * 32 + 32;
}

// Compile-time compute IND-CPA-secure Kyber PKE secret key length ( in bytes )
template<const size_t k>
static inline constexpr size_t
get_cpapke_secret_key_len()
{
  return k * 12 * 32;
}

// Compile-time compute IND-CPA-secure Kyber PKE cipher text length ( in bytes )
template<const size_t k, const size_t du, const size_t dv>
static inline constexpr size_t
get_cpapke_cipher_len()
{
  return k * du * 32 + dv * 32;
}

// Compile-time compute IND-CCA-secure Kyber KEM public key length ( in bytes )
template<const size_t k>
static inline constexpr size_t
get_ccakem_public_key_len()
{
  return get_cpapke_public_key_len<k>();
}

// Compile-time compute IND-CCA-secure Kyber KEM secret key length ( in bytes )
template<const size_t k>
static inline constexpr size_t
get_ccakem_secret_key_len()
{
  constexpr size_t t0 = get_cpapke_secret_key_len<k>();
  constexpr size_t t1 = get_ccakem_public_key_len<k>();

  return t0 + t1 + 32 + 32;
}

// Compile-time compute IND-CCA-secure Kyber KEM cipher text length ( in bytes )
template<const size_t k, const size_t du, const size_t dv>
static inline constexpr size_t
get_ccakem_cipher_len()
{
  return get_cpapke_cipher_len<k, du, dv>();
}

}
