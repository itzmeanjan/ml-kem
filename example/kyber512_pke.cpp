#include "kyber512_pke.hpp"
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include
// example/kyber512_pke.cpp
int
main()
{
  // Compile-time compute byte length of Kyber-512 PKE public key, secret key
  // and cipher text
  constexpr size_t pklen = kyber512_pke::pub_key_len();
  constexpr size_t sklen = kyber512_pke::sec_key_len();
  constexpr size_t enclen = kyber512_pke::cipher_text_len();
  constexpr size_t mlen = 32;

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

  prng::prng_t prng;

  prng.read(msg, mlen);
  prng.read(rcoin, mlen);

  kyber512_pke::keygen(prng, pubkey, seckey);
  kyber512_pke::encrypt(pubkey, msg, rcoin, enc);
  kyber512_pke::decrypt(seckey, enc, dec);

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
