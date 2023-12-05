#include "kyber512_kem.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I ./include -I ./sha3/include -I ./subtle/include/ examples/kyber512_kem.cpp
int
main()
{
  constexpr size_t SEED_LEN = 32;
  constexpr size_t KEY_LEN = 32;

  // seeds required for keypair generation
  std::vector<uint8_t> d(SEED_LEN, 0);
  std::vector<uint8_t> z(SEED_LEN, 0);

  auto _d = std::span<uint8_t, SEED_LEN>(d);
  auto _z = std::span<uint8_t, SEED_LEN>(z);

  // public/ private keypair
  std::vector<uint8_t> pkey(kyber512_kem::PKEY_LEN, 0);
  std::vector<uint8_t> skey(kyber512_kem::SKEY_LEN, 0);

  auto _pkey = std::span<uint8_t, kyber512_kem::PKEY_LEN>(pkey);
  auto _skey = std::span<uint8_t, kyber512_kem::SKEY_LEN>(skey);

  // seed required for key encapsulation
  std::vector<uint8_t> m(SEED_LEN, 0);
  std::vector<uint8_t> cipher(kyber512_kem::CIPHER_LEN, 0);

  auto _m = std::span<uint8_t, SEED_LEN>(m);
  auto _cipher = std::span<uint8_t, kyber512_kem::CIPHER_LEN>(cipher);

  // shared secret that sender/ receiver arrives at
  std::vector<uint8_t> shrd_key0(KEY_LEN, 0);
  std::vector<uint8_t> shrd_key1(KEY_LEN, 0);

  auto _shrd_key0 = std::span<uint8_t, KEY_LEN>(shrd_key0);
  auto _shrd_key1 = std::span<uint8_t, KEY_LEN>(shrd_key1);

  // pseudo-randomness source
  prng::prng_t prng;

  // fill up seeds using PRNG
  prng.read(_d);
  prng.read(_z);

  // generate a keypair
  kyber512_kem::keygen(_d, _z, _pkey, _skey);

  // fill up seed required for key encapsulation, using PRNG
  prng.read(_m);

  // encapsulate key, compute cipher text and obtain KDF
  auto skdf = kyber512_kem::encapsulate(_m, _pkey, _cipher);
  // decapsulate cipher text and obtain KDF
  auto rkdf = kyber512_kem::decapsulate(_skey, _cipher);

  // both sender's and receiver's KDF should produce same KEY_LEN many bytes
  skdf.squeeze(_shrd_key0);
  rkdf.squeeze(_shrd_key1);

  // check that both of the communicating parties arrived at same shared key
  assert(std::ranges::equal(_shrd_key0, _shrd_key1));

  {
    using namespace kyber_utils;

    std::cout << "Kyber512 KEM\n";
    std::cout << "\npubkey        : " << to_hex(_pkey);
    std::cout << "\nseckey        : " << to_hex(_skey);
    std::cout << "\ncipher        : " << to_hex(_cipher);
    std::cout << "\nshared secret : " << to_hex(_shrd_key0);
    std::cout << "\n";
  }

  return EXIT_SUCCESS;
}
