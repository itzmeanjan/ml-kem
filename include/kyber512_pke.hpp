#pragma once
#include "kyber_pke.hpp"

// Kyber Public Key Encryption (PKE) instantiated with Kyber512 parameter set.
//
// See table 1 of specification @
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
namespace kyber512_pke {

// Computes a new Kyber512 PKE keypair s.t. public key is 800 -bytes and secret
// key is 768 -bytes.
inline void
keygen(uint8_t* const __restrict pubkey, uint8_t* const __restrict seckey)
{
  kyber_pke::keygen<2, 3>(pubkey, seckey);
}

// Given a Kyber512 PKE public key ( of 800 -bytes ), a message of 32 -bytes and
// a random coin of 32 -bytes, this routine encrypts 32 -bytes message,
// computing 768 -bytes of cipher text, which can only be decrypted by
// corresponding Kyber512 PKE secret key.
inline void
encrypt(const uint8_t* const __restrict pubkey,
        const uint8_t* const __restrict msg,
        const uint8_t* const __restrict rcoin,
        uint8_t* const __restrict enc)
{
  kyber_pke::encrypt<2, 3, 2, 10, 4>(pubkey, msg, rcoin, enc);
}

// Given Kyber512 PKE secret key ( of 768 -bytes ) and a cipher text of 768
// -bytes, which was obtained by encrypting 32 -bytes message, targeting
// corresponding Kyber512 PKE public key, this routine decrypts it to 32 -bytes
// plain text.
inline void
decrypt(const uint8_t* const __restrict seckey,
        const uint8_t* const __restrict enc,
        uint8_t* const __restrict dec)
{
  kyber_pke::decrypt<2, 10, 4>(seckey, enc, dec);
}

}
