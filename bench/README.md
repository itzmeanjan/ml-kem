## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note**

> Benchmarking expects presence of `google-benchmark` library in global namespace ( so that it can be found by the compiler ).

> **Warning**

> Because most of the CPUs employ dynamic frequency boosting technique, when benchmarking routines, you may want to disable CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

> **Note**

> You might have trouble interpreting following benchmark results. If so, see below annotated screen capture.

![how_to_interpret_benchmark?](./how_to_interpret_benchmark%3F.png)

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-12-13T15:27:17+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.44, 2.07, 2.04
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  21.7 us         34.1 us        31961       46.1301k/s           159.996k               21.023k            20.542k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           23.1 us         80.4 us        30287       43.2775k/s           124.045k               22.747k            22.243k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           5.70 us         86.5 us       122776       175.442k/s           184.515k                5.598k             5.402k
bench_kyber::pke_keygen<3, 2>/manual_time                  37.7 us         50.1 us        18652       26.5512k/s            148.55k               37.008k            36.241k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           40.7 us          116 us        17257       24.5773k/s           137.957k               39.746k            38.958k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           7.73 us          123 us        90091       129.314k/s           108.717k                7.587k             7.311k
bench_kyber::pke_keygen<4, 2>/manual_time                  60.9 us         73.3 us        11450        16.431k/s           213.985k               59.758k            58.784k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           63.3 us          161 us        10492       15.7937k/s           207.915k               62.181k            61.099k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           10.2 us          171 us        68853       98.2635k/s           106.646k                9.986k             9.645k
bench_kyber::kem_keygen<2, 3>/manual_time                  23.6 us         47.7 us        29636       42.3003k/s           126.774k               23.107k            22.594k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       28.4 us         88.3 us        24683       35.2211k/s            127.33k               27.857k            27.334k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       31.4 us          120 us        22376       31.8428k/s           127.241k               30.825k            30.154k
bench_kyber::kem_keygen<3, 2>/manual_time                  41.0 us         65.2 us        17045       24.3941k/s           167.953k               40.108k            39.357k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       47.9 us          126 us        14705       20.8709k/s           218.539k               46.834k             46.07k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       51.9 us          178 us        13375       19.2716k/s            194.64k               50.798k            49.824k
bench_kyber::kem_keygen<4, 2>/manual_time                  64.8 us         88.7 us        10214       15.4315k/s           190.061k               63.851k            62.745k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time       72.7 us          174 us         9476       13.7642k/s           231.109k                71.36k            70.331k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time       78.1 us          253 us         8894       12.8067k/s           243.283k               76.602k            75.324k
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-12-13T11:37:48+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.15, 0.03, 0.01
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  57.7 us         61.2 us        12108       17.3192k/s            65.317k               57.613k            57.121k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           64.9 us          133 us        10778       15.3982k/s            83.208k               64.801k            64.285k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           21.1 us          154 us        33228        47.474k/s              45.6k               21.036k            21.012k
bench_kyber::pke_keygen<3, 2>/manual_time                   100 us          104 us         6980       9.97039k/s            119.52k              100.068k            99.348k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time            110 us          221 us         6360       9.08363k/s           129.025k              109.837k           109.068k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           28.4 us          249 us        24636       35.1988k/s            45.637k               28.368k            28.344k
bench_kyber::pke_keygen<4, 2>/manual_time                   159 us          162 us         4411       6.30144k/s           175.777k              158.329k            157.26k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time            170 us          339 us         4121       5.88635k/s           177.685k              169.525k           168.493k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           36.0 us          375 us        19448       27.7781k/s            43.525k               35.952k            35.928k
bench_kyber::kem_keygen<2, 3>/manual_time                  62.1 us         68.9 us        11262       16.0946k/s            80.304k               62.004k            61.453k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       76.1 us          148 us         9206       13.1438k/s            84.265k               75.924k            75.396k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       93.0 us          241 us         7527       10.7511k/s             101.4k               92.833k            92.268k
bench_kyber::kem_keygen<3, 2>/manual_time                   107 us          114 us         6543       9.34653k/s            114.42k              106.753k           105.985k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time        125 us          242 us         5586       7.98009k/s            145.68k              125.029k           124.237k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time        148 us          390 us         4733        6.7597k/s           156.541k              147.613k           146.737k
bench_kyber::kem_keygen<4, 2>/manual_time                   168 us          174 us         4178       5.96876k/s           186.925k              167.149k           166.105k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        190 us          367 us         3692       5.27617k/s           197.965k              189.121k           188.136k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        219 us          587 us         3202       4.57423k/s           228.145k              218.149k           217.153k
```

### On ARM Cortex-A72 ( compiled with Clang )

```bash
2022-12-13T11:39:17+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.55, 0.21, 0.08
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  58.6 us         62.5 us        11931       17.0615k/s            78.972k               58.489k            57.804k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           69.2 us          141 us        10114       14.4548k/s            80.521k               69.024k            68.556k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           23.1 us          165 us        30275       43.2102k/s              46.2k                 23.1k            23.052k
bench_kyber::pke_keygen<3, 2>/manual_time                   102 us          107 us         6876       9.82279k/s            111.66k              101.544k           100.765k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time            113 us          230 us         6199       8.85438k/s           123.997k              112.656k           111.937k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           31.0 us          261 us        22558       32.2287k/s            52.848k               30.972k            30.924k
bench_kyber::pke_keygen<4, 2>/manual_time                   159 us          163 us         4411        6.3027k/s           167.269k              158.328k           157.297k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time            169 us          340 us         4152       5.93309k/s           178.009k               168.12k           167.053k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           39.2 us          380 us        17843       25.4849k/s            56.917k               39.168k             39.12k
bench_kyber::kem_keygen<2, 3>/manual_time                  63.8 us         71.3 us        10962       15.6625k/s            85.824k               63.672k            63.204k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       81.5 us          158 us         8590       12.2752k/s            99.132k               81.288k            80.785k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       99.8 us          258 us         7010       10.0186k/s           109.501k               99.625k             99.18k
bench_kyber::kem_keygen<3, 2>/manual_time                   109 us          117 us         6416       9.16466k/s           120.912k              108.829k           108.097k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time        130 us          254 us         5378       7.68516k/s           141.613k              129.804k               129k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time        155 us          409 us         4517       6.45433k/s           165.168k              154.609k           153.672k
bench_kyber::kem_keygen<4, 2>/manual_time                   168 us          176 us         4158       5.94147k/s            187.62k              167.976k           166.824k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        191 us          372 us         3670       5.24417k/s           210.685k              190.225k           189.097k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        221 us          593 us         3172       4.53136k/s           245.065k              220.201k           218.953k
```

### On ARM Neoverse-V1 ( compiled with GCC )

```bash
2022-12-13T11:42:32+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.08, 0.02, 0.01
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  27.6 us         29.0 us        25357       36.1907k/s            33.754k               27.553k            26.895k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           29.9 us         61.1 us        23408       33.4397k/s            41.161k                29.83k            29.167k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           6.17 us         67.9 us       113582       162.153k/s            16.872k                6.157k             6.103k
bench_kyber::pke_keygen<3, 2>/manual_time                  50.9 us         52.2 us        13740       19.6405k/s            58.435k               50.781k             49.62k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           54.8 us          110 us        12774        18.244k/s            60.744k               54.677k            53.399k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           8.26 us          118 us        84741       121.027k/s             16.69k                 8.25k             8.176k
bench_kyber::pke_keygen<4, 2>/manual_time                  84.7 us         86.1 us         8262       11.8006k/s           102.672k               84.566k            82.903k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           89.1 us          179 us         7861       11.2231k/s           103.115k                88.89k            87.557k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           10.5 us          189 us        66826        95.496k/s            18.681k               10.456k            10.359k
bench_kyber::kem_keygen<2, 3>/manual_time                  29.9 us         32.5 us        23417       33.4579k/s            36.604k               29.805k            29.188k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       35.5 us         69.0 us        19711       28.1649k/s            42.625k               35.413k             34.78k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       39.8 us          109 us        17567       25.0998k/s            50.212k               39.746k            39.084k
bench_kyber::kem_keygen<3, 2>/manual_time                  54.3 us         56.9 us        12892       18.4142k/s            70.904k               54.172k             53.07k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       63.3 us          121 us        11057       15.7983k/s            70.264k               63.143k            62.093k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       68.9 us          191 us        10155       14.5077k/s            76.419k               68.775k            67.757k
bench_kyber::kem_keygen<4, 2>/manual_time                  89.2 us         91.8 us         7843       11.2073k/s            97.124k               89.054k            87.021k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        100 us          193 us         6978       9.97521k/s           106.545k              100.021k            98.485k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        107 us          301 us         6536       9.33437k/s           125.609k              106.899k           105.458k
```

### On ARM Neoverse-V1 ( compiled with Clang )

```bash
2022-12-13T11:44:01+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.48, 0.19, 0.07
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  26.9 us         28.3 us        26034       37.1819k/s            42.154k               26.832k            26.312k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           30.7 us         61.8 us        22773       32.5363k/s            48.612k               30.667k            30.128k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           8.05 us         69.8 us        87044       124.268k/s            16.575k                8.039k             7.896k
bench_kyber::pke_keygen<3, 2>/manual_time                  49.0 us         50.4 us        14284       20.4086k/s            61.854k               48.888k            48.036k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           54.3 us          107 us        12900       18.4317k/s            74.541k               54.136k            53.357k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           10.7 us          118 us        65408        93.435k/s            34.117k               10.686k            10.578k
bench_kyber::pke_keygen<4, 2>/manual_time                  79.6 us         81.0 us         8799       12.5683k/s            89.928k               79.409k            78.488k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           85.6 us          169 us         8182       11.6875k/s             91.33k               85.407k            84.305k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           13.7 us          183 us        51189       73.1372k/s            27.315k               13.649k            13.566k
bench_kyber::kem_keygen<2, 3>/manual_time                  29.0 us         31.8 us        24117       34.4682k/s            34.486k               28.948k            28.457k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       35.9 us         69.1 us        19476       27.8226k/s            43.872k               35.865k            35.359k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       41.3 us          110 us        16953        24.212k/s            57.603k               41.213k            40.713k
bench_kyber::kem_keygen<3, 2>/manual_time                  52.1 us         54.9 us        13423       19.1784k/s            59.367k               52.029k            51.376k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       61.5 us          118 us        11391       16.2682k/s            70.317k                61.34k            60.535k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       68.4 us          186 us        10230       14.6148k/s            77.969k               68.289k            67.561k
bench_kyber::kem_keygen<4, 2>/manual_time                  83.7 us         86.5 us         8361       11.9459k/s             94.75k                83.55k             82.54k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time       95.4 us          183 us         7338       10.4839k/s           101.265k               95.207k             94.14k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        104 us          287 us         6738       9.62759k/s           112.469k              103.681k           102.661k
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2022-12-13T11:46:06+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.12, 0.03, 0.01
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  43.1 us         47.3 us        16245       23.1888k/s           106.023k               42.848k            42.219k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           52.8 us          107 us        13253       18.9383k/s           116.965k                52.52k            51.777k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           14.5 us          120 us        48121       68.7621k/s            49.469k               14.484k            14.397k
bench_kyber::pke_keygen<3, 2>/manual_time                  75.8 us         80.0 us         9241       13.1999k/s           138.912k               75.361k             74.28k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           91.1 us          178 us         7688       10.9824k/s            149.69k               90.636k            89.605k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           18.9 us          196 us        37091        52.943k/s             81.17k                18.81k            18.662k
bench_kyber::pke_keygen<4, 2>/manual_time                   122 us          127 us         5726       8.17804k/s           167.505k              121.755k           120.477k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time            141 us          275 us         4946       7.07228k/s           197.111k              140.814k           139.491k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           23.7 us          298 us        29536       42.1931k/s            59.657k               23.599k            23.411k
bench_kyber::kem_keygen<2, 3>/manual_time                  47.1 us         54.6 us        14874       21.2422k/s            108.55k               46.802k            46.134k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       62.4 us          120 us        11234       16.0242k/s           123.154k               62.145k            61.425k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       72.5 us          192 us         9657       13.8022k/s           112.953k               72.163k            71.309k
bench_kyber::kem_keygen<3, 2>/manual_time                  81.7 us         89.3 us         8579        12.243k/s           143.041k               81.284k            80.185k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time        104 us          196 us         6739       9.63545k/s           165.443k              103.292k            102.15k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time        122 us          322 us         5761       8.22322k/s           185.114k              121.029k           120.031k
bench_kyber::kem_keygen<4, 2>/manual_time                   130 us          138 us         5317       7.66699k/s           193.455k              129.829k           128.419k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        157 us          298 us         4453       6.35892k/s            198.47k              156.574k           155.195k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        175 us          474 us         3998        5.7158k/s           233.986k              174.259k           172.777k
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2022-12-13T11:48:22+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.31, 0.18, 0.07
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  34.7 us         39.0 us        20150       28.8147k/s            69.272k               34.567k            33.971k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           38.0 us         83.4 us        18414       26.3086k/s           102.006k               37.883k            37.136k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           10.8 us         93.7 us        64878       92.7504k/s            75.124k               10.735k             10.51k
bench_kyber::pke_keygen<3, 2>/manual_time                  61.1 us         65.4 us        11458        16.368k/s           122.209k               60.865k            59.666k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           66.4 us          138 us        10540       15.0507k/s           132.879k               66.153k            64.916k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           14.6 us          152 us        48018       68.5854k/s            36.915k               14.504k             14.37k
bench_kyber::pke_keygen<4, 2>/manual_time                  97.6 us          102 us         7176        10.247k/s           126.204k               97.236k            95.616k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time            104 us          213 us         6724       9.60791k/s           133.753k              103.698k           102.231k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           18.5 us          231 us        37923       54.1903k/s            80.731k               18.374k            18.178k
bench_kyber::kem_keygen<2, 3>/manual_time                  37.8 us         45.6 us        18497       26.4366k/s            98.423k               37.673k            37.038k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       46.0 us         94.7 us        15239       21.7373k/s            106.21k               45.782k            44.774k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       52.6 us          147 us        13286       19.0064k/s            113.36k               52.407k            51.636k
bench_kyber::kem_keygen<3, 2>/manual_time                  65.9 us         73.8 us        10605       15.1737k/s           128.067k               65.633k            64.255k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       77.4 us          154 us         9047       12.9281k/s            149.05k               77.085k            75.556k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       86.4 us          240 us         8096       11.5784k/s            130.74k                86.03k            84.801k
bench_kyber::kem_keygen<4, 2>/manual_time                   104 us          112 us         6739       9.61824k/s           165.642k              103.572k           101.925k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time        118 us          233 us         5923       8.45945k/s           182.321k              117.759k            116.31k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        130 us          362 us         5403       7.71379k/s           191.765k              129.141k           127.585k
```

### On Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz ( compiled with GCC )

```bash
2022-12-13T11:51:12+00:00
Running ./bench/a.out
Run on (128 X 847.057 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x64)
  L1 Instruction 32 KiB (x64)
  L2 Unified 1280 KiB (x64)
  L3 Unified 55296 KiB (x2)
Load Average: 0.00, 0.00, 0.00
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  25.6 us         26.7 us        27373       39.0991k/s            28.009k               25.529k            25.035k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           31.7 us         60.5 us        22085       31.5502k/s            37.499k               31.694k             29.66k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           7.99 us         68.4 us        87780       125.211k/s            21.526k                7.924k              7.82k
bench_kyber::pke_keygen<3, 2>/manual_time                  45.1 us         46.2 us        15532       22.1867k/s            60.503k                   45k            44.203k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           54.3 us          102 us        12960       18.4023k/s            57.896k               54.799k            50.935k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           11.0 us          114 us        63837       91.1658k/s             15.58k               11.231k            10.334k
bench_kyber::pke_keygen<4, 2>/manual_time                  72.6 us         73.7 us         9644        13.776k/s            87.742k               72.537k             71.27k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           84.2 us          160 us         8311       11.8776k/s             94.51k               84.436k            79.859k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           13.6 us          173 us        51309       73.3447k/s            21.105k               13.847k            12.923k
bench_kyber::kem_keygen<2, 3>/manual_time                  28.1 us         30.2 us        24880       35.5472k/s            32.025k               28.085k            27.545k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       38.2 us         69.6 us        18322       26.1686k/s            46.762k               38.213k            36.191k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       44.3 us          114 us        15809       22.5871k/s              54.7k               44.461k            41.654k
bench_kyber::kem_keygen<3, 2>/manual_time                  48.7 us         50.8 us        14382       20.5536k/s              73.5k               48.576k            47.808k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       63.5 us          115 us        11024       15.7525k/s            76.105k               63.758k             59.81k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       70.3 us          185 us         9960       14.2171k/s            74.127k               70.742k            66.585k
bench_kyber::kem_keygen<4, 2>/manual_time                  77.2 us         79.4 us         9062       12.9461k/s            87.831k               77.192k            75.942k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time       95.6 us          176 us         7319       10.4581k/s           101.451k               95.951k            91.041k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time        105 us          280 us         6696       9.55924k/s           109.921k              104.971k            99.695k
```

### On Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz ( compiled with Clang )

```bash
2022-12-13T11:53:16+00:00
Running ./bench/a.out
Run on (128 X 855.596 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x64)
  L1 Instruction 32 KiB (x64)
  L2 Unified 1280 KiB (x64)
  L3 Unified 55296 KiB (x2)
Load Average: 0.24, 0.13, 0.05
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations items_per_second max_exec_time (ns) median_exec_time (ns) min_exec_time (ns)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>/manual_time                  20.9 us         22.1 us        33569       47.9539k/s              22.9k               20.831k            20.359k
bench_kyber::encrypt<2, 3, 2, 10, 4>/manual_time           22.7 us         47.1 us        30856       44.0667k/s            30.036k               22.666k            22.262k
bench_kyber::decrypt<2, 3, 2, 10, 4>/manual_time           5.30 us         52.5 us       132132       188.823k/s            32.961k                5.293k             5.228k
bench_kyber::pke_keygen<3, 2>/manual_time                  37.0 us         38.3 us        18899       26.9992k/s            41.795k               37.006k            36.257k
bench_kyber::encrypt<3, 2, 2, 10, 4>/manual_time           39.9 us         80.6 us        17538       25.0523k/s            47.946k               39.878k            39.166k
bench_kyber::decrypt<3, 2, 2, 10, 4>/manual_time           7.19 us         87.7 us        97396       139.133k/s            19.877k                7.183k             7.101k
bench_kyber::pke_keygen<4, 2>/manual_time                  59.8 us         61.1 us        11696       16.7087k/s            70.492k               59.804k            58.877k
bench_kyber::encrypt<4, 2, 2, 11, 5>/manual_time           63.7 us          127 us        10995       15.7037k/s            71.368k               63.637k            62.703k
bench_kyber::decrypt<4, 2, 2, 11, 5>/manual_time           9.57 us          137 us        73166       104.541k/s            16.299k                9.561k             9.454k
bench_kyber::kem_keygen<2, 3>/manual_time                  23.0 us         25.4 us        30380       43.4002k/s            31.531k               23.015k            22.499k
bench_kyber::encapsulate<2, 3, 2, 10, 4>/manual_time       28.2 us         54.9 us        24813       35.4381k/s            45.778k               28.189k            27.614k
bench_kyber::decapsulate<2, 3, 2, 10, 4>/manual_time       31.0 us         85.8 us        22605       32.2893k/s            57.147k                30.94k            30.458k
bench_kyber::kem_keygen<3, 2>/manual_time                  40.3 us         42.7 us        17358       24.8036k/s            45.959k               40.283k            39.573k
bench_kyber::encapsulate<3, 2, 2, 10, 4>/manual_time       47.6 us         91.6 us        14693       20.9883k/s            59.383k               47.606k            46.837k
bench_kyber::decapsulate<3, 2, 2, 10, 4>/manual_time       51.1 us          143 us        13702       19.5756k/s            56.089k                51.04k            50.287k
bench_kyber::kem_keygen<4, 2>/manual_time                  64.2 us         66.6 us        10898       15.5682k/s            69.843k               64.183k            63.298k
bench_kyber::encapsulate<4, 2, 2, 11, 5>/manual_time       73.6 us          141 us         9510       13.5891k/s            80.491k               73.544k             72.37k
bench_kyber::decapsulate<4, 2, 2, 11, 5>/manual_time       78.4 us          220 us         8931       12.7556k/s            86.416k               78.356k            77.144k
```
