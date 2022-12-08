## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Warning**

> Because most of the CPUs employ dynamic frequency boosting technique, when benchmarking routines, you may want to disable CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

> **Note**

> On x86_64 architecture average CPU cycle spent in keygen/ {encrypt, encapsulate}/ {decrypt, decapsulate} routine is measured using compiler built-in `__rdtsc`. I've tested it with both Clang and GCC.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-12-08T12:25:59+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.37, 1.67, 1.78
-----------------------------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations average_cpu_cycles items_per_second
-----------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 21438 ns        21395 ns        32382            51.427k        46.739k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          22996 ns        22962 ns        30500            55.172k       43.5502k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           5660 ns         5654 ns       119793            13.566k       176.875k/s
bench_kyber::pke_keygen<3, 2>                 37535 ns        37489 ns        18645             90.06k       26.6747k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          40276 ns        40223 ns        17492            96.643k       24.8612k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           7709 ns         7695 ns        88807            18.484k       129.958k/s
bench_kyber::pke_keygen<4, 2>                 60666 ns        60614 ns        11374           145.572k        16.498k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          63328 ns        63269 ns        11000           151.968k       15.8055k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          10045 ns        10038 ns        67741            24.087k       99.6218k/s
bench_kyber::kem_keygen<2, 3>                 23974 ns        23600 ns        29888            57.522k       42.3732k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      27877 ns        27847 ns        24345            66.887k       35.9108k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      32213 ns        32112 ns        22439            77.292k       31.1414k/s
bench_kyber::kem_keygen<3, 2>                 40342 ns        40315 ns        16870            96.803k       24.8049k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      47345 ns        47305 ns        14778           113.611k       21.1395k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      51619 ns        51586 ns        12987           123.868k        19.385k/s
bench_kyber::kem_keygen<4, 2>                 64723 ns        64670 ns        10216           155.316k       15.4631k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      72587 ns        72478 ns         9560           174.191k       13.7974k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      78441 ns        78362 ns         8804            188.24k       12.7613k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-12-08T08:35:19+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.08, 0.02, 0.01
----------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations items_per_second
----------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 57386 ns        57385 ns        12190        17.426k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          64532 ns        64530 ns        10798       15.4966k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          20983 ns        20983 ns        33361       47.6585k/s
bench_kyber::pke_keygen<3, 2>                100210 ns       100207 ns         6996       9.97934k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         109874 ns       109871 ns         6373       9.10162k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          28322 ns        28322 ns        24714       35.3086k/s
bench_kyber::pke_keygen<4, 2>                158289 ns       158287 ns         4422       6.31765k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         169808 ns       169807 ns         4126       5.88905k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          35835 ns        35833 ns        19535        27.907k/s
bench_kyber::kem_keygen<2, 3>                 62022 ns        62021 ns        11316       16.1236k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      75746 ns        75743 ns         9249       13.2025k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      92372 ns        92371 ns         7562        10.826k/s
bench_kyber::kem_keygen<3, 2>                106517 ns       106516 ns         6562       9.38829k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     124890 ns       124887 ns         5606       8.00725k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     147424 ns       147422 ns         4735       6.78326k/s
bench_kyber::kem_keygen<4, 2>                166919 ns       166917 ns         4193       5.99102k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     189034 ns       189028 ns         3697       5.29021k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     219775 ns       219762 ns         3192       4.55039k/s
```

### On ARM Cortex-A72 ( compiled with Clang )

```bash
2022-12-08T08:36:52+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.25, 0.09, 0.03
----------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations items_per_second
----------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 58064 ns        58064 ns        12026       17.2225k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          68359 ns        68358 ns        10252       14.6288k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          22375 ns        22375 ns        31278       44.6922k/s
bench_kyber::pke_keygen<3, 2>                100087 ns       100083 ns         7014       9.99166k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         114019 ns       114018 ns         6128       8.77053k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          30274 ns        30273 ns        23124       33.0327k/s
bench_kyber::pke_keygen<4, 2>                157806 ns       157802 ns         4439       6.33704k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         168377 ns       168374 ns         4196       5.93917k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          38756 ns        38755 ns        18061       25.8029k/s
bench_kyber::kem_keygen<2, 3>                 63094 ns        63094 ns        11072       15.8494k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      80582 ns        80581 ns         8703       12.4098k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      99613 ns        99610 ns         7013       10.0391k/s
bench_kyber::kem_keygen<3, 2>                107269 ns       107266 ns         6474       9.32262k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     130892 ns       130889 ns         5301       7.64004k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     154406 ns       154406 ns         4539       6.47645k/s
bench_kyber::kem_keygen<4, 2>                168146 ns       168145 ns         4146       5.94726k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     189495 ns       189494 ns         3679       5.27721k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     221018 ns       221014 ns         3186        4.5246k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-12-08T08:39:22+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.00, 0.00, 0.00
----------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations items_per_second
----------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 27196 ns        27195 ns        25798       36.7714k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          30182 ns        30181 ns        23215       33.1333k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           6104 ns         6104 ns       114729        163.82k/s
bench_kyber::pke_keygen<3, 2>                 51525 ns        51524 ns        13567       19.4083k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          55684 ns        55683 ns        12625        17.959k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           8204 ns         8203 ns        85277       121.901k/s
bench_kyber::pke_keygen<4, 2>                 85430 ns        85429 ns         8149       11.7057k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          89667 ns        89664 ns         7757       11.1527k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          10395 ns        10395 ns        67364       96.2027k/s
bench_kyber::kem_keygen<2, 3>                 29579 ns        29578 ns        23111       33.8084k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      35843 ns        35842 ns        19651       27.9002k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      39495 ns        39495 ns        17724       25.3199k/s
bench_kyber::kem_keygen<3, 2>                 54580 ns        54578 ns        12765       18.3223k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      63088 ns        63085 ns        11058       15.8516k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      67781 ns        67779 ns        10318       14.7539k/s
bench_kyber::kem_keygen<4, 2>                 90504 ns        90503 ns         7694       11.0494k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     100387 ns       100386 ns         6922       9.96158k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     106354 ns       106351 ns         6568       9.40286k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-12-08T08:41:34+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.11, 0.04, 0.01
----------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations items_per_second
----------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 27224 ns        27223 ns        26213       36.7331k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          30511 ns        30510 ns        23108       32.7761k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           8001 ns         8001 ns        87456       124.984k/s
bench_kyber::pke_keygen<3, 2>                 48818 ns        48816 ns        14329        20.485k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          53677 ns        53676 ns        13005       18.6304k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          10670 ns        10670 ns        65610       93.7219k/s
bench_kyber::pke_keygen<4, 2>                 79533 ns        79532 ns         8730       12.5735k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          85473 ns        85471 ns         8172       11.6999k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          13616 ns        13615 ns        51408       73.4462k/s
bench_kyber::kem_keygen<2, 3>                 28676 ns        28675 ns        24344       34.8737k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      35486 ns        35485 ns        19621       28.1811k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      41253 ns        41252 ns        17047       24.2413k/s
bench_kyber::kem_keygen<3, 2>                 51775 ns        51774 ns        13442       19.3148k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      61017 ns        61016 ns        11476       16.3893k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      68946 ns        68945 ns        10224       14.5043k/s
bench_kyber::kem_keygen<4, 2>                 84131 ns        84128 ns         8428       11.8866k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      93867 ns        93864 ns         7389       10.6537k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     103757 ns       103755 ns         6752       9.63812k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2022-12-08T08:44:43+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.08, 0.02, 0.01
-----------------------------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations average_cpu_cycles items_per_second
-----------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 42781 ns        42780 ns        16359            98.365k       23.3755k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          52746 ns        52746 ns        13300           121.291k       18.9589k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          13947 ns        13947 ns        50171            32.056k       71.6987k/s
bench_kyber::pke_keygen<3, 2>                 75217 ns        75217 ns         9319           172.959k       13.2949k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          89999 ns        89997 ns         7780           206.956k       11.1115k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          18279 ns        18279 ns        38293            42.021k       54.7076k/s
bench_kyber::pke_keygen<4, 2>                120849 ns       120844 ns         5795           277.928k       8.27514k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         141813 ns       141804 ns         4922           326.138k         7.052k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          22900 ns        22899 ns        30602            52.646k       43.6705k/s
bench_kyber::kem_keygen<2, 3>                 46402 ns        46398 ns        15161           106.694k       21.5527k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      62332 ns        62325 ns        11250           143.336k        16.045k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      73071 ns        73066 ns         9576           168.022k       13.6863k/s
bench_kyber::kem_keygen<3, 2>                 80562 ns        80557 ns         8653           185.266k       12.4136k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     104087 ns       104086 ns         6644           239.363k       9.60746k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     118249 ns       118248 ns         5968           271.946k       8.45681k/s
bench_kyber::kem_keygen<4, 2>                130242 ns       130228 ns         5372           299.526k       7.67884k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     159012 ns       158991 ns         4422           365.674k       6.28966k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     172186 ns       172180 ns         4021           395.983k       5.80789k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2022-12-08T08:46:09+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.17, 0.07, 0.02
-----------------------------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations average_cpu_cycles items_per_second
-----------------------------------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>                 33939 ns        33935 ns        20654            78.036k       29.4682k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          37289 ns        37290 ns        18613            85.742k        26.817k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          10109 ns        10109 ns        69314            23.228k        98.924k/s
bench_kyber::pke_keygen<3, 2>                 60289 ns        60289 ns        11623            138.64k       16.5868k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          65658 ns        65656 ns        10639           150.999k       15.2309k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          13878 ns        13878 ns        50502            31.897k       72.0559k/s
bench_kyber::pke_keygen<4, 2>                 96943 ns        96944 ns         7244           222.943k       10.3152k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         103181 ns       103179 ns         6788           237.277k       9.69186k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          17631 ns        17631 ns        39699            40.529k       56.7175k/s
bench_kyber::kem_keygen<2, 3>                 37171 ns        37171 ns        18863            85.467k       26.9028k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      45461 ns        45461 ns        15435            104.53k       21.9971k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      51712 ns        51707 ns        13514           118.912k       19.3397k/s
bench_kyber::kem_keygen<3, 2>                 65080 ns        65073 ns        10613           149.653k       15.3673k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      76921 ns        76916 ns         9150           176.899k       13.0013k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      85602 ns        85601 ns         8188           196.857k       11.6821k/s
bench_kyber::kem_keygen<4, 2>                102946 ns       102944 ns         6793           236.745k       9.71401k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     117328 ns       117326 ns         5972           269.825k       8.52326k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     128166 ns       128162 ns         5436           294.754k       7.80265k/s
```
