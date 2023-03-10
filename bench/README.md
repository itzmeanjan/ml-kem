## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note** Benchmarking expects presence of `google-benchmark` header and library in global namespace ( so that it can be found by the compiler ).

> **Warning** When benchmarking ensure that you've disabled CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-03-10T11:19:00+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 4.25, 5.31, 6.44
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         26.6 us         26.0 us        27124
kyber512_pke_encrypt        29.6 us         29.2 us        23871
kyber512_pke_decrypt        11.1 us         10.8 us        63981
kyber768_pke_keygen         44.8 us         44.4 us        15805
kyber768_pke_encrypt        50.0 us         48.8 us        13427
kyber768_pke_decrypt        16.3 us         14.9 us        46613
kyber1024_pke_keygen        71.1 us         70.4 us         9706
kyber1024_pke_encrypt       77.0 us         76.2 us         9184
kyber1024_pke_decrypt       19.9 us         19.5 us        36111
kyber512_kem_keygen         31.7 us         29.1 us        24879
kyber512_kem_encap          35.0 us         34.6 us        20181
kyber512_kem_decap          44.1 us         43.7 us        16037
kyber768_kem_keygen         52.4 us         48.7 us        14533
kyber768_kem_encap          57.8 us         57.0 us        12141
kyber768_kem_decap          69.3 us         68.5 us        10045
kyber1024_kem_keygen        77.4 us         75.6 us         9017
kyber1024_kem_encap         87.6 us         86.4 us         7720
kyber1024_kem_decap          110 us          103 us         6823
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-03-10T07:17:11+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.18, 0.08, 0.02
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         48.7 us         48.7 us        14378
kyber512_pke_encrypt        57.1 us         57.1 us        12246
kyber512_pke_decrypt        20.4 us         20.4 us        34229
kyber768_pke_keygen         80.4 us         80.4 us         8705
kyber768_pke_encrypt        94.5 us         94.5 us         7442
kyber768_pke_decrypt        27.7 us         27.7 us        25235
kyber1024_pke_keygen         127 us          127 us         5525
kyber1024_pke_encrypt        146 us          146 us         4782
kyber1024_pke_decrypt       36.5 us         36.5 us        19192
kyber512_kem_keygen         52.4 us         52.4 us        13364
kyber512_kem_encap          66.9 us         66.9 us        10455
kyber512_kem_decap          83.0 us         83.0 us         8420
kyber768_kem_keygen         86.6 us         86.6 us         8125
kyber768_kem_encap           108 us          108 us         6509
kyber768_kem_decap           130 us          130 us         5410
kyber1024_kem_keygen         134 us          134 us         5217
kyber1024_kem_encap          162 us          162 us         4289
kyber1024_kem_decap          191 us          191 us         3667
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-03-10T07:18:02+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.31, 0.14, 0.05
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         41.1 us         41.1 us        17040
kyber512_pke_encrypt        46.4 us         46.4 us        15096
kyber512_pke_decrypt        17.5 us         17.5 us        40032
kyber768_pke_keygen         69.1 us         69.1 us        10108
kyber768_pke_encrypt        76.8 us         76.8 us         9033
kyber768_pke_decrypt        23.7 us         23.7 us        29576
kyber1024_pke_keygen         109 us          109 us         6388
kyber1024_pke_encrypt        119 us          119 us         5833
kyber1024_pke_decrypt       31.3 us         31.3 us        22413
kyber512_kem_keygen         44.2 us         44.2 us        15847
kyber512_kem_encap          54.8 us         54.8 us        12894
kyber512_kem_decap          68.5 us         68.5 us        10220
kyber768_kem_keygen         73.8 us         73.8 us         9414
kyber768_kem_encap          87.8 us         87.8 us         7919
kyber768_kem_decap           107 us          107 us         6527
kyber1024_kem_keygen         116 us          116 us         6049
kyber1024_kem_encap          134 us          134 us         5232
kyber1024_kem_decap          158 us          158 us         4437
```
