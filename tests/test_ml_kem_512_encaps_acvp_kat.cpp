#include "ml_kem/ml_kem_512.hpp"
#include "test_helper.hpp"
#include <fstream>
#include <gtest/gtest.h>

// Test
//
// - Is ML-KEM-512 Encapsulation implemented correctly ?
// - Is it conformant with the specification https://doi.org/10.6028/NIST.FIPS.203 ?
//
// using ACVP Known Answer Tests, from
// https://github.com/usnistgov/ACVP-Server/blob/d98cad66639bf9d0822129c4bcae7a169fcf9ca6/gen-val/json-files/ML-KEM-encapDecap-FIPS203/internalProjection.json.
TEST(ML_KEM, ML_KEM_512_Encaps_ACVP_KnownAnswerTests)
{
  const std::string kat_file = "./kats/ml_kem_512_encaps.acvp.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string pk_line;

    if (!std::getline(file, pk_line).eof()) {
      std::string sk_line;
      std::string m_line;
      std::string ct_line;
      std::string ss_line;

      std::getline(file, sk_line);
      std::getline(file, m_line);
      std::getline(file, ct_line);
      std::getline(file, ss_line);

      const auto pk = extract_and_parse_hex_string<ml_kem_512::PKEY_BYTE_LEN>(pk_line);
      const auto _ = extract_and_parse_hex_string<ml_kem_512::SKEY_BYTE_LEN>(sk_line);
      const auto m = extract_and_parse_hex_string<ml_kem_512::SEED_M_BYTE_LEN>(m_line);
      const auto ct = extract_and_parse_hex_string<ml_kem_512::CIPHER_TEXT_BYTE_LEN>(ct_line);
      const auto ss = extract_and_parse_hex_string<ml_kem_512::SHARED_SECRET_BYTE_LEN>(ss_line);

      std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> computed_ctxt{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> computed_shared_secret{};

      EXPECT_TRUE(ml_kem_512::encapsulate(m, pk, computed_ctxt, computed_shared_secret));

      EXPECT_EQ(ct, computed_ctxt);
      EXPECT_EQ(ss, computed_shared_secret);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
