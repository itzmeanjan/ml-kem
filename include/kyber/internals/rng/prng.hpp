#pragma once
#include "shake256.hpp"
#include <array>
#include <random>
#include <span>

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
  shake256::shake256_t state;

public:
  // Default constructor which seeds PRNG with system randomness.
  inline prng_t()
  {
    std::array<uint8_t, 32> seed{};
    auto _seed = std::span(seed);

    // Read more @
    // https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device
    std::random_device rd{};

    size_t off = 0;
    while (off < sizeof(seed)) {
      const uint32_t v = rd();
      std::memcpy(_seed.subspan(off, sizeof(v)).data(), &v, sizeof(v));

      off += sizeof(v);
    }

    state.absorb(_seed);
    state.finalize();
  }

  // Explicit constructor which can be used for seeding PRNG.
  inline explicit prng_t(std::span<const uint8_t> seed)
  {
    state.absorb(seed);
    state.finalize();
  }

  // Once PRNG is seeded i.e. PRNG object is constructed, you can request
  // arbitrary many pseudo-random bytes from PRNG.
  inline void read(std::span<uint8_t> bytes) { state.squeeze(bytes); }
};

}
