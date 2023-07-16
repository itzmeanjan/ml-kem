#pragma once
#include "shake256.hpp"
#include <random>

// Pseudo Random Number Generator
namespace prng {

// Pseudo Random Number Generator s.t. N (>0) -many random bytes are read from
// SHAKE256 XOF state, arbitrary many times s.t. SHAKE256 state is obtained by
//
// - either hashing 32 -bytes sampled using std::random_device ( default )
// - or hashing M(>0) -bytes supplied as argument ( explicit )
//
// Note, std::random_device's behaviour is implementation defined feature, so
// this PRNG implementation doesn't guarantee that it'll generate cryptographic
// secure random bytes if you opt for using default constructor of this struct.
//
// I suggest you read
// https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device
// before using default constructor. When using explicit constructor, it's
// your responsibility to supply M -many random seed bytes.
struct prng_t
{
private:
  shake256::shake256 state;

public:
  inline prng_t()
  {
    uint8_t seed[32];

    // Read more @
    // https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device
    std::random_device rd{};

    size_t off = 0;
    while (off < sizeof(seed)) {
      const uint32_t v = rd();
      std::memcpy(seed + off, &v, sizeof(v));

      off += sizeof(v);
    }

    state.absorb(seed, sizeof(seed));
    state.finalize();
  }

  inline explicit prng_t(const uint8_t* const seed, const size_t slen)
  {
    state.absorb(seed, slen);
    state.finalize();
  }

  inline void read(uint8_t* const bytes, const size_t len)
  {
    state.squeeze(bytes, len);
  }
};

}
