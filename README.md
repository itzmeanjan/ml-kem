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
2022-10-31T12:35:32+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.75, 1.97, 2.00
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            4.05 ns         4.05 ns    165447085
bench_kyber::ff_compound_add                   1.95 ns         1.95 ns    359610593
bench_kyber::ff_sub                            4.25 ns         4.25 ns    163606065
bench_kyber::ff_compound_sub                   2.11 ns         2.10 ns    336567588
bench_kyber::ff_neg                           0.804 ns        0.804 ns    826163415
bench_kyber::ff_mul                            6.74 ns         6.74 ns    101245317
bench_kyber::ff_compound_mul                   4.50 ns         4.50 ns    155357733
bench_kyber::ff_inv                            75.0 ns         74.9 ns     11804982
bench_kyber::ff_div                            92.7 ns         92.6 ns      8217892
bench_kyber::ff_exp                             407 ns          407 ns      1716755
bench_kyber::parse                             1835 ns         1834 ns       383268
bench_kyber::generate_matrix<2, true>          7948 ns         7942 ns        84572
bench_kyber::generate_matrix<2, false>         7902 ns         7895 ns        86572
bench_kyber::generate_matrix<3, true>         18028 ns        17934 ns        38735
bench_kyber::generate_matrix<3, false>        17855 ns        17843 ns        37450
bench_kyber::generate_matrix<4, true>         31888 ns        31842 ns        22073
bench_kyber::generate_matrix<4, false>        31746 ns        31718 ns        22033
bench_kyber::cbd<2>                            39.4 ns         39.4 ns     17915643
bench_kyber::cbd<3>                            97.4 ns         97.3 ns      7081651
bench_kyber::encode<1>                         44.7 ns         44.7 ns     15611619 bytes_per_second=683.368M/s
bench_kyber::decode<1>                         35.8 ns         35.7 ns     19459363 bytes_per_second=853.883M/s
bench_kyber::encode<4>                         18.0 ns         17.9 ns     38748962 bytes_per_second=6.64491G/s
bench_kyber::decode<4>                         15.9 ns         15.9 ns     44083381 bytes_per_second=7.49107G/s
bench_kyber::encode<5>                         79.1 ns         79.0 ns      8905626 bytes_per_second=1.88513G/s
bench_kyber::decode<5>                         63.1 ns         62.9 ns     11197313 bytes_per_second=2.36831G/s
bench_kyber::encode<10>                         399 ns          399 ns      1764349 bytes_per_second=764.994M/s
bench_kyber::decode<10>                        67.9 ns         67.8 ns     10257613 bytes_per_second=4.39385G/s
bench_kyber::encode<11>                         128 ns          128 ns      5502972 bytes_per_second=2.56567G/s
bench_kyber::decode<11>                         121 ns          121 ns      5791586 bytes_per_second=2.7031G/s
bench_kyber::encode<12>                         194 ns          194 ns      3612325 bytes_per_second=1.84316G/s
bench_kyber::decode<12>                        37.8 ns         37.8 ns     18502023 bytes_per_second=9.46565G/s
bench_kyber::compress<1>                       5.24 ns         5.24 ns    128987083
bench_kyber::decompress<1>                     4.62 ns         4.61 ns    151619185
bench_kyber::compress<4>                       5.28 ns         5.28 ns    128504030
bench_kyber::decompress<4>                     4.91 ns         4.90 ns    137314136
bench_kyber::compress<5>                       5.32 ns         5.30 ns    128130034
bench_kyber::decompress<5>                     4.87 ns         4.86 ns    137852262
bench_kyber::compress<10>                      4.98 ns         4.98 ns    133850890
bench_kyber::decompress<10>                    4.82 ns         4.82 ns    143810696
bench_kyber::compress<11>                      5.04 ns         5.04 ns    133592885
bench_kyber::decompress<11>                    4.85 ns         4.85 ns    145348535
bench_kyber::ntt                               1510 ns         1509 ns       475098
bench_kyber::intt                              1450 ns         1449 ns       476051
bench_kyber::polymul                            373 ns          373 ns      1876178
bench_kyber::random_data<32>                  11597 ns        11586 ns        58938
bench_kyber::pke_keygen<2, 3>                 32288 ns        32259 ns        21781 items_per_second=30.9992k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          21591 ns        21567 ns        32308 items_per_second=46.3671k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           5660 ns         5657 ns       120329 items_per_second=176.764k/s
bench_kyber::pke_keygen<3, 2>                 46581 ns        46565 ns        13689 items_per_second=21.4752k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          37423 ns        37389 ns        18687 items_per_second=26.7459k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           7784 ns         7774 ns        89445 items_per_second=128.627k/s
bench_kyber::pke_keygen<4, 2>                 67596 ns        67490 ns         9957 items_per_second=14.8169k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          58052 ns        57998 ns        11428 items_per_second=17.2418k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          10228 ns        10221 ns        67749 items_per_second=97.8366k/s
bench_kyber::kem_keygen<2, 3>                 46386 ns        46312 ns        15349 items_per_second=21.5925k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      39533 ns        39497 ns        17682 items_per_second=25.3187k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      30320 ns        30304 ns        23049 items_per_second=32.9989k/s
bench_kyber::kem_keygen<3, 2>                 62104 ns        62050 ns        10827 items_per_second=16.1159k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      58047 ns        58006 ns        11197 items_per_second=17.2396k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      48589 ns        48562 ns        13753 items_per_second=20.5921k/s
bench_kyber::kem_keygen<4, 2>                 83598 ns        83550 ns         7860 items_per_second=11.9689k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      80787 ns        80705 ns         8390 items_per_second=12.3908k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      72537 ns        72495 ns         8962 items_per_second=13.7941k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-10-31T08:39:18+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.15, 0.03, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            7.83 ns         7.83 ns     89365656
bench_kyber::ff_compound_add                   2.61 ns         2.61 ns    268171504
bench_kyber::ff_sub                            7.83 ns         7.83 ns     89385279
bench_kyber::ff_compound_sub                   2.61 ns         2.61 ns    268169310
bench_kyber::ff_neg                            2.25 ns         2.25 ns    310782527
bench_kyber::ff_mul                            11.7 ns         11.7 ns     59591184
bench_kyber::ff_compound_mul                   6.53 ns         6.53 ns    107263281
bench_kyber::ff_inv                            51.1 ns         51.1 ns     13693248
bench_kyber::ff_div                            50.2 ns         50.2 ns     10000000
bench_kyber::ff_exp                             844 ns          844 ns       848151
bench_kyber::parse                             9854 ns         9854 ns        71035
bench_kyber::generate_matrix<2, true>         41858 ns        41857 ns        16823
bench_kyber::generate_matrix<2, false>        41556 ns        41556 ns        16735
bench_kyber::generate_matrix<3, true>         93904 ns        93903 ns         7467
bench_kyber::generate_matrix<3, false>        94266 ns        94262 ns         7458
bench_kyber::generate_matrix<4, true>        167749 ns       167746 ns         4186
bench_kyber::generate_matrix<4, false>       167297 ns       167295 ns         4184
bench_kyber::cbd<2>                             221 ns          221 ns      3161288
bench_kyber::cbd<3>                             328 ns          328 ns      2133907
bench_kyber::encode<1>                         95.5 ns         95.5 ns      7332397 bytes_per_second=319.679M/s
bench_kyber::decode<1>                          344 ns          344 ns      2032331 bytes_per_second=88.6064M/s
bench_kyber::encode<4>                         79.2 ns         79.2 ns      8837986 bytes_per_second=1.50519G/s
bench_kyber::decode<4>                          110 ns          110 ns      6383062 bytes_per_second=1113.12M/s
bench_kyber::encode<5>                          247 ns          247 ns      2831901 bytes_per_second=617.377M/s
bench_kyber::decode<5>                          269 ns          269 ns      2603469 bytes_per_second=567.516M/s
bench_kyber::encode<10>                         279 ns          279 ns      2510097 bytes_per_second=1094.31M/s
bench_kyber::decode<10>                         342 ns          342 ns      2044161 bytes_per_second=891.193M/s
bench_kyber::encode<11>                         331 ns          331 ns      2117791 bytes_per_second=1013.75M/s
bench_kyber::decode<11>                         352 ns          352 ns      1988305 bytes_per_second=953.566M/s
bench_kyber::encode<12>                         151 ns          151 ns      4705524 bytes_per_second=2.37382G/s
bench_kyber::decode<12>                         151 ns          151 ns      4641327 bytes_per_second=2.37118G/s
bench_kyber::compress<1>                       6.96 ns         6.96 ns    100555555
bench_kyber::decompress<1>                     8.27 ns         8.27 ns     84682827
bench_kyber::compress<4>                       6.96 ns         6.96 ns    100559057
bench_kyber::decompress<4>                     8.27 ns         8.27 ns     84679631
bench_kyber::compress<5>                       6.96 ns         6.96 ns    100544549
bench_kyber::decompress<5>                     8.27 ns         8.27 ns     84682778
bench_kyber::compress<10>                      6.96 ns         6.96 ns    100558502
bench_kyber::decompress<10>                    8.27 ns         8.27 ns     84680380
bench_kyber::compress<11>                      6.96 ns         6.96 ns    100559022
bench_kyber::decompress<11>                    8.27 ns         8.27 ns     84683515
bench_kyber::ntt                               5268 ns         5268 ns       132884
bench_kyber::intt                              5178 ns         5178 ns       135232
bench_kyber::polymul                            859 ns          859 ns       815030
bench_kyber::random_data<32>                   4635 ns         4635 ns       151022
bench_kyber::pke_keygen<2, 3>                 90190 ns        90187 ns         7762 items_per_second=11.088k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          91388 ns        91386 ns         7659 items_per_second=10.9426k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          19963 ns        19963 ns        35039 items_per_second=50.093k/s
bench_kyber::pke_keygen<3, 2>                153538 ns       153536 ns         4559 items_per_second=6.51315k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         160091 ns       160087 ns         4372 items_per_second=6.24659k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          27105 ns        27104 ns        25842 items_per_second=36.8949k/s
bench_kyber::pke_keygen<4, 2>                247746 ns       247742 ns         2826 items_per_second=4.03646k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         255858 ns       255846 ns         2730 items_per_second=3.9086k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          34153 ns        34152 ns        20502 items_per_second=29.2811k/s
bench_kyber::kem_keygen<2, 3>                106812 ns       106808 ns         6553 items_per_second=9.36263k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>     125773 ns       125770 ns         5575 items_per_second=7.95099k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     128546 ns       128544 ns         5444 items_per_second=7.77943k/s
bench_kyber::kem_keygen<3, 2>                175962 ns       175961 ns         3978 items_per_second=5.68309k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     205912 ns       205910 ns         3394 items_per_second=4.8565k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     210572 ns       210569 ns         3329 items_per_second=4.74904k/s
bench_kyber::kem_keygen<4, 2>                275852 ns       275848 ns         2537 items_per_second=3.62519k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     313770 ns       313769 ns         2225 items_per_second=3.18706k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     319246 ns       319242 ns         2187 items_per_second=3.13242k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-10-31T08:42:31+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.08, 0.02, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.17 ns         6.17 ns    113514176
bench_kyber::ff_compound_add                   2.31 ns         2.31 ns    302859820
bench_kyber::ff_sub                            6.21 ns         6.21 ns    113335473
bench_kyber::ff_compound_sub                   2.31 ns         2.31 ns    302839173
bench_kyber::ff_neg                            1.33 ns         1.33 ns    528988682
bench_kyber::ff_mul                            8.48 ns         8.47 ns     82594912
bench_kyber::ff_compound_mul                   4.62 ns         4.62 ns    151400656
bench_kyber::ff_inv                            34.2 ns         34.2 ns     34510730
bench_kyber::ff_div                            44.2 ns         44.2 ns     38788993
bench_kyber::ff_exp                             581 ns          581 ns      1219969
bench_kyber::parse                             4584 ns         4583 ns       152853
bench_kyber::generate_matrix<2, true>         18944 ns        18944 ns        37064
bench_kyber::generate_matrix<2, false>        18983 ns        18982 ns        36802
bench_kyber::generate_matrix<3, true>         42735 ns        42734 ns        16420
bench_kyber::generate_matrix<3, false>        42670 ns        42669 ns        16368
bench_kyber::generate_matrix<4, true>         76215 ns        76214 ns         9233
bench_kyber::generate_matrix<4, false>        75513 ns        75510 ns         9238
bench_kyber::cbd<2>                            55.9 ns         55.9 ns     12551156
bench_kyber::cbd<3>                             102 ns          102 ns      6836812
bench_kyber::encode<1>                         46.2 ns         46.2 ns     15152155 bytes_per_second=660.023M/s
bench_kyber::decode<1>                          130 ns          130 ns      5369221 bytes_per_second=233.997M/s
bench_kyber::encode<4>                         23.8 ns         23.8 ns     29338347 bytes_per_second=5.00644G/s
bench_kyber::decode<4>                         29.8 ns         29.8 ns     23507105 bytes_per_second=4.00289G/s
bench_kyber::encode<5>                         81.9 ns         81.9 ns      8545608 bytes_per_second=1.81923G/s
bench_kyber::decode<5>                          110 ns          110 ns      6347798 bytes_per_second=1.35091G/s
bench_kyber::encode<10>                         102 ns          102 ns      6896720 bytes_per_second=2.93609G/s
bench_kyber::decode<10>                         115 ns          115 ns      6100283 bytes_per_second=2.5982G/s
bench_kyber::encode<11>                         115 ns          115 ns      6087746 bytes_per_second=2.84887G/s
bench_kyber::decode<11>                         132 ns          132 ns      5313773 bytes_per_second=2.48774G/s
bench_kyber::encode<12>                        34.9 ns         34.9 ns     20024595 bytes_per_second=10.237G/s
bench_kyber::decode<12>                        43.2 ns         43.2 ns     16186703 bytes_per_second=8.27499G/s
bench_kyber::compress<1>                       5.45 ns         5.45 ns    127925833
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113568436
bench_kyber::compress<4>                       5.45 ns         5.45 ns    128264933
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113573959
bench_kyber::compress<5>                       5.45 ns         5.45 ns    128150906
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113573835
bench_kyber::compress<10>                      5.45 ns         5.45 ns    128323329
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113577303
bench_kyber::compress<11>                      5.44 ns         5.44 ns    128932971
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113570979
bench_kyber::ntt                               2042 ns         2042 ns       343084
bench_kyber::intt                              2143 ns         2142 ns       326278
bench_kyber::polymul                            296 ns          296 ns      2365999
bench_kyber::random_data<32>                   1673 ns         1673 ns       418595
bench_kyber::pke_keygen<2, 3>                 38704 ns        38703 ns        18086 items_per_second=25.8376k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          38590 ns        38589 ns        18126 items_per_second=25.9139k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7519 ns         7519 ns        93108 items_per_second=133.003k/s
bench_kyber::pke_keygen<3, 2>                 66576 ns        66574 ns        10505 items_per_second=15.0208k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          68202 ns        68201 ns        10242 items_per_second=14.6625k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          10082 ns        10082 ns        69438 items_per_second=99.1908k/s
bench_kyber::pke_keygen<4, 2>                108592 ns       108589 ns         6447 items_per_second=9.20899k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         110383 ns       110380 ns         6348 items_per_second=9.05957k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          12767 ns        12767 ns        54841 items_per_second=78.329k/s
bench_kyber::kem_keygen<2, 3>                 45772 ns        45771 ns        15291 items_per_second=21.848k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      54434 ns        54433 ns        12855 items_per_second=18.3711k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      54189 ns        54187 ns        12923 items_per_second=18.4545k/s
bench_kyber::kem_keygen<3, 2>                 75423 ns        75421 ns         9299 items_per_second=13.2589k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      89968 ns        89965 ns         7782 items_per_second=11.1154k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      89476 ns        89473 ns         7824 items_per_second=11.1765k/s
bench_kyber::kem_keygen<4, 2>                119179 ns       119177 ns         5871 items_per_second=8.39088k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     137433 ns       137431 ns         5086 items_per_second=7.2764k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     136844 ns       136842 ns         5111 items_per_second=7.30771k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-10-31T08:44:59+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.31, 0.21, 0.09
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            5.78 ns         5.78 ns    121097528
bench_kyber::ff_compound_add                   3.47 ns         3.47 ns    201781447
bench_kyber::ff_sub                            6.16 ns         6.16 ns    113567418
bench_kyber::ff_compound_sub                   3.85 ns         3.85 ns    181708670
bench_kyber::ff_neg                           0.890 ns        0.890 ns    785981186
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82588770
bench_kyber::ff_compound_mul                   6.16 ns         6.16 ns    113564704
bench_kyber::ff_inv                            22.7 ns         22.7 ns     23553929
bench_kyber::ff_div                            32.2 ns         32.2 ns     24719836
bench_kyber::ff_exp                             448 ns          448 ns      1481816
bench_kyber::parse                             2443 ns         2443 ns       286455
bench_kyber::generate_matrix<2, true>         10687 ns        10687 ns        65662
bench_kyber::generate_matrix<2, false>        10688 ns        10688 ns        65539
bench_kyber::generate_matrix<3, true>         24079 ns        24078 ns        29132
bench_kyber::generate_matrix<3, false>        24208 ns        24208 ns        29086
bench_kyber::generate_matrix<4, true>         42420 ns        42419 ns        16408
bench_kyber::generate_matrix<4, false>        42550 ns        42548 ns        16404
bench_kyber::cbd<2>                             101 ns          101 ns      6920052
bench_kyber::cbd<3>                             152 ns          152 ns      4589456
bench_kyber::encode<1>                         46.5 ns         46.5 ns     15060891 bytes_per_second=656.654M/s
bench_kyber::decode<1>                         44.3 ns         44.3 ns     15812565 bytes_per_second=688.99M/s
bench_kyber::encode<4>                         9.64 ns         9.64 ns     72637667 bytes_per_second=12.3691G/s
bench_kyber::decode<4>                         15.8 ns         15.8 ns     44120425 bytes_per_second=7.52511G/s
bench_kyber::encode<5>                         57.9 ns         57.9 ns     12097707 bytes_per_second=2.57564G/s
bench_kyber::decode<5>                         69.2 ns         69.2 ns     10121316 bytes_per_second=2.15465G/s
bench_kyber::encode<10>                        53.5 ns         53.5 ns     13070395 bytes_per_second=5.56729G/s
bench_kyber::decode<10>                        55.6 ns         55.6 ns     12611169 bytes_per_second=5.36209G/s
bench_kyber::encode<11>                        91.3 ns         91.3 ns      7672638 bytes_per_second=3.59223G/s
bench_kyber::decode<11>                         106 ns          106 ns      6615420 bytes_per_second=3.09784G/s
bench_kyber::encode<12>                        24.4 ns         24.4 ns     28666143 bytes_per_second=14.6386G/s
bench_kyber::decode<12>                        32.6 ns         32.6 ns     21445560 bytes_per_second=10.9541G/s
bench_kyber::compress<1>                       6.55 ns         6.55 ns    106883647
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113573061
bench_kyber::compress<4>                       6.55 ns         6.55 ns    106881195
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113565238
bench_kyber::compress<5>                       6.55 ns         6.55 ns    106883264
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113562472
bench_kyber::compress<10>                      6.55 ns         6.55 ns    106871923
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113567851
bench_kyber::compress<11>                      6.55 ns         6.55 ns    106887068
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113559307
bench_kyber::ntt                               1720 ns         1720 ns       406946
bench_kyber::intt                              1990 ns         1990 ns       351979
bench_kyber::polymul                            385 ns          385 ns      1816444
bench_kyber::random_data<32>                   1731 ns         1731 ns       404471
bench_kyber::pke_keygen<2, 3>                 26026 ns        26025 ns        26896 items_per_second=38.4239k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          27871 ns        27870 ns        25119 items_per_second=35.8804k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7945 ns         7945 ns        88106 items_per_second=125.866k/s
bench_kyber::pke_keygen<3, 2>                 44206 ns        44205 ns        15835 items_per_second=22.622k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          47948 ns        47946 ns        14629 items_per_second=20.8567k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          10622 ns        10621 ns        65899 items_per_second=94.1495k/s
bench_kyber::pke_keygen<4, 2>                 70820 ns        70819 ns         9884 items_per_second=14.1206k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          75511 ns        75509 ns         9260 items_per_second=13.2434k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          13569 ns        13569 ns        51578 items_per_second=73.6972k/s
bench_kyber::kem_keygen<2, 3>                 30767 ns        30766 ns        22750 items_per_second=32.5035k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      36587 ns        36586 ns        19105 items_per_second=27.3327k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      39655 ns        39654 ns        17628 items_per_second=25.2182k/s
bench_kyber::kem_keygen<3, 2>                 50357 ns        50356 ns        13900 items_per_second=19.8587k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      59388 ns        59386 ns        11787 items_per_second=16.8391k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      63720 ns        63719 ns        10992 items_per_second=15.694k/s
bench_kyber::kem_keygen<4, 2>                 78463 ns        78460 ns         8922 items_per_second=12.7453k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      89957 ns        89955 ns         7791 items_per_second=11.1166k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      95837 ns        95835 ns         7304 items_per_second=10.4346k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2022-10-31T08:48:20+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.14, 0.04, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.72 ns         6.72 ns    103983448
bench_kyber::ff_compound_add                   2.97 ns         2.97 ns    235707930
bench_kyber::ff_sub                            6.70 ns         6.69 ns    104571298
bench_kyber::ff_compound_sub                   2.97 ns         2.97 ns    235582833
bench_kyber::ff_neg                           0.909 ns        0.909 ns    771055145
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64945709
bench_kyber::ff_compound_mul                   7.06 ns         7.06 ns     99166921
bench_kyber::ff_inv                             103 ns          103 ns      8112757
bench_kyber::ff_div                            71.0 ns         71.0 ns      8551809
bench_kyber::ff_exp                             778 ns          778 ns       879123
bench_kyber::parse                             4983 ns         4983 ns       140507
bench_kyber::generate_matrix<2, true>         21974 ns        21972 ns        31901
bench_kyber::generate_matrix<2, false>        21881 ns        21881 ns        31784
bench_kyber::generate_matrix<3, true>         49821 ns        49815 ns        14212
bench_kyber::generate_matrix<3, false>        49541 ns        49537 ns        14153
bench_kyber::generate_matrix<4, true>         88418 ns        88408 ns         7984
bench_kyber::generate_matrix<4, false>        87097 ns        87097 ns         7982
bench_kyber::cbd<2>                            67.5 ns         67.5 ns     10368715
bench_kyber::cbd<3>                             155 ns          155 ns      4505938
bench_kyber::encode<1>                         56.9 ns         56.9 ns     12310270 bytes_per_second=536.583M/s
bench_kyber::decode<1>                          293 ns          293 ns      2389686 bytes_per_second=104.225M/s
bench_kyber::encode<4>                         29.9 ns         29.9 ns     23404515 bytes_per_second=3.98584G/s
bench_kyber::decode<4>                         29.0 ns         29.0 ns     24119374 bytes_per_second=4.10816G/s
bench_kyber::encode<5>                          168 ns          168 ns      4178519 bytes_per_second=910.218M/s
bench_kyber::decode<5>                          216 ns          216 ns      3251270 bytes_per_second=707.797M/s
bench_kyber::encode<10>                         210 ns          210 ns      3336719 bytes_per_second=1.42168G/s
bench_kyber::decode<10>                         291 ns          291 ns      2403482 bytes_per_second=1048.03M/s
bench_kyber::encode<11>                         276 ns          276 ns      2530829 bytes_per_second=1.18569G/s
bench_kyber::decode<11>                         284 ns          284 ns      2461649 bytes_per_second=1.15356G/s
bench_kyber::encode<12>                        86.2 ns         86.2 ns      8115134 bytes_per_second=4.14778G/s
bench_kyber::decode<12>                        89.6 ns         89.6 ns      7801747 bytes_per_second=3.99085G/s
bench_kyber::compress<1>                       7.42 ns         7.42 ns     94299883
bench_kyber::decompress<1>                     6.11 ns         6.11 ns    114614096
bench_kyber::compress<4>                       7.43 ns         7.43 ns     94193432
bench_kyber::decompress<4>                     6.43 ns         6.43 ns    108956144
bench_kyber::compress<5>                       7.43 ns         7.43 ns     94260062
bench_kyber::decompress<5>                     6.42 ns         6.42 ns    109010082
bench_kyber::compress<10>                      7.43 ns         7.43 ns     94256106
bench_kyber::decompress<10>                    6.42 ns         6.42 ns    108991888
bench_kyber::compress<11>                      7.43 ns         7.43 ns     94248791
bench_kyber::decompress<11>                    6.42 ns         6.42 ns    108889235
bench_kyber::ntt                               3268 ns         3268 ns       214302
bench_kyber::intt                              5183 ns         5183 ns       135126
bench_kyber::polymul                            626 ns          626 ns      1118030
bench_kyber::random_data<32>                   3305 ns         3305 ns       211844
bench_kyber::pke_keygen<2, 3>                 57896 ns        57895 ns        12099 items_per_second=17.2727k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          61052 ns        61051 ns        11469 items_per_second=16.3799k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          14555 ns        14554 ns        48210 items_per_second=68.7074k/s
bench_kyber::pke_keygen<3, 2>                 92096 ns        92094 ns         7602 items_per_second=10.8585k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         102106 ns       102106 ns         6872 items_per_second=9.79378k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          18953 ns        18951 ns        36873 items_per_second=52.7682k/s
bench_kyber::pke_keygen<4, 2>                146357 ns       146341 ns         4783 items_per_second=6.83337k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         159644 ns       159636 ns         4370 items_per_second=6.26425k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          23663 ns        23662 ns        29605 items_per_second=42.2624k/s
bench_kyber::kem_keygen<2, 3>                 64018 ns        64013 ns        10939 items_per_second=15.6217k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      80460 ns        80449 ns         8689 items_per_second=12.4302k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      85235 ns        85223 ns         8237 items_per_second=11.734k/s
bench_kyber::kem_keygen<3, 2>                105087 ns       105076 ns         6660 items_per_second=9.51693k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     130027 ns       130021 ns         5384 items_per_second=7.69109k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     134300 ns       134294 ns         5217 items_per_second=7.44632k/s
bench_kyber::kem_keygen<4, 2>                162708 ns       162685 ns         4302 items_per_second=6.14685k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     189720 ns       189700 ns         3689 items_per_second=5.27147k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     198246 ns       198223 ns         3515 items_per_second=5.04483k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2022-10-31T08:50:43+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.41, 0.25, 0.10
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.74 ns         6.74 ns    103868898
bench_kyber::ff_compound_add                   3.34 ns         3.34 ns    209292760
bench_kyber::ff_sub                            7.10 ns         7.10 ns     98676625
bench_kyber::ff_compound_sub                   3.78 ns         3.78 ns    185407086
bench_kyber::ff_neg                            1.11 ns         1.11 ns    628307123
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64990975
bench_kyber::ff_compound_mul                   7.42 ns         7.42 ns     94290151
bench_kyber::ff_inv                             108 ns          108 ns      5850620
bench_kyber::ff_div                            98.0 ns         98.0 ns      8040528
bench_kyber::ff_exp                             629 ns          629 ns      1109808
bench_kyber::parse                             2716 ns         2716 ns       257780
bench_kyber::generate_matrix<2, true>         11911 ns        11911 ns        58735
bench_kyber::generate_matrix<2, false>        11871 ns        11871 ns        59090
bench_kyber::generate_matrix<3, true>         26718 ns        26717 ns        26116
bench_kyber::generate_matrix<3, false>        26709 ns        26708 ns        26048
bench_kyber::generate_matrix<4, true>         47197 ns        47195 ns        14753
bench_kyber::generate_matrix<4, false>        47091 ns        47090 ns        14867
bench_kyber::cbd<2>                            73.1 ns         73.1 ns      9571038
bench_kyber::cbd<3>                             166 ns          166 ns      4225899
bench_kyber::encode<1>                         37.1 ns         37.1 ns     18845934 bytes_per_second=821.824M/s
bench_kyber::decode<1>                         61.3 ns         61.3 ns     11424920 bytes_per_second=498.233M/s
bench_kyber::encode<4>                         47.9 ns         47.9 ns     14612493 bytes_per_second=2.48779G/s
bench_kyber::decode<4>                         35.7 ns         35.7 ns     19623190 bytes_per_second=3.34375G/s
bench_kyber::encode<5>                         72.0 ns         71.9 ns      9727406 bytes_per_second=2.07108G/s
bench_kyber::decode<5>                         83.6 ns         83.6 ns      8367266 bytes_per_second=1.78164G/s
bench_kyber::encode<10>                         217 ns          217 ns      3227127 bytes_per_second=1.37431G/s
bench_kyber::decode<10>                         104 ns          104 ns      6723019 bytes_per_second=2.86167G/s
bench_kyber::encode<11>                         182 ns          182 ns      3864285 bytes_per_second=1.80456G/s
bench_kyber::decode<11>                         193 ns          193 ns      3619132 bytes_per_second=1.6944G/s
bench_kyber::encode<12>                         113 ns          113 ns      6200737 bytes_per_second=3.16777G/s
bench_kyber::decode<12>                        70.8 ns         70.8 ns      9884241 bytes_per_second=5.05031G/s
bench_kyber::compress<1>                       8.58 ns         8.58 ns     81562381
bench_kyber::decompress<1>                     7.57 ns         7.57 ns     92453301
bench_kyber::compress<4>                       8.57 ns         8.57 ns     81717442
bench_kyber::decompress<4>                     7.93 ns         7.93 ns     88226896
bench_kyber::compress<5>                       8.57 ns         8.57 ns     81729622
bench_kyber::decompress<5>                     7.93 ns         7.93 ns     88322652
bench_kyber::compress<10>                      8.17 ns         8.17 ns     85730960
bench_kyber::decompress<10>                    7.92 ns         7.92 ns     88270166
bench_kyber::compress<11>                      8.17 ns         8.17 ns     85714363
bench_kyber::decompress<11>                    7.93 ns         7.93 ns     88317295
bench_kyber::ntt                               2503 ns         2503 ns       279640
bench_kyber::intt                              2468 ns         2468 ns       283587
bench_kyber::polymul                            742 ns          741 ns       943903
bench_kyber::random_data<32>                   3381 ns         3381 ns       206944
bench_kyber::pke_keygen<2, 3>                 35808 ns        35808 ns        19543 items_per_second=27.9269k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          35759 ns        35758 ns        19668 items_per_second=27.9659k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          10153 ns        10152 ns        68947 items_per_second=98.4992k/s
bench_kyber::pke_keygen<3, 2>                 59904 ns        59901 ns        11686 items_per_second=16.6942k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          61777 ns        61775 ns        11377 items_per_second=16.1877k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          13920 ns        13920 ns        50275 items_per_second=71.8407k/s
bench_kyber::pke_keygen<4, 2>                 93013 ns        93011 ns         7528 items_per_second=10.7514k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          95590 ns        95588 ns         7315 items_per_second=10.4616k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          17704 ns        17704 ns        39518 items_per_second=56.4841k/s
bench_kyber::kem_keygen<2, 3>                 42551 ns        42549 ns        16447 items_per_second=23.5021k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      47255 ns        47255 ns        14804 items_per_second=21.1617k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      50224 ns        50221 ns        13925 items_per_second=19.912k/s
bench_kyber::kem_keygen<3, 2>                 68210 ns        68207 ns        10257 items_per_second=14.6612k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      76505 ns        76500 ns         9157 items_per_second=13.0719k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      81976 ns        81977 ns         8554 items_per_second=12.1985k/s
bench_kyber::kem_keygen<4, 2>                102966 ns       102965 ns         6800 items_per_second=9.71202k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     113421 ns       113415 ns         6181 items_per_second=8.81717k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     121215 ns       121207 ns         5763 items_per_second=8.25033k/s
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
