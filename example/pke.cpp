#include "kyber_pke.hpp"
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/pke.cpp
int
main()
{
  // Kyber-512 Public Key Encryption (PKE) parameters
  //
  // See table 1 of Kyber specification for all suggested parameters
  // https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
  constexpr size_t k = 2;
  constexpr size_t eta1 = 3;
  constexpr size_t eta2 = 2;
  constexpr size_t du = 10;
  constexpr size_t dv = 4;

  // compile time compute byte length of Kyber-512 PKE public key, secret key
  // and cipher text
  constexpr size_t pklen = kyber_utils::get_cpapke_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_cpapke_secret_key_len<k>();
  constexpr size_t enclen = kyber_utils::get_cpapke_cipher_len<k, du, dv>();
  constexpr size_t mlen = 32;

  // dynamic allocation request of memory resources
  uint8_t* pubkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* seckey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dec = static_cast<uint8_t*>(std::malloc(mlen));

  std::memset(pubkey, 0, pklen);
  std::memset(seckey, 0, sklen);
  std::memset(enc, 0, enclen);
  std::memset(dec, 0, mlen);

  // generate 32 -bytes random plain text to be encrypted
  kyber_utils::random_data<uint8_t>(msg, mlen);
  // generate 32 -bytes random coin used during PKE
  kyber_utils::random_data<uint8_t>(rcoin, mlen);

  // CPA-secure PKE key generation
  kyber_pke::keygen<k, eta1>(pubkey, seckey);
  // CPA-secure 32 -bytes message encryption
  kyber_pke::encrypt<k, eta1, eta2, du, dv>(pubkey, msg, rcoin, enc);
  // CPA-secure decryption to 32 -bytes message
  kyber_pke::decrypt<k, du, dv>(seckey, enc, dec);

  // check that encrypted ( using public key ) plain text is same as decrypted (
  // using secret key ) one
  bool flg = false;
  for (size_t i = 0; i < mlen; i++) {
    flg |= static_cast<bool>(msg[i] ^ dec[i]);
  }

  std::cout << "pubkey : " << kyber_utils::to_hex(pubkey, pklen) << "\n";
  std::cout << "seckey : " << kyber_utils::to_hex(seckey, sklen) << "\n";
  std::cout << "plain text : " << kyber_utils::to_hex(msg, mlen) << "\n";
  std::cout << "cipher     : " << kyber_utils::to_hex(enc, enclen) << "\n";
  std::cout << "decrypted  : " << kyber_utils::to_hex(dec, mlen) << "\n";

  // deallocate acquired memory resources
  std::free(pubkey);
  std::free(seckey);
  std::free(enc);
  std::free(msg);
  std::free(rcoin);
  std::free(dec);

  // don't assert until memory allocation is freed !
  assert(!flg);

  return EXIT_SUCCESS;
}
