#!/usr/bin/python3

import kyber_kem

# Shared secret length
KLEN = 32


def test_kyber512():
    """
    Ensures correctness and compatibility of this Kyber512 KEM implementation,
    against original reference implementation https://github.com/pq-crystals/kyber

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710
    """
    with open("kyber512.kat", "r") as fd:
        while True:
            d = fd.readline()
            if not d:
                # no more KAT
                break

            z = fd.readline()
            pk = fd.readline()
            sk = fd.readline()
            m = fd.readline()
            ct = fd.readline()
            ss = fd.readline()

            # extract out required fields
            d = [i.strip() for i in d.split("=")][-1]
            z = [i.strip() for i in z.split("=")][-1]
            pk = [i.strip() for i in pk.split("=")][-1]
            sk = [i.strip() for i in sk.split("=")][-1]
            m = [i.strip() for i in m.split("=")][-1]
            ct = [i.strip() for i in ct.split("=")][-1]
            ss = [i.strip() for i in ss.split("=")][-1]

            # convert input hex strings to bytes object
            d = bytes(
                [
                    int(f"0x{d[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(d) >> 1)
                ]
            )
            z = bytes(
                [
                    int(f"0x{z[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(z) >> 1)
                ]
            )
            pk = bytes(
                [
                    int(f"0x{pk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pk) >> 1)
                ]
            )
            sk = bytes(
                [
                    int(f"0x{sk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(sk) >> 1)
                ]
            )
            m = bytes(
                [
                    int(f"0x{m[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(m) >> 1)
                ]
            )
            ct = bytes(
                [
                    int(f"0x{ct[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ct) >> 1)
                ]
            )
            ss = bytes(
                [
                    int(f"0x{ss[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ss) >> 1)
                ]
            )

            (pkey, skey) = kyber_kem.keygen512(d, z)
            (ctxt, shrd_key0) = kyber_kem.encapsulate512(m, pkey, KLEN)
            shrd_key1 = kyber_kem.decapsulate512(skey, ctxt, KLEN)

            assert pk == pkey, "[Kyber512 KEM] Public Key doesn't match"
            assert sk == skey, "[Kyber512 KEM] Secret Key doesn't match"
            assert ct == ctxt, "[Kyber512 KEM] Cipher Text doesn't match"
            assert ss == shrd_key0, "[Kyber512 KEM] Shared Secret Key doesn't match"
            assert (
                shrd_key0 == shrd_key1
            ), "[Kyber512 KEM] Shared Secret Key doesn't match"

            fd.readline()


def test_kyber768():
    """
    Ensures correctness and compatibility of this Kyber768 KEM implementation,
    against original reference implementation https://github.com/pq-crystals/kyber

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710
    """
    with open("kyber768.kat", "r") as fd:
        while True:
            d = fd.readline()
            if not d:
                # no more KAT
                break

            z = fd.readline()
            pk = fd.readline()
            sk = fd.readline()
            m = fd.readline()
            ct = fd.readline()
            ss = fd.readline()

            # extract out required fields
            d = [i.strip() for i in d.split("=")][-1]
            z = [i.strip() for i in z.split("=")][-1]
            pk = [i.strip() for i in pk.split("=")][-1]
            sk = [i.strip() for i in sk.split("=")][-1]
            m = [i.strip() for i in m.split("=")][-1]
            ct = [i.strip() for i in ct.split("=")][-1]
            ss = [i.strip() for i in ss.split("=")][-1]

            # convert input hex strings to bytes object
            d = bytes(
                [
                    int(f"0x{d[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(d) >> 1)
                ]
            )
            z = bytes(
                [
                    int(f"0x{z[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(z) >> 1)
                ]
            )
            pk = bytes(
                [
                    int(f"0x{pk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pk) >> 1)
                ]
            )
            sk = bytes(
                [
                    int(f"0x{sk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(sk) >> 1)
                ]
            )
            m = bytes(
                [
                    int(f"0x{m[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(m) >> 1)
                ]
            )
            ct = bytes(
                [
                    int(f"0x{ct[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ct) >> 1)
                ]
            )
            ss = bytes(
                [
                    int(f"0x{ss[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ss) >> 1)
                ]
            )

            (pkey, skey) = kyber_kem.keygen768(d, z)
            (ctxt, shrd_key0) = kyber_kem.encapsulate768(m, pkey, KLEN)
            shrd_key1 = kyber_kem.decapsulate768(skey, ctxt, KLEN)

            assert pk == pkey, "[Kyber768 KEM] Public Key doesn't match"
            assert sk == skey, "[Kyber768 KEM] Secret Key doesn't match"
            assert ct == ctxt, "[Kyber768 KEM] Cipher Text doesn't match"
            assert ss == shrd_key0, "[Kyber768 KEM] Shared Secret Key doesn't match"
            assert (
                shrd_key0 == shrd_key1
            ), "[Kyber768 KEM] Shared Secret Key doesn't match"

            fd.readline()


def test_kyber1024():
    """
    Ensures correctness and compatibility of this Kyber1024 KEM implementation,
    against original reference implementation https://github.com/pq-crystals/kyber

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710
    """
    with open("kyber1024.kat", "r") as fd:
        while True:
            d = fd.readline()
            if not d:
                # no more KAT
                break

            z = fd.readline()
            pk = fd.readline()
            sk = fd.readline()
            m = fd.readline()
            ct = fd.readline()
            ss = fd.readline()

            # extract out required fields
            d = [i.strip() for i in d.split("=")][-1]
            z = [i.strip() for i in z.split("=")][-1]
            pk = [i.strip() for i in pk.split("=")][-1]
            sk = [i.strip() for i in sk.split("=")][-1]
            m = [i.strip() for i in m.split("=")][-1]
            ct = [i.strip() for i in ct.split("=")][-1]
            ss = [i.strip() for i in ss.split("=")][-1]

            # convert input hex strings to bytes object
            d = bytes(
                [
                    int(f"0x{d[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(d) >> 1)
                ]
            )
            z = bytes(
                [
                    int(f"0x{z[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(z) >> 1)
                ]
            )
            pk = bytes(
                [
                    int(f"0x{pk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pk) >> 1)
                ]
            )
            sk = bytes(
                [
                    int(f"0x{sk[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(sk) >> 1)
                ]
            )
            m = bytes(
                [
                    int(f"0x{m[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(m) >> 1)
                ]
            )
            ct = bytes(
                [
                    int(f"0x{ct[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ct) >> 1)
                ]
            )
            ss = bytes(
                [
                    int(f"0x{ss[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ss) >> 1)
                ]
            )

            (pkey, skey) = kyber_kem.keygen1024(d, z)
            (ctxt, shrd_key0) = kyber_kem.encapsulate1024(m, pkey, KLEN)
            shrd_key1 = kyber_kem.decapsulate1024(skey, ctxt, KLEN)

            assert pk == pkey, "[Kyber1024 KEM] Public Key doesn't match"
            assert sk == skey, "[Kyber1024 KEM] Secret Key doesn't match"
            assert ct == ctxt, "[Kyber1024 KEM] Cipher Text doesn't match"
            assert ss == shrd_key0, "[Kyber1024 KEM] Shared Secret Key doesn't match"
            assert (
                shrd_key0 == shrd_key1
            ), "[Kyber1024 KEM] Shared Secret Key doesn't match"

            fd.readline()


if __name__ == "__main__":
    print("Use `pytest` for running test cases")
