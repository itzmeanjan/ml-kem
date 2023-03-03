> **Warning** **This Kyber implementation is conformant with Kyber [specification](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf) and I also *try* to make it constant-time but be informed that it is not yet audited. If you consider using it in production, be careful !**

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

While with **IND-CCA2-secure Kyber KEM**, two parties interested in secretly communicating over public & insecure channel, can generate a shared secret key ( of arbitrary byte length ) from a key derivation function ( i.e. KDF which is SHAKE256 XOF in this context ) which is obtained by both of these parties as result of seeding SHAKE256 XOF with same secret. This secret is 32 -bytes and that's what is communicated by sender to receiver using underlying Kyber PKE.

Algorithm | Input | Output
--- | :-: | --:
KEM KeyGen | - | Public Key and Secret Key
Encapsulation | Public Key | Cipher Text and SHAKE256 KDF
Decapsulation | Secret Key and Cipher Text | SHAKE256 KDF

> IND-CPA-secure Kyber PKE can be used for asynchronous secure communication such as email.

> IND-CCA2-secure Kyber KEM can be used for synchronous secure communication such as TLS.

Here I'm developing & maintaining `kyber` - a zero-dependency, header-only and easy-to-use C++ library implementing Kyber PKE and KEM, supporting Kyber-{512, 768, 1024} parameter sets, as defined in table 1 of Kyber specification. 

Only dependency is `sha3` and `subtle` - both of them are zero-dependency, header-only C++ libraries themselves. I decided to write `sha3` so that I can modularize a fairly common PQC dependency because SHA3 hash functions and extendable output functions are pretty common symmetric key primitives used in post-quantum cryptographic constructions such as Kyber, Dilithium, Falcon and SPHINCS+ etc.. While `subtle` is a pretty light-weight library which helps achieving constant-timeness in cryptographic libraries. Here it's used for performing constant-time byte comparison and conditional selection without using booleans - only relying on integer addition, subtration and bit-wise operations.

- Both `sha3` and `subtle` are pinned to specific commit, using git submodule.
- See [usage](#usage) section below for git submodule set up guide.

> **Note**

> Find Kyber specification [here](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf). I suggest you go through the specification to get an in-depth understanding of Kyber PQC suite.

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

# Download Python dependencies
$ python3 -m pip install -r wrapper/python/requirements.txt --user
```

- For benchmarking Kyber implementation on CPU systems, you'll need to have `google-benchmark` header and library globally installed. You may want to follow [this](https://github.com/google/benchmark/tree/3b19d722#installation) guide.

- For importing `sha3` and `subtle`, initialize & update git submodule after cloning this repository

```bash
git clone https://github.com/itzmeanjan/kyber.git
cd kyber
git submodule update --init

# Now you can {test, benchmark, use} `kyber`
```

## Testing

For testing functional correctness and compatibility ( with Kyber specification and reference implementation ) of this Kyber implementation ( along with its component units ), you have to issue

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

Find micro-benchmarking ( using `google-benchmark` ) results [here](./bench).

## Usage

`kyber` is written to be a zero-dependency, header-only C++ library such that it's pretty easy-to-use in your project. All you need to do

- Clone `kyber` repository

```bash
cd
git clone https://github.com/itzmeanjan/kyber.git
```

- Initialize and update git submodule, so that `sha3` and `subtle` are available inside `kyber` source tree

```bash
cd kyber
git submodule update --init
```

- Write your program while including [kyber.hpp](./include/kyber.hpp), which includes declarations ( and definitions ) of all required routines, spread across various namespaces

```cpp
// main.cpp

#include "kyber512_kem.hpp"
#include <cassert>

int main() {
    uint8_t pkey[kyber512_kem::pub_key_len()];
    uint8_t skey[kyber512_kem::sec_key_len()];
    uint8_t cipher[kyber512_kem::cipher_text_len()];

    // Be careful !
    //
    // Read API documentation in include/prng.hpp
    prng::prng_t prng;

    kyber512_kem::keygen(prng, pkey, skey);
    auto skdf = kyber512_kem::encapsulate(prng, pkey, cipher);
    auto rkdf = kyber512_kem::decapsulate(skey, cipher);

    uint8_t sender_key[32];
    skdf.read(sender_key, sizeof(sender_key));

    uint8_t receiver_key[32];
    rkdf.read(receiver_key, sizeof(receiver_key));

    for (size_t i = 0; i < sizeof(sender_key); i++) {
        assert(sender_key[i] == receiver_key[i]);
    }

    return 0;
}
```

> **Note** You may only want to use Kyber PKE or KEM, then you can simply include either of [kyber_pke.hpp](./include/kyber_pke.hpp) or [kyber_kem.hpp](./include/kyber_kem.hpp). If you want parameter instantiated PKE or KEM routines, consider looking at `kyber{512, 768, 1024}_{pke, kem}.hpp`.

- When compiling your program, let your compiler know where it can find `kyber` and `sha3` headers, which includes their definitions too

```bash
# Assuming `kyber` was cloned just under $HOME

KYBER_HEADERS=~/kyber/include
SHA3_HEADERS=~/kyber/sha3/include

g++ -std=c++20 -Wall -O3 -march=native -I $KYBER_HEADERS -I $SHA3_HEADERS main.cpp
```

Kyber | Namespace | Header
--- | :-: | --:
IND-CPA-secure Public Key Encryption | `kyber_pke::` | [kyber_pke.hpp](./include/kyber_pke.hpp)
IND-CCA-secure Key Encapsulation Mechanism | `kyber_kem::` | [kyber_kem.hpp](./include/kyber_kem.hpp)
Utility Routine | `kyber_utils::` | -

> **Note** Select Kyber parameter set from table 1 of Kyber [specification](https://pq-crystals.org/kyber/data/kyber-specification-round3-20210804.pdf)

---

### Kyber Public Key Encryption (PKE)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
PKE KeyGen | `kyber_pke::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 12 * 32) -bytes secret key | Imagine two parties i.e. sender and receiver. Both of them generate PKE keypair & publish their public keys to each other.
Encrypt | `kyber_pke::` | `encrypt<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key, 32 -bytes plain text ( to be encrypted ) & 32 -bytes random coin ( used as seed of randomness ) | (k * du * 32 + dv * 32) -bytes encrypted text | Message sender takes receiver's public key and encrypts a 32 -bytes message. Finally it sends cipher text to the receiver.
Decrypt | `kyber_pke::` | `decrypt<k, du, dv>(...)` | (k * 12 * 32) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | 32 -bytes plain text | Receiver takes their secret key and cipher text ( received from sender ) to recover 32 -bytes plain text message, that was encrypted by the sender.

> **Note** In [pke.cpp](./example/kyber512_pke.cpp), I show how to use Kyber PKE API, while sticking to Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include example/kyber512_pke.cpp && ./a.out

pubkey : fac9337d888430016152ba06cc41c5a92710ed9787f6caa710d7c57c879d3de018c50bbffc14a92e233c6b3acddae9ca9543cfb5f16aa4209a65269a98a4820ad0a26579c71d1ca52cd65446c92835cb0f30b65c50a89048d57a6ce9a45818b3acd9a078ba9b2b5421fc02084a1b9d884563fad88c66cb7c25171aa4b5051d0b567240065f4841cf5904f73474a498932ab858acb108d486c62f9c2b8d6a3c6880353b15b4a9e91697081c6e551966db191fb284a7e5a8e985cf46b941811b8f8d7c574470b3e90b7530f7c8c8a58b1c2bb3029c2fc99168544375b102627b860d1ba8803f594a9a2704ed286559814061c2729bf1ba562827e2b5af3874acf0451eccca42f420985f07bfd2634883c3c44dc91f4894aca33387a205abd7f84219e47bfc556fb368c7b8fc62aeb9243dc11b0e647b37d19b25ec807bb243430182a2994107c157b539b5c446bc3ce08daf9697561baa6837adbc121a6e0643f50317be733fc76624a0e530f8f42e2835bc7d5a166768309fe82291f9a6fe4679f213052355a9d669277e715af3e14d830859bac0034b6247009d57f50bc5b023bfc40918d1c6ba8db016d70b2aa181324d624f38f5a0fa81aa96338bf71c1696aa0bfff3651eb217cdacb5d5e1147fd6287ae83ab5bc62f150a7dc21cf7af65003d4467028b7929078fdb73659300a8feb9e5a126e0d33c780b7497c151f91b11d0294a9d67540b7750de313cf3ef032e120cab7f5b9802164b4d136c41b56686aa182e8bdb0728e1033734685246568858e9629e8f4068d037303b144163abe1780a09c94168d501bf6a61d2f88215ea86f860b8353b9338ae8311428bef17c6cf0581eeab7b96bd00fd123cf5a3445ffaa0413d05f01202061840db89c2f4dbb74264c8da370ba21dc3baac4381d7895c8bc2e7512ab8890051a306908c67433ca44d2f9ac14320e48251116fcb1c9f343349cc4077cceb395118189204319891ef85df03a68f8d22030880396e7cc3d42473bcbaab6641d7af725f3d3940af46a068ba7d62bc125e9723d60bf4f6133ec068544051a8f697515240fda2418c5419aa22e1f25903eec7541d8de09ed43fe6560fbde5aa0fc0bd3de8fd6f180
seckey : a08379055a37664c6b3d58cfcfb6af6cdac956c570e4e68a0a4b9eec7bcb05a19f9663353d97a42f4503aba5c224c95e924691b2b957160cbd7d0b2c98602b2858036aa8664a7718a363c8988b2ca23a8bd32c8f1c0ab8901555463c3307959d75a66e5671c2398c6e740cc636da984e801044727e3fd51964e85c77f45fd759696577570bac75d13156daf64b42d338db59a10bb29935c0c45374019c09c17521a95784592f3733e3c576a816195a59bc03d66f844b60f7aab6ea5cb168a3136a3c487699c294053e8b7813960b6503d37c33b2169bc2067a9035ee941cfbeb5a4d1a1dbc293089c1265082afcea0600734b9392473c15c94656b93750766cb59809e210086aaa0658407c0ec61421bc333811914e779bc580a1014ac433c53f0c157bba765e7f8555e0c0241350fc07c9b4f435665925664695a36eaab568a71ea101fdbb6902fc79fcf0a688e8a58544753aa201ec68ab5862b4c24185ef3869d0ed519e81121d00b4dc4c762b3a90971f174aae8c7f05267e3db6690c99ef7c67e362b89f0a13721e66c7e855adbe21cb1e32c1fb53e0f71746f30335cd9a1c569609b6a5306ba87fff8bed720c3c34092df5c4f0301a85ff9cdcca103e018c5629bc46349427fb2cadd86c4d5f5adcdb715c9864e0446bfd2c6232e2858a5892a12aa58668b13beb924e079c474d12f2f6556ec2c24f4640122c37ea0f97aa862a5e5541ef6dac4a6486383bc7729775102c89541d67a35499909c91237fc052652780d5a25e2c84d7d509f17c0092a37cf7edbcded108935597122243efea541f9cb020a4b329d7b653a27ce77926f2918651f66b55c6bae71f46410ea702f571d09401e234cb71c2c627c119bcdb96accda98cd461868a33f9b0ccc8464b469369cc6e033b316417bab47f43915a42a00f61012f032c2fa6a78f3ab1cb62122a7b171885a7c651c7f43c5b8fb6c5807f040d2749f74301767899b62ac0e98ba608045bd4cc0a3dd648452cab2153acee6a22effc73aca36a6cac8008d2cb4f19c71d82b1b1f411ff94c577d1b1429e8a451b48c71288c6a55724cd1c3
plain text : c327a0162e13cd9a37212f9a2fb39784a822d7d31cb657aa06d86c4e6124775f
cipher     : c53d24b3843938e8570338e61e47a751b9944e7fd07cf77d022612987f271cca62cbad4fe751d322b550178b5e9887ccc854673f34bd2d7b566455a022c02d75c6e99d8376dc39d2b611a000c8453e89192277d61027a6b2aab2f5f00ce3bab84edb2596389a39bc9e6aa822e583cd26bf4548cc818f57e87cbaacc0f08da2a88e8beed151e9d6bbadf69d60d28da9af907a1a93f25189a71b8e56871b128897d7c7771f0eabd11fb0179f60a7579ebd4c14286cb8ec75574cf697f13f0df131bb91125e3c0fb720e1ee3771fc6ef3e4faed52bb7f1b9bae1a11036227b7fd954b27609c7224333e4025eb02aab1b562aa0dfe62f27ddfd81508ae424870ac6454ea71738405888541a453c8cc72b245e406a90984232e9cd6f92791e9029f36667ca53ed639e374eb20269985d0e89710182b68f900e862f78f07cc281f18cc40afc42050a6b468ed2f61c0c8a3bf09642bcc6c95492c5c418038e688ccef2cc8fbd23e2b415cd42c3ce4f721da28cb15a1cbabf3b6ef24bda94ccefdafd7126dcf3733dd75208afb460dd2e1a59eb41720eda04e99e0ca69e0c2132d89ef830fec8687aeebaeb2f5192210bf19404fa3dbf79c37b5e7874ce69156a251d94d2f11e3950f2f9de449be2757702a400cb46ecbd341876f0159089f00a4857a0180938619a3f01c01a965e65dc9fbd258a2d3be374c30753b8d8c29ec07e043510527a04c6c126cd01e0e33d3b15f953230b5d4612ec8a4e7eb5290ec7b7020278981a624ef6f479774361f9bc991571ec00721549cfa5bbf240f9ede390af70550a677dedbe40a5e5ace670399363f7837520d2a6e6d078223a204510ddf03fd7cb8094cddc8937c5021d3ff2dd5440acd24582f0fc2df145338536bbca3dbf2a357ba02d13ff53e8fe158aff2fb45381e169267efbde7ee40ca5d055b183621a2a612e67e718b8f5911125af0cc52e60304c4d9357811cc7aa1ecc93a6adb4b76658498868c8d64f001cd900627d1081b4766ac21c59c0720a977fc6deafe70a07a0ce68272c663a1fc4c1c7aaa757cd2c8ad1b17f6ff29b13da4e9656d8804
decrypted  : c327a0162e13cd9a37212f9a2fb39784a822d7d31cb657aa06d86c4e6124775f
```

### Kyber Key Encapsulation Mechanism (KEM)

Step | Namespace | Routine | Input | Output | How is it used ?
--- | --- | :-: | --: | --: | --:
KEM KeyGen | `kyber_kem::` | `keygen<k, η1>()` | - | (k * 12 * 32 + 32) -bytes public key & (k * 24 * 32 + 96) -bytes secret key | Imagine two parties peer1 & peer2, want to securely ( using symmetric key encryption i.e. AES ) communicate over insecure channel. Both of them generate their KEM keypair and publish their public keys to each other.
Encapsulation | `kyber_kem::` | `encapsulate<k, η1, η2, du, dv>(...)` | (k * 12 * 32 + 32) -bytes public key | (k * du * 32 + dv * 32) -bytes cipher text and a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer1 wants to establish a secure connection with Peer2 & both of them have already agreed to use AES with 256 -bit symmetric keys. Peer1 encapsulates 32 -bytes random seed inside cipher text, using Peer2's public key, which it shares with Peer2, over insecure channel. Peer1 derives a 256 -bit key from KDF, that it obtained at end of encapsulation.
Decapsulation | `kyber_kem::` | `decapsulate<k, η1, η2, du, dv>(...)` | (k * 24 * 32 + 96) -bytes secret key & (k * du * 32 + dv * 32) -bytes cipher text | a KDF ( i.e. SHAKE256 XOF object ) which can be used for deriving shared secret key ( of arbitrary length ) | Peer2 uses its secret key to decapsulate cipher text ( received from Peer1 ), recovering 32 -bytes random seed, which it uses for seeding a KDF, deriving a 256 -bit key. Now both the parties have same 256 -bit key, which they can use with AES to encrypt/ decrypt ( much more efficiently ) their messages.

> **Note** In [kem.cpp](./example/kyber512_kem.cpp), I keep an example demonstrating usage of Kyber KEM API, while using Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -march=native -I ./include -I ./sha3/include example/kyber512_kem.cpp && ./a.out

pubkey : 45580b5ecb5c78eb335992afc2ea9c0fb192919269d8068e9ae855bc705d26f3c4257b8db267c1ac381d480442aca3767f7c8cefbb6b28818aec4118f98906769728c7b74ce4aaa8fce11d8503579867a026e0bfc05945171a9c4157664056562e978a86f7a6e5f45b621689ea27bc92545c06a6b44ae163abf5039d207e060017e548acce256c40fbb6a9b9073cc4630e1823e658333c83aa9f661b6bc29f4adcab5691adba747d23a1c8a5e7bf973c2a600a4aeeab4ad793b2ade8c2fc8225c8a8634ba31c0cb896e3305a7e3483374609b8254427b6cbb9ac26fc8439b5632d7134921e178196940b482bbc1d9520e7bb94ab2ab53b7c97c1639146062fbcd634c57060d6088baea4884ad839907a6a7edb3b26d0b3ed3a2f512b5b3f38bb0a0a47b75cb619ca471f79af13c0a869863c11d48e9a9a02b0998c24a490c4373a6d95346a7712e91a5dc7271fa8a355eed756bed5a0b0f13fafe3c45115918216ceca15085471a7857055156753cd38c4cfe4208f241325357b228904fdba923f228f6f37b50f04027c65831c9179fa03120e01b00f321335c05d89f952fdca8c78f61ce0f474a7e552c26b133504187b947412656a31e7b8324b4b50b5c710cc5fea9bc032518a70a2b7ae964e1f62a4e55086c6964dd2e178558144d6d2bbee6725e0e77d7e5a81f8ba0657aa6d92339b4b68bcbb558f3d1a4415b3c993e86c2275600d877c2480480b687f20107f40e16506941fd7dc52e975b765a19a9d77b8bca7b1b39575688a879af75fe0a1296b158a6db7aeb3619271f1488376058107970a31705e0a949c5169361a0122861610d75f7cd3cd85870f64e2649a7b550a61a009950b0ed4496449c6e16c5bdf7bc10f07ac3182c98e901cffdc686b2b646e286820194ed4b559e17210f54268996615dc3cc9f89840d2bc2e5c134fcb733f7e99b596672d3326392e506967ea9db231b15ce777a1ec7bf36a32ce549f7f619d75595f09db42bdf8ccf4aba2a2c226993c0b29a4bb3801805db193cab268b9c6cb83e3a18ad1644ba87e494710c08a18589856d2f761796210eb54c1a4c9e3f19a6fc00e9b49384bb8fb38aee60a7fa63a3ba77cc1d9d23da107b081
seckey : 83d308ba12ce27a42abb331cd739793a50c326875853112ccdaa05765a8b6c254d22d3a2836c1feef4014a5167224a2a181c089195ad90e89489154e82a8101a2a21e43393fecaa8d0524dcdc2c4b59a06fc27bee61b131e811ce99161d6c183af50411a328a621337ffe1a227b0bbe624123d517587d63aebba776bc823b38a8c785c0fdb651fd8c297a6bb040c8417bd94003ff79b4e293cd69958aae07e1ab81a15bb3f60347c596913baa6b68f5c253c2908c101596a4c43b4e93480209471612b9bc55d7eba3e04ea507dac13316892403a5270a523e7778e8051187ff66b0cd98910d93215627847dc7b51092559b018e8d0a353369eab13bf8c2b30adb87deb32b5907586eca68ed92350b9e130a37acf99ac00ad2528792663a253492182759ad259fc058c9df031ce091d39c36c3d0b9dd3834ab8c8483a92678e378bc8e655eae48fec25976359241aa70189e375c9fc1989cabe1fa924967425c8925aec6a0c72ca9f81b22c314378b726c2c4ea410212a7cea88cd87a907f41989e570e040092eb6a3aea5c4a4dc8b5fcba5e36e80f810ab9c26cba25b116c5e7bce3c54729f913d45b746d9b7caaab034bdc3b9b438764269f39f8cb1b163de0c7a9b28b4de93cbabaf73dcafa61b2cc0a417c4494d431ed2b99e5b16be6b83867410773b4765559c84071297e6549d9136687c4897e33b6e6a5677fb380d5d14a60677293560fe723063fb81ef2d66ff83a17351b9c0bab1839744d34799f97094b2aa955a58892286609bcd7a07eb766d5631dab38825ef53643236251758a30306d12b032be8cc530cb911e5a21760bc838214655150737eac4e130aaed52a78032a5e7625edfa991527aa69695cde2c69ea4d99a2006b0f24b215285a8e8b73556ccaacdb66cc27a7d2e5b1af849554a3bb30af91dc48230be25722266bba250b0069b8ac0d4a489ecbfbb724012b6183074a93748a81dd840dc5952bbf683d93634c6b3bddd27c3d6e14826ab65e2bb9c2adc01b8b515509804dc6ba8b575076460135f48bd88e865372019e78b7fc48a92b484c58c4650e1661746431a45580b5ecb5c78eb335992afc2ea9c0fb192919269d8068e9ae855bc705d26f3c4257b8db267c1ac381d480442aca3767f7c8cefbb6b28818aec4118f98906769728c7b74ce4aaa8fce11d8503579867a026e0bfc05945171a9c4157664056562e978a86f7a6e5f45b621689ea27bc92545c06a6b44ae163abf5039d207e060017e548acce256c40fbb6a9b9073cc4630e1823e658333c83aa9f661b6bc29f4adcab5691adba747d23a1c8a5e7bf973c2a600a4aeeab4ad793b2ade8c2fc8225c8a8634ba31c0cb896e3305a7e3483374609b8254427b6cbb9ac26fc8439b5632d7134921e178196940b482bbc1d9520e7bb94ab2ab53b7c97c1639146062fbcd634c57060d6088baea4884ad839907a6a7edb3b26d0b3ed3a2f512b5b3f38bb0a0a47b75cb619ca471f79af13c0a869863c11d48e9a9a02b0998c24a490c4373a6d95346a7712e91a5dc7271fa8a355eed756bed5a0b0f13fafe3c45115918216ceca15085471a7857055156753cd38c4cfe4208f241325357b228904fdba923f228f6f37b50f04027c65831c9179fa03120e01b00f321335c05d89f952fdca8c78f61ce0f474a7e552c26b133504187b947412656a31e7b8324b4b50b5c710cc5fea9bc032518a70a2b7ae964e1f62a4e55086c6964dd2e178558144d6d2bbee6725e0e77d7e5a81f8ba0657aa6d92339b4b68bcbb558f3d1a4415b3c993e86c2275600d877c2480480b687f20107f40e16506941fd7dc52e975b765a19a9d77b8bca7b1b39575688a879af75fe0a1296b158a6db7aeb3619271f1488376058107970a31705e0a949c5169361a0122861610d75f7cd3cd85870f64e2649a7b550a61a009950b0ed4496449c6e16c5bdf7bc10f07ac3182c98e901cffdc686b2b646e286820194ed4b559e17210f54268996615dc3cc9f89840d2bc2e5c134fcb733f7e99b596672d3326392e506967ea9db231b15ce777a1ec7bf36a32ce549f7f619d75595f09db42bdf8ccf4aba2a2c226993c0b29a4bb3801805db193cab268b9c6cb83e3a18ad1644ba87e494710c08a18589856d2f761796210eb54c1a4c9e3f19a6fc00e9b49384bb8fb38aee60a7fa63a3ba77cc1d9d23da107b08147f790da9f2a09c78a0e53434a5326f74eb175776a1628f6c7be3213a2a2401c48e67f6d7afe95c35fe15c4454b838c926a9ec53b9c324f0168151e6e954c546
shared secret 0 : 3364912ae34021241f4aafd8253d909db0e5a9b685de85e1252f8533382296ff
shared secret 1 : 3364912ae34021241f4aafd8253d909db0e5a9b685de85e1252f8533382296ff
```

## Important Implementation Note

You may notice that Kyber PKE implementation ( present in [kyber_pke.hpp](./include/kyber_pke.hpp) ) is a thin wrapper on top of routines ( such as keygen, encrypt & decrypt ) living under namespace `cpapke::` in files

- PKE KeyGen : [pke_keygen.hpp](./include/pke_keygen.hpp)
- Encryption : [encryption.hpp](./include/encryption.hpp)
- Decryption : [decryption.hpp](./include/decryption.hpp)

which have little different ( somewhat generic ) looking interface. They are kept that way so that it becomes easier to write test cases using Known Answer Tests ( read test vectors ), obtained from Kyber reference implementation, following steps described in this [gist](https://gist.github.com/itzmeanjan/c8f5bc9640d0f0bdd2437dfe364d7710). This compartmentalization also helps benchmarking keygen, encrypt, decrypt routines, without getting influenced by the delay of sampling of randomness using pseudo random number generator (PRNG).

Similarly, you'll notice [kyber_kem.hpp](./include/kyber_kem.hpp) follows same approach, acting as a thin wrapper on top of underlying generic Kyber KEM implementation ( living under namespace `ccakem::` ), exposing nicer APIs.

> **Note** There are also easy-to-use parameterized instances of Kyber{512, 768, 1024} PKE and KEM, kept in header files `kyber{512, 768, 1024}_{pke, kem}.hpp`.

Before you start using PRNG implementation ( see [prng.hpp](./include/prng.hpp) ), I want you to take a moment and understand what can be the implication of using the default constructor of `prng::prng_t`. In case default constructor is used, `std::random_device` is requested for 32 random bytes ( in form of eight `uint32_t`s ), which is hashed using SHAKE256 XOF. When you request ( using `read()` function ) arbitrary many random bytes from that initialized PRNG, it's actually squeezed out from SHAKE256 XOF state. Now one thing to note here is `std::random_device` itself is not guaranteed to provide you with system randomness in all possible usecases. It's an implementation defined behaviour. So it's better to be careful. Read https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device 's notes section. But there's another way of using `prng::prng_t` - you can use its explicit constructor for creating a PRNG by hashing N -many random bytes. These N random bytes can be presampled from any secure randomness source that you may have in mind. After that same underlying SHAKE256 XOF is used for squeezing arbitrary many bytes arbitrary many times from PRNG.

```cpp
#include "prng.hpp"

// Prefer N to be >= 32
constexpr size_t slen = 32; // = N bytes
uint8_t seed[slen];

// fill `seed` with N many random bytes

prng::prng_t prng0; // default initialization ( not recommended )
prng::prng_t prng1{seed, slen}; // explicit initialization ( safer alternative )
```

If you're not happy with SHAKE256 XOF backed PRNG, you can always start using the underlying API, which lives under namespace `cpapke::` or `ccakem::`. For how to use, take a look at [kyber_pke.hpp](./include/kyber_pke.hpp) or [kyber_kem.hpp](./include/kyber_kem.hpp).
