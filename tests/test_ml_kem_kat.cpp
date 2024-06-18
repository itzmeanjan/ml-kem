#include "ml_kem/ml_kem_1024.hpp"
#include "ml_kem/ml_kem_512.hpp"
#include "ml_kem/ml_kem_768.hpp"
#include <charconv>
#include <fstream>
#include <gtest/gtest.h>

// Given a hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
template<size_t L>
static inline std::array<uint8_t, L>
from_hex(std::string_view bytes)
{
  const size_t blen = bytes.length();

  assert(blen % 2 == 0);
  assert(blen / 2 == L);

  std::array<uint8_t, L> res{};

  for (size_t i = 0; i < L; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = bytes.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
}

// Test if
//
// - Is ML-KEM-512 implemented correctly ?
// - Is it conformant with the specification ?
//
// using Known Answer Tests, generated following
// https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710.
TEST(ML_KEM, ML_KEM_512_KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/ml_kem_512.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string d;

    if (!std::getline(file, d).eof()) {
      std::string z;
      std::string pk;
      std::string sk;
      std::string m;
      std::string ct;
      std::string ss;

      std::getline(file, z);
      std::getline(file, pk);
      std::getline(file, sk);
      std::getline(file, m);
      std::getline(file, ct);
      std::getline(file, ss);

      auto _d = std::string_view(d);
      auto __d = _d.substr(_d.find("="sv) + 2, _d.size());
      auto ___d = from_hex<32>(__d);

      auto _z = std::string_view(z);
      auto __z = _z.substr(_z.find("="sv) + 2, _z.size());
      auto ___z = from_hex<32>(__z);

      auto _pk = std::string_view(pk);
      auto __pk = _pk.substr(_pk.find("="sv) + 2, _pk.size());
      auto ___pk = from_hex<ml_kem_512::PKEY_BYTE_LEN>(__pk);

      auto _sk = std::string_view(sk);
      auto __sk = _sk.substr(_sk.find("="sv) + 2, _sk.size());
      auto ___sk = from_hex<ml_kem_512::SKEY_BYTE_LEN>(__sk);

      auto _m = std::string_view(m);
      auto __m = _m.substr(_m.find("="sv) + 2, _m.size());
      auto ___m = from_hex<32>(__m);

      auto _ct = std::string_view(ct);
      auto __ct = _ct.substr(_ct.find("="sv) + 2, _ct.size());
      auto ___ct = from_hex<ml_kem_512::CIPHER_TEXT_BYTE_LEN>(__ct);

      auto _ss = std::string_view(ss);
      auto __ss = _ss.substr(_ss.find("="sv) + 2, _ss.size());
      auto ___ss = from_hex<32>(__ss);

      std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> pkey{};
      std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> skey{};
      std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> ctxt{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> shrd_sec0{};
      std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> shrd_sec1{};

      ml_kem_512::keygen(___d, ___z, pkey, skey);
      (void)ml_kem_512::encapsulate(___m, pkey, ctxt, shrd_sec0);
      ml_kem_512::decapsulate(skey, ctxt, shrd_sec1);

      EXPECT_EQ(___pk, pkey);
      EXPECT_EQ(___sk, skey);
      EXPECT_EQ(___ct, ctxt);
      EXPECT_EQ(___ss, shrd_sec0);
      EXPECT_EQ(shrd_sec0, shrd_sec1);

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
// - Is ML-KEM-768 implemented correctly ?
// - Is it conformant with the specification ?
//
// using Known Answer Tests, generated following
// https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710.
TEST(ML_KEM, ML_KEM_768_KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/ml_kem_768.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string d;

    if (!std::getline(file, d).eof()) {
      std::string z;
      std::string pk;
      std::string sk;
      std::string m;
      std::string ct;
      std::string ss;

      std::getline(file, z);
      std::getline(file, pk);
      std::getline(file, sk);
      std::getline(file, m);
      std::getline(file, ct);
      std::getline(file, ss);

      auto _d = std::string_view(d);
      auto __d = _d.substr(_d.find("="sv) + 2, _d.size());
      auto ___d = from_hex<32>(__d);

      auto _z = std::string_view(z);
      auto __z = _z.substr(_z.find("="sv) + 2, _z.size());
      auto ___z = from_hex<32>(__z);

      auto _pk = std::string_view(pk);
      auto __pk = _pk.substr(_pk.find("="sv) + 2, _pk.size());
      auto ___pk = from_hex<ml_kem_768::PKEY_BYTE_LEN>(__pk);

      auto _sk = std::string_view(sk);
      auto __sk = _sk.substr(_sk.find("="sv) + 2, _sk.size());
      auto ___sk = from_hex<ml_kem_768::SKEY_BYTE_LEN>(__sk);

      auto _m = std::string_view(m);
      auto __m = _m.substr(_m.find("="sv) + 2, _m.size());
      auto ___m = from_hex<32>(__m);

      auto _ct = std::string_view(ct);
      auto __ct = _ct.substr(_ct.find("="sv) + 2, _ct.size());
      auto ___ct = from_hex<ml_kem_768::CIPHER_TEXT_BYTE_LEN>(__ct);

      auto _ss = std::string_view(ss);
      auto __ss = _ss.substr(_ss.find("="sv) + 2, _ss.size());
      auto ___ss = from_hex<32>(__ss);

      std::array<uint8_t, ml_kem_768::PKEY_BYTE_LEN> pkey{};
      std::array<uint8_t, ml_kem_768::SKEY_BYTE_LEN> skey{};
      std::array<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN> ctxt{};
      std::array<uint8_t, 32> shrd_sec0{};
      std::array<uint8_t, 32> shrd_sec1{};

      ml_kem_768::keygen(___d, ___z, pkey, skey);
      (void)ml_kem_768::encapsulate(___m, pkey, ctxt, shrd_sec0);
      ml_kem_768::decapsulate(skey, ctxt, shrd_sec1);

      EXPECT_EQ(___pk, pkey);
      EXPECT_EQ(___sk, skey);
      EXPECT_EQ(___ct, ctxt);
      EXPECT_EQ(___ss, shrd_sec0);
      EXPECT_EQ(shrd_sec0, shrd_sec1);

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
// - Is ML-KEM-1024 implemented correctly ?
// - Is it conformant with the specification ?
//
// using Known Answer Tests, generated following
// https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710.
TEST(ML_KEM, ML_KEM_1024_KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/ml_kem_1024.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string d;

    if (!std::getline(file, d).eof()) {
      std::string z;
      std::string pk;
      std::string sk;
      std::string m;
      std::string ct;
      std::string ss;

      std::getline(file, z);
      std::getline(file, pk);
      std::getline(file, sk);
      std::getline(file, m);
      std::getline(file, ct);
      std::getline(file, ss);

      auto _d = std::string_view(d);
      auto __d = _d.substr(_d.find("="sv) + 2, _d.size());
      auto ___d = from_hex<32>(__d);

      auto _z = std::string_view(z);
      auto __z = _z.substr(_z.find("="sv) + 2, _z.size());
      auto ___z = from_hex<32>(__z);

      auto _pk = std::string_view(pk);
      auto __pk = _pk.substr(_pk.find("="sv) + 2, _pk.size());
      auto ___pk = from_hex<ml_kem_1024::PKEY_BYTE_LEN>(__pk);

      auto _sk = std::string_view(sk);
      auto __sk = _sk.substr(_sk.find("="sv) + 2, _sk.size());
      auto ___sk = from_hex<ml_kem_1024::SKEY_BYTE_LEN>(__sk);

      auto _m = std::string_view(m);
      auto __m = _m.substr(_m.find("="sv) + 2, _m.size());
      auto ___m = from_hex<32>(__m);

      auto _ct = std::string_view(ct);
      auto __ct = _ct.substr(_ct.find("="sv) + 2, _ct.size());
      auto ___ct = from_hex<ml_kem_1024::CIPHER_TEXT_BYTE_LEN>(__ct);

      auto _ss = std::string_view(ss);
      auto __ss = _ss.substr(_ss.find("="sv) + 2, _ss.size());
      auto ___ss = from_hex<32>(__ss);

      std::array<uint8_t, ml_kem_1024::PKEY_BYTE_LEN> pkey{};
      std::array<uint8_t, ml_kem_1024::SKEY_BYTE_LEN> skey{};
      std::array<uint8_t, ml_kem_1024::CIPHER_TEXT_BYTE_LEN> ctxt{};
      std::array<uint8_t, 32> shrd_sec0{};
      std::array<uint8_t, 32> shrd_sec1{};

      ml_kem_1024::keygen(___d, ___z, pkey, skey);
      (void)ml_kem_1024::encapsulate(___m, pkey, ctxt, shrd_sec0);
      ml_kem_1024::decapsulate(skey, ctxt, shrd_sec1);

      EXPECT_EQ(___pk, pkey);
      EXPECT_EQ(___sk, skey);
      EXPECT_EQ(___ct, ctxt);
      EXPECT_EQ(___ss, shrd_sec0);
      EXPECT_EQ(shrd_sec0, shrd_sec1);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
