#!/usr/bin/python3

from ctypes import c_size_t, CDLL, c_char_p, create_string_buffer
from posixpath import exists, abspath
from typing import Tuple

SO_PATH: str = abspath("../libkyber_kem.so")
assert exists(SO_PATH), "Use `make lib` to generate shared library object !"

SO_LIB: CDLL = CDLL(SO_PATH)


def keygen512(d: bytes, z: bytes) -> Tuple[bytes, bytes]:
    """
    Given two 32 -bytes seeds, this routine deterministically computes Kyber512
    KEM's key pair ( i.e. public key and secret key, in order )
    """
    pkey = create_string_buffer(800)
    skey = create_string_buffer(1632)

    args = [c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.keygen512.argtypes = args

    SO_LIB.keygen512(d, z, pkey, skey)

    return pkey.raw, skey.raw


def encapsulate512(m: bytes, pkey: bytes, klen: int) -> Tuple[bytes, bytes]:
    """
    Given a 32 -bytes seed ( used for deterministically computing the shared
    secret ) and Kyber512 KEM public key, this routine deterministically
    computes cipher text ( of length 768 -bytes ) and klen bytes shared
    secret key, in order.
    """
    ctxt = create_string_buffer(768)
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.encapsulate512.argtypes = args

    SO_LIB.encapsulate512(m, pkey, ctxt, shrd_key, klen)

    return ctxt.raw, shrd_key.raw


def decapsulate512(skey: bytes, ctxt: bytes, klen: int) -> bytes:
    """
    Given Kyber512 KEM secret key & cipher text ( of 768 -bytes ), this routine
    recovers 32 -bytes plain text from cipher text, which is used for seeding a
    KDF to extract out klen -bytes shared secret key.
    """
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.decapsulate512.argtypes = args

    SO_LIB.decapsulate512(skey, ctxt, shrd_key, klen)

    return shrd_key.raw


def keygen768(d: bytes, z: bytes) -> Tuple[bytes, bytes]:
    """
    Given two 32 -bytes seeds, this routine deterministically computes Kyber768
    KEM's key pair ( i.e. public key and secret key, in order )
    """
    pkey = create_string_buffer(1184)
    skey = create_string_buffer(2400)

    args = [c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.keygen768.argtypes = args

    SO_LIB.keygen768(d, z, pkey, skey)

    return pkey.raw, skey.raw


def encapsulate768(m: bytes, pkey: bytes, klen: int) -> Tuple[bytes, bytes]:
    """
    Given a 32 -bytes seed ( used for deterministically computing the shared
    secret ) and Kyber768 KEM public key, this routine deterministically
    computes cipher text ( of length 1088 -bytes ) and klen bytes shared
    secret key, in order.
    """
    ctxt = create_string_buffer(1088)
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.encapsulate768.argtypes = args

    SO_LIB.encapsulate768(m, pkey, ctxt, shrd_key, klen)

    return ctxt.raw, shrd_key.raw


def decapsulate768(skey: bytes, ctxt: bytes, klen: int) -> bytes:
    """
    Given Kyber768 KEM secret key & cipher text ( of 1088 -bytes ), this routine
    recovers 32 -bytes plain text from cipher text, which is used for seeding a
    KDF to extract out klen -bytes shared secret key.
    """
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.decapsulate768.argtypes = args

    SO_LIB.decapsulate768(skey, ctxt, shrd_key, klen)

    return shrd_key.raw


def keygen1024(d: bytes, z: bytes) -> Tuple[bytes, bytes]:
    """
    Given two 32 -bytes seeds, this routine deterministically computes Kyber1024
    KEM's key pair ( i.e. public key and secret key, in order )
    """
    pkey = create_string_buffer(1568)
    skey = create_string_buffer(3168)

    args = [c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.keygen1024.argtypes = args

    SO_LIB.keygen1024(d, z, pkey, skey)

    return pkey.raw, skey.raw


def encapsulate1024(m: bytes, pkey: bytes, klen: int) -> Tuple[bytes, bytes]:
    """
    Given a 32 -bytes seed ( used for deterministically computing the shared
    secret ) and Kyber1024 KEM public key, this routine deterministically
    computes cipher text ( of length 1568 -bytes ) and klen bytes shared
    secret key, in order.
    """
    ctxt = create_string_buffer(1568)
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.encapsulate1024.argtypes = args

    SO_LIB.encapsulate1024(m, pkey, ctxt, shrd_key, klen)

    return ctxt.raw, shrd_key.raw


def decapsulate1024(skey: bytes, ctxt: bytes, klen: int) -> bytes:
    """
    Given Kyber1024 KEM secret key & cipher text ( of 1568 -bytes ), this routine
    recovers 32 -bytes plain text from cipher text, which is used for seeding a
    KDF to extract out klen -bytes shared secret key.
    """
    shrd_key = create_string_buffer(klen)

    args = [c_char_p, c_char_p, c_char_p, c_size_t]
    SO_LIB.decapsulate1024.argtypes = args

    SO_LIB.decapsulate1024(skey, ctxt, shrd_key, klen)

    return shrd_key.raw


if __name__ == "__main__":
    print("Use `kyber_kem` as library module")
