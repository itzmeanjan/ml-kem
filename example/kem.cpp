#include "kyber.hpp"
#include <cassert>
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/kem.cpp
int
main()
{
  // Kyber-512 Key Encapsulation Mechanism (KEM) parameters
  //
  // See table 1 of Kyber specification for all suggested parameters
  // https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip
  constexpr size_t k = 2;
  constexpr size_t eta1 = 3;
  constexpr size_t eta2 = 2;
  constexpr size_t du = 10;
  constexpr size_t dv = 4;

  // compile time compute byte length of Kyber-512 KEM public key, secret key
  // and cipher text
  constexpr size_t pklen = kyber_utils::get_ccakem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_ccakem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_ccakem_cipher_len<k, du, dv>();
  constexpr size_t klen = 32;

  // dynamic allocation request of memory resources
  uint8_t* pubkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* seckey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* shrd_key0 = static_cast<uint8_t*>(std::malloc(klen));
  uint8_t* shrd_key1 = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pubkey, 0, pklen);
  std::memset(seckey, 0, sklen);
  std::memset(cipher, 0, ctlen);

  // CCA-secure KEM key generation
  ccakem::keygen<k, eta1>(pubkey, seckey);
  // CCA-secure key encapsulation using public key, producing KDF
  auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(pubkey, cipher);
  // CCA-secure key decapsulation using secret key, producing KDF
  auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(seckey, cipher);

  // key encapsulator ( who had public key ), derives 32 -bytes key from its KDF
  skdf.read(shrd_key0, klen);
  // key decapsulator ( who used secret key ), derives same 32 -bytes key
  rkdf.read(shrd_key1, klen);

  // check that both parties who intended to share a secret key ( can be used
  // with symmetric key primitives ) over insecure public channel did that and
  // arrived at same 32 -bytes value.
  bool flg = false;
  for (size_t i = 0; i < klen; i++) {
    flg |= static_cast<bool>(shrd_key0[i] ^ shrd_key1[i]);
  }
  assert(!flg);

  {
    using namespace kyber_utils;

    std::cout << "pubkey : " << to_hex(pubkey, pklen) << "\n\n";
    std::cout << "seckey : " << to_hex(seckey, sklen) << "\n\n";
    std::cout << "shared secret 0 : " << to_hex(shrd_key0, klen) << "\n";
    std::cout << "shared secret 1 : " << to_hex(shrd_key1, klen) << "\n";
  }

  std::free(pubkey);
  std::free(seckey);
  std::free(cipher);
  std::free(shrd_key0);
  std::free(shrd_key1);

  return EXIT_SUCCESS;
}
