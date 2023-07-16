> **Warning** **This Kyber implementation is conformant with Kyber [specification](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf) and I also *try* to make it constant-time but be informed that it is not yet audited. If you consider using it in production, be careful !**

# kyber
CRYSTALS-Kyber: Post-Quantum Public-key Encryption &amp; Key-establishment Algorithm

## Motivation

Kyber is selected by NIST as post-quantum secure public key encryption (PKE) and key exchange mechanism (KEM) as part of NIST's post-quantum cryptography (PQC) standardization initiative.

Kyber offers both

- IND-CPA-secure public key encryption [Kyber CPAPKE]
- IND-CCA2-secure key encapsulation mechanism [Kyber CCAKEM]

while its security is based on hardness of solving learning-with-errors (LWE) problem in module (i.e. structured) lattices.

Under **IND-CPA-secure Kyber PKE**, two communicating parties both generating their key pairs while publishing their public keys to each other, can encrypt fixed length ( = 32 -bytes ) message using peer's public key. Cipher text can be decrypted by respective secret key ( which is private to key owner ) and 32 -bytes message can be recovered back.

Algorithm | Input | Output
--- | :-: | --:
PKE KeyGen | - | Public Key and Secret Key
Encryption | Public Key, 32 -bytes message and 32 -bytes random coin | Cipher Text
Decryption | Secret Key and Cipher Text | 32 -bytes message

> **Note** When a slightly tweaked Fujisaki–Okamoto (FO) transform is applied on IND-CPA-secure Kyber PKE, we can construct an IND-CCA2-secure KEM.

While with **IND-CCA2-secure Kyber KEM**, two parties interested in secretly communicating over public & insecure channel, can generate a shared secret key ( of arbitrary byte length ) from a key derivation function ( i.e. KDF which is SHAKE256 XOF in this context ) which is obtained by both of these parties as result of seeding SHAKE256 XOF with same secret. This secret is 32 -bytes and that's what is communicated by sender to receiver using underlying Kyber PKE.

Algorithm | Input | Output
--- | :-: | --:
KEM KeyGen | - | Public Key and Secret Key
Encapsulation | Public Key | Cipher Text and SHAKE256 KDF
Decapsulation | Secret Key and Cipher Text | SHAKE256 KDF

> **Note** IND-CPA-secure Kyber PKE can be used for asynchronous secure communication such as email.

> **Note** IND-CCA2-secure Kyber KEM can be used for synchronous secure communication such as TLS.

Here I'm developing & maintaining `kyber` - a zero-dependency, header-only and easy-to-use C++ library implementing Kyber KEM, supporting Kyber-{512, 768, 1024} parameter sets, as defined in table 1 of Kyber specification. 

Only dependencies are `sha3` and `subtle` - both of them are zero-dependency, header-only C++ libraries themselves. I decided to write `sha3` so that I can modularize a fairly common PQC dependency, because of the fact that - SHA3 hash functions and extendable output functions are common symmetric key primitives used in post-quantum cryptographic constructions such as Kyber, Dilithium, Falcon and SPHINCS+ etc.. While `subtle` is a pretty light-weight library which helps achieving constant-timeness in cryptographic libraries. Here it's used for performing constant-time byte comparison and conditional selection without using booleans - only relying on integer addition, subtration and bit-wise operations.

> **Note** Both `sha3` and `subtle` are pinned to specific commit, using git submodule. See [usage](#usage) section below for git submodule set up guide.

> **Note** Find Kyber specification [here](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf) - this is the document that I followed when implementing Kyber. I suggest you go through the specification to get an in-depth understanding of Kyber PQC suite.

> **Note** Find progress of NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography)

## Prerequisites

- A C++ compiler with C++20 standard library such as `g++`/ `clang++`

```bash
$ clang++ --version
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 12.2.0-17ubuntu1) 12.2.0
```

- Build tools such as `make`, `cmake`.

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.22.1
```

- For testing Kyber implementation, you need to globally install `google-test` library and headers. Follow [this](https://github.com/google/googletest/tree/main/googletest#standalone-cmake-project) guide if you haven't installed it yet.
- For benchmarking Kyber implementation, targeting CPU systems, you'll need to have `google-benchmark` header and library globally installed. I found [this](https://github.com/google/benchmark#installation) guide helpful.
- If you are on a machine running GNU/Linux kernel and you want to obtain CPU Cycle count for KEM routines, you should consider building `google-benchmark` library with `libPFM` support, following [this](https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7) step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.
- For importing dependencies `sha3`, `subtle` - initialize & update git submodule after cloning this repository.

```bash
git clone https://github.com/itzmeanjan/kyber.git

pushd kyber
git submodule update --init
# Now you can {test, benchmark, use} `kyber`
popd
```

## Testing

For testing functional correctness and conformance ( with Kyber specification and reference implementation ) of this Kyber implementation, you have to issue

```bash
$ make -j8

[==========] Running 10 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 10 tests from KyberKEM
[ RUN      ] KyberKEM.CompressDecompressZq
[       OK ] KyberKEM.CompressDecompressZq (202 ms)
[ RUN      ] KyberKEM.ArithmeticOverZq
[       OK ] KyberKEM.ArithmeticOverZq (298 ms)
[ RUN      ] KyberKEM.Kyber512KeygenEncapsDecaps
[       OK ] KyberKEM.Kyber512KeygenEncapsDecaps (0 ms)
[ RUN      ] KyberKEM.Kyber768KeygenEncapsDecaps
[       OK ] KyberKEM.Kyber768KeygenEncapsDecaps (0 ms)
[ RUN      ] KyberKEM.Kyber1024KeygenEncapsDecaps
[       OK ] KyberKEM.Kyber1024KeygenEncapsDecaps (0 ms)
[ RUN      ] KyberKEM.Kyber512KnownAnswerTests
[       OK ] KyberKEM.Kyber512KnownAnswerTests (8 ms)
[ RUN      ] KyberKEM.Kyber768KnownAnswerTests
[       OK ] KyberKEM.Kyber768KnownAnswerTests (13 ms)
[ RUN      ] KyberKEM.Kyber1024KnownAnswerTests
[       OK ] KyberKEM.Kyber1024KnownAnswerTests (20 ms)
[ RUN      ] KyberKEM.NumberTheoreticTransform
[       OK ] KyberKEM.NumberTheoreticTransform (0 ms)
[ RUN      ] KyberKEM.PolynomialSerialization
[       OK ] KyberKEM.PolynomialSerialization (0 ms)
[----------] 10 tests from KyberKEM (545 ms total)

[----------] Global test environment tear-down
[==========] 10 tests from 1 test suite ran. (545 ms total)
[  PASSED  ] 10 tests.
```

## Benchmarking

For benchmarking Kyber KEM routines ( i.e. keygen, encaps and decaps ) for various suggested parameter sets, targeting CPU systems, you need to issue.

```bash
make benchmark  # If you haven't built google-benchmark library with libPFM support.
make perf       # If you have built google-benchmark library with libPFM support.
```

> **Note** Benchmarking expects presence of `google-benchmark` header and library in global namespace ( so that it can be found by the compiler ).

> **Warning** When benchmarking, ensure that you've disabled CPU frequency scaling, by following [this](https://github.com/google/benchmark/blob/main/docs/reducing_variance.md) guide.

> **Note** `make perf` - was issued when collecting following benchmarks. Notice, *cycles* column, denoting cost of executing Kyber KEM routines in terms of CPU cycles. Follow [this](https://github.com/google/benchmark/blob/main/docs/perf_counters.md) for more details.

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with GCC )

```bash
2023-07-16T15:32:26+04:00
Running ./benchmarks/perf.out
Run on (16 X 1311.11 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.12, 0.27, 0.32
***WARNING*** There are 9 benchmarks with threads and 1 performance counters were requested. Beware counters will reflect the combined usage across all threads.
---------------------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations     CYCLES items_per_second
---------------------------------------------------------------------------------------
kyber512/keygen        18.1 us         18.1 us        38639   84.8877k       55.1314k/s
kyber512/encap         23.7 us         23.7 us        29527    111.18k       42.1187k/s
kyber512/decap         29.3 us         29.3 us        23826   137.434k       34.0758k/s
kyber768/keygen        30.9 us         30.9 us        22640    144.59k       32.3781k/s
kyber768/encap         38.8 us         38.8 us        18069   181.814k       25.7492k/s
kyber768/decap         46.2 us         46.2 us        15162   216.234k       21.6523k/s
kyber1024/keygen       47.9 us         47.9 us        14610   224.347k       20.8675k/s
kyber1024/encap        57.9 us         57.9 us        12074   271.079k       17.2612k/s
kyber1024/decap        67.9 us         67.9 us        10307    317.69k       14.7282k/s
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with Clang )

```bash
2023-07-16T15:33:15+04:00
Running ./benchmarks/perf.out
Run on (16 X 4371.72 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.26, 0.29, 0.33
***WARNING*** There are 9 benchmarks with threads and 1 performance counters were requested. Beware counters will reflect the combined usage across all threads.
---------------------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations     CYCLES items_per_second
---------------------------------------------------------------------------------------
kyber512/keygen        15.5 us         15.5 us        44767     72.75k       64.3635k/s
kyber512/encap         19.1 us         19.1 us        36484   89.6099k        52.268k/s
kyber512/decap         23.7 us         23.7 us        29515   110.922k       42.1968k/s
kyber768/keygen        26.4 us         26.4 us        26596   123.574k       37.8928k/s
kyber768/encap         31.5 us         31.5 us        22228   147.527k       31.7306k/s
kyber768/decap         37.4 us         37.4 us        18705   175.022k       26.7379k/s
kyber1024/keygen       40.6 us         40.6 us        17351   189.919k       24.6478k/s
kyber1024/encap        46.9 us         47.0 us        14932   219.581k       21.2966k/s
kyber1024/decap        55.4 us         55.5 us        12557   259.598k       18.0263k/s
```

## Usage

`kyber` is written to be a zero-dependency, header-only C++ library such that it's pretty easy to start using it in your project. All you need to do is following.

- Clone `kyber` repository.

```bash
cd
git clone https://github.com/itzmeanjan/kyber.git
```

- Initialize and update git submodule, so that dependencies i.e. `sha3` and `subtle` are available inside `kyber` source tree.

```bash
cd kyber
git submodule update --init
```

- Write your program while including proper header file ( based on which variant of Kyber KEM you want to use, see [include](./include) directory ), which includes declarations ( and definitions ) of all required KEM routines and constants ( such as byte length of public/ private keys and cipher text ).

```cpp
// main.cpp

#include "kyber512_kem.hpp"
#include <algorithm>
#include <cassert>

int
main()
{
  uint8_t d[32]{}; // seed
  uint8_t z[32]{}; // seed
  uint8_t pkey[kyber512_kem::PKEY_LEN]{};
  uint8_t skey[kyber512_kem::SKEY_LEN]{};
  uint8_t m[32]{}; // seed
  uint8_t cipher[kyber512_kem::CIPHER_LEN]{};

  // Be careful !
  //
  // Read API documentation in include/prng.hpp
  prng::prng_t prng;

  prng.read(d, sizeof(d));
  prng.read(z, sizeof(z));
  prng.read(m, sizeof(m));

  kyber512_kem::keygen(d, z, pkey, skey);
  auto skdf = kyber512_kem::encapsulate(m, pkey, cipher);
  auto rkdf = kyber512_kem::decapsulate(skey, cipher);

  uint8_t sender_key[32]{};
  skdf.squeeze(sender_key, sizeof(sender_key));

  uint8_t receiver_key[32]{};
  rkdf.squeeze(receiver_key, sizeof(receiver_key));

  assert(std::ranges::equal(sender_key, receiver_key));
  return 0;
}
```

- When compiling your program, let your compiler know where it can find `kyber`, `sha3` and `subtle` headers, which includes their definitions ( kyber being a header-only library ) too

```bash
# Assuming `kyber` was cloned just under $HOME

KYBER_HEADERS=~/kyber/include
SHA3_HEADERS=~/kyber/sha3/include
SUBTLE_HEADERS=~/kyber/subtle/include

g++ -std=c++20 -Wall -O3 -march=native -I $KYBER_HEADERS -I $SHA3_HEADERS -I $SUBTLE_HEADERS main.cpp
```

Kyber KEM Variant | Namespace | Header
:-- | :-: | --:
Kyber512 KEM Routines | `kyber512_kem::` | [include/kyber512_kem.hpp](include/kyber512_kem.hpp)
Kyber768 KEM Routines | `kyber768_kem::` | [include/kyber768_kem.hpp](include/kyber768_kem.hpp)
Kyber1024 KEM Routines | `kyber1024_kem::` | [include/kyber1024_kem.hpp](include/kyber1024_kem.hpp)

> **Note** Kyber parameter sets are selected from table 1 of Kyber [specification](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf)

---

### Kyber Public Key Encryption (PKE)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
PKE KeyGen | `pke::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 12 * 32) -bytes secret key | Imagine two parties i.e. sender and receiver. Both of them generate PKE keypair & publish their public keys to each other.
Encrypt | `pke::` | `encrypt<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key, 32 -bytes plain text ( to be encrypted ) & 32 -bytes random coin ( used as seed of randomness ) | (k * du * 32 + dv * 32) -bytes encrypted text | Message sender takes receiver's public key and encrypts a 32 -bytes message. Finally it sends cipher text to the receiver.
Decrypt | `pke::` | `decrypt<k, du, dv>(...)` | (k * 12 * 32) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | 32 -bytes plain text | Receiver takes their secret key and cipher text ( received from sender ) to recover 32 -bytes plain text message, that was encrypted by the sender.

### Kyber Key Encapsulation Mechanism (KEM)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
KEM KeyGen | `kem::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 24 * 32 + 96) -bytes secret key | Imagine two parties peer1 & peer2, want to securely ( using symmetric key encryption i.e. AES ) communicate over insecure channel. Both of them generate their KEM keypair and publish their public keys to each other.
Encapsulation | `kem::` | `encapsulate<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key | (k * du * 32 + dv * 32) -bytes cipher text and a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer1 wants to establish a secure connection with Peer2 & both of them have already agreed to use AES with 256 -bit symmetric keys. Peer1 encapsulates 32 -bytes random seed inside cipher text, using Peer2's public key, which it shares with Peer2, over insecure channel. Peer1 derives a 256 -bit key from KDF, that it obtained at end of encapsulation.
Decapsulation | `kem::` | `decapsulate<k, η1, η2, du, dv>(...)` | (k * 24 * 32 + 96) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer2 uses its secret key to decapsulate cipher text ( received from Peer1 ), recovering 32 -bytes random seed, which it uses for seeding a KDF, deriving a 256 -bit key. Now both the parties have same 256 -bit key, which they can use with AES to encrypt/ decrypt ( much more efficiently ) their messages.

See example [program](./example/kyber512_kem.cpp), where I show how to use Kyber512 KEM API. You can almost similarly use Kyber768 or Kyber1024 KEM API, by just importing correct header file and using KEM functions/ constants from proper respective namespace.

```bash
$ g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include -I ./subtle/include/ example/kyber512_kem.cpp && ./a.out 
Kyber512 KEM

pubkey : baab4973d53ba839ab7ec568d2a23b546147a03abedd38ce53f83e0cf87bf8c7b722d939c694c9c1bac8b01ab53c615851e63b2bd88e66809866f539c3c88573f497b4d580338ac174721ccc466d91f131604b690e759297e60636aaa3366b73138c54d4aa40f43651bffb024ef81676b3755e6ba351c776fa526d645c10ab6612f12c86dae254893468f111c6f409b626104314222bed129992f8846e0698e05610185a96ab80aadc964fa6ba14a8540221562f0a0099bd349dbfb905ed36962654929aa7170cf73dfe397ffef1258da4c97668669f188b465841c9614f3425cf7d3b99ba03073a5b66a5580df075034290cf5100c5f5310983ac413e7c8441e27d6676c9b0e069203a54c7a7274738cc2445319a8627a76a69ee33a84f602feb031671d138ad76551fc17b011acf5ea415119042167a089bd4a8bac90c2179184de61c71eaa7be5324e7810d49c90389b3090acc7d02f2189eac2a3f9996baba4641a96cdf024029098d4bd01417745399b31b1a4a9355794c94f5533feb21d1fb3dfe132d8985651d351bcb63a4c3bc785cd6cb31893abc7cafc940c62de7afcb71bbbe714284b7619281cf355b5771d47d15996694f630182413493b92cf4538f197a41781b2d43614d3b0b5dde08c1508083d39c58068cd52bbbe6761a0b61c886e68b6f2f4632e069620b6ae4fe48613da399b505242fc6ce9a7b67e7727fd511452c93b56877d062a551477ad385171c6889ddea0383cc53b0314bf063bc8fe8000d7d6675d372cfbd83fb1d00d6e67bef530313eb8a4e111511359688ff36bcee03ee3a1352ab313c1aacc1b068e250a39498015caf82829358c59c6aaf854b2ca87a162496ee8918e0ed696aa26587c88c5dcc9ac49fbc037a354fdbbca3d3947e72a7977d07d45a411faa784d6873937a3ac208a6429f272f14a5c1019a073e521dc35555d0052ef151a37e5230f2556dd30a55f6c485b83155299481ae17faf970ae38655a5f26bcbb7744c6c7c318a87076535968bb1895a0080052788684d8774cb84b52a3fc7307753c5b11a1c9c5a1383e649218a08e096519f4ad9d16091a795adf7f7a171150a957470b0e1c04f118ce7e6fad5f98b9763
seckey : 097318acb55488097ffd38768e01105bbc8972452fcbb30d10e2026e8741d851817f895988db0fa361476a47cf871559dc69808aa846ecb54d2f0a1f91f647615a5ba67a847180943a10b5294cb2158c97a57b1f5fe413485c25e232157ab82b6cd434e1fc09d698c6c5a79414d105106c7bcd22a011218274277b7785968ed919612b7a5888134d48ba262a4d63aa3280614793b85ce5899cd1a821f4352707b15c0c507c7d12a67d3b3cbff055cf54731eb2aae85a2fa86507ce931f01b3203814599f1200707b56f4f45feeb1c2eee51683e44ad28882acf0a8194274c51421cfc457d1c502227799c1124485d5766198ab2c09127ed88c41f1c92a51a0fe4451f6d13665c92efcd8953f3bbe6cc81cad0708f3c61cb1374a19f65706e0c448f9a897953ec55bade36825634ab83a4205dc13bb8350c21a5c3c9d408ef2504f00b4bd02d626e9c70043017f064762cc628972b5bd22fc8621d871d2123676b573c76957584c64e229c892965247f540a3b180077bbcfb3652d5aca39c581f4a62c5ac2b01d70136a4a6353355328a96215427cff65a0f8339cdd2ec1a2c693117ab1011056214ab8736f0aa823643bd4993b7c57352a4b0d6243f962c1a8be9965f0381dbf7723a528308a52c609708d97b8589a6965eca0a4f5ca8575879265280fbf9a768fa1755b45385e0341dcc0b0ff41fa7316413a30f8e047a8755213d940264d7bc1581338d172c06708f76a72ad5d54522571ee758383b523073c6c762f320cb0825cc9579b67463d534a87ff8ca00c9362c18a4ff677cde9b6436ea2cc43b0ebf8994ad6245fd4705565040b7d95b8ef80d3b50c2ab030ef4e30c58437609390fcd15a80b113575e70882747366123412a42c53d60f7873b5d94c2ad9c1ad88609ee050384e6a59ef30c098ba015f09b8b7f87068a9a96430a392840957749f67205454468994ab31a867188c8711fda3a728085d7996194628119a6501b5f87bcc7b974036b3764526cc2b068d8a9ecd30cdb9b2bcac922541a2a786f85791b7a9a9297d0cd6c36e631cf0bb0d33641edfaa8b699368f0a465baab4973d53ba839ab7ec568d2a23b546147a03abedd38ce53f83e0cf87bf8c7b722d939c694c9c1bac8b01ab53c615851e63b2bd88e66809866f539c3c88573f497b4d580338ac174721ccc466d91f131604b690e759297e60636aaa3366b73138c54d4aa40f43651bffb024ef81676b3755e6ba351c776fa526d645c10ab6612f12c86dae254893468f111c6f409b626104314222bed129992f8846e0698e05610185a96ab80aadc964fa6ba14a8540221562f0a0099bd349dbfb905ed36962654929aa7170cf73dfe397ffef1258da4c97668669f188b465841c9614f3425cf7d3b99ba03073a5b66a5580df075034290cf5100c5f5310983ac413e7c8441e27d6676c9b0e069203a54c7a7274738cc2445319a8627a76a69ee33a84f602feb031671d138ad76551fc17b011acf5ea415119042167a089bd4a8bac90c2179184de61c71eaa7be5324e7810d49c90389b3090acc7d02f2189eac2a3f9996baba4641a96cdf024029098d4bd01417745399b31b1a4a9355794c94f5533feb21d1fb3dfe132d8985651d351bcb63a4c3bc785cd6cb31893abc7cafc940c62de7afcb71bbbe714284b7619281cf355b5771d47d15996694f630182413493b92cf4538f197a41781b2d43614d3b0b5dde08c1508083d39c58068cd52bbbe6761a0b61c886e68b6f2f4632e069620b6ae4fe48613da399b505242fc6ce9a7b67e7727fd511452c93b56877d062a551477ad385171c6889ddea0383cc53b0314bf063bc8fe8000d7d6675d372cfbd83fb1d00d6e67bef530313eb8a4e111511359688ff36bcee03ee3a1352ab313c1aacc1b068e250a39498015caf82829358c59c6aaf854b2ca87a162496ee8918e0ed696aa26587c88c5dcc9ac49fbc037a354fdbbca3d3947e72a7977d07d45a411faa784d6873937a3ac208a6429f272f14a5c1019a073e521dc35555d0052ef151a37e5230f2556dd30a55f6c485b83155299481ae17faf970ae38655a5f26bcbb7744c6c7c318a87076535968bb1895a0080052788684d8774cb84b52a3fc7307753c5b11a1c9c5a1383e649218a08e096519f4ad9d16091a795adf7f7a171150a957470b0e1c04f118ce7e6fad5f98b9763063fd79c6d0535c4be3ce7b52645bb09507470b20b62aafbfb7f72db4266c4e5c9f614f83a50008fe3985128dda4839534d6a04b6337a7baed6a7ab8a47700d9
cipher : a72e28e64fd2d80d91c8ed5f412bab5bf2d8c6aba0fccd502ba33f0883d80de7844ad81349500ef6fe6cdbdca1fb137a34704b8def89199439def4c21704c7927c9666cbf34de96db2af340962a50963f038ba7354799d826025371643c89c412412196d37d17ee9d25a0498253dfe4b5c5a90356f066dc912987bb96786906d535e53e37c6f085747cedd8eea0100d7b57bded553c84129555a1a8ea96c615d85dc1527b51a613f40a1f1f59dfdeb38af998019e408f11f4e10af76f0ccf80b808518ce810cc06c3132425e630ea0a00623d95e619598054655bed6b5c82571e42de56e77400fa308cc9e0fa793cb1e0165eccc30c1f42505755c564e06c70f8f36ef54fb8e9882736b001a85ed265ea2daa8e7b482338ec2efad860a832d5c7541d30343e8084a84181a98b6e51190a1be3efa3b41d06563eea5a3aed58af8cd6c4ae1b4fd739d3cd5d44c4c05685eeba246a1833468deb61787c4f5480d2a85e7c50f9bd3088592452f33b77c8cf7bac678f9f63a0cb148825ad1e6811e6f5236431842195c203d4a7c9702d5d1e7c3fd11caa1aa1d0f4c154c28fc8f88acb9bc1ad74c0acee0c655153d8c2532f975cc86a5e21c7c7df95c0720750a786a584fa593110779b8a426bd7b304c7f724c5739d61aefe718f7286b303a1f62fe302905bb257c6df328d7d762ab0c811de729fe482b9b8e9ad2e063731902871cdaa88f1270d98c4e024ac94f1a3bf9b803b7b3176f67286474d4423c2c48c1d24519f8d460ee316582137a39a37b5f3b816b4c5324b140ddac4726a03cfc42ac2d5ad180fcfe5cf0e448c9dc7d20eb97e113c1f3efb24c3681a087965b256085520dfe58aa8950d9679cd76d7f672136f2db25d20bc6371ad0f686a431ed32a8e6d4991035fb044028bb8e42561d78629ad391e029845e2752f0da32081301f85353dde5626f04c88740f216fe9ebd4df96969554c2760f288be3b08cd1e7e91c771557d00f015e28541c3754fba9e34c624ec3ee8de9805ed053120343f5a2e5963b88f491be95b5b834c510dc6caae0ee43ff8742cf1d80149075154c5781a
shared secret 0 : 6015c21a27bb6dd6a10f0a73edd652ee62950020256441b5e12695cd48b6c498
shared secret 1 : 6015c21a27bb6dd6a10f0a73edd652ee62950020256441b5e12695cd48b6c498
```

---

## Important Implementation Note

You'll notice Kyber KEM API accepts 32 -bytes seeds ( `d`, `z` for key generation and `m` for encapsulation ) for all three concrete instantiations ( i.e. Kyber{512, 768, 1024} ) - this is what you, as an user of Kyber KEM API, need to ensure that you call those routines with uniformly random sampled seeds.

I provide you with a PRNG implementation, which lives in [include/prng.hpp](./include/prng.hpp). Before you start using that, I want you to take a moment and understand what can be the implication of using the default constructor of `prng::prng_t`. 

- In case default constructor is used, `std::random_device` is requested for 32 random bytes ( in form of eight `uint32_t`s ), which is hashed using SHAKE256 XOF. When you request ( using `read()` function ) arbitrary many random bytes from that initialized PRNG, it's actually squeezed out from SHAKE256 XOF state. Now one thing to note here is `std::random_device` itself is not guaranteed to provide you with system randomness in all possible usecases/ targets. It's an implementation defined behaviour. So it's better to be careful. Read https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device 's notes section. 
- But there's another way of using `prng::prng_t` - you can use its explicit constructor for creating a PRNG by hashing N -many random bytes, supplied as input. These N bytes input can be presampled from any secure randomness source that you may have access to. After that same underlying SHAKE256 XOF is used for squeezing arbitrary many bytes arbitrary many times from PRNG.

```cpp
#include "prng.hpp"

// Prefer N to be >= 32
constexpr size_t slen = 32; // = N bytes
uint8_t seed[slen];

// fill `seed` with N many random bytes

// default initialization ( recommended only if you're sure that target system provides you with reliable randomness source when accessing `std::random_device` )
prng::prng_t prng0;
// explicit initialization ( safer alternative )
prng::prng_t prng1{seed, slen};
```
