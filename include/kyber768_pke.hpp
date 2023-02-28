#pragma once
#include "kyber_pke.hpp"

// Kyber Public Key Encryption (PKE) instantiated with Kyber768 parameter set.
//
// See table 1 of specification @
// https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf
namespace kyber768_pke {

// Computes a new Kyber768 PKE keypair s.t. public key is 1184 -bytes and secret
// key is 1152 -bytes.
inline void
keygen(uint8_t* const __restrict pubkey, uint8_t* const __restrict seckey)
{
  kyber_pke::keygen<3, 2>(pubkey, seckey);
}

// Given a Kyber768 PKE public key ( of 1184 -bytes ), a message of 32 -bytes
// and a random coin of 32 -bytes, this routine encrypts 32 -bytes message,
// computing 1088 -bytes of cipher text, which can only be decrypted by
// corresponding Kyber768 PKE secret key.
inline void
encrypt(const uint8_t* const __restrict pubkey,
        const uint8_t* const __restrict msg,
        const uint8_t* const __restrict rcoin,
        uint8_t* const __restrict enc)
{
  kyber_pke::encrypt<3, 2, 2, 10, 4>(pubkey, msg, rcoin, enc);
}

// Given Kyber768 PKE secret key ( of 1152 -bytes ) and a cipher text of 1088
// -bytes, which was obtained by encrypting 32 -bytes message, targeting
// corresponding Kyber768 PKE public key, this routine decrypts it to 32 -bytes
// plain text.
inline void
decrypt(const uint8_t* const __restrict seckey,
        const uint8_t* const __restrict enc,
        uint8_t* const __restrict dec)
{
  kyber_pke::decrypt<3, 10, 4>(seckey, enc, dec);
}

}
