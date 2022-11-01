#include "kyber_kem.hpp"

// Thin C wrapper on top of underlying C++ implementation of Kyber{512, 768,
// 1024} KEM, which can be used for producing shared library object with
// conformant C-ABI & interfaced from other languages such as Rust, Python etc.

// Function prototypes
extern "C"
{

  // Kyber512 KEM keypair generation
  void keygen512(uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey);

  // Kyber512 KEM encapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void encapsulate512(const uint8_t* const __restrict pubkey,
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
  void keygen768(uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey);

  // Kyber768 KEM encapsulation, with shared secret key ( of length klen bytes )
  // derivation
  void encapsulate768(const uint8_t* const __restrict pubkey,
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
  void keygen1024(uint8_t* const __restrict pubkey,
                  uint8_t* const __restrict seckey);

  // Kyber1024 KEM encapsulation, with shared secret key ( of length klen bytes
  // ) derivation
  void encapsulate1024(const uint8_t* const __restrict pubkey,
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

  void keygen512(uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey)
  {
    kyber_kem::keygen<2, 3>(pubkey, seckey);
  }

  void encapsulate512(const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = kyber_kem::encapsulate<2, 3, 2, 10, 4>(pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate512(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = kyber_kem::decapsulate<2, 3, 2, 10, 4>(seckey, cipher);
    kdf.read(shared_key, klen);
  }

  void keygen768(uint8_t* const __restrict pubkey,
                 uint8_t* const __restrict seckey)
  {
    kyber_kem::keygen<3, 2>(pubkey, seckey);
  }

  void encapsulate768(const uint8_t* const __restrict pubkey,
                      uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = kyber_kem::encapsulate<3, 2, 2, 10, 4>(pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate768(const uint8_t* const __restrict seckey,
                      const uint8_t* const __restrict cipher,
                      uint8_t* const __restrict shared_key,
                      const size_t klen)
  {
    auto kdf = kyber_kem::decapsulate<3, 2, 2, 10, 4>(seckey, cipher);
    kdf.read(shared_key, klen);
  }

  void keygen1024(uint8_t* const __restrict pubkey,
                  uint8_t* const __restrict seckey)
  {
    kyber_kem::keygen<4, 2>(pubkey, seckey);
  }

  void encapsulate1024(const uint8_t* const __restrict pubkey,
                       uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen)
  {
    auto kdf = kyber_kem::encapsulate<4, 2, 2, 11, 5>(pubkey, cipher);
    kdf.read(shared_key, klen);
  }

  void decapsulate1024(const uint8_t* const __restrict seckey,
                       const uint8_t* const __restrict cipher,
                       uint8_t* const __restrict shared_key,
                       const size_t klen)
  {
    auto kdf = kyber_kem::decapsulate<4, 2, 2, 11, 5>(seckey, cipher);
    kdf.read(shared_key, klen);
  }
}
