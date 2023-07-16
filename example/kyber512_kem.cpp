#include "kyber512_kem.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include -I
// ./subtle/include/ example/kyber512_kem.cpp
int
main()
{
  constexpr size_t SEED_LEN = 32;
  constexpr size_t KEY_LEN = 32;

  // seeds required for keypair generation
  std::vector<uint8_t> d(SEED_LEN, 0);
  std::vector<uint8_t> z(SEED_LEN, 0);

  // public/ private keypair
  std::vector<uint8_t> pkey(kyber512_kem::PKEY_LEN, 0);
  std::vector<uint8_t> skey(kyber512_kem::SKEY_LEN, 0);

  // seed required for key encapsulation
  std::vector<uint8_t> m(SEED_LEN, 0);
  std::vector<uint8_t> cipher(kyber512_kem::CIPHER_LEN, 0);

  // shared secret that sender/ receiver arrives at
  std::vector<uint8_t> shrd_key0(KEY_LEN, 0);
  std::vector<uint8_t> shrd_key1(KEY_LEN, 0);

  // pseudo-randomness source
  prng::prng_t prng;

  // fill up seeds using PRNG
  prng.read(d.data(), d.size());
  prng.read(z.data(), z.size());

  // generate a keypair
  kyber512_kem::keygen(d.data(), z.data(), pkey.data(), skey.data());

  // fill up seed required for key encapsulation, using PRNG
  prng.read(m.data(), m.size());

  // encapsulate key, compute cipher text and obtain KDF
  auto skdf = kyber512_kem::encapsulate(m.data(), pkey.data(), cipher.data());
  // decapsulate cipher text and obtain KDF
  auto rkdf = kyber512_kem::decapsulate(skey.data(), cipher.data());

  // both sender's and receiver's KDF should produce same KEY_LEN many bytes
  skdf.squeeze(shrd_key0.data(), KEY_LEN);
  rkdf.squeeze(shrd_key1.data(), KEY_LEN);

  // check that both of the communicating parties arrived at same shared key
  assert(std::ranges::equal(shrd_key0, shrd_key1));

  {
    using namespace kyber_utils;

    std::cout << "Kyber512 KEM\n";
    std::cout << "\npubkey : " << to_hex(pkey.data(), pkey.size());
    std::cout << "\nseckey : " << to_hex(skey.data(), skey.size());
    std::cout << "\ncipher : " << to_hex(cipher.data(), cipher.size());
    std::cout << "\nshared secret 0 : " << to_hex(shrd_key0.data(), KEY_LEN);
    std::cout << "\nshared secret 1 : " << to_hex(shrd_key1.data(), KEY_LEN);
    std::cout << "\n";
  }

  return EXIT_SUCCESS;
}
