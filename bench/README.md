## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note** Benchmarking expects presence of `google-benchmark` header and library in global namespace ( so that it can be found by the compiler ).

> **Warning** When benchmarking ensure that you've disabled CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-02-27T13:09:55+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.76, 1.95, 1.91
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         18.2 us         18.2 us        38533
kyber512_pke_encrypt        20.0 us         20.0 us        34027
kyber512_pke_decrypt        5.80 us         5.77 us       117661
kyber768_pke_keygen         31.2 us         31.2 us        22671
kyber768_pke_encrypt        33.6 us         33.6 us        20787
kyber768_pke_decrypt        7.67 us         7.66 us        88869
kyber1024_pke_keygen        48.8 us         48.7 us        13735
kyber1024_pke_encrypt       50.9 us         50.8 us        13175
kyber1024_pke_decrypt       10.1 us         10.1 us        64465
kyber512_kem_keygen         20.3 us         20.2 us        34569
kyber512_kem_encap          25.1 us         25.1 us        27921
kyber512_kem_decap          28.5 us         28.4 us        24889
kyber768_kem_keygen         34.2 us         34.2 us        20850
kyber768_kem_encap          40.5 us         40.4 us        17326
kyber768_kem_decap          45.4 us         45.3 us        15534
kyber1024_kem_keygen        53.4 us         53.4 us        12770
kyber1024_kem_encap         60.2 us         60.1 us        10952
kyber1024_kem_decap         66.1 us         66.0 us        10224
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-02-27T09:13:34+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.27, 0.19, 0.10
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         35.0 us         35.0 us        20045
kyber512_pke_encrypt        44.6 us         44.6 us        15677
kyber512_pke_decrypt        13.9 us         13.9 us        50370
kyber768_pke_keygen         59.1 us         59.1 us        11830
kyber768_pke_encrypt        73.0 us         73.0 us         9577
kyber768_pke_decrypt        18.4 us         18.4 us        37957
kyber1024_pke_keygen        92.8 us         92.8 us         7554
kyber1024_pke_encrypt        110 us          110 us         6340
kyber1024_pke_decrypt       22.6 us         22.6 us        30997
kyber512_kem_keygen         38.7 us         38.7 us        18054
kyber512_kem_encap          55.0 us         55.0 us        12745
kyber512_kem_decap          65.0 us         65.0 us        10775
kyber768_kem_keygen         64.6 us         64.6 us        10849
kyber768_kem_encap          86.7 us         86.7 us         8099
kyber768_kem_decap          99.7 us         99.7 us         7024
kyber1024_kem_keygen         101 us          101 us         6962
kyber1024_kem_encap          128 us          128 us         5447
kyber1024_kem_decap          144 us          144 us         4874
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-02-27T09:14:34+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.40, 0.24, 0.12
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         29.5 us         29.5 us        23719
kyber512_pke_encrypt        31.7 us         31.7 us        22010
kyber512_pke_decrypt        9.77 us         9.77 us        71689
kyber768_pke_keygen         49.2 us         49.2 us        14226
kyber768_pke_encrypt        53.1 us         53.1 us        13149
kyber768_pke_decrypt        13.3 us         13.3 us        52825
kyber1024_pke_keygen        77.5 us         77.5 us         9023
kyber1024_pke_encrypt       82.3 us         82.3 us         8557
kyber1024_pke_decrypt       16.6 us         16.6 us        42071
kyber512_kem_keygen         32.7 us         32.7 us        21392
kyber512_kem_encap          39.7 us         39.7 us        17620
kyber512_kem_decap          45.9 us         45.9 us        15237
kyber768_kem_keygen         54.0 us         54.0 us        12974
kyber768_kem_encap          65.0 us         65.0 us        10873
kyber768_kem_decap          72.6 us         72.5 us         9656
kyber1024_kem_keygen        83.9 us         83.9 us         8346
kyber1024_kem_encap         96.2 us         96.2 us         7276
kyber1024_kem_decap          107 us          107 us         6565
```
