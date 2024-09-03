#pragma once
#include "ml_kem/internals/math/field.hpp"
#include <cstddef>

// Holds compile-time executable functions, ensuring that functions are invoked with proper arguments.
namespace ml_kem_params {

// Compile-time check to ensure that number of bits ( read `d` ) to consider during
// polynomial coefficient compression/ decompression is within tolerable bounds.
//
// See "Compression and Decompression" section on page 21 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_d(const size_t d)
{
  return d < ml_kem_field::Q_BIT_WIDTH;
}

// Compile-time check to ensure that functions requiring `η` as parameter are invoked with proper argument.
consteval bool
check_eta(const size_t eta)
{
  return (eta == 2) || (eta == 3);
}

// Compile-time check to ensure that functions requiring `k` as parameter are invoked with proper argument.
consteval bool
check_k(const size_t k)
{
  return (k == 2) || (k == 3) || (k == 4);
}

// Compile-time check to ensure that polynomial to byte array encoding ( and decoding ) routines are invoked with proper params.
consteval bool
check_l(const size_t l)
{
  return (l == 1) || (l == 4) || (l == 5) || (l == 10) || (l == 11) || (l == 12);
}

// Compile-time check to ensure that operand matrices are having compatible dimension for matrix multiplication.
consteval bool
check_matrix_dim(const size_t a_cols, const size_t b_rows)
{
  return !static_cast<bool>(a_cols ^ b_rows);
}

// Compile-time check to ensure that both K-PKE, ML-KEM key generation routine is invoked with proper parameter set.
//
// See table 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_keygen_params(const size_t k, const size_t eta1)
{
  bool flg0 = (k == 2) && (eta1 == 3);
  bool flg1 = (k == 3) && (eta1 == 2);
  bool flg2 = (k == 4) && (eta1 == 2);

  return flg0 || flg1 || flg2;
}

// Compile-time check to ensure that K-PKE encryption routine is invoked with proper parameter set.
//
// See table 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_encrypt_params(const size_t k, const size_t η1, const size_t η2, const size_t du, const size_t dv)
{
  bool flg0 = (k == 2) && (η1 == 3) && (η2 == 2) && (du == 10) && (dv == 4);
  bool flg1 = (k == 3) && (η1 == 2) && (η2 == 2) && (du == 10) && (dv == 4);
  bool flg2 = (k == 4) && (η1 == 2) && (η2 == 2) && (du == 11) && (dv == 5);

  return flg0 || flg1 || flg2;
}

// Compile-time check to ensure that K-PKE decryption routine is invoked with proper parameter set.
//
// See table 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_decrypt_params(const size_t k, const size_t du, const size_t dv)
{
  bool flg0 = (k == 2) && (du == 10) && (dv == 4);
  bool flg1 = (k == 3) && (du == 10) && (dv == 4);
  bool flg2 = (k == 4) && (du == 11) && (dv == 5);

  return flg0 || flg1 || flg2;
}

// Compile-time check to ensure that ML-KEM encapsulation routine is invoked with proper parameter set.
//
// See table 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_encap_params(const size_t k, const size_t η1, const size_t η2, const size_t du, const size_t dv)
{
  return check_encrypt_params(k, η1, η2, du, dv);
}

// Compile-time check to ensure that ML-KEM encapsulation routine is invoked with proper parameter set.
//
// See table 2 of ML-KEM specification https://doi.org/10.6028/NIST.FIPS.203.
consteval bool
check_decap_params(const size_t k, const size_t η1, const size_t η2, const size_t du, const size_t dv)
{
  return check_encap_params(k, η1, η2, du, dv);
}

}
