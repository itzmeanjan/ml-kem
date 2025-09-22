#include "ml_kem/ml_kem_1024.hpp"
#include "test_helper.hpp"
#include <fstream>
#include <gtest/gtest.h>

// Test if
//
// - Is ML-KEM-1024 Keygen implemented correctly ?
// - Is it conformant with the specification https://doi.org/10.6028/NIST.FIPS.203 ?
//
// using ACVP Known Answer Tests, from
// https://github.com/usnistgov/ACVP-Server/blob/d98cad66639bf9d0822129c4bcae7a169fcf9ca6/gen-val/json-files/ML-KEM-keyGen-FIPS203/internalProjection.json.
TEST(ML_KEM, ML_KEM_1024_Keygen_ACVP_KnownAnswerTests)
{
  const std::string kat_file = "./kats/ml_kem_1024_keygen.acvp.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string d_line;

    if (!std::getline(file, d_line).eof()) {
      std::string z_line;
      std::string pk_line;
      std::string sk_line;

      std::getline(file, z_line);
      std::getline(file, pk_line);
      std::getline(file, sk_line);

      const auto d = extract_and_parse_hex_string<ml_kem_1024::SEED_D_BYTE_LEN>(d_line);
      const auto z = extract_and_parse_hex_string<ml_kem_1024::SEED_Z_BYTE_LEN>(z_line);
      const auto pk = extract_and_parse_hex_string<ml_kem_1024::PKEY_BYTE_LEN>(pk_line);
      const auto sk = extract_and_parse_hex_string<ml_kem_1024::SKEY_BYTE_LEN>(sk_line);

      std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> computed_pkey{};
      std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> computed_skey{};

      ml_kem_1024::keygen(d, z, computed_pkey, computed_skey);

      EXPECT_EQ(pk, computed_pkey);
      EXPECT_EQ(sk, computed_skey);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
