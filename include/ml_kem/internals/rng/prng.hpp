#pragma once
#include "ml_kem/internals/utility/force_inline.hpp"
#include "shake256.hpp"
#include <limits>
#include <random>

namespace ml_kem_prng {

// Pseudo Random Number Generator s.t. N (>0) -many random bytes are read from SHAKE256 Xof state, arbitrary many times s.t. SHAKE256 state is obtained by
//
// - either hashing `bit_security_level / 8` -bytes sampled using std::random_device ( default )
// - or hashing `bit_security_level / 8` -bytes supplied as argument ( explicit )
//
// Note, std::random_device's behaviour is implementation defined feature, so this PRNG implementation doesn't guarantee that
// it'll generate cryptographic secure random bytes if you opt for using default constructor of this struct.
//
// I strongly suggest you read https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device before using default constructor.
// When using explicit constructor, it's your responsibility to supply `bit_security_level / 8` -many random seed bytes.
template<size_t bit_security_level>
  requires((bit_security_level == 128) || (bit_security_level == 192) || (bit_security_level == 256))
struct prng_t
{
private:
  shake256::shake256_t state{};

public:
  // Default constructor which seeds PRNG with system randomness.
  forceinline prng_t()
  {
    std::array<uint8_t, bit_security_level / std::numeric_limits<uint8_t>::digits> seed{};
    auto _seed = std::span(seed);

    // Read more @ https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device
    std::random_device rd{};

    size_t off = 0;
    while (off < _seed.size()) {
      const uint32_t v = rd();
      std::memcpy(_seed.subspan(off, sizeof(v)).data(), &v, sizeof(v));

      off += sizeof(v);
    }

    state.absorb(_seed);
    state.finalize();
  }

  // Explicit constructor which can be used for seeding PRNG.
  forceinline explicit constexpr prng_t(std::span<const uint8_t, bit_security_level / std::numeric_limits<uint8_t>::digits> seed)
  {
    state.absorb(seed);
    state.finalize();
  }

  // Once PRNG is seeded i.e. PRNG object is constructed, you can request arbitrary many pseudo-random bytes from PRNG.
  forceinline constexpr void read(std::span<uint8_t> bytes) { state.squeeze(bytes); }
};

}
