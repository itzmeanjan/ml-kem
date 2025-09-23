#pragma once
#include "ml_kem/internals/math/field.hpp"
#include "ml_kem/internals/utility/force_inline.hpp"
#include "randomshake/randomshake.hpp"
#include <array>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <string_view>

// Given a hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
template<size_t L>
static forceinline std::array<uint8_t, L>
from_hex(std::string_view chars)
{
  const size_t clen = chars.length();

  assert(clen % 2 == 0);
  assert(clen / 2 == L);

  std::array<uint8_t, L> res{};

  for (size_t i = 0; i < L; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = chars.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
}

// Given a string of following format, this lambda function can extract out the hex string portion
// and then it can parse it, returning a byte array of requested length.
//
// DATA = 010203....0d0e0f
template<size_t byte_len>
static forceinline std::array<uint8_t, byte_len>
extract_and_parse_hex_string(std::string_view in_str)
{
  using namespace std::literals;

  const auto hex_str = in_str.substr(in_str.find("="sv) + 2, in_str.size());
  return from_hex<byte_len>(hex_str);
};

// Given a valid ML-KEM-{512, 768, 1024} public key, this function mutates the last coefficient
// of serialized polynomial vector s.t. it produces a malformed (i.e. non-reduced) polynomial vector.
template<size_t pubkey_byte_len>
static forceinline constexpr void
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

// Given a ML-KEM-{512, 768, 1024} cipher text, this function flips a random bit of it, while sampling choice of random index from input PRNG.
template<size_t cipher_byte_len, size_t bit_sec_lvl>
static forceinline constexpr void
random_bitflip_in_cipher_text(std::span<uint8_t, cipher_byte_len> cipher, randomshake::randomshake_t<bit_sec_lvl>& csprng)
{
  size_t random_u64 = 0;
  csprng.generate(std::span<uint8_t, sizeof(random_u64)>(reinterpret_cast<uint8_t*>(&random_u64), sizeof(random_u64)));

  const size_t random_byte_idx = random_u64 % cipher_byte_len;
  const size_t random_bit_idx = random_u64 % 8;

  const uint8_t hi_bit_mask = 0xffu << (random_bit_idx + 1);
  const uint8_t lo_bit_mask = 0xffu >> (std::numeric_limits<uint8_t>::digits - random_bit_idx);

  const uint8_t selected_byte = cipher[random_byte_idx];
  const uint8_t selected_bit = (selected_byte >> random_bit_idx) & 0b1u;
  const uint8_t selected_bit_flipped = (~selected_bit) & 0b1;

  cipher[random_byte_idx] = (selected_byte & hi_bit_mask) ^ (selected_bit_flipped << random_bit_idx) ^ (selected_byte & lo_bit_mask);
}
