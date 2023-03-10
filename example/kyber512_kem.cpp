#include "kyber512_kem.hpp"
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include -I
// ./subtle/include/ example/kyber512_kem.cpp
int
main()
{
  // Compile-time compute byte length of Kyber-512 KEM public key, secret key
  // and cipher text
  constexpr size_t pklen = kyber512_kem::pub_key_len();
  constexpr size_t sklen = kyber512_kem::sec_key_len();
  constexpr size_t ctlen = kyber512_kem::cipher_text_len();
  constexpr size_t klen = 32;

  uint8_t* pubkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* seckey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* shrd_key0 = static_cast<uint8_t*>(std::malloc(klen));
  uint8_t* shrd_key1 = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pubkey, 0, pklen);
  std::memset(seckey, 0, sklen);
  std::memset(cipher, 0, ctlen);

  prng::prng_t prng;

  kyber512_kem::keygen(prng, pubkey, seckey);
  auto skdf = kyber512_kem::encapsulate(prng, pubkey, cipher);
  auto rkdf = kyber512_kem::decapsulate(seckey, cipher);

  // key encapsulator ( who had public key ), derives 32 -bytes key from its KDF
  skdf.read(shrd_key0, klen);
  // key decapsulator ( who used secret key ), derives same 32 -bytes key
  rkdf.read(shrd_key1, klen);

  // check that both parties who intended to share a secret key ( can be used
  // with symmetric key primitives ) over insecure public channel, arrived at
  // same 32 -bytes value.
  bool flg = false;
  for (size_t i = 0; i < klen; i++) {
    flg |= static_cast<bool>(shrd_key0[i] ^ shrd_key1[i]);
  }

  {
    using namespace kyber_utils;

    std::cout << "pubkey : " << to_hex(pubkey, pklen) << "\n";
    std::cout << "seckey : " << to_hex(seckey, sklen) << "\n";
    std::cout << "shared secret 0 : " << to_hex(shrd_key0, klen) << "\n";
    std::cout << "shared secret 1 : " << to_hex(shrd_key1, klen) << "\n";
  }

  std::free(pubkey);
  std::free(seckey);
  std::free(cipher);
  std::free(shrd_key0);
  std::free(shrd_key1);

  // defer assertion until end ( let memory allocation be freed first )
  assert(!flg);

  return EXIT_SUCCESS;
}
