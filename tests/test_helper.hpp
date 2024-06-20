#pragma once
#include "ml_kem/internals/math/field.hpp"
#include <array>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

// Given a hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
template<size_t L>
static inline std::array<uint8_t, L>
from_hex(std::string_view bytes)
{
  const size_t blen = bytes.length();

  assert(blen % 2 == 0);
  assert(blen / 2 == L);

  std::array<uint8_t, L> res{};

  for (size_t i = 0; i < L; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = bytes.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
}

// Given a valid ML-KEM-{512, 768, 1024} public key, this function mutates the last coefficient
// of serialized polynomial vector s.t. it produces a malformed (i.e. non-reduced) polynomial vector.
template<size_t pubkey_byte_len>
static inline constexpr void
make_malformed_pubkey(std::span<uint8_t, pubkey_byte_len> pubkey)
{
  constexpr auto last_coeff_ends_at = pubkey_byte_len - 32;
  constexpr auto last_coeff_begins_at = last_coeff_ends_at - 2;

  // <                                      16 -bit word                                        >
  // (MSB)      ---- | ---- | ----              |                      ----                 (LSB)
  // | 12 -bits of last coeff, to be mutated    | Most significant 4 -bits of second last coeff |
  const uint16_t last_coeff = (static_cast<uint16_t>(pubkey[last_coeff_begins_at + 1]) << 8) | static_cast<uint16_t>(pubkey[last_coeff_begins_at + 0]);

  constexpr uint16_t hi = ml_kem_field::Q << 4; // Q (=3329) is not a valid element of Zq. Any value >= Q && < 2^12, would work.
  const uint16_t lo = last_coeff & 0xfu;        // Don't touch most significant 4 -bits of second last coefficient
  const uint16_t updated_last_coeff = hi ^ lo;  // 16 -bit word s.t. last coefficient is not reduced modulo prime Q

  pubkey[last_coeff_begins_at + 0] = static_cast<uint8_t>(updated_last_coeff >> 0);
  pubkey[last_coeff_begins_at + 1] = static_cast<uint8_t>(updated_last_coeff >> 8);
}
