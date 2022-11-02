#include "decapsulation.hpp"
#include "encapsulation.hpp"
#include "kem_keygen.hpp"

// Thin C wrapper on top of underlying C++ implementation of Kyber{512, 768,
// 1024} KEM, which can be used for producing shared library object with
// conformant C-ABI & interfaced from other languages such as Rust, Python etc.
//
// This interface is being exposed so that it becomes easier to test this
// implementation's functional correctness and conformance with Kyber
// specification
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
// using known answer tests produced using
// https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710

// Function prototypes
extern "C"
{

  // Kyber512 KEM keypair generation
  void keygen512(const uint8_t* const __restrict d,
                 const uint8_t* const __restrict z,
                 uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey);

  // Kyber512 KEM encapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void encapsulate512(const uint8_t* const __restrict m,
                      const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen);

  // Kyber512 KEM decapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void decapsulate512(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen);

  // Kyber768 KEM keypair generation
  void keygen768(const uint8_t* const __restrict d,
                 const uint8_t* const __restrict z,
                 uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey);

  // Kyber768 KEM encapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void encapsulate768(const uint8_t* const __restrict m,
                      const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen);

  // Kyber768 KEM decapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void decapsulate768(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen);

  // Kyber1024 KEM keypair generation
  void keygen1024(const uint8_t* const __restrict d,
                  const uint8_t* const __restrict z,
                  uint8_t* const __restrict pubkey,
                  uint8_t* const __restrict seckey);

  // Kyber1024 KEM encapsulation, with shared secret key ( of length klen bytes
  // ) derivation
  void encapsulate1024(const uint8_t* const __restrict m,
                       const uint8_t* const __restrict pubkey,
                       uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen);

  // Kyber1024 KEM decapsulation, with shared secret key ( of length klen bytes
  // ) derivation
  void decapsulate1024(const uint8_t* const __restrict seckey,
                       const uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen);
}

// Function implementations
extern "C"
{

  void keygen512(const uint8_t* const __restrict d,
                 const uint8_t* const __restrict z,
                 uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey)
  {
    ccakem::keygen<2, 3>(d, z, pubkey, seckey);
  }

  void encapsulate512(const uint8_t* const __restrict m,
                      const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = ccakem::encapsulate<2, 3, 2, 10, 4>(m, pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate512(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = ccakem::decapsulate<2, 3, 2, 10, 4>(seckey, cipher);
    kdf.read(shared_key, klen);
  }

  void keygen768(const uint8_t* const __restrict d,
                 const uint8_t* const __restrict z,
                 uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey)
  {
    ccakem::keygen<3, 2>(d, z, pubkey, seckey);
  }

  void encapsulate768(const uint8_t* const __restrict m,
                      const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = ccakem::encapsulate<3, 2, 2, 10, 4>(m, pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate768(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = ccakem::decapsulate<3, 2, 2, 10, 4>(seckey, cipher);
    kdf.read(shared_key, klen);
  }

  void keygen1024(const uint8_t* const __restrict d,
                  const uint8_t* const __restrict z,
                  uint8_t* const __restrict pubkey,
                  uint8_t* const __restrict seckey)
  {
    ccakem::keygen<4, 2>(d, z, pubkey, seckey);
  }

  void encapsulate1024(const uint8_t* const __restrict m,
                       const uint8_t* const __restrict pubkey,
                       uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen)
  {
    auto kdf = ccakem::encapsulate<4, 2, 2, 11, 5>(m, pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate1024(const uint8_t* const __restrict seckey,
                       const uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen)
  {
    auto kdf = ccakem::decapsulate<4, 2, 2, 11, 5>(seckey, cipher);
    kdf.read(shared_key, klen);
  }
}
