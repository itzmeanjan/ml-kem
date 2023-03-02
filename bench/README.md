## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note** Benchmarking expects presence of `google-benchmark` header and library in global namespace ( so that it can be found by the compiler ).

> **Warning** When benchmarking ensure that you've disabled CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-03-02T16:48:19+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.38, 1.52, 1.58
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         18.3 us         18.3 us        37845
kyber512_pke_encrypt        20.1 us         20.0 us        34266
kyber512_pke_decrypt        5.69 us         5.68 us       119746
kyber768_pke_keygen         31.0 us         31.0 us        22730
kyber768_pke_encrypt        34.1 us         34.0 us        20695
kyber768_pke_decrypt        7.74 us         7.73 us        84223
kyber1024_pke_keygen        48.8 us         48.8 us        13922
kyber1024_pke_encrypt       51.7 us         51.6 us        13456
kyber1024_pke_decrypt       10.1 us         10.1 us        68305
kyber512_kem_keygen         20.3 us         20.3 us        34056
kyber512_kem_encap          25.2 us         25.2 us        27670
kyber512_kem_decap          28.5 us         28.5 us        24088
kyber768_kem_keygen         33.9 us         33.9 us        20609
kyber768_kem_encap          40.8 us         40.7 us        17166
kyber768_kem_decap          45.2 us         45.2 us        15309
kyber1024_kem_keygen        53.6 us         53.5 us        12094
kyber1024_kem_encap         60.9 us         60.8 us        11317
kyber1024_kem_decap         68.2 us         68.1 us        10102
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-03-02T12:45:32+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.08, 0.02, 0.01
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         34.9 us         34.9 us        20044
kyber512_pke_encrypt        45.4 us         45.4 us        15373
kyber512_pke_decrypt        13.9 us         13.9 us        50228
kyber768_pke_keygen         59.0 us         59.0 us        11860
kyber768_pke_encrypt        72.2 us         72.2 us         9695
kyber768_pke_decrypt        18.2 us         18.2 us        38453
kyber1024_pke_keygen        92.5 us         92.5 us         7576
kyber1024_pke_encrypt        109 us          109 us         6406
kyber1024_pke_decrypt       22.9 us         22.9 us        30623
kyber512_kem_keygen         38.8 us         38.8 us        18040
kyber512_kem_encap          55.1 us         55.1 us        12683
kyber512_kem_decap          64.7 us         64.7 us        10818
kyber768_kem_keygen         64.5 us         64.5 us        10827
kyber768_kem_encap          85.8 us         85.8 us         8148
kyber768_kem_decap          99.5 us         99.5 us         7027
kyber1024_kem_keygen         101 us          101 us         6949
kyber1024_kem_encap          127 us          127 us         5513
kyber1024_kem_decap          142 us          142 us         4886
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-03-02T12:46:40+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.30, 0.10, 0.03
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
kyber512_pke_keygen         29.6 us         29.6 us        23664
kyber512_pke_encrypt        32.0 us         32.0 us        21853
kyber512_pke_decrypt        9.77 us         9.77 us        71624
kyber768_pke_keygen         49.7 us         49.7 us        14087
kyber768_pke_encrypt        53.5 us         53.5 us        13091
kyber768_pke_decrypt        13.2 us         13.2 us        53000
kyber1024_pke_keygen        77.7 us         77.7 us         9007
kyber1024_pke_encrypt       82.7 us         82.7 us         8508
kyber1024_pke_decrypt       16.8 us         16.8 us        41724
kyber512_kem_keygen         32.8 us         32.8 us        21330
kyber512_kem_encap          40.0 us         40.0 us        17439
kyber512_kem_decap          46.1 us         46.1 us        15174
kyber768_kem_keygen         54.7 us         54.7 us        12834
kyber768_kem_encap          64.4 us         64.4 us        10874
kyber768_kem_decap          72.8 us         72.8 us         9611
kyber1024_kem_keygen        83.8 us         83.8 us         8322
kyber1024_kem_encap         96.0 us         96.0 us         7279
kyber1024_kem_decap          106 us          106 us         6602
```
