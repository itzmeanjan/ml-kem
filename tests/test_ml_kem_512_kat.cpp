#include "ml_kem/ml_kem_512.hpp"
#include "randomshake/randomshake.hpp"
#include "test_helper.hpp"
#include <fstream>
#include <gtest/gtest.h>

// Test if
//
// - Is ML-KEM-512 implemented correctly ?
// - Is it conformant with the specification https://doi.org/10.6028/NIST.FIPS.203 ?
//
// using Known Answer Tests, generated following https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710.
TEST(ML_KEM, ML_KEM_512_KnownAnswerTests)
{
  const std::string kat_file = "./kats/ml_kem_512.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string d_line;

    if (!std::getline(file, d_line).eof()) {
      std::string z_line;
      std::string pk_line;
      std::string sk_line;
      std::string m_line;
      std::string ct_line;
      std::string ss_line;

      std::getline(file, z_line);
      std::getline(file, pk_line);
      std::getline(file, sk_line);
      std::getline(file, m_line);
      std::getline(file, ct_line);
      std::getline(file, ss_line);

      const auto d = extract_and_parse_hex_string<ml_kem_512::SEED_D_BYTE_LEN>(d_line);
      const auto z = extract_and_parse_hex_string<ml_kem_512::SEED_Z_BYTE_LEN>(z_line);
      const auto pk = extract_and_parse_hex_string<ml_kem_512::PKEY_BYTE_LEN>(pk_line);
      const auto sk = extract_and_parse_hex_string<ml_kem_512::SKEY_BYTE_LEN>(sk_line);
      const auto m = extract_and_parse_hex_string<ml_kem_512::SEED_M_BYTE_LEN>(m_line);
      const auto ct = extract_and_parse_hex_string<ml_kem_512::CIPHER_TEXT_BYTE_LEN>(ct_line);
      const auto ss = extract_and_parse_hex_string<ml_kem_512::SHARED_SECRET_BYTE_LEN>(ss_line);

      std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> computed_pkey{};
      std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> computed_skey{};
      std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> computed_ctxt{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> computed_shared_secret_sender{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> computed_shared_secret_receiver{};

      ml_kem_512::keygen(d, z, computed_pkey, computed_skey);
      EXPECT_TRUE(ml_kem_512::encapsulate(m, computed_pkey, computed_ctxt, computed_shared_secret_sender));
      ml_kem_512::decapsulate(computed_skey, computed_ctxt, computed_shared_secret_receiver);

      EXPECT_EQ(pk, computed_pkey);
      EXPECT_EQ(sk, computed_skey);
      EXPECT_EQ(ct, computed_ctxt);
      EXPECT_EQ(ss, computed_shared_secret_sender);
      EXPECT_EQ(computed_shared_secret_sender, computed_shared_secret_receiver);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}

// Test if
//
// - Is ML-KEM-512 Keygen implemented correctly ?
// - Is it conformant with the specification https://doi.org/10.6028/NIST.FIPS.203 ?
//
// using ACVP Known Answer Tests, from
// https://github.com/usnistgov/ACVP-Server/blob/d98cad66639bf9d0822129c4bcae7a169fcf9ca6/gen-val/json-files/ML-KEM-keyGen-FIPS203/internalProjection.json.
TEST(ML_KEM, ML_KEM_512_Keygen_ACVP_KnownAnswerTests)
{
  const std::string kat_file = "./kats/ml_kem_512_keygen.acvp.kat";
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

      const auto d = extract_and_parse_hex_string<ml_kem_512::SEED_D_BYTE_LEN>(d_line);
      const auto z = extract_and_parse_hex_string<ml_kem_512::SEED_Z_BYTE_LEN>(z_line);
      const auto pk = extract_and_parse_hex_string<ml_kem_512::PKEY_BYTE_LEN>(pk_line);
      const auto sk = extract_and_parse_hex_string<ml_kem_512::SKEY_BYTE_LEN>(sk_line);

      std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> computed_pkey{};
      std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> computed_skey{};

      ml_kem_512::keygen(d, z, computed_pkey, computed_skey);

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

// Test
//
// - Given bad ML-KEM-512 secret key, derived shared secret should not match.
// - Is it conformant with the specification https://doi.org/10.6028/NIST.FIPS.203 ?
//
// using ACVP Known Answer Tests, from
// https://github.com/usnistgov/ACVP-Server/blob/d98cad66639bf9d0822129c4bcae7a169fcf9ca6/gen-val/json-files/ML-KEM-encapDecap-FIPS203/internalProjection.json.
TEST(ML_KEM, ML_KEM_512_SeckeyCheck_ACVP_KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/ml_kem_512_seckeyCheck.acvp.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string pk_line;

    if (!std::getline(file, pk_line).eof()) {
      std::string sk_line;
      std::string tp_line;
      std::string reason_line;

      std::getline(file, sk_line);
      std::getline(file, tp_line);
      std::getline(file, reason_line);

      const auto pk = extract_and_parse_hex_string<ml_kem_512::PKEY_BYTE_LEN>(pk_line);
      const auto sk = extract_and_parse_hex_string<ml_kem_512::SKEY_BYTE_LEN>(sk_line);

      const auto test_passed = tp_line.substr(tp_line.find("="sv) + 2, tp_line.size()) == "True";

      std::array<uint8_t, ml_kem_512::SEED_M_BYTE_LEN> random_m{};
      std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> computed_ctxt{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> computed_shared_secret_sender{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> computed_shared_secret_receiver{};

      randomshake::randomshake_t<128> csprng;
      csprng.generate(random_m);

      EXPECT_TRUE(ml_kem_512::encapsulate(random_m, pk, computed_ctxt, computed_shared_secret_sender));
      ml_kem_512::decapsulate(sk, computed_ctxt, computed_shared_secret_receiver);

      if (test_passed) {
        EXPECT_EQ(computed_shared_secret_sender, computed_shared_secret_receiver);
      } else {
        EXPECT_NE(computed_shared_secret_sender, computed_shared_secret_receiver);
      }

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
