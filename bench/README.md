## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note**

> Benchmarking expects presence of `google-benchmark` library in global namespace ( so that it can be found by the compiler ).

> **Warning**

> Because most of the CPUs employ dynamic frequency boosting technique, when benchmarking routines, you must disable CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

> **Note**

> You might have trouble interpreting following benchmark results. If so, see below annotated screen capture.

![how_to_interpret_benchmark?](./how_to_interpret_benchmark%3F.png)

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-02-21T18:40:08+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.04, 1.92, 1.90
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  19.6 us         31.3 us        36107       50.9676k/s           170.065k               19.178k             18.72k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           20.9 us         74.7 us        33475       47.7954k/s           177.248k               20.538k            19.934k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           5.72 us         80.9 us       122738       174.727k/s           137.063k                5.598k             5.386k
bench_kyber::pke_keygen<3, 2>/manual_time                  33.0 us         44.8 us        21163       30.2941k/s           193.999k               32.405k            31.579k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           35.7 us          104 us        19637       28.0286k/s           198.138k               34.924k            34.113k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           8.17 us          113 us        86007       122.374k/s           121.164k                8.007k             7.704k
bench_kyber::pke_keygen<4, 2>/manual_time                  52.6 us         64.6 us        13076       18.9943k/s           240.811k               51.633k            50.635k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           54.6 us          142 us        12730       18.3249k/s           231.804k               53.635k            52.487k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           10.2 us          153 us        68868        97.978k/s           102.045k                9.989k             9.631k
bench_kyber::kem_keygen<2, 3>/manual_time                  21.5 us         43.9 us        31900       46.4792k/s            163.36k               21.246k             20.73k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       26.2 us         82.2 us        26662       38.2139k/s           133.525k               25.707k             25.13k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       29.0 us          113 us        24403       34.4999k/s           176.871k               28.161k            27.524k
bench_kyber::kem_keygen<3, 2>/manual_time                  36.8 us         60.0 us        18908       27.1549k/s           173.685k               35.556k            34.735k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       43.0 us          114 us        16094       23.2718k/s           192.226k               42.038k            41.276k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       46.3 us          161 us        14990       21.5803k/s           188.068k               45.543k            44.592k
bench_kyber::kem_keygen<4, 2>/manual_time                  56.9 us         79.9 us        11912       17.5669k/s           200.653k                55.73k             54.74k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time       64.0 us          156 us        10866       15.6161k/s           199.608k               62.796k            61.613k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time       68.5 us          226 us        10175       14.6014k/s           217.885k               67.106k            65.633k
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-02-21T14:52:55+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.42, 0.21, 0.08
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  36.8 us         41.1 us        19013       27.2037k/s           240.459k               36.618k            36.177k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           46.0 us         93.6 us        15204       21.7336k/s            207.87k               45.855k            45.375k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           14.6 us          108 us        47851       68.2936k/s           223.029k               14.589k            14.446k
bench_kyber::pke_keygen<3, 2>/manual_time                  61.7 us         66.0 us        11360       16.2031k/s           304.261k               61.254k             60.59k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           75.0 us          148 us         9319       13.3298k/s           273.869k               74.654k            73.933k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           18.9 us          166 us        37026       52.8914k/s           269.356k               18.827k            18.725k
bench_kyber::pke_keygen<4, 2>/manual_time                  97.5 us          102 us         7163       10.2542k/s           399.628k               97.058k            96.107k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time            114 us          222 us         6152        8.8056k/s           348.481k              113.144k           112.097k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           23.6 us          245 us        29594       42.2898k/s           207.225k               23.536k            23.382k
bench_kyber::kem_keygen<2, 3>/manual_time                  40.6 us         48.1 us        17260       24.6549k/s            93.625k               40.408k            39.924k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       56.1 us          108 us        12469       17.8266k/s           226.146k               55.861k             55.39k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       65.4 us          172 us        10711       15.2888k/s           235.517k               65.127k            64.806k
bench_kyber::kem_keygen<3, 2>/manual_time                  67.3 us         74.8 us        10423        14.861k/s             238.7k                66.98k            66.316k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       88.3 us          167 us         7939       11.3231k/s           309.082k               87.909k            87.236k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time        101 us          267 us         6961       9.93932k/s           238.364k              100.169k            99.571k
bench_kyber::kem_keygen<4, 2>/manual_time                   105 us          113 us         6646       9.51768k/s           346.317k              104.591k            103.69k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        131 us          247 us         5351       7.65319k/s             333.4k              130.115k           129.245k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        146 us          393 us         4795       6.85041k/s             185.4k              145.448k           144.458k
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-02-21T14:55:15+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.30, 0.27, 0.12
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  31.4 us         35.9 us        22373       31.8152k/s           254.211k                31.19k             30.67k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           33.6 us         76.1 us        20807       29.7349k/s           256.891k               33.456k            32.946k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           10.5 us         85.9 us        66784       95.5557k/s            151.46k               10.431k            10.362k
bench_kyber::pke_keygen<3, 2>/manual_time                  52.8 us         57.3 us        13233       18.9482k/s           236.556k               52.539k            51.858k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           56.5 us          120 us        12362       17.7128k/s           201.361k               56.206k            55.413k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           13.9 us          134 us        50238       71.7506k/s           260.588k               13.871k            13.784k
bench_kyber::pke_keygen<4, 2>/manual_time                  83.3 us         87.8 us         8411       12.0117k/s           225.236k               82.899k            81.986k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           87.0 us          182 us         8068       11.4937k/s            246.76k               86.432k            85.525k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           17.6 us          198 us        39842       56.9174k/s           182.393k               17.485k            17.397k
bench_kyber::kem_keygen<2, 3>/manual_time                  34.6 us         42.6 us        20241       28.9343k/s           196.188k               34.431k                34k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       41.7 us         87.5 us        16766       23.9753k/s            222.39k               41.524k            41.055k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       47.2 us          134 us        14819       21.1927k/s           239.251k               46.979k            46.569k
bench_kyber::kem_keygen<3, 2>/manual_time                  57.6 us         65.7 us        12125       17.3614k/s           202.788k               57.387k            56.706k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       67.7 us          137 us        10347        14.766k/s           211.393k               67.379k            66.637k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       74.8 us          211 us         9367       13.3641k/s           261.883k               74.502k            73.716k
bench_kyber::kem_keygen<4, 2>/manual_time                  89.7 us         97.8 us         7795       11.1515k/s           279.784k               89.314k            88.371k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        102 us          203 us         6881       9.82798k/s           149.101k              101.368k           100.298k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        110 us          313 us         6371       9.10282k/s           329.752k              109.402k           108.221k
```
