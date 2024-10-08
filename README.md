> [!CAUTION]
> This ML-KEM implementation is conformant with ML-KEM standard https://doi.org/10.6028/NIST.FIPS.203 and I also *try* to make it timing leakage free, but be informed that this implementation is not *yet* audited. **If you consider using it in production, please be careful !**

# ML-KEM (formerly known as Kyber)
Module-Lattice -based Key Encapsulation Mechanism Standard by NIST i.e. FIPS 203.

## Motivation

ML-KEM has been standardized by NIST as post-quantum secure key encapsulation mechanism (KEM), which can be used for key establishment, between two parties, communicating over insecure channel.

ML-KEM offers an *IND-CCA-secure* Key Encapsulation Mechanism - its security is based on the hardness of solving the learning-with-errors (LWE) problem in module (i.e. structured) lattices.

ML-KEM is built on top of *IND-CPA-secure K-PKE*, where two communicating parties, both generating their key pairs, while publishing only their public keys to each other, can encrypt fixed length ( = 32 -bytes ) message using peer's public key. Cipher text can be decrypted by corresponding secret key ( which is private to the keypair owner ) and 32 -bytes message can be recovered back. Then a slightly tweaked Fujisaki–Okamoto (FO) transform is applied on *IND-CPA-secure K-PKE* - giving us the *IND-CCA-secure ML-KEM* construction. In KEM scheme, two parties interested in establishing a secure communication channel, over public & insecure channel, can generate a 32 -bytes shared secret key. Now they can be use this 32 -bytes shared secret key in any symmetric key primitive, either for encrypting their communication (in much faster way) or deriving new/ longer keys.

Algorithm | Input | Output
--- | :-: | --:
KeyGen | - | Public Key and Secret Key
Encapsulation | Public Key | Cipher Text and 32B Shared Secret
Decapsulation | Secret Key and Cipher Text | 32B Shared Secret

Here I'm maintaining `ml-kem` - a C++20 header-only fully `constexpr` library, implementing ML-KEM, supporting ML-KEM-{512, 768, 1024} parameter sets, as defined in table 2 of ML-KEM standard. It's pretty easy to use, see [usage](#usage).

> [!NOTE]
> Find ML-KEM standard @ https://doi.org/10.6028/NIST.FIPS.203 - this is the document that I followed when implementing ML-KEM. I suggest you go through the specification to get an in-depth understanding of the scheme.

## Prerequisites

- A C++ compiler with C++20 standard library such as `clang++`/ `g++`.

```bash
$ clang++ --version
Ubuntu clang version 17.0.6 (9ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- Build tools such as `make`, `cmake`.
- For testing ML-KEM implementation, you need to globally install `google-test` library and headers. Follow guide @ https://github.com/google/googletest/tree/main/googletest#standalone-cmake-project, if you don't have it installed.
- For benchmarking ML-KEM implementation, you'll need to have `google-benchmark` header and library globally installed. I found guide @ https://github.com/google/benchmark#installation helpful.

> [!NOTE]
> If you are on a machine running GNU/Linux kernel and you want to obtain *CPU cycle* count for ML-KEM routines, you should consider building `google-benchmark` library with `libPFM` support, following https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7, a step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.

> [!TIP]
> Git submodule based dependencies will normally be imported automatically, but in case that doesn't work, you can manually initialize and update them by issuing `$ git submodule update --init` from inside the root of this repository.

## Testing

For testing functional correctness of this implementation and conformance with ML-KEM standard, you have to issue

> [!NOTE]
> Known Answer Test (KAT) files living in [this](./kats/) directory are generated by following (reproducible) steps, described in https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710.

```bash
# You can switch to non-default compiler, by setting variable `CXX` i.e. invoke like `$ CXX=clang++ make -j`.
#
make -j                    # Run tests without any sort of sanitizers
make debug_asan_test -j    # Run tests with AddressSanitizer enabled, with `-O1`
make release_asan_test -j  # Run tests with AddressSanitizer enabled, with `-O3 -march=native`
make debug_ubsan_test -j   # Run tests with UndefinedBehaviourSanitizer enabled, with `-O1`
make release_ubsan_test -j # Run tests with UndefinedBehaviourSanitizer enabled, with `-O3 -march=native`
```

```bash
PASSED TESTS (15/15):
       1 ms: build/test/test.out ML_KEM.ML_KEM_512_EncapsFailureDueToNonReducedPubKey
       2 ms: build/test/test.out ML_KEM.ML_KEM_768_DecapsFailureDueToBitFlippedCipherText
       2 ms: build/test/test.out ML_KEM.ML_KEM_1024_EncapsFailureDueToNonReducedPubKey
       2 ms: build/test/test.out ML_KEM.ML_KEM_512_DecapsFailureDueToBitFlippedCipherText
       2 ms: build/test/test.out ML_KEM.ML_KEM_768_KeygenEncapsDecaps
       2 ms: build/test/test.out ML_KEM.ML_KEM_1024_DecapsFailureDueToBitFlippedCipherText
       3 ms: build/test/test.out ML_KEM.PolynomialSerialization
       3 ms: build/test/test.out ML_KEM.ML_KEM_1024_KeygenEncapsDecaps
       3 ms: build/test/test.out ML_KEM.ML_KEM_768_EncapsFailureDueToNonReducedPubKey
       3 ms: build/test/test.out ML_KEM.ML_KEM_512_KeygenEncapsDecaps
      17 ms: build/test/test.out ML_KEM.ML_KEM_768_KnownAnswerTests
      18 ms: build/test/test.out ML_KEM.ML_KEM_512_KnownAnswerTests
      32 ms: build/test/test.out ML_KEM.ML_KEM_1024_KnownAnswerTests
     100 ms: build/test/test.out ML_KEM.CompressDecompressZq
     156 ms: build/test/test.out ML_KEM.ArithmeticOverZq
```

## Benchmarking

For benchmarking ML-KEM public functions such as keygen, encaps and decaps, for various suggested parameter sets, you have to issue.

```bash
make benchmark -j  # If you haven't built google-benchmark library with libPFM support.
make perf -j       # If you have built google-benchmark library with libPFM support.
```

> [!CAUTION]
> When benchmarking, ensure that you've disabled CPU frequency scaling, by following guide @ https://github.com/google/benchmark/blob/main/docs/reducing_variance.md.

### On 12th Gen Intel(R) Core(TM) i7-1260P

Compiled with **gcc (Ubuntu 14-20240412-0ubuntu1) 14.0.1 20240412**.

```bash
$ uname -srm
Linux 6.8.0-41-generic x86_64
```

```bash
Running ./build/benchmark/perf.out
Run on (16 X 4512.33 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.67, 0.66, 0.77
------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES items_per_second
------------------------------------------------------------------------------------------------
ml_kem_512/decap_mean           19.7 us         19.7 us           10   91.9767k       50.8334k/s
ml_kem_512/decap_median         19.7 us         19.7 us           10   91.9204k       50.8766k/s
ml_kem_512/decap_stddev        0.071 us        0.067 us           10    189.166        172.876/s
ml_kem_512/decap_cv             0.36 %          0.34 %            10      0.21%            0.34%
ml_kem_512/decap_min            19.6 us         19.6 us           10   91.7206k        50.445k/s
ml_kem_512/decap_max            19.8 us         19.8 us           10   92.3585k       51.0262k/s
ml_kem_1024/keygen_mean         35.5 us         35.5 us           10   165.876k       28.1537k/s
ml_kem_1024/keygen_median       35.4 us         35.4 us           10   165.836k        28.211k/s
ml_kem_1024/keygen_stddev      0.223 us        0.217 us           10    653.566        170.496/s
ml_kem_1024/keygen_cv           0.63 %          0.61 %            10      0.39%            0.61%
ml_kem_1024/keygen_min          35.3 us         35.3 us           10   165.106k       27.7649k/s
ml_kem_1024/keygen_max          36.0 us         36.0 us           10   167.138k       28.3399k/s
ml_kem_768/keygen_mean          22.7 us         22.7 us           10   106.239k       44.0363k/s
ml_kem_768/keygen_median        22.7 us         22.7 us           10   106.245k       44.0287k/s
ml_kem_768/keygen_stddev       0.096 us        0.096 us           10    449.139        185.991/s
ml_kem_768/keygen_cv            0.42 %          0.42 %            10      0.42%            0.42%
ml_kem_768/keygen_min           22.6 us         22.6 us           10   105.649k       43.8031k/s
ml_kem_768/keygen_max           22.8 us         22.8 us           10   106.865k       44.2845k/s
ml_kem_1024/encap_mean          39.5 us         39.5 us           10   184.973k       25.2883k/s
ml_kem_1024/encap_median        39.5 us         39.5 us           10    184.69k       25.3388k/s
ml_kem_1024/encap_stddev       0.182 us        0.183 us           10    747.023        116.145/s
ml_kem_1024/encap_cv            0.46 %          0.46 %            10      0.40%            0.46%
ml_kem_1024/encap_min           39.4 us         39.4 us           10   184.241k       25.0329k/s
ml_kem_1024/encap_max           39.9 us         39.9 us           10   186.379k       25.3857k/s
ml_kem_1024/decap_mean          46.9 us         46.9 us           10   219.244k       21.3258k/s
ml_kem_1024/decap_median        46.9 us         46.9 us           10    219.46k       21.3138k/s
ml_kem_1024/decap_stddev       0.120 us        0.120 us           10    507.886        54.7725/s
ml_kem_1024/decap_cv            0.26 %          0.26 %            10      0.23%            0.26%
ml_kem_1024/decap_min           46.7 us         46.7 us           10   218.364k       21.2385k/s
ml_kem_1024/decap_max           47.1 us         47.1 us           10   219.722k       21.4278k/s
ml_kem_512/keygen_mean          13.5 us         13.5 us           10   63.1494k       73.9958k/s
ml_kem_512/keygen_median        13.5 us         13.5 us           10   63.0695k       74.0784k/s
ml_kem_512/keygen_stddev       0.064 us        0.064 us           10    337.757        350.424/s
ml_kem_512/keygen_cv            0.48 %          0.48 %            10      0.53%            0.47%
ml_kem_512/keygen_min           13.5 us         13.5 us           10   62.7346k       73.1112k/s
ml_kem_512/keygen_max           13.7 us         13.7 us           10   63.9768k       74.3286k/s
ml_kem_768/encap_mean           25.9 us         25.9 us           10   120.857k       38.6398k/s
ml_kem_768/encap_median         25.8 us         25.8 us           10    120.73k       38.7252k/s
ml_kem_768/encap_stddev        0.195 us        0.194 us           10    434.128         286.24/s
ml_kem_768/encap_cv             0.76 %          0.75 %            10      0.36%            0.74%
ml_kem_768/encap_min            25.7 us         25.7 us           10   120.315k       37.9192k/s
ml_kem_768/encap_max            26.4 us         26.4 us           10   121.593k       38.8849k/s
ml_kem_768/decap_mean           31.3 us         31.3 us           10   146.348k       31.9402k/s
ml_kem_768/decap_median         31.2 us         31.2 us           10   146.055k       32.0125k/s
ml_kem_768/decap_stddev        0.155 us        0.155 us           10    740.319        157.033/s
ml_kem_768/decap_cv             0.49 %          0.49 %            10      0.51%            0.49%
ml_kem_768/decap_min            31.2 us         31.2 us           10   145.743k        31.652k/s
ml_kem_768/decap_max            31.6 us         31.6 us           10   147.862k         32.09k/s
ml_kem_512/encap_mean           15.6 us         15.6 us           10   72.7434k       64.0886k/s
ml_kem_512/encap_median         15.6 us         15.6 us           10   72.7235k       64.1631k/s
ml_kem_512/encap_stddev        0.068 us        0.064 us           10    141.896        261.103/s
ml_kem_512/encap_cv             0.44 %          0.41 %            10      0.20%            0.41%
ml_kem_512/encap_min            15.5 us         15.5 us           10   72.5719k       63.7001k/s
ml_kem_512/encap_max            15.7 us         15.7 us           10   73.1062k       64.4231k/s
```

### On ARM Cortex-A72 i.e. Raspberry Pi 4B

Compiled with **gcc (GCC) 12.1.0**.

```bash
$ uname -srm
Linux 6.6.33-2-MANJARO-RPI4 aarch64
```

```bash
2024-09-02T21:13:57+04:00
Running ./build/benchmark/bench.out
Run on (4 X 1500 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 48 KiB (x4)
  L2 Unified 1024 KiB (x1)
Load Average: 3.75, 4.36, 2.95
-------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------
ml_kem_1024/keygen_mean          225 us          223 us           10       4.47447k/s
ml_kem_1024/keygen_median        225 us          224 us           10       4.47389k/s
ml_kem_1024/keygen_stddev      0.896 us        0.828 us           10        16.5682/s
ml_kem_1024/keygen_cv           0.40 %          0.37 %            10            0.37%
ml_kem_1024/keygen_min           223 us          222 us           10       4.44656k/s
ml_kem_1024/keygen_max           226 us          225 us           10       4.50358k/s
ml_kem_512/keygen_mean          84.1 us         83.7 us           10       11.9468k/s
ml_kem_512/keygen_median        84.0 us         83.6 us           10       11.9584k/s
ml_kem_512/keygen_stddev       0.298 us        0.282 us           10        40.1688/s
ml_kem_512/keygen_cv            0.35 %          0.34 %            10            0.34%
ml_kem_512/keygen_min           83.7 us         83.3 us           10       11.8656k/s
ml_kem_512/keygen_max           84.6 us         84.3 us           10       12.0022k/s
ml_kem_512/decap_mean            130 us          130 us           10       7.70299k/s
ml_kem_512/decap_median          130 us          130 us           10       7.70396k/s
ml_kem_512/decap_stddev        0.320 us        0.273 us           10        16.1626/s
ml_kem_512/decap_cv             0.25 %          0.21 %            10            0.21%
ml_kem_512/decap_min             130 us          129 us           10       7.67157k/s
ml_kem_512/decap_max             131 us          130 us           10       7.72261k/s
ml_kem_768/decap_mean            207 us          206 us           10       4.85013k/s
ml_kem_768/decap_median          207 us          206 us           10       4.85556k/s
ml_kem_768/decap_stddev         1.31 us         1.12 us           10        26.2579/s
ml_kem_768/decap_cv             0.63 %          0.55 %            10            0.54%
ml_kem_768/decap_min             206 us          205 us           10        4.7875k/s
ml_kem_768/decap_max             210 us          209 us           10       4.87723k/s
ml_kem_512/encap_mean            102 us          101 us           10       9.89142k/s
ml_kem_512/encap_median          101 us          101 us           10        9.9073k/s
ml_kem_512/encap_stddev        0.515 us        0.491 us           10        47.6312/s
ml_kem_512/encap_cv             0.51 %          0.49 %            10            0.48%
ml_kem_512/encap_min             101 us          101 us           10       9.76968k/s
ml_kem_512/encap_max             103 us          102 us           10       9.93945k/s
ml_kem_768/keygen_mean           142 us          141 us           10       7.06934k/s
ml_kem_768/keygen_median         142 us          141 us           10       7.06916k/s
ml_kem_768/keygen_stddev       0.293 us        0.259 us           10          12.91/s
ml_kem_768/keygen_cv            0.21 %          0.18 %            10            0.18%
ml_kem_768/keygen_min            142 us          141 us           10       7.04302k/s
ml_kem_768/keygen_max            143 us          142 us           10       7.08679k/s
ml_kem_1024/encap_mean           254 us          253 us           10       3.94775k/s
ml_kem_1024/encap_median         254 us          253 us           10       3.95198k/s
ml_kem_1024/encap_stddev        1.46 us         1.41 us           10        21.6985/s
ml_kem_1024/encap_cv            0.57 %          0.56 %            10            0.55%
ml_kem_1024/encap_min            253 us          252 us           10       3.89026k/s
ml_kem_1024/encap_max            258 us          257 us           10       3.96754k/s
ml_kem_1024/decap_mean           306 us          304 us           10       3.28684k/s
ml_kem_1024/decap_median         305 us          304 us           10       3.29067k/s
ml_kem_1024/decap_stddev        1.93 us         1.84 us           10        19.7821/s
ml_kem_1024/decap_cv            0.63 %          0.60 %            10            0.60%
ml_kem_1024/decap_min            304 us          302 us           10       3.24947k/s
ml_kem_1024/decap_max            309 us          308 us           10        3.3074k/s
ml_kem_768/encap_mean            167 us          167 us           10       6.00584k/s
ml_kem_768/encap_median          167 us          166 us           10       6.00611k/s
ml_kem_768/encap_stddev        0.845 us        0.721 us           10        25.9414/s
ml_kem_768/encap_cv             0.50 %          0.43 %            10            0.43%
ml_kem_768/encap_min             166 us          166 us           10       5.95361k/s
ml_kem_768/encap_max             169 us          168 us           10       6.03767k/s
```

## Usage

`ml-kem` is written as a header-only C++20 fully `constexpr` library, majorly targeting 64 -bit desktop/ server grade platforms and it's pretty easy to get started with. All you need to do is following.

- Clone `ml-kem` repository.

```bash
cd

# Multi-step cloning and importing of submodules
git clone https://github.com/itzmeanjan/ml-kem.git && pushd ml-kem && git submodule update --init && popd
# Or do single step cloning and importing of submodules
git clone https://github.com/itzmeanjan/ml-kem.git --recurse-submodules
# Or clone and then run tests, which will automatically bring in dependencies
git clone https://github.com/itzmeanjan/ml-kem.git && pushd ml-kem && make -j && popd
```

- Write your program while including proper header files ( based on which variant of ML-KEM you want to use, see [include](./include/ml_kem/) directory ), which includes declarations ( and definitions ) of all required ML-KEM routines and constants ( such as byte length of public/ private key, cipher text etc. ).

```cpp
// main.cpp

#include "ml_kem/ml_kem_512.hpp"
#include <algorithm>
#include <array>
#include <cassert>

int
main()
{
  std::array<uint8_t, ml_kem_512::SEED_D_BYTE_LEN> d{};
  std::array<uint8_t, ml_kem_512::SEED_Z_BYTE_LEN> z{};

  std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> pkey{};
  std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> skey{};

  std::array<uint8_t, ml_kem_512::SEED_M_BYTE_LEN> m{};
  std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> sender_key{};
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> receiver_key{};

  // Be careful !
  //
  // Read API documentation in include/ml_kem/internals/rng/prng.hpp
  ml_kem_prng::prng_t<128> prng;

  prng.read(d);
  prng.read(z);
  prng.read(m);

  ml_kem_512::keygen(d, z, pkey, skey);
  assert(ml_kem_512::encapsulate(m, pkey, cipher, sender_key)); // Key Encapsulation might fail, if input public key is malformed
  ml_kem_512::decapsulate(skey, cipher, receiver_key);

  assert(sender_key == receiver_key);
  return 0;
}
```

- When compiling your program, let your compiler know where it can find `ml-kem`, `sha3` and `subtle` headers, which includes their definitions ( all of them are header-only libraries ) too.

```bash
# Assuming `ml-kem` was cloned just under $HOME

ML_KEM_HEADERS=~/ml-kem/include
SHA3_HEADERS=~/ml-kem/sha3/include
SUBTLE_HEADERS=~/ml-kem/subtle/include

g++ -std=c++20 -Wall -Wextra -Wpedantic -O3 -march=native -I $ML_KEM_HEADERS -I $SHA3_HEADERS -I $SUBTLE_HEADERS main.cpp
```

ML-KEM Variant | Namespace | Header
:-- | :-: | --:
ML-KEM-512 Routines | `ml_kem_512::` | `include/ml_kem/ml_kem_512.hpp`
ML-KEM-768 Routines | `ml_kem_768::` | `include/ml_kem/ml_kem_768.hpp`
ML-KEM-1024 Routines | `ml_kem_1024::` | `include/ml_kem/ml_kem_1024.hpp`

> [!NOTE]
> ML-KEM parameter sets are taken from table 2 of ML-KEM standard @ https://doi.org/10.6028/NIST.FIPS.203.

All the functions, in this ML-KEM header-only library, are implemented as `constexpr` functions. Hence you should be able to evaluate ML-KEM key generation, encapsulation or decapsulation at compile-time itself, given that all inputs are known at compile-time. I present you with following demonstration program, which generates a ML-KEM-512 keypair and encapsulates a message, producing a ML-KEM-512 cipher text and a fixed size shared secret, given `seed_{d, z, m}` as input - all at program compile-time. Notice, the *static assertion*.

```cpp
// compile-time-ml-kem-512.cpp
//
// Compile and run this program with
// $ g++ -std=c++20 -Wall -Wextra -Wpedantic -I include -I sha3/include -I subtle/include compile-time-ml-kem-512.cpp && ./a.out
// or
// $ clang++ -std=c++20 -Wall -Wextra -Wpedantic -fconstexpr-steps=4000000 -I include -I sha3/include -I subtle/include compile-time-ml-kem-512.cpp && ./a.out

#include "ml_kem/ml_kem_512.hpp"

// Compile-time evaluation of ML-KEM-512 key generation and encapsulation, using NIST official KAT no. (1).
constexpr auto
eval_ml_kem_768_encaps() -> auto
{
  using seed_t = std::array<uint8_t, ml_kem_512::SEED_D_BYTE_LEN>;

  // 7c9935a0b07694aa0c6d10e4db6b1add2fd81a25ccb148032dcd739936737f2d
  constexpr seed_t seed_d = { 124, 153, 53, 160, 176, 118, 148, 170, 12, 109, 16,  228, 219, 107, 26,  221, 47,  216, 26, 37,  204, 177, 72,  3,   45, 205, 115, 153, 54,  115, 127, 45 };
  // b505d7cfad1b497499323c8686325e4792f267aafa3f87ca60d01cb54f29202a
  constexpr seed_t seed_z = {181, 5, 215, 207, 173, 27, 73, 116, 153, 50, 60, 134, 134, 50, 94, 71, 146, 242, 103, 170, 250, 63, 135, 202, 96, 208, 28, 181, 79, 41, 32, 42};
  // eb4a7c66ef4eba2ddb38c88d8bc706b1d639002198172a7b1942eca8f6c001ba
  constexpr seed_t seed_m = {235, 74, 124, 102, 239, 78, 186, 45, 219, 56, 200, 141, 139, 199, 6, 177, 214, 57, 0, 33, 152, 23, 42, 123, 25, 66, 236, 168, 246, 192, 1, 186};

  std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN> seckey{};
  std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN> cipher{};

  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> shared_secret{};

  ml_kem_512::keygen(seed_d, seed_z, pubkey, seckey);
  (void)ml_kem_512::encapsulate(seed_m, pubkey, cipher, shared_secret);

  return shared_secret;
}

int
main()
{
  // This step is being evaluated at compile-time, thanks to the fact that my ML-KEM implementation is `constexpr`.
  static constexpr auto computed_shared_secret = eval_ml_kem_768_encaps();
  // b4c8e3c4115f9511f2fddb288c4b78c5cd7c89d2d4d321f46b4edc54ddf0eb36
  constexpr std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN> expected_shared_secret = { 180, 200, 227, 196, 17, 95, 149, 17, 242, 253, 219, 40, 140, 75, 120, 197, 205, 124, 137, 210, 212, 211, 33, 244, 107, 78, 220, 84, 221, 240, 235, 54 };

  // Notice static_assert, yay !
  static_assert(computed_shared_secret == expected_shared_secret, "Must be able to compute shared secret at compile-time !");
  return 0;
}
```

See example [program](./examples/ml_kem_768.cpp), where I show how to use ML-KEM-768 API.

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -O3 -march=native -I ./include -I ./sha3/include -I ./subtle/include/ examples/ml_kem_768.cpp && ./a.out
```

```bash
ML-KEM-768
Pubkey         : 8ad4a0396b6d30127451d7bbeb0941560689c7952015635c0174c46c761a849c875f4a5973c6b510fa2fd31a690b01c63966a085b499e5352c0e0a45f2faa824012df176800f8348713c7f8aa9a0e1d3adab402b9fe3cbd2b11c70f3079eda7e7171cb4df1a77d178168d18637ba77342b9430d1b843842862ea44aa4c791de60aa84ccdf5515f42b0811276addf276676532fb15570a037201b4b41b09745aefb46608109ef46c071f2901fec1108e83d84a7493e661447c4210904276812ce56b0a312b57444e60c2923b7a6d75772859c53da264b061bd7205eae357cd35135a4310d8e3b1a684cc44c614f0b468ad75b95cb985d901653279ba4b96999c4779c68c1cbdfb50207b3cec7f48484a650d785c14bf0543ca4511be9ceb1da0124b5c0cc696e1300c5349636a5d290ca1b883b7643a90a00ef244f5734b5a9bb5783b7219a1618aae4b7a0f66d87e075c5410ade1a635877ac0a618ff846ac52ba5370684777f78b5ee8b6a3ac376320b68aeb626af2bb236b75ceeb91d166678e0a189014b437a17d8400413f677b363522ffe9a987ebaad22576a3b6858ca5796f304c082c9bd6385f9ff98e64ba2057e731a5d0b3550c22694bbac32173163a69512626a7624b414621730ca793255216e092905710e318a75865b4e1670b512700cd29afcb674b2d3b3338a2ad9d9b82890c98605bb8b531a8901307bcbbcc3e744603353109f095c96a5c50641335b3439c3522e12bc5ae9b47566826b58c2c7bf207f87b5f0bf25340671142e01cc908abfe68ba09935166c191834942e6ac47148c493bf1c2f1b755346a6bf926c5d5f44ec9c258634386c301400cd9a29b58568d12bb57b74bbc11a1eef550a395720d345a285658a0660cffbc49fec8b83b77a3aff25f014b01d4f72322f74d87801dafd3b1e2176d5f855915077ef376b031536f5a8c26012a39f3d201386a4c8c2246661a30888b45431398d400286ab06001553174468797e6ab01a65c1a6792ff06c4c36a2acad8a6c3c115c536aa8358787ce4c75aac85ec5602c2e67823a40d6ad497764800522055cf76b7631b20ae097458db53f2908eaa5a8d3528107caa992fb05a845c109b62c7045c8fc8a0ccc702b6b4874449f781cb4430799b577ce7b4120b950dd20179011a10144c54b7a9f6f1a01c484ee8968d1116c460da728958b574ccaa6e7b3b67a9235ca111d7fc575414c3f1751aa87960524b4ccebcca2272809d5a63bb456ac1380d06438c29524233f59c17d0888901d0b6c32be93b75b13152d40b358a705a04bc5b78a4227fb1be739558ec748c27e74660e65b0b2003f2216b6b103ba7313268a5783e9073c4b74387a30db99605d6f0bf15c93f2f80063f846239c21b22c1616d343280d46745e6609a83a657914bd00968253b5176a939ca2a2c4eba7f5d93cb0cb59e3bb6b230a65dfad5490ddb6fa0a3c99ec8764869330ee94f6df08080e996f7c9095cf12d46a7956986ae58597d0d8037ef7a73ece5349a995aab807e1603192d5a354ffc1020540d6fe8c854b1cc9a2c30a7a621d1173c3991084765b90fd2a58b06862b29aaa307c16004aefa268adc85985cfc361590410adab289eb79cb8f9abd87b9da457b5fdb2b8d48809f2c975e7140149b7fc57958a60a
Seckey         : db99a8f00c2a1e3998056c0f4f27bae4c23c58b03fb19711e044a3737016410b97f3b432fd9b75ab45ac841005d5750a5de5ac9a5bb751f7b3931336ab1c5c7677186c76a4bcb3c79ac6c824d6b72d304e27fab86c31440a3a3a54cb025e8158c7ca92ffa342325c016b8b8263a7279bd681b4b796e198b25eac22f4a7026d0645f761113aa5bb50bb751be8b650701d6904035bbc441295806f6227a4e3cd4866bbcc83b713859e1883ad77838eba14629a2b3af069c00575a527e2ca611a5c6ee610699a097e7642cce908d720094a8301cf6223a8161513c76a93fa0ebd2a4efd713ce13abaea7b3fca9c221054503b7a792e374274ca25ae0756457cca59bb8d4522a3b03b52510913fed080a3833dcc020b0d620cd1dc65c0a23b732c197ad07828c126dad253ce63ac14a9a2ab42a78bd21fb91a069a237485e76b39e1948c13ac42a07ea1ebbccc35872fa25afd8908524829aea16e5e4b49d4095c9cc17bc70a8ee8e3b877da50c04703fe3a15e4ea447c085ab14875ef6776b850683a41197e995e5111a87d527a3fc883fe6720c61236f96433d27c03f5706ecbf7602034b4aebb8e132255b72623d5cc1a70aa9ce53b9932852b163a61a6a9701a44448f956fda155d11996e83183d9a003cb6b07182d427239149f317342c17742f1b612b90b4cd909eb5303d13b06a6af422ced67da1eabad2e950701218dfb54ca757ac2576a39d3b827f28c1356130e8a864725a61f723cbedb19fb2444e6fd73ca3e90dfe2a59a222831e26a4720a30f4824702b0442c52b832848a62e2b0778a72eed4882996a58890cf31970f9e93b5a85ab193b2abff64a3f1b10f9d5202f2946d84075e87191e81851ccffa2c591031d0bc17aff346e3cb391796c0c6c74444c161b348906ef8bb54c022c8870395f58dfa1cb33f25c32df2ce7a2813476514acd72aba073f2a7937de46a69c01b1a6e03df270af5bb65fa1db92e9ba612019926b8c4d2df472108ab863316652544596f3b51cf97b7179c886258d88d42af6546029b5332ffb506a1b0259d31972bb6900864c0d1a98af5b1f9d500810925493b31ab1e3b129a0cc08092613e43425bc03e3840bc5119563d43beb9849a3f4094ae8ae6afbba068563d8918dc2b711084c47e2db3ea5b95791fbca566ca751ab83f9e49f4e61764ce88f7c24116d246eecf3a229f0bbccccc4392370103667d5472fd193c4530a0764e8c705a168c651aa9b0825eed30d94a7111b895e8495a138f9a8b7ba5e563b4f529a25ad2867fe8b1ce13b6d41f16a2de991af874163a824111779236c1d4eeb7e415a5d686514bbf92c1ef06929e7881555608c7a3183a719121698b35216eddb27379577301449d5f3a5c1bc892562650709b7369b7707c6620cb54680226e3e102a66d53b26970b09c73c93b90acd7b335afb2d183979c6c7abb8ac0a06e8cf542b0c4fb91ff4183d51957ff2169b16961802c50f4ec64024faca5f9707727b866271244822a11d2015ea581a140a53b7078c71914bb2e04f3deab03bcb1117c3778070c5eeab155da285931b68b9f21749ec0985349c8cd03fe2585080e71c6665cb05ea091ff41e306c892e60686ad5a58ad4a0396b6d30127451d7bbeb0941560689c7952015635c0174c46c761a849c875f4a5973c6b510fa2fd31a690b01c63966a085b499e5352c0e0a45f2faa824012df176800f8348713c7f8aa9a0e1d3adab402b9fe3cbd2b11c70f3079eda7e7171cb4df1a77d178168d18637ba77342b9430d1b843842862ea44aa4c791de60aa84ccdf5515f42b0811276addf276676532fb15570a037201b4b41b09745aefb46608109ef46c071f2901fec1108e83d84a7493e661447c4210904276812ce56b0a312b57444e60c2923b7a6d75772859c53da264b061bd7205eae357cd35135a4310d8e3b1a684cc44c614f0b468ad75b95cb985d901653279ba4b96999c4779c68c1cbdfb50207b3cec7f48484a650d785c14bf0543ca4511be9ceb1da0124b5c0cc696e1300c5349636a5d290ca1b883b7643a90a00ef244f5734b5a9bb5783b7219a1618aae4b7a0f66d87e075c5410ade1a635877ac0a618ff846ac52ba5370684777f78b5ee8b6a3ac376320b68aeb626af2bb236b75ceeb91d166678e0a189014b437a17d8400413f677b363522ffe9a987ebaad22576a3b6858ca5796f304c082c9bd6385f9ff98e64ba2057e731a5d0b3550c22694bbac32173163a69512626a7624b414621730ca793255216e092905710e318a75865b4e1670b512700cd29afcb674b2d3b3338a2ad9d9b82890c98605bb8b531a8901307bcbbcc3e744603353109f095c96a5c50641335b3439c3522e12bc5ae9b47566826b58c2c7bf207f87b5f0bf25340671142e01cc908abfe68ba09935166c191834942e6ac47148c493bf1c2f1b755346a6bf926c5d5f44ec9c258634386c301400cd9a29b58568d12bb57b74bbc11a1eef550a395720d345a285658a0660cffbc49fec8b83b77a3aff25f014b01d4f72322f74d87801dafd3b1e2176d5f855915077ef376b031536f5a8c26012a39f3d201386a4c8c2246661a30888b45431398d400286ab06001553174468797e6ab01a65c1a6792ff06c4c36a2acad8a6c3c115c536aa8358787ce4c75aac85ec5602c2e67823a40d6ad497764800522055cf76b7631b20ae097458db53f2908eaa5a8d3528107caa992fb05a845c109b62c7045c8fc8a0ccc702b6b4874449f781cb4430799b577ce7b4120b950dd20179011a10144c54b7a9f6f1a01c484ee8968d1116c460da728958b574ccaa6e7b3b67a9235ca111d7fc575414c3f1751aa87960524b4ccebcca2272809d5a63bb456ac1380d06438c29524233f59c17d0888901d0b6c32be93b75b13152d40b358a705a04bc5b78a4227fb1be739558ec748c27e74660e65b0b2003f2216b6b103ba7313268a5783e9073c4b74387a30db99605d6f0bf15c93f2f80063f846239c21b22c1616d343280d46745e6609a83a657914bd00968253b5176a939ca2a2c4eba7f5d93cb0cb59e3bb6b230a65dfad5490ddb6fa0a3c99ec8764869330ee94f6df08080e996f7c9095cf12d46a7956986ae58597d0d8037ef7a73ece5349a995aab807e1603192d5a354ffc1020540d6fe8c854b1cc9a2c30a7a621d1173c3991084765b90fd2a58b06862b29aaa307c16004aefa268adc85985cfc361590410adab289eb79cb8f9abd87b9da457b5fdb2b8d48809f2c975e7140149b7fc57958a60a14b4dadf74818f1bbd16eab6f940840f81c2745ab77e22e873e285014b154b9e59d27e164c061cfaf0d595f45d7c821ab54bb7bf1a50108cf605247e5867d150
Encapsulated ? : true
Cipher         : 618d4938da6a966795627c52fea714ae433de7faefdbbe3339cfd3fcce66c8c02b0fcdb3e73b2e579abc9d971d343e683d63c7c2c77941ec68774175f86ce9fbb35a80d0b417feabee12a359fec9b24af585560b8075f88e60050b30db3306948727dc104e66c5814355d96eb9204130b8463fdb9d8b41fe7d27a1a23ad06191443a3e8011dd4cb7368c10ddc0b0fb02547f5f0599a9cf3f4f3d805a77dba717a1c10b9350ff495bc0041f76e7369c58d9be90e79ea6ed7609988a1550557a691f80e8b06258ac703ba90c6f3d090d1195ec78dd536529fa0c7406845c885af50857eca3c0a2a4a90aa0c22dd121756c10f986f1614f3db3fcabead02df5567cdcc2851bb685bd3137cfc2dcc0ac5c1558ff144dd800602435790e7c0d478dae0563b50cefdee7790da47319ff245b13971d0523398cc685b3de2a4c3d1a2f60f5018234397d1c4c46c10b81118ea8e8b123c74cadae42c516ac3c5e7c39daabff369444c851ee299880bff64d6781487c4c3022fa559a5ad3919d1c5b644f36de02a8e1073ad29a6e516f71d7ced0d605bf2c5b16d1821fe4568cccb86896b04973daa3ba196e889382636678f2a39ea0ae09bf3ff44f3b9dc4e84d9666f40c206e0b180f3054e6a4ab34979030bfc82a045a457c37f6d103962f59080e1a86b68b568d8065e9258e7c9ae3afd059ab3c8686485796c020639387e404771749aac794f9d1cd9b1b6d9de137fe7b290199f13ff6a37538816924ca28f50310c8d490a25b86985e0677c2b8f5781c9897f499a764f1c5399840f8bd6c4b86c480b21492efa0e996569edad7873501415361621c402d97c77984d76dd5553278e8e9ebe7cac85803022803d48508b98715405977350c949657f46d042834f7b26dd734d25bab7f38e702491518141416841221b217b62f4dc1edbd2ed9974fc5b64ea8221ca7afc2bf58c277c5bbc0f5a17c61e6c33a9a163c35832641d8d825665b59931ab5d69fa672b5572ca134b6782df841045ce7f7fc47707e6083fa95967eecd243550b890d5c7c3560ecf5149f22884ce9dfdd4529b891def5fbeba8ba5b42e545e8f1a6b76ac8b50ea0a168035cfb5381bbe2defdf1b7182ecdd26fc19b4bdec5914fccc6cca5b925bf69e0d59702d85b67ed625ca27333174ce324ba454ba0d5116c88dd23fc4233dceb2aefb345652408b7e45905e0ab1fbbda1c6622e0210ffe6a0571f94535f84a427ad73d7f4b772b94f3d2e9307dd8ec5054f4956c54181c8cc3bf0cd6ce7f02375453450181c6c433884fe399a5943d4953f408497fba4d9901f5149577a955aa45b9eb5c97253314409990d069946fbf5ad8468823ae9befb27e5d31c6f489b98141488b98f894876f316e21856f07fc0156ac04ee1a6b2853ce6a90e97e948879eefa96fed1154a140487b00467106888c8c1df98737976814302a2d62030dfa4a5f70d83e5e4d819b39e5a155599930c4ddf357a6a57bfc92b77e39c5cc665ab354b4cde2b13dd03ff7d8b375887956470
Shared secret  : e6a9fc79df8a91733c7f385bc66602a526b54bbf78ed2ac11029a42a2a56f515
```

> [!CAUTION]
> Before you consider using Psuedo Random Number Generator which comes with this library implementation, I strongly advice you to go through [include/ml_kem/internals/rng/prng.hpp](./include/ml_kem/internals/rng/prng.hpp).

> [!NOTE]
> Looking at API documentation, in header files, can give you good idea of how to use ML-KEM API. Note, this library doesn't expose any raw pointer based interface, rather everything is wrapped under statically defined `std::span` - which one can easily create from `std::{array, vector}`. I opt for using statically defined `std::span` based function interfaces because we always know, at compile-time, how many bytes the seeds/ keys/ cipher-texts/ shared-secrets are, for various different ML-KEM parameters. This gives much better type safety and compile-time error reporting.
