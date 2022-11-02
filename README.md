# kyber
CRYSTALS-Kyber: Post-Quantum Public-key Encryption &amp; Key-establishment Algorithm

## Motivation

Kyber is selected by NIST as post-quantum secure public key encryption (PKE) and key exchange mechanism (KEM) as part of NIST's post-quantum cryptography (PQC) standardization initiative.

Kyber offers both

- IND-CPA-secure public key encryption [Kyber CPAPKE]
- IND-CCA2-secure key encapsulation mechanism [Kyber CCAKEM]

while its security is based on hardness of solving learning-with-errors (LWE) problem in module lattices.

Under **IND-CPA-secure Kyber PKE**, two communicating parties both generating their key pairs while publishing their public keys to each other, can encrypt fixed length ( = 32 -bytes ) message using peer's public key. Cipher text can be decrypted by respective secret key ( which is private to key owner ) and 32 -bytes message can be recovered back.

Algorithm | Input | Output
--- | :-: | --:
PKE KeyGen | - | Public Key and Secret Key
Encryption | Public Key, 32 -bytes message and 32 -bytes random coin | Cipher Text
Decryption | Secret Key and Cipher Text | 32 -bytes message

> **Note**

> When a slightly tweaked Fujisaki–Okamoto (FO) transform is applied on IND-CPA-secure Kyber PKE, we can construct an IND-CCA2-secure KEM.

While with **IND-CCA2-secure Kyber KEM**, two parties interested in secretly communicating over public & insecure channel, can generate a shared secret key ( of arbitrary byte length ) from a key derivation function ( i.e. KDF which is SHAKE256 in this context ) which is obtained by both of these parties as result of seeding SHAKE256 with same secret. This secret is 32 -bytes and that's what is communicated by sender to receiver using underlying Kyber PKE.

Algorithm | Input | Output
--- | :-: | --:
KEM KeyGen | - | Public Key and Secret Key
Encapsulation | Public Key | Cipher Text and SHAKE256 KDF
Decapsulation | Secret Key and Cipher Text | SHAKE256 KDF

> IND-CPA-secure Kyber PKE can be used for asynchronous secure communication such as email.

> IND-CCA2-secure Kyber KEM can be used for synchronous secure communication such as TLS.

Here I'm developing & maintaining `kyber` - a zero-dependency, header-only and easy-to-use C++ library implementing Kyber PKE and KEM, supporting Kyber-{512, 768, 1024} parameter sets, as defined in table 1 of Kyber specification. 

Only dependency is `sha3`, which itself is a zero-dependency, header-only C++ library that I decided to write to modularize a common PQC dependency i.e. SHA3 hash functions and extendable output functions are fairly common symmetric key primitive used in post-quantum cryptographic constructions such as Kyber, Dilithium, Falcon etc.

- `sha3` is pinned to specific commit, using git submodule, in `kyber`
- See [usage](#usage) section below for git submodule set up guide.
- Find more about `sha3` [here](https://github.com/itzmeanjan/sha3.git)

> **Note**

> Find Kyber specification [here](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf)

> See NIST selected PQC candidates [here](https://csrc.nist.gov/Projects/post-quantum-cryptography/selected-algorithms-2022)

> Find progress of NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography)

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

- For testing functional correctness and conformance to standard, you'll need to have `python3` and related Python dependencies installed.

```bash
$ python3 --version
Python 3.10.8

# Downlaod Python dependencies
$ python3 -m pip install -r wrapper/python/requirements.txt --user
```

- For benchmarking Kyber implementation on CPU systems, you'll need to have `google-benchmark` globally installed. You may want to follow [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide.

- For importing `sha3` dependency, initialize & update git submodule after cloning this repository

```bash
git clone https://github.com/itzmeanjan/kyber.git
cd kyber
git submodule update --init

# Now you can {test, benchmark, use} `kyber`
```

## Testing

For testing functional correctness and compatibility of this Kyber implementation ( along with its component units ), you have to issue

```bash
make # testing + test_kat

# If you want to avoid running tests against KATs, issue
make testing

# If you want to only test against KATs, issue
make test_kat
```

```bash
[test] Kyber prime field operations
[test] (i)NTT over degree-255 polynomial
[test] Polynomial serialization/ deserialization
[test] Coefficient compression/ decompression
[test] INDCPA-secure Public Key Encryption
[test] INDCCA2-secure Key Encapsulation Mechanism
bash test_kat.sh
make[1]: Nothing to be done for `lib'.
~/Documents/work/kyber/wrapper/python ~/Documents/work/kyber
=========================================================================================== test session starts ============================================================================================
platform darwin -- Python 3.10.8, pytest-7.1.3, pluggy-1.0.0 -- /usr/local/opt/python@3.10/bin/python3.10
cachedir: .pytest_cache
benchmark: 3.4.1 (defaults: timer=time.perf_counter disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
rootdir: /Users/anjan/Documents/work/kyber/wrapper/python
plugins: benchmark-3.4.1
collected 3 items

test_kyber_kem.py::test_kyber512 PASSED                                                                                                                                                              [ 33%]
test_kyber_kem.py::test_kyber768 PASSED                                                                                                                                                              [ 66%]
test_kyber_kem.py::test_kyber1024 PASSED                                                                                                                                                             [100%]

============================================================================================ 3 passed in 0.63s =============================================================================================
~/Documents/work/kyber
```

## Benchmarking

For benchmarking this Kyber implementation ( along with its components ) on CPU systems, you need to issue

```bash
make benchmark
```

> **Warning**

> Because most of the CPUs employ dynamic frequency boosting technique, when benchmarking routines, you may want to disable CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-11-01T12:58:37+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.53, 1.82, 2.22
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            3.98 ns         3.98 ns    173441627
bench_kyber::ff_compound_add                   2.09 ns         2.07 ns    358886018
bench_kyber::ff_sub                            4.36 ns         4.35 ns    158630880
bench_kyber::ff_compound_sub                   2.20 ns         2.18 ns    326427069
bench_kyber::ff_neg                           0.806 ns        0.805 ns    854826104
bench_kyber::ff_mul                            6.75 ns         6.74 ns    102879146
bench_kyber::ff_compound_mul                   4.48 ns         4.47 ns    157113422
bench_kyber::ff_inv                             100 ns          100 ns     10000000
bench_kyber::ff_div                            51.1 ns         51.1 ns     10092709
bench_kyber::ff_exp                             410 ns          410 ns      1734696
bench_kyber::parse                             2162 ns         2161 ns       314605
bench_kyber::generate_matrix<2, true>          9293 ns         9284 ns        74171
bench_kyber::generate_matrix<2, false>         9205 ns         9198 ns        75258
bench_kyber::generate_matrix<3, true>         20605 ns        20596 ns        33275
bench_kyber::generate_matrix<3, false>        21074 ns        20982 ns        33803
bench_kyber::generate_matrix<4, true>         36987 ns        36936 ns        19007
bench_kyber::generate_matrix<4, false>        37258 ns        37218 ns        19208
bench_kyber::cbd<2>                            39.3 ns         39.2 ns     17917982
bench_kyber::cbd<3>                            97.9 ns         97.8 ns      7007989
bench_kyber::encode<1>                         44.4 ns         44.4 ns     15793404 bytes_per_second=687.902M/s
bench_kyber::decode<1>                         35.7 ns         35.7 ns     19597797 bytes_per_second=855.24M/s
bench_kyber::encode<4>                         18.0 ns         17.9 ns     39057705 bytes_per_second=6.64392G/s
bench_kyber::decode<4>                         15.9 ns         15.9 ns     44094210 bytes_per_second=7.51432G/s
bench_kyber::encode<5>                         78.1 ns         78.1 ns      8905852 bytes_per_second=1.90839G/s
bench_kyber::decode<5>                         62.1 ns         62.1 ns     11123647 bytes_per_second=2.4008G/s
bench_kyber::encode<10>                         399 ns          398 ns      1764513 bytes_per_second=766.292M/s
bench_kyber::decode<10>                        67.4 ns         67.3 ns     10249502 bytes_per_second=4.42792G/s
bench_kyber::encode<11>                         127 ns          127 ns      5497569 bytes_per_second=2.58286G/s
bench_kyber::decode<11>                         121 ns          121 ns      5671139 bytes_per_second=2.7094G/s
bench_kyber::encode<12>                         192 ns          192 ns      3597751 bytes_per_second=1.86614G/s
bench_kyber::decode<12>                        37.9 ns         37.9 ns     18675382 bytes_per_second=9.44575G/s
bench_kyber::compress<1>                       3.48 ns         3.48 ns    199230401
bench_kyber::decompress<1>                     4.23 ns         4.23 ns    164535540
bench_kyber::compress<4>                       3.47 ns         3.47 ns    199662286
bench_kyber::decompress<4>                     4.21 ns         4.21 ns    166740112
bench_kyber::compress<5>                       3.49 ns         3.48 ns    201197990
bench_kyber::decompress<5>                     4.21 ns         4.21 ns    166470469
bench_kyber::compress<10>                      3.50 ns         3.49 ns    200092042
bench_kyber::decompress<10>                    4.21 ns         4.21 ns    166744878
bench_kyber::compress<11>                      3.54 ns         3.53 ns    198303654
bench_kyber::decompress<11>                    4.22 ns         4.21 ns    166788979
bench_kyber::ntt                               1484 ns         1482 ns       468299
bench_kyber::intt                              1425 ns         1425 ns       491438
bench_kyber::polymul                            369 ns          369 ns      1889803
bench_kyber::random_data<32>                  11687 ns        11670 ns        58157
bench_kyber::pke_keygen<2, 3>                 20972 ns        20958 ns        33039 items_per_second=47.7135k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          22719 ns        22705 ns        28724 items_per_second=44.043k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           5671 ns         5663 ns       111028 items_per_second=176.588k/s
bench_kyber::pke_keygen<3, 2>                 37180 ns        37155 ns        18633 items_per_second=26.9144k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          41523 ns        41334 ns        16629 items_per_second=24.193k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           8107 ns         8071 ns        84272 items_per_second=123.904k/s
bench_kyber::pke_keygen<4, 2>                 63764 ns        63625 ns        10917 items_per_second=15.717k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          65369 ns        65201 ns         9475 items_per_second=15.3373k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          10236 ns        10218 ns        67106 items_per_second=97.8644k/s
bench_kyber::kem_keygen<2, 3>                 23048 ns        23025 ns        30009 items_per_second=43.4318k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      27794 ns        27768 ns        25043 items_per_second=36.0129k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      30928 ns        30912 ns        22176 items_per_second=32.3497k/s
bench_kyber::kem_keygen<3, 2>                 40539 ns        40506 ns        17152 items_per_second=24.6877k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      47923 ns        47879 ns        14828 items_per_second=20.8862k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      51955 ns        51902 ns        12725 items_per_second=19.2671k/s
bench_kyber::kem_keygen<4, 2>                 64043 ns        63997 ns        10380 items_per_second=15.6258k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      71999 ns        71930 ns         9489 items_per_second=13.9025k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      77664 ns        77599 ns         8569 items_per_second=12.8868k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-11-01T09:02:23+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.36, 0.09, 0.03
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            7.83 ns         7.83 ns     89351926
bench_kyber::ff_compound_add                   2.61 ns         2.61 ns    268163220
bench_kyber::ff_sub                            7.83 ns         7.83 ns     89387539
bench_kyber::ff_compound_sub                   2.61 ns         2.61 ns    268173304
bench_kyber::ff_neg                            2.25 ns         2.25 ns    310700374
bench_kyber::ff_mul                            11.7 ns         11.7 ns     59591221
bench_kyber::ff_compound_mul                   6.53 ns         6.53 ns    107266950
bench_kyber::ff_inv                            48.4 ns         48.4 ns     12992777
bench_kyber::ff_div                            35.3 ns         35.3 ns     15751668
bench_kyber::ff_exp                             840 ns          840 ns       846324
bench_kyber::parse                             7866 ns         7866 ns        88972
bench_kyber::generate_matrix<2, true>         32264 ns        32264 ns        21847
bench_kyber::generate_matrix<2, false>        32390 ns        32389 ns        21639
bench_kyber::generate_matrix<3, true>         72586 ns        72584 ns         9642
bench_kyber::generate_matrix<3, false>        72703 ns        72701 ns         9647
bench_kyber::generate_matrix<4, true>        129328 ns       129327 ns         5433
bench_kyber::generate_matrix<4, false>       128551 ns       128549 ns         5416
bench_kyber::cbd<2>                             222 ns          222 ns      3147606
bench_kyber::cbd<3>                             328 ns          328 ns      2133918
bench_kyber::encode<1>                         95.5 ns         95.5 ns      7332494 bytes_per_second=319.677M/s
bench_kyber::decode<1>                          344 ns          344 ns      2035004 bytes_per_second=88.7186M/s
bench_kyber::encode<4>                         78.8 ns         78.8 ns      8887744 bytes_per_second=1.51362G/s
bench_kyber::decode<4>                          108 ns          108 ns      6483824 bytes_per_second=1.10426G/s
bench_kyber::encode<5>                          249 ns          249 ns      2812914 bytes_per_second=613.179M/s
bench_kyber::decode<5>                          271 ns          271 ns      2586122 bytes_per_second=563.774M/s
bench_kyber::encode<10>                         279 ns          279 ns      2510103 bytes_per_second=1094.31M/s
bench_kyber::decode<10>                         343 ns          343 ns      2042620 bytes_per_second=890.514M/s
bench_kyber::encode<11>                         325 ns          325 ns      2156674 bytes_per_second=1034.26M/s
bench_kyber::decode<11>                         352 ns          352 ns      1988130 bytes_per_second=953.444M/s
bench_kyber::encode<12>                         149 ns          149 ns      4710053 bytes_per_second=2.40356G/s
bench_kyber::decode<12>                         151 ns          151 ns      4647373 bytes_per_second=2.37431G/s
bench_kyber::compress<1>                       4.79 ns         4.79 ns    146276236
bench_kyber::decompress<1>                     7.40 ns         7.40 ns     94641368
bench_kyber::compress<4>                       4.79 ns         4.79 ns    146276269
bench_kyber::decompress<4>                     7.40 ns         7.40 ns     94648524
bench_kyber::compress<5>                       4.79 ns         4.79 ns    146274376
bench_kyber::decompress<5>                     7.40 ns         7.40 ns     94644731
bench_kyber::compress<10>                      4.79 ns         4.79 ns    146276819
bench_kyber::decompress<10>                    7.40 ns         7.40 ns     94648078
bench_kyber::compress<11>                      4.79 ns         4.79 ns    146275183
bench_kyber::decompress<11>                    7.40 ns         7.40 ns     94645222
bench_kyber::ntt                               5240 ns         5240 ns       133601
bench_kyber::intt                              5181 ns         5181 ns       135115
bench_kyber::polymul                            845 ns          845 ns       828389
bench_kyber::random_data<32>                   4679 ns         4679 ns       149634
bench_kyber::pke_keygen<2, 3>                 75153 ns        75152 ns         9316 items_per_second=13.3063k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          79989 ns        79988 ns         8744 items_per_second=12.5019k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          19280 ns        19280 ns        36311 items_per_second=51.8666k/s
bench_kyber::pke_keygen<3, 2>                126275 ns       126273 ns         5551 items_per_second=7.91936k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         136362 ns       136359 ns         5119 items_per_second=7.3336k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          26013 ns        26013 ns        26907 items_per_second=38.4421k/s
bench_kyber::pke_keygen<4, 2>                204584 ns       204583 ns         3444 items_per_second=4.88799k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         215498 ns       215495 ns         3207 items_per_second=4.64047k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          32809 ns        32809 ns        21332 items_per_second=30.4792k/s
bench_kyber::kem_keygen<2, 3>                 85759 ns        85759 ns         8209 items_per_second=11.6606k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>     107750 ns       107749 ns         6603 items_per_second=9.28083k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     114161 ns       114158 ns         6123 items_per_second=8.75981k/s
bench_kyber::kem_keygen<3, 2>                141852 ns       141849 ns         4935 items_per_second=7.04976k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     172686 ns       172683 ns         4017 items_per_second=5.79095k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     183051 ns       183048 ns         3830 items_per_second=5.46303k/s
bench_kyber::kem_keygen<4, 2>                223604 ns       223603 ns         3127 items_per_second=4.47221k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     261831 ns       261824 ns         2656 items_per_second=3.81935k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     275633 ns       275632 ns         2543 items_per_second=3.62803k/s
```

### On ARM Cortex-A72 ( compiled with Clang )

```bash
2022-11-01T09:10:09+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.22, 0.12, 0.08
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            8.51 ns         8.51 ns     82926178
bench_kyber::ff_compound_add                   5.55 ns         5.55 ns    127026407
bench_kyber::ff_sub                            8.36 ns         8.36 ns     83752358
bench_kyber::ff_compound_sub                   6.00 ns         6.00 ns    117081322
bench_kyber::ff_neg                            1.74 ns         1.74 ns    402243119
bench_kyber::ff_mul                            12.2 ns         12.2 ns     56914817
bench_kyber::ff_compound_mul                   8.31 ns         8.31 ns     84274572
bench_kyber::ff_inv                            52.0 ns         52.0 ns     12897199
bench_kyber::ff_div                            52.4 ns         52.4 ns     21450425
bench_kyber::ff_exp                             770 ns          770 ns      1054975
bench_kyber::parse                             6262 ns         6262 ns       111814
bench_kyber::generate_matrix<2, true>         26198 ns        26197 ns        26728
bench_kyber::generate_matrix<2, false>        26625 ns        26625 ns        26462
bench_kyber::generate_matrix<3, true>         59504 ns        59501 ns        11803
bench_kyber::generate_matrix<3, false>        59554 ns        59553 ns        11724
bench_kyber::generate_matrix<4, true>        105769 ns       105768 ns         6643
bench_kyber::generate_matrix<4, false>       106321 ns       106318 ns         6587
bench_kyber::cbd<2>                             300 ns          300 ns      2331886
bench_kyber::cbd<3>                             368 ns          368 ns      1899560
bench_kyber::encode<1>                          192 ns          192 ns      3651495 bytes_per_second=159.182M/s
bench_kyber::decode<1>                          218 ns          218 ns      3211419 bytes_per_second=140.008M/s
bench_kyber::encode<4>                         31.8 ns         31.8 ns     21997977 bytes_per_second=3.74623G/s
bench_kyber::decode<4>                         58.7 ns         58.7 ns     11916494 bytes_per_second=2.02959G/s
bench_kyber::encode<5>                          149 ns          148 ns      4713605 bytes_per_second=1027.54M/s
bench_kyber::decode<5>                          193 ns          193 ns      3623820 bytes_per_second=789.941M/s
bench_kyber::encode<10>                         185 ns          185 ns      3779281 bytes_per_second=1.60908G/s
bench_kyber::decode<10>                         176 ns          176 ns      3971060 bytes_per_second=1.68947G/s
bench_kyber::encode<11>                         214 ns          214 ns      3263455 bytes_per_second=1.52834G/s
bench_kyber::decode<11>                         259 ns          259 ns      2704093 bytes_per_second=1.2664G/s
bench_kyber::encode<12>                        79.2 ns         79.2 ns      8840033 bytes_per_second=4.51665G/s
bench_kyber::decode<12>                         112 ns          112 ns      6236059 bytes_per_second=3.18607G/s
bench_kyber::compress<1>                       6.56 ns         6.56 ns    107267226
bench_kyber::decompress<1>                     8.20 ns         8.20 ns     85403715
bench_kyber::compress<4>                       6.61 ns         6.61 ns    106035408
bench_kyber::decompress<4>                     8.20 ns         8.20 ns     85581435
bench_kyber::compress<5>                       7.18 ns         7.18 ns     97527972
bench_kyber::decompress<5>                     8.27 ns         8.27 ns     85018249
bench_kyber::compress<10>                      7.14 ns         7.14 ns     98055630
bench_kyber::decompress<10>                    8.20 ns         8.20 ns     86150097
bench_kyber::compress<11>                      7.49 ns         7.48 ns     95828965
bench_kyber::decompress<11>                    8.22 ns         8.22 ns     85506079
bench_kyber::ntt                               5691 ns         5690 ns       123026
bench_kyber::intt                              5086 ns         5086 ns       137613
bench_kyber::polymul                            882 ns          882 ns       793672
bench_kyber::random_data<32>                   4888 ns         4888 ns       143207
bench_kyber::pke_keygen<2, 3>                 68053 ns        68052 ns        10320 items_per_second=14.6947k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          73295 ns        73295 ns         9544 items_per_second=13.6435k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          22265 ns        22265 ns        31437 items_per_second=44.9136k/s
bench_kyber::pke_keygen<3, 2>                115663 ns       115660 ns         6041 items_per_second=8.64603k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         124915 ns       124915 ns         5605 items_per_second=8.00547k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          30083 ns        30083 ns        23270 items_per_second=33.2413k/s
bench_kyber::pke_keygen<4, 2>                186275 ns       186273 ns         3776 items_per_second=5.36848k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         195347 ns       195341 ns         3590 items_per_second=5.11925k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          38405 ns        38405 ns        18227 items_per_second=26.0383k/s
bench_kyber::kem_keygen<2, 3>                 75857 ns        75856 ns         9214 items_per_second=13.1829k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      95223 ns        95221 ns         7495 items_per_second=10.5018k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     107170 ns       107170 ns         6556 items_per_second=9.33099k/s
bench_kyber::kem_keygen<3, 2>                127567 ns       127567 ns         5489 items_per_second=7.83904k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     153117 ns       153116 ns         4565 items_per_second=6.531k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     170086 ns       170077 ns         4121 items_per_second=5.8797k/s
bench_kyber::kem_keygen<4, 2>                201657 ns       201652 ns         3464 items_per_second=4.95905k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     231690 ns       231689 ns         3024 items_per_second=4.31614k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     253299 ns       253297 ns         2769 items_per_second=3.94793k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-11-01T09:05:14+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.18, 0.05, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.17 ns         6.17 ns    113365024
bench_kyber::ff_compound_add                   2.31 ns         2.31 ns    302862107
bench_kyber::ff_sub                            6.21 ns         6.21 ns    112795719
bench_kyber::ff_compound_sub                   2.31 ns         2.31 ns    302855803
bench_kyber::ff_neg                            1.32 ns         1.32 ns    532421787
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82597045
bench_kyber::ff_compound_mul                   4.62 ns         4.62 ns    151388544
bench_kyber::ff_inv                            51.4 ns         51.4 ns     10000000
bench_kyber::ff_div                            24.4 ns         24.4 ns     34073839
bench_kyber::ff_exp                             604 ns          604 ns      1160935
bench_kyber::parse                             3681 ns         3681 ns       190065
bench_kyber::generate_matrix<2, true>         14637 ns        14637 ns        47944
bench_kyber::generate_matrix<2, false>        14624 ns        14623 ns        44635
bench_kyber::generate_matrix<3, true>         33281 ns        33281 ns        21064
bench_kyber::generate_matrix<3, false>        33089 ns        33088 ns        21217
bench_kyber::generate_matrix<4, true>         59053 ns        59052 ns        11796
bench_kyber::generate_matrix<4, false>        58838 ns        58837 ns        11889
bench_kyber::cbd<2>                            55.8 ns         55.7 ns     12557006
bench_kyber::cbd<3>                             102 ns          102 ns      6836846
bench_kyber::encode<1>                         46.2 ns         46.2 ns     15146742 bytes_per_second=660.86M/s
bench_kyber::decode<1>                          125 ns          125 ns      5615531 bytes_per_second=244.999M/s
bench_kyber::encode<4>                         19.2 ns         19.2 ns     36360271 bytes_per_second=6.19797G/s
bench_kyber::decode<4>                         28.6 ns         28.6 ns     24501560 bytes_per_second=4.16713G/s
bench_kyber::encode<5>                         81.7 ns         81.7 ns      8567833 bytes_per_second=1.82408G/s
bench_kyber::decode<5>                          110 ns          110 ns      6382373 bytes_per_second=1.35875G/s
bench_kyber::encode<10>                         102 ns          102 ns      6892848 bytes_per_second=2.93169G/s
bench_kyber::decode<10>                         115 ns          115 ns      6095239 bytes_per_second=2.59419G/s
bench_kyber::encode<11>                         115 ns          115 ns      6104813 bytes_per_second=2.85985G/s
bench_kyber::decode<11>                         132 ns          132 ns      5297263 bytes_per_second=2.48012G/s
bench_kyber::encode<12>                        34.5 ns         34.5 ns     20385394 bytes_per_second=10.3753G/s
bench_kyber::decode<12>                        43.8 ns         43.8 ns     15978514 bytes_per_second=8.16941G/s
bench_kyber::compress<1>                       3.85 ns         3.85 ns    181706164
bench_kyber::decompress<1>                     5.39 ns         5.39 ns    129781087
bench_kyber::compress<4>                       3.85 ns         3.85 ns    181716088
bench_kyber::decompress<4>                     5.39 ns         5.39 ns    129795400
bench_kyber::compress<5>                       3.85 ns         3.85 ns    181714344
bench_kyber::decompress<5>                     5.39 ns         5.39 ns    129793812
bench_kyber::compress<10>                      3.85 ns         3.85 ns    181704078
bench_kyber::decompress<10>                    5.39 ns         5.39 ns    129790705
bench_kyber::compress<11>                      3.85 ns         3.85 ns    181714944
bench_kyber::decompress<11>                    5.39 ns         5.39 ns    129780268
bench_kyber::ntt                               2038 ns         2038 ns       343589
bench_kyber::intt                              2135 ns         2135 ns       327980
bench_kyber::polymul                            296 ns          296 ns      2366542
bench_kyber::random_data<32>                   1663 ns         1663 ns       420025
bench_kyber::pke_keygen<2, 3>                 32694 ns        32694 ns        21414 items_per_second=30.5868k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          33318 ns        33317 ns        21015 items_per_second=30.015k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7429 ns         7429 ns        94258 items_per_second=134.615k/s
bench_kyber::pke_keygen<3, 2>                 53423 ns        53422 ns        13084 items_per_second=18.719k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          58078 ns        58076 ns        12197 items_per_second=17.2189k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           9940 ns         9940 ns        70421 items_per_second=100.603k/s
bench_kyber::pke_keygen<4, 2>                 86506 ns        86503 ns         8103 items_per_second=11.5603k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          91906 ns        91902 ns         7705 items_per_second=10.8811k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          12601 ns        12600 ns        55546 items_per_second=79.3631k/s
bench_kyber::kem_keygen<2, 3>                 35902 ns        35901 ns        19493 items_per_second=27.8541k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      43784 ns        43782 ns        15994 items_per_second=22.8405k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      47195 ns        47194 ns        14848 items_per_second=21.189k/s
bench_kyber::kem_keygen<3, 2>                 60265 ns        60262 ns        11801 items_per_second=16.5941k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      72573 ns        72571 ns         9779 items_per_second=13.7796k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      76683 ns        76682 ns         9121 items_per_second=13.0409k/s
bench_kyber::kem_keygen<4, 2>                 94530 ns        94526 ns         7403 items_per_second=10.5791k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     109032 ns       109030 ns         6417 items_per_second=9.17179k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     114192 ns       114188 ns         6140 items_per_second=8.75749k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-11-01T09:07:38+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.32, 0.22, 0.09
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            5.78 ns         5.78 ns    121106956
bench_kyber::ff_compound_add                   3.47 ns         3.47 ns    201837114
bench_kyber::ff_sub                            6.16 ns         6.16 ns    113558171
bench_kyber::ff_compound_sub                   3.85 ns         3.85 ns    181709503
bench_kyber::ff_neg                           0.891 ns        0.890 ns    786295119
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82591275
bench_kyber::ff_compound_mul                   6.16 ns         6.16 ns    113565092
bench_kyber::ff_inv                            25.5 ns         25.5 ns     22759174
bench_kyber::ff_div                            34.4 ns         34.4 ns     20346439
bench_kyber::ff_exp                             489 ns          489 ns      1456322
bench_kyber::parse                             2326 ns         2326 ns       300896
bench_kyber::generate_matrix<2, true>         10140 ns        10139 ns        69301
bench_kyber::generate_matrix<2, false>        10028 ns        10028 ns        69574
bench_kyber::generate_matrix<3, true>         22270 ns        22270 ns        31365
bench_kyber::generate_matrix<3, false>        22399 ns        22399 ns        31353
bench_kyber::generate_matrix<4, true>         42361 ns        42360 ns        16729
bench_kyber::generate_matrix<4, false>        41695 ns        41694 ns        16855
bench_kyber::cbd<2>                             101 ns          101 ns      6926846
bench_kyber::cbd<3>                             153 ns          153 ns      4563846
bench_kyber::encode<1>                         46.5 ns         46.5 ns     15060330 bytes_per_second=656.625M/s
bench_kyber::decode<1>                         44.4 ns         44.4 ns     15735572 bytes_per_second=686.798M/s
bench_kyber::encode<4>                         9.62 ns         9.62 ns     72871904 bytes_per_second=12.394G/s
bench_kyber::decode<4>                         15.9 ns         15.9 ns     44234277 bytes_per_second=7.50922G/s
bench_kyber::encode<5>                         57.5 ns         57.5 ns     12171148 bytes_per_second=2.58982G/s
bench_kyber::decode<5>                         69.7 ns         69.7 ns     10095784 bytes_per_second=2.13925G/s
bench_kyber::encode<10>                        53.3 ns         53.3 ns     13101569 bytes_per_second=5.58896G/s
bench_kyber::decode<10>                        55.4 ns         55.4 ns     12612631 bytes_per_second=5.37706G/s
bench_kyber::encode<11>                        91.9 ns         91.9 ns      7625590 bytes_per_second=3.56817G/s
bench_kyber::decode<11>                         106 ns          106 ns      6607063 bytes_per_second=3.09577G/s
bench_kyber::encode<12>                        24.4 ns         24.4 ns     28727948 bytes_per_second=14.6837G/s
bench_kyber::decode<12>                        32.7 ns         32.7 ns     21441635 bytes_per_second=10.9525G/s
bench_kyber::compress<1>                       5.01 ns         5.01 ns    139762836
bench_kyber::decompress<1>                     4.62 ns         4.62 ns    151433777
bench_kyber::compress<4>                       5.01 ns         5.01 ns    139773332
bench_kyber::decompress<4>                     4.62 ns         4.62 ns    151419827
bench_kyber::compress<5>                       5.01 ns         5.01 ns    139788126
bench_kyber::decompress<5>                     4.62 ns         4.62 ns    151426354
bench_kyber::compress<10>                      5.01 ns         5.01 ns    139785371
bench_kyber::decompress<10>                    4.62 ns         4.62 ns    151426002
bench_kyber::compress<11>                      5.01 ns         5.01 ns    139762898
bench_kyber::decompress<11>                    4.62 ns         4.62 ns    151422419
bench_kyber::ntt                               1724 ns         1724 ns       406703
bench_kyber::intt                              1993 ns         1993 ns       351298
bench_kyber::polymul                            385 ns          385 ns      1816713
bench_kyber::random_data<32>                   1723 ns         1723 ns       406272
bench_kyber::pke_keygen<2, 3>                 23712 ns        23712 ns        29626 items_per_second=42.1735k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          27534 ns        27534 ns        25910 items_per_second=36.3192k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7743 ns         7743 ns        90383 items_per_second=129.144k/s
bench_kyber::pke_keygen<3, 2>                 40882 ns        40881 ns        17207 items_per_second=24.4612k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          45974 ns        45973 ns        15226 items_per_second=21.752k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          10395 ns        10395 ns        67328 items_per_second=96.2015k/s
bench_kyber::pke_keygen<4, 2>                 67457 ns        67456 ns        10346 items_per_second=14.8246k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          73381 ns        73379 ns         9316 items_per_second=13.6279k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          13234 ns        13234 ns        52903 items_per_second=75.5635k/s
bench_kyber::kem_keygen<2, 3>                 26296 ns        26296 ns        26468 items_per_second=38.029k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      34453 ns        34451 ns        20619 items_per_second=29.0266k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      38807 ns        38807 ns        18019 items_per_second=25.7689k/s
bench_kyber::kem_keygen<3, 2>                 45580 ns        45579 ns        15557 items_per_second=21.9399k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      55821 ns        55820 ns        12537 items_per_second=17.9149k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      61482 ns        61480 ns        11269 items_per_second=16.2654k/s
bench_kyber::kem_keygen<4, 2>                 73308 ns        73306 ns         9588 items_per_second=13.6414k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      86185 ns        86183 ns         8153 items_per_second=11.6032k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      93007 ns        93005 ns         7474 items_per_second=10.7522k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2022-11-01T09:12:53+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.30, 0.08, 0.02
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.73 ns         6.73 ns    103909403
bench_kyber::ff_compound_add                   2.97 ns         2.97 ns    235666360
bench_kyber::ff_sub                            6.70 ns         6.70 ns    104436966
bench_kyber::ff_compound_sub                   2.97 ns         2.97 ns    235676010
bench_kyber::ff_neg                           0.917 ns        0.917 ns    764622674
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64824143
bench_kyber::ff_compound_mul                   7.06 ns         7.06 ns     99116660
bench_kyber::ff_inv                            59.5 ns         59.4 ns      8608749
bench_kyber::ff_div                             104 ns          104 ns      9850444
bench_kyber::ff_exp                             800 ns          800 ns       893689
bench_kyber::parse                             4749 ns         4749 ns       147400
bench_kyber::generate_matrix<2, true>         20687 ns        20687 ns        32113
bench_kyber::generate_matrix<2, false>        20724 ns        20723 ns        33801
bench_kyber::generate_matrix<3, true>         46667 ns        46667 ns        14920
bench_kyber::generate_matrix<3, false>        46868 ns        46868 ns        15089
bench_kyber::generate_matrix<4, true>         83370 ns        83371 ns         8329
bench_kyber::generate_matrix<4, false>        82678 ns        82677 ns         8470
bench_kyber::cbd<2>                            69.4 ns         69.4 ns     10076878
bench_kyber::cbd<3>                             155 ns          155 ns      4503816
bench_kyber::encode<1>                         54.7 ns         54.6 ns     13055642 bytes_per_second=558.431M/s
bench_kyber::decode<1>                          292 ns          292 ns      2394225 bytes_per_second=104.396M/s
bench_kyber::encode<4>                         32.5 ns         32.5 ns     21504282 bytes_per_second=3.66394G/s
bench_kyber::decode<4>                         29.1 ns         29.1 ns     24059595 bytes_per_second=4.09791G/s
bench_kyber::encode<5>                          171 ns          171 ns      4098817 bytes_per_second=892.796M/s
bench_kyber::decode<5>                          216 ns          216 ns      3232425 bytes_per_second=705.226M/s
bench_kyber::encode<10>                         210 ns          210 ns      3339801 bytes_per_second=1.42123G/s
bench_kyber::decode<10>                         292 ns          292 ns      2396666 bytes_per_second=1045.76M/s
bench_kyber::encode<11>                         277 ns          277 ns      2532126 bytes_per_second=1.18243G/s
bench_kyber::decode<11>                         284 ns          284 ns      2467984 bytes_per_second=1.15557G/s
bench_kyber::encode<12>                        84.6 ns         84.6 ns      8277791 bytes_per_second=4.22556G/s
bench_kyber::decode<12>                        87.4 ns         87.4 ns      8003627 bytes_per_second=4.09043G/s
bench_kyber::compress<1>                       4.83 ns         4.83 ns    145015268
bench_kyber::decompress<1>                     5.94 ns         5.94 ns    117859987
bench_kyber::compress<4>                       4.48 ns         4.48 ns    156321883
bench_kyber::decompress<4>                     5.94 ns         5.94 ns    117859174
bench_kyber::compress<5>                       4.13 ns         4.13 ns    169597992
bench_kyber::decompress<5>                     5.94 ns         5.94 ns    117848945
bench_kyber::compress<10>                      4.47 ns         4.47 ns    156449987
bench_kyber::decompress<10>                    5.94 ns         5.94 ns    117862162
bench_kyber::compress<11>                      4.47 ns         4.47 ns    156462995
bench_kyber::decompress<11>                    5.94 ns         5.94 ns    117750877
bench_kyber::ntt                               3282 ns         3282 ns       213146
bench_kyber::intt                              5207 ns         5207 ns       134493
bench_kyber::polymul                            632 ns          632 ns      1109547
bench_kyber::random_data<32>                   3277 ns         3277 ns       213283
bench_kyber::pke_keygen<2, 3>                 48398 ns        48398 ns        14374 items_per_second=20.6619k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          59286 ns        59285 ns        12036 items_per_second=16.8678k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          14252 ns        14252 ns        48861 items_per_second=70.1672k/s
bench_kyber::pke_keygen<3, 2>                 84221 ns        84217 ns         8299 items_per_second=11.8741k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         100385 ns       100375 ns         6991 items_per_second=9.96264k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          18649 ns        18648 ns        37581 items_per_second=53.6265k/s
bench_kyber::pke_keygen<4, 2>                136006 ns       135999 ns         5154 items_per_second=7.35297k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         156468 ns       156461 ns         4473 items_per_second=6.39135k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          23433 ns        23432 ns        29886 items_per_second=42.6764k/s
bench_kyber::kem_keygen<2, 3>                 55636 ns        55634 ns        12611 items_per_second=17.9746k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      77838 ns        77833 ns         9145 items_per_second=12.848k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      83294 ns        83293 ns         8396 items_per_second=12.0058k/s
bench_kyber::kem_keygen<3, 2>                 94929 ns        94928 ns         7363 items_per_second=10.5343k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     125472 ns       125472 ns         5593 items_per_second=7.96993k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     133954 ns       133950 ns         5236 items_per_second=7.46549k/s
bench_kyber::kem_keygen<4, 2>                149831 ns       149826 ns         4663 items_per_second=6.67439k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     188777 ns       188771 ns         3705 items_per_second=5.29743k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     200172 ns       200156 ns         3495 items_per_second=4.9961k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2022-11-01T09:15:04+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.51, 0.28, 0.11
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.75 ns         6.74 ns    103753961
bench_kyber::ff_compound_add                   3.35 ns         3.35 ns    208965726
bench_kyber::ff_sub                            7.10 ns         7.10 ns     98520875
bench_kyber::ff_compound_sub                   3.78 ns         3.78 ns    185154523
bench_kyber::ff_neg                            1.11 ns         1.11 ns    628782520
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64990397
bench_kyber::ff_compound_mul                   7.43 ns         7.43 ns     94212483
bench_kyber::ff_inv                            97.3 ns         97.3 ns     11756774
bench_kyber::ff_div                             130 ns          130 ns     10876137
bench_kyber::ff_exp                             627 ns          627 ns      1170213
bench_kyber::parse                             3816 ns         3816 ns       183319
bench_kyber::generate_matrix<2, true>         16706 ns        16705 ns        46910
bench_kyber::generate_matrix<2, false>        17545 ns        17545 ns        43886
bench_kyber::generate_matrix<3, true>         33348 ns        33349 ns        19635
bench_kyber::generate_matrix<3, false>        35701 ns        35700 ns        20427
bench_kyber::generate_matrix<4, true>         63136 ns        63136 ns        11112
bench_kyber::generate_matrix<4, false>        63677 ns        63672 ns        11405
bench_kyber::cbd<2>                            73.3 ns         73.3 ns      9556771
bench_kyber::cbd<3>                             166 ns          166 ns      4214236
bench_kyber::encode<1>                         37.2 ns         37.2 ns     18813398 bytes_per_second=821.368M/s
bench_kyber::decode<1>                         61.4 ns         61.4 ns     11416905 bytes_per_second=497.257M/s
bench_kyber::encode<4>                         47.9 ns         47.9 ns     14597853 bytes_per_second=2.48694G/s
bench_kyber::decode<4>                         35.7 ns         35.7 ns     19603871 bytes_per_second=3.33915G/s
bench_kyber::encode<5>                         72.3 ns         72.3 ns      9718351 bytes_per_second=2.06188G/s
bench_kyber::decode<5>                         83.6 ns         83.6 ns      8371618 bytes_per_second=1.78201G/s
bench_kyber::encode<10>                         217 ns          217 ns      3225364 bytes_per_second=1.37327G/s
bench_kyber::decode<10>                         104 ns          104 ns      6719658 bytes_per_second=2.86156G/s
bench_kyber::encode<11>                         192 ns          192 ns      3634980 bytes_per_second=1.70343G/s
bench_kyber::decode<11>                         182 ns          182 ns      3855317 bytes_per_second=1.80495G/s
bench_kyber::encode<12>                         113 ns          113 ns      6200497 bytes_per_second=3.16732G/s
bench_kyber::decode<12>                        70.8 ns         70.8 ns      9877692 bytes_per_second=5.04965G/s
bench_kyber::compress<1>                       5.96 ns         5.96 ns    117443723
bench_kyber::decompress<1>                     7.06 ns         7.06 ns     99233209
bench_kyber::compress<4>                       5.99 ns         5.99 ns    116991216
bench_kyber::decompress<4>                     7.06 ns         7.06 ns     99145392
bench_kyber::compress<5>                       5.99 ns         5.99 ns    116857105
bench_kyber::decompress<5>                     7.06 ns         7.06 ns     99267713
bench_kyber::compress<10>                      5.99 ns         5.99 ns    116999861
bench_kyber::decompress<10>                    7.06 ns         7.06 ns     99227109
bench_kyber::compress<11>                      5.99 ns         5.99 ns    116992145
bench_kyber::decompress<11>                    7.05 ns         7.05 ns     99236562
bench_kyber::ntt                               2508 ns         2508 ns       279155
bench_kyber::intt                              2465 ns         2465 ns       283828
bench_kyber::polymul                            742 ns          742 ns       943307
bench_kyber::random_data<32>                   3381 ns         3381 ns       206916
bench_kyber::pke_keygen<2, 3>                 37080 ns        37078 ns        19278 items_per_second=26.9701k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          40018 ns        40017 ns        17657 items_per_second=24.9897k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          10118 ns        10118 ns        69141 items_per_second=98.8376k/s
bench_kyber::pke_keygen<3, 2>                 64048 ns        64043 ns        10827 items_per_second=15.6144k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          66479 ns        66474 ns        10458 items_per_second=15.0434k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          13810 ns        13810 ns        50689 items_per_second=72.4133k/s
bench_kyber::pke_keygen<4, 2>                103526 ns       103524 ns         6823 items_per_second=9.65955k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         113624 ns       113619 ns         6177 items_per_second=8.80135k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          17613 ns        17612 ns        39752 items_per_second=56.7802k/s
bench_kyber::kem_keygen<2, 3>                 40115 ns        40114 ns        17703 items_per_second=24.9288k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      46529 ns        46528 ns        14956 items_per_second=21.4926k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      52913 ns        52912 ns        13329 items_per_second=18.8993k/s
bench_kyber::kem_keygen<3, 2>                 69023 ns        69016 ns        10145 items_per_second=14.4895k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      83534 ns        83529 ns         8731 items_per_second=11.9719k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      89694 ns        89689 ns         7736 items_per_second=11.1496k/s
bench_kyber::kem_keygen<4, 2>                111698 ns       111695 ns         6391 items_per_second=8.95296k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     122291 ns       122290 ns         5606 items_per_second=8.17727k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     131389 ns       131389 ns         5076 items_per_second=7.611k/s
```

## Usage

`kyber` is written to be a zero-dependency, header-only C++ library such that it's pretty easy-to-use in your project. All you need to do

- Clone `kyber` repository

```bash
cd
git clone https://github.com/itzmeanjan/kyber.git
```

- Initialize and update git submodule, so that `sha3` dependency is available inside `kyber` source tree

```bash
cd kyber
git submodule update --init
```

> **Note**

> Find more about my project `sha3`: https://github.com/itzmeanjan/sha3

- Write your program while including [kyber.hpp](./include/kyber.hpp), which includes declarations ( and definitions ) of all required routines, spread across various namespaces

> **Note**

> You may only want to use Kyber PKE or KEM, then you can simply include either of [kyber_pke.hpp](./include/kyber_pke.hpp) or [kyber_kem.hpp](./include/kyber_kem.hpp)

- When compiling your program, let your compiler know where it can find `kyber` and `sha3` headers, which includes their definitions too

```bash
# Assuming `kyber` was cloned just under $HOME

KYBER_HEADERS=~/kyber/include
SHA3_HEADERS=~/kyber/sha3/include

g++ -std=c++20 -I $KYBER_HEADERS -I $SHA3_HEADERS main.cpp
```

Kyber | Namespace | Header
--- | :-: | --:
IND-CPA-secure Public Key Encryption | `kyber_pke::` | [kyber_pke.hpp](./include/kyber_pke.hpp)
IND-CCA-secure Key Encapsulation Mechanism | `kyber_kem::` | [kyber_kem.hpp](./include/kyber_kem.hpp)
Utility Routine | `kyber_utils::` | -

> **Note**

> Select Kyber parameter set from table 1 of Kyber [specification](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf)

---

### Kyber Public Key Encryption (PKE)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
PKE KeyGen | `kyber_pke::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 12 * 32) -bytes secret key | Imagine two parties i.e. sender and receiver. Both of them generate PKE keypair & publish their public keys to each other.
Encrypt | `kyber_pke::` | `encrypt<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key, 32 -bytes plain text ( to be encrypted ) & 32 -bytes random coin ( used as seed of randomness ) | (k * du * 32 + dv * 32) -bytes encrypted text | Message sender takes receiver's public key and encrypts a 32 -bytes message. It also sends cipher text to the receiver.
Decrypt | `kyber_pke::` | `decrypt<k, du, dv>(...)` | (k * 12 * 32) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | 32 -bytes plain text | Receiver takes their secret key and cipher text ( received from sender ) to recover 32 -bytes plain text message, that encrypted by sender.

> **Note**

> In [pke.cpp](./example/pke.cpp), I show how to use Kyber PKE API, while sticking to Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/pke.cpp && ./a.out

pubkey : fac9337d888430016152ba06cc41c5a92710ed9787f6caa710d7c57c879d3de018c50bbffc14a92e233c6b3acddae9ca9543cfb5f16aa4209a65269a98a4820ad0a26579c71d1ca52cd65446c92835cb0f30b65c50a89048d57a6ce9a45818b3acd9a078ba9b2b5421fc02084a1b9d884563fad88c66cb7c25171aa4b5051d0b567240065f4841cf5904f73474a498932ab858acb108d486c62f9c2b8d6a3c6880353b15b4a9e91697081c6e551966db191fb284a7e5a8e985cf46b941811b8f8d7c574470b3e90b7530f7c8c8a58b1c2bb3029c2fc99168544375b102627b860d1ba8803f594a9a2704ed286559814061c2729bf1ba562827e2b5af3874acf0451eccca42f420985f07bfd2634883c3c44dc91f4894aca33387a205abd7f84219e47bfc556fb368c7b8fc62aeb9243dc11b0e647b37d19b25ec807bb243430182a2994107c157b539b5c446bc3ce08daf9697561baa6837adbc121a6e0643f50317be733fc76624a0e530f8f42e2835bc7d5a166768309fe82291f9a6fe4679f213052355a9d669277e715af3e14d830859bac0034b6247009d57f50bc5b023bfc40918d1c6ba8db016d70b2aa181324d624f38f5a0fa81aa96338bf71c1696aa0bfff3651eb217cdacb5d5e1147fd6287ae83ab5bc62f150a7dc21cf7af65003d4467028b7929078fdb73659300a8feb9e5a126e0d33c780b7497c151f91b11d0294a9d67540b7750de313cf3ef032e120cab7f5b9802164b4d136c41b56686aa182e8bdb0728e1033734685246568858e9629e8f4068d037303b144163abe1780a09c94168d501bf6a61d2f88215ea86f860b8353b9338ae8311428bef17c6cf0581eeab7b96bd00fd123cf5a3445ffaa0413d05f01202061840db89c2f4dbb74264c8da370ba21dc3baac4381d7895c8bc2e7512ab8890051a306908c67433ca44d2f9ac14320e48251116fcb1c9f343349cc4077cceb395118189204319891ef85df03a68f8d22030880396e7cc3d42473bcbaab6641d7af725f3d3940af46a068ba7d62bc125e9723d60bf4f6133ec068544051a8f697515240fda2418c5419aa22e1f25903eec7541d8de09ed43fe6560fbde5aa0fc0bd3de8fd6f180
seckey : a08379055a37664c6b3d58cfcfb6af6cdac956c570e4e68a0a4b9eec7bcb05a19f9663353d97a42f4503aba5c224c95e924691b2b957160cbd7d0b2c98602b2858036aa8664a7718a363c8988b2ca23a8bd32c8f1c0ab8901555463c3307959d75a66e5671c2398c6e740cc636da984e801044727e3fd51964e85c77f45fd759696577570bac75d13156daf64b42d338db59a10bb29935c0c45374019c09c17521a95784592f3733e3c576a816195a59bc03d66f844b60f7aab6ea5cb168a3136a3c487699c294053e8b7813960b6503d37c33b2169bc2067a9035ee941cfbeb5a4d1a1dbc293089c1265082afcea0600734b9392473c15c94656b93750766cb59809e210086aaa0658407c0ec61421bc333811914e779bc580a1014ac433c53f0c157bba765e7f8555e0c0241350fc07c9b4f435665925664695a36eaab568a71ea101fdbb6902fc79fcf0a688e8a58544753aa201ec68ab5862b4c24185ef3869d0ed519e81121d00b4dc4c762b3a90971f174aae8c7f05267e3db6690c99ef7c67e362b89f0a13721e66c7e855adbe21cb1e32c1fb53e0f71746f30335cd9a1c569609b6a5306ba87fff8bed720c3c34092df5c4f0301a85ff9cdcca103e018c5629bc46349427fb2cadd86c4d5f5adcdb715c9864e0446bfd2c6232e2858a5892a12aa58668b13beb924e079c474d12f2f6556ec2c24f4640122c37ea0f97aa862a5e5541ef6dac4a6486383bc7729775102c89541d67a35499909c91237fc052652780d5a25e2c84d7d509f17c0092a37cf7edbcded108935597122243efea541f9cb020a4b329d7b653a27ce77926f2918651f66b55c6bae71f46410ea702f571d09401e234cb71c2c627c119bcdb96accda98cd461868a33f9b0ccc8464b469369cc6e033b316417bab47f43915a42a00f61012f032c2fa6a78f3ab1cb62122a7b171885a7c651c7f43c5b8fb6c5807f040d2749f74301767899b62ac0e98ba608045bd4cc0a3dd648452cab2153acee6a22effc73aca36a6cac8008d2cb4f19c71d82b1b1f411ff94c577d1b1429e8a451b48c71288c6a55724cd1c3
plain text : c327a0162e13cd9a37212f9a2fb39784a822d7d31cb657aa06d86c4e6124775f
cipher     : c53d24b3843938e8570338e61e47a751b9944e7fd07cf77d022612987f271cca62cbad4fe751d322b550178b5e9887ccc854673f34bd2d7b566455a022c02d75c6e99d8376dc39d2b611a000c8453e89192277d61027a6b2aab2f5f00ce3bab84edb2596389a39bc9e6aa822e583cd26bf4548cc818f57e87cbaacc0f08da2a88e8beed151e9d6bbadf69d60d28da9af907a1a93f25189a71b8e56871b128897d7c7771f0eabd11fb0179f60a7579ebd4c14286cb8ec75574cf697f13f0df131bb91125e3c0fb720e1ee3771fc6ef3e4faed52bb7f1b9bae1a11036227b7fd954b27609c7224333e4025eb02aab1b562aa0dfe62f27ddfd81508ae424870ac6454ea71738405888541a453c8cc72b245e406a90984232e9cd6f92791e9029f36667ca53ed639e374eb20269985d0e89710182b68f900e862f78f07cc281f18cc40afc42050a6b468ed2f61c0c8a3bf09642bcc6c95492c5c418038e688ccef2cc8fbd23e2b415cd42c3ce4f721da28cb15a1cbabf3b6ef24bda94ccefdafd7126dcf3733dd75208afb460dd2e1a59eb41720eda04e99e0ca69e0c2132d89ef830fec8687aeebaeb2f5192210bf19404fa3dbf79c37b5e7874ce69156a251d94d2f11e3950f2f9de449be2757702a400cb46ecbd341876f0159089f00a4857a0180938619a3f01c01a965e65dc9fbd258a2d3be374c30753b8d8c29ec07e043510527a04c6c126cd01e0e33d3b15f953230b5d4612ec8a4e7eb5290ec7b7020278981a624ef6f479774361f9bc991571ec00721549cfa5bbf240f9ede390af70550a677dedbe40a5e5ace670399363f7837520d2a6e6d078223a204510ddf03fd7cb8094cddc8937c5021d3ff2dd5440acd24582f0fc2df145338536bbca3dbf2a357ba02d13ff53e8fe158aff2fb45381e169267efbde7ee40ca5d055b183621a2a612e67e718b8f5911125af0cc52e60304c4d9357811cc7aa1ecc93a6adb4b76658498868c8d64f001cd900627d1081b4766ac21c59c0720a977fc6deafe70a07a0ce68272c663a1fc4c1c7aaa757cd2c8ad1b17f6ff29b13da4e9656d8804
decrypted  : c327a0162e13cd9a37212f9a2fb39784a822d7d31cb657aa06d86c4e6124775f
```

### Kyber Key Encapsulation Mechanism (KEM)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
KEM KeyGen | `kyber_kem::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 24 * 32 + 96) -bytes secret key | Imagine two parties peer1 & peer2, want to securely ( using symmetric key encryption i.e. AES ) communicate over insecure communication channel. Both of them generate their KEM keypair and publish their public keys to each other.
Encapsulation | `kyber_kem::` | `encapsulate<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key | (k * du * 32 + dv * 32) -bytes cipher text and a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer1 wants to establish a secure connection with Peer2 & both of them have already agreed they'll use AES with 256 -bit symmetric keys. Peer1 encapsulates 32 -bytes random seed inside cipher text, using Peer2's public key, which it shares with Peer2, over insecure channel. Peer1 derives 256 -bit key from KDF, that it obtained as result of encapsulation.
Decapsulation | `kyber_kem::` | `decapsulate<k, η1, η2, du, dv>(...)` | (k * 24 * 32 + 96) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer2 uses its secret key to decapsulate cipher text ( received from Peer1 ), recovering 32 -bytes random seed, which is used for seeding KDF, deriving 256 -bit key. Now both parties have same 256 -bit key, which they can use with AES to encrypt ( much more efficiently ) their messages.

> **Note**

> In [kem.cpp](./example/kem.cpp), I keep an example demonstrating usage of Kyber KEM API, while using Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/kem.cpp && ./a.out

pubkey : 45580b5ecb5c78eb335992afc2ea9c0fb192919269d8068e9ae855bc705d26f3c4257b8db267c1ac381d480442aca3767f7c8cefbb6b28818aec4118f98906769728c7b74ce4aaa8fce11d8503579867a026e0bfc05945171a9c4157664056562e978a86f7a6e5f45b621689ea27bc92545c06a6b44ae163abf5039d207e060017e548acce256c40fbb6a9b9073cc4630e1823e658333c83aa9f661b6bc29f4adcab5691adba747d23a1c8a5e7bf973c2a600a4aeeab4ad793b2ade8c2fc8225c8a8634ba31c0cb896e3305a7e3483374609b8254427b6cbb9ac26fc8439b5632d7134921e178196940b482bbc1d9520e7bb94ab2ab53b7c97c1639146062fbcd634c57060d6088baea4884ad839907a6a7edb3b26d0b3ed3a2f512b5b3f38bb0a0a47b75cb619ca471f79af13c0a869863c11d48e9a9a02b0998c24a490c4373a6d95346a7712e91a5dc7271fa8a355eed756bed5a0b0f13fafe3c45115918216ceca15085471a7857055156753cd38c4cfe4208f241325357b228904fdba923f228f6f37b50f04027c65831c9179fa03120e01b00f321335c05d89f952fdca8c78f61ce0f474a7e552c26b133504187b947412656a31e7b8324b4b50b5c710cc5fea9bc032518a70a2b7ae964e1f62a4e55086c6964dd2e178558144d6d2bbee6725e0e77d7e5a81f8ba0657aa6d92339b4b68bcbb558f3d1a4415b3c993e86c2275600d877c2480480b687f20107f40e16506941fd7dc52e975b765a19a9d77b8bca7b1b39575688a879af75fe0a1296b158a6db7aeb3619271f1488376058107970a31705e0a949c5169361a0122861610d75f7cd3cd85870f64e2649a7b550a61a009950b0ed4496449c6e16c5bdf7bc10f07ac3182c98e901cffdc686b2b646e286820194ed4b559e17210f54268996615dc3cc9f89840d2bc2e5c134fcb733f7e99b596672d3326392e506967ea9db231b15ce777a1ec7bf36a32ce549f7f619d75595f09db42bdf8ccf4aba2a2c226993c0b29a4bb3801805db193cab268b9c6cb83e3a18ad1644ba87e494710c08a18589856d2f761796210eb54c1a4c9e3f19a6fc00e9b49384bb8fb38aee60a7fa63a3ba77cc1d9d23da107b081
seckey : 83d308ba12ce27a42abb331cd739793a50c326875853112ccdaa05765a8b6c254d22d3a2836c1feef4014a5167224a2a181c089195ad90e89489154e82a8101a2a21e43393fecaa8d0524dcdc2c4b59a06fc27bee61b131e811ce99161d6c183af50411a328a621337ffe1a227b0bbe624123d517587d63aebba776bc823b38a8c785c0fdb651fd8c297a6bb040c8417bd94003ff79b4e293cd69958aae07e1ab81a15bb3f60347c596913baa6b68f5c253c2908c101596a4c43b4e93480209471612b9bc55d7eba3e04ea507dac13316892403a5270a523e7778e8051187ff66b0cd98910d93215627847dc7b51092559b018e8d0a353369eab13bf8c2b30adb87deb32b5907586eca68ed92350b9e130a37acf99ac00ad2528792663a253492182759ad259fc058c9df031ce091d39c36c3d0b9dd3834ab8c8483a92678e378bc8e655eae48fec25976359241aa70189e375c9fc1989cabe1fa924967425c8925aec6a0c72ca9f81b22c314378b726c2c4ea410212a7cea88cd87a907f41989e570e040092eb6a3aea5c4a4dc8b5fcba5e36e80f810ab9c26cba25b116c5e7bce3c54729f913d45b746d9b7caaab034bdc3b9b438764269f39f8cb1b163de0c7a9b28b4de93cbabaf73dcafa61b2cc0a417c4494d431ed2b99e5b16be6b83867410773b4765559c84071297e6549d9136687c4897e33b6e6a5677fb380d5d14a60677293560fe723063fb81ef2d66ff83a17351b9c0bab1839744d34799f97094b2aa955a58892286609bcd7a07eb766d5631dab38825ef53643236251758a30306d12b032be8cc530cb911e5a21760bc838214655150737eac4e130aaed52a78032a5e7625edfa991527aa69695cde2c69ea4d99a2006b0f24b215285a8e8b73556ccaacdb66cc27a7d2e5b1af849554a3bb30af91dc48230be25722266bba250b0069b8ac0d4a489ecbfbb724012b6183074a93748a81dd840dc5952bbf683d93634c6b3bddd27c3d6e14826ab65e2bb9c2adc01b8b515509804dc6ba8b575076460135f48bd88e865372019e78b7fc48a92b484c58c4650e1661746431a45580b5ecb5c78eb335992afc2ea9c0fb192919269d8068e9ae855bc705d26f3c4257b8db267c1ac381d480442aca3767f7c8cefbb6b28818aec4118f98906769728c7b74ce4aaa8fce11d8503579867a026e0bfc05945171a9c4157664056562e978a86f7a6e5f45b621689ea27bc92545c06a6b44ae163abf5039d207e060017e548acce256c40fbb6a9b9073cc4630e1823e658333c83aa9f661b6bc29f4adcab5691adba747d23a1c8a5e7bf973c2a600a4aeeab4ad793b2ade8c2fc8225c8a8634ba31c0cb896e3305a7e3483374609b8254427b6cbb9ac26fc8439b5632d7134921e178196940b482bbc1d9520e7bb94ab2ab53b7c97c1639146062fbcd634c57060d6088baea4884ad839907a6a7edb3b26d0b3ed3a2f512b5b3f38bb0a0a47b75cb619ca471f79af13c0a869863c11d48e9a9a02b0998c24a490c4373a6d95346a7712e91a5dc7271fa8a355eed756bed5a0b0f13fafe3c45115918216ceca15085471a7857055156753cd38c4cfe4208f241325357b228904fdba923f228f6f37b50f04027c65831c9179fa03120e01b00f321335c05d89f952fdca8c78f61ce0f474a7e552c26b133504187b947412656a31e7b8324b4b50b5c710cc5fea9bc032518a70a2b7ae964e1f62a4e55086c6964dd2e178558144d6d2bbee6725e0e77d7e5a81f8ba0657aa6d92339b4b68bcbb558f3d1a4415b3c993e86c2275600d877c2480480b687f20107f40e16506941fd7dc52e975b765a19a9d77b8bca7b1b39575688a879af75fe0a1296b158a6db7aeb3619271f1488376058107970a31705e0a949c5169361a0122861610d75f7cd3cd85870f64e2649a7b550a61a009950b0ed4496449c6e16c5bdf7bc10f07ac3182c98e901cffdc686b2b646e286820194ed4b559e17210f54268996615dc3cc9f89840d2bc2e5c134fcb733f7e99b596672d3326392e506967ea9db231b15ce777a1ec7bf36a32ce549f7f619d75595f09db42bdf8ccf4aba2a2c226993c0b29a4bb3801805db193cab268b9c6cb83e3a18ad1644ba87e494710c08a18589856d2f761796210eb54c1a4c9e3f19a6fc00e9b49384bb8fb38aee60a7fa63a3ba77cc1d9d23da107b08147f790da9f2a09c78a0e53434a5326f74eb175776a1628f6c7be3213a2a2401c48e67f6d7afe95c35fe15c4454b838c926a9ec53b9c324f0168151e6e954c546
shared secret 0 : 3364912ae34021241f4aafd8253d909db0e5a9b685de85e1252f8533382296ff
shared secret 1 : 3364912ae34021241f4aafd8253d909db0e5a9b685de85e1252f8533382296ff
```
