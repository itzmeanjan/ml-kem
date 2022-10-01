# kyber
CRYSTALS-Kyber: Post-Quantum Public-key Encryption &amp; Key-establishment Algorithm

## Motivation

Kyber is selected by NIST as post-quantum secure public key encryption (PKE) and key exchange mechanism (KEM) as part of NIST's post-quantum cryptography (PQC) standardization initiative.

Kyber offers both

- IND-CPA-secure public key encryption [Kyber CPAPKE]
- IND-CCA2-secure key encapsulation mechanism [Kyber CCAKEM]

while its security is based on hardness of solving learning-with-errors (LWE) problem in module lattices.

Under **IND-CPA-secure Kyber PKE**, two communicating parties both generating their key pairs while publishing their public keys to each other, can encrypt fixed length ( = 32 -bytes ) message using peer's public key. Cipher text can be decrypted by respective secret key ( which is private to key owner ) and 32 -bytes message can be recovered back.

Algorithm | Input | Output | Namespace | Header
--- | --- | --- | --- | ---
PKE KeyGen | ... | Public Key and Secret Key | `cpapke::` | [pke_keygen.hpp](./include/pke_keygen.hpp)
Encryption | Public Key, 32 -bytes message and 32 -bytes random coin | Cipher Text | `cpapke::` | [encryption.hpp](include/encryption.hpp)
Decryption | Secret Key and Cipher Text | 32 -bytes message | `cpapke::` | [decryption.hpp](./include/decryption.hpp)

> When a slightly tweaked Fujisaki–Okamoto (FO) transform is applied on IND-CPA-secure Kyber PKE, we can construct an IND-CCA2-secure KEM.

While with **IND-CCA2-secure Kyber KEM**, two parties interested in secretly communicating over public & insecure channel, can generate a shared secret key ( of arbitrary byte length ) from a key derivation function ( i.e. KDF which is SHAKE256 in this context ) which is obtained by both of these parties as result of seeding SHAKE256 with same secret. This secret is 32 -bytes and that's what is communicated by sender to receiver using underlying Kyber PKE.

Algorithm | Input | Output | Namespace | Header
--- | --- | --- | --- | ---
KEM KeyGen | ... | Public Key and Secret Key | `ccakem::` | [kem_keygen.hpp](./include/kem_keygen.hpp)
Encapsulation | Public Key | Cipher Text and SHAKE256 KDF | `ccakem::` | [encapsulation.hpp](./include/encapsulation.hpp)
Decapsulation | Secret Key and Cipher Text | SHAKE256 KDF | `ccakem::` | [decapsulation.hpp](./include/decapsulation.hpp)

> IND-CPA-secure Kyber PKE can be used for asynchronous secure communication such as email.

> IND-CCA2-secure Kyber KEM can be used for synchronous secure communication such as TLS.

Here I'm developing & maintaining `kyber` - a zero-dependency, header-only and easy-to-use C++ library implementing Kyber PKE and KEM, supporting Kyber-{512, 768, 1024} parameter sets, as defined in table 1 of Kyber specification, as submitted to NIST PQC final round call. Only dependency is `sha3`, which is itself a zero-dependency, header-only C++ library that I decided to write to modularize SHA3 implementation, which is fairly common symmetric key primitive used in post-quantum cryptographic constructions such as Kyber, Dilithium, Falcon etc.

- `sha3` is pinned to specific commit using git submodule in `kyber`
- See [usage](#usage) section below for git submodule set up guide.
- Find more about `sha3` [here](https://github.com/itzmeanjan/sha3.git)

> Find Kyber specification [here](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip)

> See NIST selected PQC candidates [here](https://csrc.nist.gov/Projects/post-quantum-cryptography/selected-algorithms-2022)

Variant | `k` | `η1` | `η2` | `du` | `dv`
--- | --- | --- | --- | --- | ---
Kyber512 | 2 | 3 | 2 | 10 | 4
Kyber768 | 3 | 2 | 2 | 10 | 4
Kyber1024 | 4 | 2 | 2 | 11 | 5

## Prerequisites

- A C++ compiler with C++20 standard library such as `g++`/ `clang++`

```bash
$ clang++ --version
Ubuntu clang version 14.0.0-1ubuntu1
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0
```

- System development utilities such as `make`, `cmake` & `git`

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.22.1

$ git --version
git version 2.34.1
```

- For benchmarking Kyber implementation on CPU systems, you'll need to have `google-benchmark` globally installed. You may want to follow [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide.

- For importing `sha3` dependency, initialize & update git submodule after cloning this repository

```bash
git clone https://github.com/itzmeanjan/kyber.git
cd kyber
git submodule update --init

# now you can {test, benchmark, use} `kyber`
```

## Testing

For testing functional correctness of Kyber implementation along with its component units, you have to issue

```bash
make
```

```bash
[test] Kyber prime field operations
[test] (i)NTT over degree-255 polynomial
[test] Polynomial serialization/ deserialization
[test] Coefficient compression/ decompression
[test] INDCPA-secure Public Key Encryption
[test] INDCCA2-secure Key Encapsulation Mechanism
```

## Benchmarking

For benchmarking Kyber implementation along with its components on CPU systems, you need to issue

```bash
make benchmark
```

> When benchmarking program, you may want to disable CPU frequency scaling technique by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-09-30T15:44:09+05:30
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.62, 1.75, 1.71
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            4.03 ns         4.02 ns    172986010
bench_kyber::ff_compound_add                   1.94 ns         1.94 ns    351391510
bench_kyber::ff_sub                            4.26 ns         4.25 ns    164160098
bench_kyber::ff_compound_sub                   2.10 ns         2.10 ns    330022111
bench_kyber::ff_neg                           0.815 ns        0.813 ns    852514919
bench_kyber::ff_mul                            6.79 ns         6.79 ns    102218133
bench_kyber::ff_compound_mul                   4.47 ns         4.47 ns    155608808
bench_kyber::ff_inv                            81.7 ns         81.4 ns     11811954
bench_kyber::ff_div                            75.7 ns         75.7 ns     11726974
bench_kyber::ff_exp                             403 ns          403 ns      1684729
bench_kyber::encode<1>                          148 ns          148 ns      4713773 bytes_per_second=206.501M/s
bench_kyber::decode<1>                          145 ns          145 ns      4735617 bytes_per_second=210.906M/s
bench_kyber::encode<4>                          966 ns          965 ns       727552 bytes_per_second=126.473M/s
bench_kyber::decode<4>                          782 ns          781 ns       898403 bytes_per_second=156.217M/s
bench_kyber::encode<5>                         1820 ns         1818 ns       382190 bytes_per_second=83.9395M/s
bench_kyber::decode<5>                         1659 ns         1657 ns       424775 bytes_per_second=92.0665M/s
bench_kyber::encode<10>                        2950 ns         2945 ns       234962 bytes_per_second=103.612M/s
bench_kyber::decode<10>                        2934 ns         2930 ns       240899 bytes_per_second=104.145M/s
bench_kyber::encode<11>                        4197 ns         4191 ns       169029 bytes_per_second=80.0959M/s
bench_kyber::decode<11>                        3863 ns         3860 ns       180931 bytes_per_second=86.9752M/s
bench_kyber::encode<12>                        3505 ns         3501 ns       196607 bytes_per_second=104.604M/s
bench_kyber::decode<12>                        3650 ns         3646 ns       191990 bytes_per_second=100.432M/s
bench_kyber::compress<1>                       5.25 ns         5.25 ns    126855258
bench_kyber::decompress<1>                     4.61 ns         4.61 ns    150765566
bench_kyber::compress<4>                       5.27 ns         5.26 ns    125475013
bench_kyber::decompress<4>                     4.85 ns         4.85 ns    143769935
bench_kyber::compress<5>                       5.27 ns         5.26 ns    126440519
bench_kyber::decompress<5>                     4.87 ns         4.87 ns    144445453
bench_kyber::compress<10>                      5.06 ns         5.06 ns    131376450
bench_kyber::decompress<10>                    4.86 ns         4.85 ns    145199899
bench_kyber::compress<11>                      5.05 ns         5.04 ns    132320139
bench_kyber::decompress<11>                    4.84 ns         4.83 ns    143702937
bench_kyber::ntt                               1482 ns         1480 ns       470247
bench_kyber::intt                              1432 ns         1430 ns       487492
bench_kyber::polymul                            371 ns          370 ns      1901828
bench_kyber::random_data<32>                  10225 ns        10212 ns        66316
bench_kyber::pke_keygen<2, 3>                 49604 ns        49520 ns        13351 items_per_second=20.1939k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          38406 ns        38367 ns        18299 items_per_second=26.0638k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          19857 ns        19829 ns        35156 items_per_second=50.4316k/s
bench_kyber::pke_keygen<3, 2>                 69705 ns        69643 ns         9588 items_per_second=14.3589k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          61679 ns        61626 ns        10974 items_per_second=16.2268k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          28696 ns        28663 ns        24275 items_per_second=34.8883k/s
bench_kyber::pke_keygen<4, 2>                100987 ns       100858 ns         6826 items_per_second=9.9149k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          96140 ns        96066 ns         7083 items_per_second=10.4095k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          41984 ns        41929 ns        16971 items_per_second=23.8496k/s
bench_kyber::kem_keygen<2, 3>                 60816 ns        60775 ns        10769 items_per_second=16.4542k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      54742 ns        54702 ns        11333 items_per_second=18.2809k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      62299 ns        62199 ns        10750 items_per_second=16.0775k/s
bench_kyber::kem_keygen<3, 2>                 84558 ns        84478 ns         8101 items_per_second=11.8374k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      81137 ns        81016 ns         8495 items_per_second=12.3433k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      95342 ns        95238 ns         7328 items_per_second=10.5k/s
bench_kyber::kem_keygen<4, 2>                114002 ns       113873 ns         6006 items_per_second=8.78168k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     117725 ns       117592 ns         5738 items_per_second=8.504k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     143638 ns       143391 ns         4814 items_per_second=6.97392k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-09-30T10:19:14+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.22, 0.06, 0.02
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            7.83 ns         7.83 ns     89311884
bench_kyber::ff_compound_add                   2.61 ns         2.61 ns    268164859
bench_kyber::ff_sub                            7.83 ns         7.83 ns     89384279
bench_kyber::ff_compound_sub                   2.61 ns         2.61 ns    268161950
bench_kyber::ff_neg                            2.25 ns         2.25 ns    310680897
bench_kyber::ff_mul                            11.7 ns         11.7 ns     59587367
bench_kyber::ff_compound_mul                   6.53 ns         6.53 ns    107259987
bench_kyber::ff_inv                            40.2 ns         40.2 ns     13028042
bench_kyber::ff_div                            54.2 ns         54.2 ns     10000000
bench_kyber::ff_exp                             811 ns          811 ns       814831
bench_kyber::encode<1>                          610 ns          610 ns      1148180 bytes_per_second=50.0667M/s
bench_kyber::decode<1>                          625 ns          625 ns      1120450 bytes_per_second=48.8464M/s
bench_kyber::encode<4>                         2729 ns         2729 ns       256472 bytes_per_second=44.724M/s
bench_kyber::decode<4>                         2748 ns         2748 ns       254698 bytes_per_second=44.4154M/s
bench_kyber::encode<5>                         3964 ns         3964 ns       176586 bytes_per_second=38.4929M/s
bench_kyber::decode<5>                         3795 ns         3795 ns       184451 bytes_per_second=40.2059M/s
bench_kyber::encode<10>                        7746 ns         7746 ns        90368 bytes_per_second=39.3987M/s
bench_kyber::decode<10>                        6808 ns         6808 ns       102885 bytes_per_second=44.8292M/s
bench_kyber::encode<11>                        9886 ns         9886 ns        70800 bytes_per_second=33.9551M/s
bench_kyber::decode<11>                       10190 ns        10190 ns        68691 bytes_per_second=32.9429M/s
bench_kyber::encode<12>                        9292 ns         9292 ns        75315 bytes_per_second=39.4128M/s
bench_kyber::decode<12>                        8126 ns         8126 ns        86144 bytes_per_second=45.0686M/s
bench_kyber::compress<1>                       6.96 ns         6.96 ns    100557254
bench_kyber::decompress<1>                     8.27 ns         8.27 ns     84681524
bench_kyber::compress<4>                       6.96 ns         6.96 ns    100553735
bench_kyber::decompress<4>                     8.27 ns         8.27 ns     84681057
bench_kyber::compress<5>                       6.96 ns         6.96 ns    100556023
bench_kyber::decompress<5>                     8.27 ns         8.27 ns     84671887
bench_kyber::compress<10>                      6.96 ns         6.96 ns    100554099
bench_kyber::decompress<10>                    8.27 ns         8.27 ns     84673042
bench_kyber::compress<11>                      6.96 ns         6.96 ns    100556890
bench_kyber::decompress<11>                    8.27 ns         8.27 ns     84680479
bench_kyber::ntt                               5277 ns         5277 ns       132758
bench_kyber::intt                              5173 ns         5173 ns       135091
bench_kyber::polymul                            864 ns          864 ns       810502
bench_kyber::random_data<32>                   4580 ns         4580 ns       152851
bench_kyber::pke_keygen<2, 3>                128902 ns       128894 ns         5445 items_per_second=7.75831k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>         125555 ns       125553 ns         5564 items_per_second=7.96479k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          52269 ns        52268 ns        13377 items_per_second=19.1322k/s
bench_kyber::pke_keygen<3, 2>                198871 ns       198868 ns         3520 items_per_second=5.02845k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         202628 ns       202620 ns         3455 items_per_second=4.93534k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          73318 ns        73314 ns         9545 items_per_second=13.6399k/s
bench_kyber::pke_keygen<4, 2>                298013 ns       298008 ns         2349 items_per_second=3.35561k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         308343 ns       308333 ns         2273 items_per_second=3.24325k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>         108994 ns       108993 ns         6421 items_per_second=9.17488k/s
bench_kyber::kem_keygen<2, 3>                143694 ns       143690 ns         4871 items_per_second=6.95942k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>     158518 ns       158517 ns         4415 items_per_second=6.30847k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     193171 ns       193161 ns         3622 items_per_second=5.17703k/s
bench_kyber::kem_keygen<3, 2>                218857 ns       218852 ns         3198 items_per_second=4.5693k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     242635 ns       242628 ns         2887 items_per_second=4.12153k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     294937 ns       294936 ns         2367 items_per_second=3.39057k/s
bench_kyber::kem_keygen<4, 2>                323647 ns       323638 ns         2163 items_per_second=3.08987k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     360859 ns       360855 ns         1939 items_per_second=2.7712k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     444195 ns       444181 ns         1580 items_per_second=2.25133k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-09-30T10:22:06+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.15, 0.03, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.17 ns         6.17 ns    113392478
bench_kyber::ff_compound_add                   2.31 ns         2.31 ns    302830795
bench_kyber::ff_sub                            6.20 ns         6.20 ns    113198222
bench_kyber::ff_compound_sub                   2.31 ns         2.31 ns    302881677
bench_kyber::ff_neg                            1.33 ns         1.33 ns    533425034
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82596904
bench_kyber::ff_compound_mul                   4.62 ns         4.62 ns    151400454
bench_kyber::ff_inv                            36.6 ns         36.6 ns     20358533
bench_kyber::ff_div                            37.8 ns         37.8 ns     21502050
bench_kyber::ff_exp                             583 ns          583 ns      1163559
bench_kyber::encode<1>                          283 ns          283 ns      2468457 bytes_per_second=107.653M/s
bench_kyber::decode<1>                          179 ns          179 ns      3907377 bytes_per_second=170.248M/s
bench_kyber::encode<4>                         1306 ns         1306 ns       535911 bytes_per_second=93.4533M/s
bench_kyber::decode<4>                         1429 ns         1429 ns       489676 bytes_per_second=85.4341M/s
bench_kyber::encode<5>                         2371 ns         2371 ns       295222 bytes_per_second=64.3667M/s
bench_kyber::decode<5>                         3055 ns         3055 ns       229046 bytes_per_second=49.9412M/s
bench_kyber::encode<10>                        4729 ns         4729 ns       148012 bytes_per_second=64.5293M/s
bench_kyber::decode<10>                        5379 ns         5379 ns       130131 bytes_per_second=56.7359M/s
bench_kyber::encode<11>                        6375 ns         6374 ns       109830 bytes_per_second=52.6631M/s
bench_kyber::decode<11>                        6103 ns         6103 ns       114634 bytes_per_second=55.0022M/s
bench_kyber::encode<12>                        5677 ns         5676 ns       123318 bytes_per_second=64.5143M/s
bench_kyber::decode<12>                        6048 ns         6048 ns       115675 bytes_per_second=60.5486M/s
bench_kyber::compress<1>                       5.47 ns         5.46 ns    127956045
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113567873
bench_kyber::compress<4>                       5.47 ns         5.47 ns    127829247
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113565028
bench_kyber::compress<5>                       5.47 ns         5.47 ns    128097327
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113570437
bench_kyber::compress<10>                      5.47 ns         5.47 ns    128274544
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113562725
bench_kyber::compress<11>                      5.47 ns         5.47 ns    128326081
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113575792
bench_kyber::ntt                               2037 ns         2037 ns       343688
bench_kyber::intt                              2131 ns         2131 ns       328471
bench_kyber::polymul                            296 ns          296 ns      2366128
bench_kyber::random_data<32>                   1667 ns         1667 ns       419026
bench_kyber::pke_keygen<2, 3>                 59393 ns        59390 ns        11801 items_per_second=16.8378k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          60174 ns        60172 ns        11641 items_per_second=16.6189k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          31684 ns        31683 ns        22103 items_per_second=31.5627k/s
bench_kyber::pke_keygen<3, 2>                 91784 ns        91782 ns         7620 items_per_second=10.8953k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          96102 ns        96100 ns         7379 items_per_second=10.4058k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          45671 ns        45669 ns        15307 items_per_second=21.8965k/s
bench_kyber::pke_keygen<4, 2>                137057 ns       137054 ns         5107 items_per_second=7.29638k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         146427 ns       146422 ns         4787 items_per_second=6.82957k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          63820 ns        63818 ns        10957 items_per_second=15.6694k/s
bench_kyber::kem_keygen<2, 3>                 65004 ns        65001 ns        10796 items_per_second=15.3844k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      73261 ns        73259 ns         9550 items_per_second=13.6501k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      97735 ns        97733 ns         7160 items_per_second=10.232k/s
bench_kyber::kem_keygen<3, 2>                 99780 ns        99777 ns         7036 items_per_second=10.0223k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     110666 ns       110664 ns         6322 items_per_second=9.03634k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     151169 ns       151163 ns         4598 items_per_second=6.61536k/s
bench_kyber::kem_keygen<4, 2>                146998 ns       146995 ns         4761 items_per_second=6.80296k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     166235 ns       166231 ns         4217 items_per_second=6.01572k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     219358 ns       219353 ns         3193 items_per_second=4.55886k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-09-30T10:26:18+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.11, 0.13, 0.07
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            5.78 ns         5.78 ns    121091250
bench_kyber::ff_compound_add                   3.47 ns         3.47 ns    201838264
bench_kyber::ff_sub                            6.16 ns         6.16 ns    113562603
bench_kyber::ff_compound_sub                   3.85 ns         3.85 ns    181689918
bench_kyber::ff_neg                           0.891 ns        0.891 ns    786106215
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82600260
bench_kyber::ff_compound_mul                   6.16 ns         6.16 ns    113560415
bench_kyber::ff_inv                            43.0 ns         43.0 ns     18435618
bench_kyber::ff_div                            32.2 ns         32.2 ns     37858435
bench_kyber::ff_exp                             482 ns          482 ns      1438597
bench_kyber::encode<1>                          305 ns          305 ns      2296436 bytes_per_second=100.112M/s
bench_kyber::decode<1>                          224 ns          224 ns      3106310 bytes_per_second=136.073M/s
bench_kyber::encode<4>                         1405 ns         1405 ns       498212 bytes_per_second=86.8947M/s
bench_kyber::decode<4>                         1444 ns         1444 ns       484896 bytes_per_second=84.5568M/s
bench_kyber::encode<5>                         2283 ns         2283 ns       306593 bytes_per_second=66.843M/s
bench_kyber::decode<5>                         2331 ns         2331 ns       300100 bytes_per_second=65.4735M/s
bench_kyber::encode<10>                        4122 ns         4122 ns       169832 bytes_per_second=74.0397M/s
bench_kyber::decode<10>                        4206 ns         4206 ns       166431 bytes_per_second=72.5535M/s
bench_kyber::encode<11>                        4537 ns         4537 ns       154341 bytes_per_second=73.9935M/s
bench_kyber::decode<11>                        4635 ns         4635 ns       151027 bytes_per_second=72.4251M/s
bench_kyber::encode<12>                        4945 ns         4944 ns       141583 bytes_per_second=74.0643M/s
bench_kyber::decode<12>                        5062 ns         5062 ns       138315 bytes_per_second=72.3488M/s
bench_kyber::compress<1>                       6.55 ns         6.55 ns    106895713
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113568518
bench_kyber::compress<4>                       6.55 ns         6.55 ns    106876487
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113571337
bench_kyber::compress<5>                       6.55 ns         6.55 ns    106875450
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113569435
bench_kyber::compress<10>                      6.55 ns         6.55 ns    106880245
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113566517
bench_kyber::compress<11>                      6.55 ns         6.55 ns    106881815
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113560245
bench_kyber::ntt                               1718 ns         1718 ns       407465
bench_kyber::intt                              1990 ns         1990 ns       351691
bench_kyber::polymul                            386 ns          386 ns      1814750
bench_kyber::random_data<32>                   1731 ns         1731 ns       404285
bench_kyber::pke_keygen<2, 3>                 48541 ns        48540 ns        14428 items_per_second=20.6018k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          50133 ns        50132 ns        13959 items_per_second=19.9472k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          27981 ns        27980 ns        24998 items_per_second=35.7392k/s
bench_kyber::pke_keygen<3, 2>                 74034 ns        74032 ns         9457 items_per_second=13.5076k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          77820 ns        77818 ns         9005 items_per_second=12.8505k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          39851 ns        39850 ns        17571 items_per_second=25.094k/s
bench_kyber::pke_keygen<4, 2>                109324 ns       109322 ns         6402 items_per_second=9.14731k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         116363 ns       116359 ns         6028 items_per_second=8.59408k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          54278 ns        54277 ns        12888 items_per_second=18.424k/s
bench_kyber::kem_keygen<2, 3>                 53162 ns        53161 ns        13169 items_per_second=18.8109k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      58841 ns        58840 ns        11876 items_per_second=16.9953k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      82004 ns        82002 ns         8530 items_per_second=12.1948k/s
bench_kyber::kem_keygen<3, 2>                 80238 ns        80236 ns         8723 items_per_second=12.4633k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      89241 ns        89239 ns         7788 items_per_second=11.2059k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     123071 ns       123068 ns         5681 items_per_second=8.12557k/s
bench_kyber::kem_keygen<4, 2>                116837 ns       116835 ns         5987 items_per_second=8.55909k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     130569 ns       130567 ns         5365 items_per_second=7.65893k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     177034 ns       177030 ns         3948 items_per_second=5.64875k/s
```

## Usage

`kyber` is written to be a zero-dependency, header-only C++ library such that it's pretty easy-to-use in your project. All you need to do

- Clone `kyber` repository
- Initialize and update git submodule

```bash
cd kyber
git submodule update --init
```

- Previous step is required for importing `sha3`, which is the only dependency of `kyber`

> Find more about my project `sha3`: https://github.com/itzmeanjan/sha3

- Write your program while including [kyber.hpp](./include/kyber.hpp), which includes declarations ( and definitions ) of all required routines in various namespaces
- When compiling your program, let your compiler know where it can find `kyber` and `sha3` headers, which includes their definitions too

```bash
g++ -std=c++20 -I./kyber/include -I./kyber/sha3/include main.cpp
```

Kyber | Namespace | Header
--- | --- | ---
IND-CPA-secure Public Key Encryption | `cpapke::` | [kyber.hpp](./include/kyber.hpp)
IND-CCA-secure Key Encapsulation Mechanism | `ccakem::` | [kyber.hpp](./include/kyber.hpp)

---

### Kyber Public Key Encryption (PKE)

- Select Kyber parameter set from table 1 of Kyber [specification](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip)

- Generate PKE key pair using [`cpapke::keygen<k, η1>(...)`](./include/pke_keygen.hpp)
- Encrypt fixed length 32 -bytes message using [`cpapke::encrypt<k, η1, η2, du, dv>(...)`](./include/encryption.hpp) [ **Sender uses receiver's public key** ]
- Decrypt cipher text to extract 32 -bytes message using [`cpapke::decrypt<k, du, dv>(...)`](./include/decryption.hpp) [ **Receiver uses its own secret key** ]

In [pke.cpp](./example/pke.cpp), I show how to use Kyber PKE API, while sticking to Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/pke.cpp && ./a.out

pubkey : e7eb6ff3a3ba11f23199f73c8ec9756bba7cfbfba7bac60558c3a3ec86c9fc4c9668242076bc11950309f25881e4c156a78718755a25dc4428463ca3ac90c32c0659f3e4bb2fec3f89f9a99ee7617858131c04c7424c256a27baa08a6cb32771c2a8ac403902d33124e3960c7133403ca5610a982e5dd8147167a149d3cc603b6373f22f540684ef113acdf91ed72a4adac7ac2244ac78e513d198039118753fb30d6a1b80dc82c0393c8eaba47320b97bf5faaef5ec220bcc7a83a3c04839cfea359d31a4adcc7b73e4dcce8ca83bdf4cb104a363c8e64fd0a9ccebb49aa1d85360200e341226362494a5dcbe7e9332c6bbaae253646da615ec0278f10595556347a6223161944eb4db78a307c47ec93490f6000341c4d4f561a576cf504454b7bc80f5abcbfff717cea353513629461094b2a25dfd826d6c1c1182f95444b83fb7bbc5264a056ae4c681133e5736ad8cd553b0d54f1fa120efdb88e6faa192a56e5ff9a7b6e403b6e66bbd117fcd194244172c88e885eb526be3828b314b6eaaeb4f9a2010d43c2f34514100d0676b2004099c3d2963cbcbc51f1e1a5ced809bf36008fac781e94a3c4fe5280274a8b5535ac7653700210b82314459bb1f8ab7810a318a3f3299d307b96f3468674a720d8603911b96a42acbb1dcc16c998f96535ce93b827f56a68e05bc0daa8e9cb30cac5328e3645ca81649ea9753fbb27e469062b47545e2b1cc98cc0f4cb78a71f0211fdca6812b3fb7d838b8cbccc863875a2c9f1cf9442015adb62628620620b41b9f3e2c03d1377440446717b44ff76970bdd0713d6c5c2500c53f3b93172c1b7f147b07878030d6cfa6b25b89ca04425cc451f1078d1c546acbbfca590cfad41befab9bbff6584cf000c8a6a2d8847c033a9f81216151606827878bb96613cd428212475ad32c6d30a46ddcf0caf60bcd2d6569d8445b81c47d05676a37093cdf671c7f0024d4c3553729bb5cd85e60027ea931863f295b3db3b0f4e91121358887f755314a428b5b27cfe208d4a802aaca336822a5dd82ad2e8aac745883b46059f9d6944030b1ed2a8466a82e394b163e92f0df870bd9b328c7cf0cee81cff8ec796f0f990ae847eaa8e13261
seckey : f0310f26b811f703519907cb03e0c55d4269d091837c79a259c62d7be69357a94fc281952395b4adebca2c0c651b6c58838b5b0a291751824e1ce7743f04841d7a1dece35c8ebb3bc6a338868517612217fa6caedc8b7b99d528c6572a58963db4a65524989d38f6458b46101241b8d2b6aa35174fe3d377f7a440bf015354fc4ec7119cc804248d4668b9e05d72530eb921474ac8187e8492cf0877a5abaf71d9b133cb0e1a38c10cbc676f1b2c2fc277682b14482b7ebf77264ec5c65e984a60ca5baf79088cec888fd111b6f09e8e5602135933b693c4810b28bb6448f7b3c6357c2bcd52b93299ad0b98c986e91c688383464a7c7cc3472f588958099a24e180605a2d9a6ba939671fcc93a2d52324fe621b45ac2250cb67fa3488ddd59224203be1a112c27925b92bbd80f3019f0994f930a9476019376178e709936d6720a3b283e51139fc2409ae73307a3699bbdb48025584c8882b6b85095ab7289128c3a303b476350d1802369fa5b6bd6a4ecce37378132a63a968a9dba2714809f4494535e33a1bf7a8c096424f6ab505440de8c6cec89c9dd9b99b2a2b9bd0451c8fc81e8f9a85fad7050a286d2ca64983650f3b621ed291268794cc3d02ae62b61726ab75c8e626e6e23ee162732ab8033bbb7f1eca4bc0b70c59bb561175053fd5af522399830791cecc0d00eba9d7414d7e0826db3104ef301ecfd48ae15838fd49c4cd0438daca84d5815ccc5604378c98deca2f4499a75ac5242393a7e482c53b306a8666a81c6630787974f1d6abce4089d16822f212cb76941942ec10b6dbbbe672c0f9d90e614869a4db2417b283a9985db2a2a052f625a6c3beae2225f1f28971f95d40465a43700bf7eb48d596269253790d9056b2569886f2321eea03b0027c1fe152b59174e9388fcc3552e6892e1b25333a4a670bd074481a021124abb126c7bdb746daeb016fba1ae0500ecd9363225266d12032737794deaca5e3b93e5e8b04d490b2ce01653f9809aeb4931c73c17f34367fb366c696ca775243c92482e1776eb43091d77bc6a8dc25c5162544e6bda7790bb21b570102c7
plain text : ce2e64597794b97c56e39e3040daea16f26ac8656d61d338c2f4a1a6e8931766
cipher     : 9018d1f94ec2e12cce357f7564c5ca1af359d5d2dc3430a44f1794915bee1da9913d349eaf7292acf01370c4c185ed7097dc00fcc96809bb39ff8854bd3abf33579d5a1e0ada7dedc0480d1410487fb137525047934bda990c5160ca307f7fa6c144af7e3b61fab97399f0968dc9a2e3c06397036f356b53e82ea7475c665f1f0528f6ae80ae6be0ce8db01c75bbe0087ceb0244c0083ae4d26f8ba0ab4fd91bde40633a025d7a7a4827e1f3bbbcec351ebb96b118a6377438f1761a9c2111693861dd8bfa3ebb3602664441ba1b8d9e5c4eee3243241bddadbd9bc844dcfbc6511cd2a534ae62218dea970f9992e3e1c9c91f9186bb69ba02d6fd248b476b39256b3ccedd2d97122f4a6d4331b56f02d90e470443db7797f19021062cd46064909504a94e7438bce741b65b7b9649a3025215e72a72c6286bbe86fb510934ba14811cb553acc9f19e3098c39e70385c3f1e313138da72f999a56f77a33d65f1ca25c9afadea394f8a2c9256830c66fcb5514241d958bcc49fae79167d63d055b797bebefb0537fbe96f79c74a4be337ea5ba5cd0397aaaf08d6a4c52304f14dcddde6acdbe4a60de6faf98a4328fc9b41f882f0e2e8bceb12a0c887de7c09cae5aa9fd5acf97141bf04e66acf16db6689c4c11b50bc8c07b55c4661021df4577b60f6390fcc8d11bcd284d7360602d95be194d1dd55da098afed93f2d664822159658417807fa62656723c0d8f9069e08a6aea4c71609278a7217d453d7a261bf1d3691148099527b97215848605870d864ada019b5e0124e0d0a0c7b3a7cf35a8026fc8e7023c5dfdc9b611b56fbf63122dae7612f98990e9d88e72cdefce3f4c12ca336281944c8bed6316beb477e4d88df3db2b543652ca508bc6e49e35f8aaf29df1a3ae193ec982b586e2ada99effb32f86ff1a20094cc41c7be64403d8a61fac7117553517a2802412a5fc09e4b39ff328fc1a3c94ebcbca0de8f03aa1d734bc58c390e8701821e3080ffba67cc6fb7da5a4bb734babf204d8412a13da313c4d93d05d92dc6082bad5daeb9fdbc351eea9d920c14c39731e60002cc41
decrypted  : ce2e64597794b97c56e39e3040daea16f26ac8656d61d338c2f4a1a6e8931766
```

### Kyber Key Encapsulation Mechanism (KEM)

- Select which Kyber parameter set to work with
- Generate KEM key pair using [`ccakem::keygen<k, η1>(...)`](./include/kem_keygen.hpp)
- Encapsulate shared secret into cipher text using [`ccakem::encapsulate<k, η1, η2, du, dv>(...)`](./include/encapsulation.hpp) [ **Peer0 uses Peer1's public key** ]
- Decapsulate shared secret from cipher message using [`ccakem::decapsulate<k, η1, η2, du, dv>(...)`](./include/decapsulation.hpp) [ **Peer1 uses its secret key** ]

After a round of communication ( i.e. encapsulation and decapsulation ) both communicating parties reach to a key derivation function ( KDF ) seeded with same secret, which is SHAKE256 in this context.

This allows two parties to share a secret value ( of arbitrary byte length ) over insecure channel, which can be used ( in next phase of communication ) as secret key of some symmetric key primitives ( say AES ) which are much efficient & faster for secure communication.

In [kem.cpp](./example/kem.cpp), I keep an example demonstrating usage of Kyber KEM API, while using Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/kem.cpp && ./a.out

pubkey : 894c25348bc69aa302d49491e5951b62e2084d258b5ed9b64228a4d0c284b7fa05d829711d8b0a27a283a7f2b0f8559fe9dc04b63bb2c857bf51b99a33d37cddcc6feab25e16cb275bf14d677c3fcc56c8fea8ba3474657c4c8d21b38738a852f0d0669a68a79be55c12e90d7128c066151e74b098c70162013cc85275bb18e350f0e49500466acbe7bd6b807360669dbdf97ea6ca6b9b8a59a98501a3b0bfbc600194116565641871190da03a359e9406b210b239d3428a6614321bb2b315ada3f64633f5217277b176b47e57a92338a19de52346575c35ee9accdaaabce4e47586348c30563cc24a84a112399b324a54512814037ef02b8155d214c6501c016a98b2d2c6538765258a4597c7254949a0d2d0c595b65ba5798cfc19ab6a0a85a4a54a3370a37fc0428ed14560098bb89c5c75b55a6e82c354672d9278032ed2ab463985be719d650361cbfac672e278bbf21fdf2a4ce68707bf3547b62a9779a41674aab7c437203dc7c1f6dc6494015bb5ebb78a09b146177beac48c3d430f33f18a404c2269d1aa74d4203ef7b36159ab4f59bdfa743d95c8c8a0272b643cb448d547e53a9f7ef21e87b10fb326c6fe7301398a2f9415a248182204ebb0b8f91441444f19d68d1a8777c3d724a8135cf09bcb3521cb55392d86ba4d49a8aedd904181899a3f42962f16863e5b565332835a3bc74d738b9410951f67c29c5340221a038d0711308559b4c448c9429af5566964611b1fd875d17c685da12611f694b034442b56a9b426889172438b484aafa39aee76b19423ac43230a80699184607b61aa553038b7478c24cac497c1fa43157196d7f679a13b9f250ab31851c6aab9bc8f4b4181019a5598404411ad851271d59825b00b3d6321487eb733f392c06a647cdc1c1e9fc58d22c17691a1549302320f7485e0e01a92e83c539aa650595b2338a93721a56d8c2ebd8b3ce3133006c5afc7db99785a93292c36f347aaa08a4633354262872741816285e145395bbf4ee625c54185c96c193fbb489942a3e377ceb8b113bc04154bc198b0200bb76357f14cc6e359bea9682654e4a1ebe633ab4fdfd6e0d46a29e6f4da1584c1008654fe4a66c65ed58602801e4ad4
seckey : a466cd9ef043e9459f4503a1af751669490f4ecb96869c4a52d91cc972559f53a9aa240bb1ea86b2e8254b258c5476158376a319281f9930bf859a9dfd0a904c1c593f825c5e880a29dc91391069494811faac756d2c8598a655ae977d8f16b8e0159959d80cb48b162c0363098704e7b177627816dfb65c6022260fc655eb67b337452a560510e5fbafc67c81bc183be59b35789b4efdcb796f51b86d29cd4b543fac5b374ca653900138e4a6bdb40a37eb710b6a53ce376731ec9380ca70447658b728b48575aa201b0aaa56d333108521bed9c13e0aa9b5a3c0ee85912ba16df1cc2051e1b6ba0a8d4068859697351dd15bd4698e9ea6760d0876a2a700007547c23b92f4e13ef3511e8ff84e6cb946d5a87f8f9c005cf2857175abef7136b4a7ce9a9725c5974e06ba2915c71201a79b22972589680778405500357704812ad56138a8d2adc219584813244e206504261c8e404e0267a012d884843493c32556b0b4197c70b477e0081ed22524768bcd6219af273b1439278aa8c9b3327685461ef0e0316fb9c0df22251c37b633f38d40777e2c5c2967f51bef1ca07755a90f3c24016a02efd278f3c6b4b1d92121c72376da0c9da905dcbc8331d29385f951318629ff93982d703afbb3538e63a9ee3a6a4ab775d4e9ac2f293e66c92a95d845f76cc906f27dd8034cf490210e502c3e1008c223875e5c8d7fe03cbcb64185a8858f1bccd9d7b57428a0e2a92e73d648ecc7c0e97c0f5668b58b724969ab4a5348680086044e832b4fc102be05397a9c56c7085701116d2bf30374866e00eb94d245c27748457e961f07c13063f1a4a0d196c050a96de37d15da4ec014906b867487776dc34a7c80cca65043a6a5ac02e2b9597445440fc3a3da0348794047850b47cacb34826a8b60aa8ed0d9b16d167c711ac58e0ca37635be478729474c9c1496299b88cf72508c8a03b34a46522c11c89191186c5c31ada674552491229243adea304179b2882c6809d7a95a11108f75c99e8b5cfd05b1b663a6d4339ff948b68161ba157116cdfaac2014745da9349299741c596ed3306bcf1245894c25348bc69aa302d49491e5951b62e2084d258b5ed9b64228a4d0c284b7fa05d829711d8b0a27a283a7f2b0f8559fe9dc04b63bb2c857bf51b99a33d37cddcc6feab25e16cb275bf14d677c3fcc56c8fea8ba3474657c4c8d21b38738a852f0d0669a68a79be55c12e90d7128c066151e74b098c70162013cc85275bb18e350f0e49500466acbe7bd6b807360669dbdf97ea6ca6b9b8a59a98501a3b0bfbc600194116565641871190da03a359e9406b210b239d3428a6614321bb2b315ada3f64633f5217277b176b47e57a92338a19de52346575c35ee9accdaaabce4e47586348c30563cc24a84a112399b324a54512814037ef02b8155d214c6501c016a98b2d2c6538765258a4597c7254949a0d2d0c595b65ba5798cfc19ab6a0a85a4a54a3370a37fc0428ed14560098bb89c5c75b55a6e82c354672d9278032ed2ab463985be719d650361cbfac672e278bbf21fdf2a4ce68707bf3547b62a9779a41674aab7c437203dc7c1f6dc6494015bb5ebb78a09b146177beac48c3d430f33f18a404c2269d1aa74d4203ef7b36159ab4f59bdfa743d95c8c8a0272b643cb448d547e53a9f7ef21e87b10fb326c6fe7301398a2f9415a248182204ebb0b8f91441444f19d68d1a8777c3d724a8135cf09bcb3521cb55392d86ba4d49a8aedd904181899a3f42962f16863e5b565332835a3bc74d738b9410951f67c29c5340221a038d0711308559b4c448c9429af5566964611b1fd875d17c685da12611f694b034442b56a9b426889172438b484aafa39aee76b19423ac43230a80699184607b61aa553038b7478c24cac497c1fa43157196d7f679a13b9f250ab31851c6aab9bc8f4b4181019a5598404411ad851271d59825b00b3d6321487eb733f392c06a647cdc1c1e9fc58d22c17691a1549302320f7485e0e01a92e83c539aa650595b2338a93721a56d8c2ebd8b3ce3133006c5afc7db99785a93292c36f347aaa08a4633354262872741816285e145395bbf4ee625c54185c96c193fbb489942a3e377ceb8b113bc04154bc198b0200bb76357f14cc6e359bea9682654e4a1ebe633ab4fdfd6e0d46a29e6f4da1584c1008654fe4a66c65ed58602801e4ad4f65339b4fed21779afadd0396ddb387ca83815c96d5eee0965c374bf866a82029e603d377bd02d5ca9728d08f81cb24f25688a7837404ce669ade0d08cd5e91e
shared secret 0 : 988707d9d3f460488f961a5a3d0346d9e6332713b6d36fcfeb5f75bf8eb73d98
shared secret 1 : 988707d9d3f460488f961a5a3d0346d9e6332713b6d36fcfeb5f75bf8eb73d98
```
