# kyber
CRYSTALS-Kyber: Post-Quantum Public-key Encryption &amp; Key-establishment Algorithm

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

- For benchmarking Kyber implementation on CPU systems, you'll need to have `google-benchmark` globally installed. You may want to follow [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide.

## Testing

For testing functional correctness of Kyber implementation along with its component units, you have to issue

```bash
make
```

```bash
[test] Kyber prime field operations
[test] (i)NTT over degree-255 polynomial
[test] Polynomial serialization/ deserialization
[test] Coefficient compression/ decompression
[test] INDCPA-secure Public Key Encryption
[test] INDCCA2-secure Key Encapsulation Mechanism
```

## Benchmarking

For benchmarking Kyber implementation along with its components on CPU systems, you need to issue

```bash
make benchmark
```

> When benchmarking program, you may want to disable CPU frequency scaling technique by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-09-30T15:44:09+05:30
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.62, 1.75, 1.71
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            4.03 ns         4.02 ns    172986010
bench_kyber::ff_compound_add                   1.94 ns         1.94 ns    351391510
bench_kyber::ff_sub                            4.26 ns         4.25 ns    164160098
bench_kyber::ff_compound_sub                   2.10 ns         2.10 ns    330022111
bench_kyber::ff_neg                           0.815 ns        0.813 ns    852514919
bench_kyber::ff_mul                            6.79 ns         6.79 ns    102218133
bench_kyber::ff_compound_mul                   4.47 ns         4.47 ns    155608808
bench_kyber::ff_inv                            81.7 ns         81.4 ns     11811954
bench_kyber::ff_div                            75.7 ns         75.7 ns     11726974
bench_kyber::ff_exp                             403 ns          403 ns      1684729
bench_kyber::encode<1>                          148 ns          148 ns      4713773 bytes_per_second=206.501M/s
bench_kyber::decode<1>                          145 ns          145 ns      4735617 bytes_per_second=210.906M/s
bench_kyber::encode<4>                          966 ns          965 ns       727552 bytes_per_second=126.473M/s
bench_kyber::decode<4>                          782 ns          781 ns       898403 bytes_per_second=156.217M/s
bench_kyber::encode<5>                         1820 ns         1818 ns       382190 bytes_per_second=83.9395M/s
bench_kyber::decode<5>                         1659 ns         1657 ns       424775 bytes_per_second=92.0665M/s
bench_kyber::encode<10>                        2950 ns         2945 ns       234962 bytes_per_second=103.612M/s
bench_kyber::decode<10>                        2934 ns         2930 ns       240899 bytes_per_second=104.145M/s
bench_kyber::encode<11>                        4197 ns         4191 ns       169029 bytes_per_second=80.0959M/s
bench_kyber::decode<11>                        3863 ns         3860 ns       180931 bytes_per_second=86.9752M/s
bench_kyber::encode<12>                        3505 ns         3501 ns       196607 bytes_per_second=104.604M/s
bench_kyber::decode<12>                        3650 ns         3646 ns       191990 bytes_per_second=100.432M/s
bench_kyber::compress<1>                       5.25 ns         5.25 ns    126855258
bench_kyber::decompress<1>                     4.61 ns         4.61 ns    150765566
bench_kyber::compress<4>                       5.27 ns         5.26 ns    125475013
bench_kyber::decompress<4>                     4.85 ns         4.85 ns    143769935
bench_kyber::compress<5>                       5.27 ns         5.26 ns    126440519
bench_kyber::decompress<5>                     4.87 ns         4.87 ns    144445453
bench_kyber::compress<10>                      5.06 ns         5.06 ns    131376450
bench_kyber::decompress<10>                    4.86 ns         4.85 ns    145199899
bench_kyber::compress<11>                      5.05 ns         5.04 ns    132320139
bench_kyber::decompress<11>                    4.84 ns         4.83 ns    143702937
bench_kyber::ntt                               1482 ns         1480 ns       470247
bench_kyber::intt                              1432 ns         1430 ns       487492
bench_kyber::polymul                            371 ns          370 ns      1901828
bench_kyber::random_data<32>                  10225 ns        10212 ns        66316
bench_kyber::pke_keygen<2, 3>                 49604 ns        49520 ns        13351 items_per_second=20.1939k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          38406 ns        38367 ns        18299 items_per_second=26.0638k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          19857 ns        19829 ns        35156 items_per_second=50.4316k/s
bench_kyber::pke_keygen<3, 2>                 69705 ns        69643 ns         9588 items_per_second=14.3589k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          61679 ns        61626 ns        10974 items_per_second=16.2268k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          28696 ns        28663 ns        24275 items_per_second=34.8883k/s
bench_kyber::pke_keygen<4, 2>                100987 ns       100858 ns         6826 items_per_second=9.9149k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          96140 ns        96066 ns         7083 items_per_second=10.4095k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          41984 ns        41929 ns        16971 items_per_second=23.8496k/s
bench_kyber::kem_keygen<2, 3>                 60816 ns        60775 ns        10769 items_per_second=16.4542k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      54742 ns        54702 ns        11333 items_per_second=18.2809k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      62299 ns        62199 ns        10750 items_per_second=16.0775k/s
bench_kyber::kem_keygen<3, 2>                 84558 ns        84478 ns         8101 items_per_second=11.8374k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      81137 ns        81016 ns         8495 items_per_second=12.3433k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      95342 ns        95238 ns         7328 items_per_second=10.5k/s
bench_kyber::kem_keygen<4, 2>                114002 ns       113873 ns         6006 items_per_second=8.78168k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     117725 ns       117592 ns         5738 items_per_second=8.504k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     143638 ns       143391 ns         4814 items_per_second=6.97392k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-09-30T10:19:14+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.22, 0.06, 0.02
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            7.83 ns         7.83 ns     89311884
bench_kyber::ff_compound_add                   2.61 ns         2.61 ns    268164859
bench_kyber::ff_sub                            7.83 ns         7.83 ns     89384279
bench_kyber::ff_compound_sub                   2.61 ns         2.61 ns    268161950
bench_kyber::ff_neg                            2.25 ns         2.25 ns    310680897
bench_kyber::ff_mul                            11.7 ns         11.7 ns     59587367
bench_kyber::ff_compound_mul                   6.53 ns         6.53 ns    107259987
bench_kyber::ff_inv                            40.2 ns         40.2 ns     13028042
bench_kyber::ff_div                            54.2 ns         54.2 ns     10000000
bench_kyber::ff_exp                             811 ns          811 ns       814831
bench_kyber::encode<1>                          610 ns          610 ns      1148180 bytes_per_second=50.0667M/s
bench_kyber::decode<1>                          625 ns          625 ns      1120450 bytes_per_second=48.8464M/s
bench_kyber::encode<4>                         2729 ns         2729 ns       256472 bytes_per_second=44.724M/s
bench_kyber::decode<4>                         2748 ns         2748 ns       254698 bytes_per_second=44.4154M/s
bench_kyber::encode<5>                         3964 ns         3964 ns       176586 bytes_per_second=38.4929M/s
bench_kyber::decode<5>                         3795 ns         3795 ns       184451 bytes_per_second=40.2059M/s
bench_kyber::encode<10>                        7746 ns         7746 ns        90368 bytes_per_second=39.3987M/s
bench_kyber::decode<10>                        6808 ns         6808 ns       102885 bytes_per_second=44.8292M/s
bench_kyber::encode<11>                        9886 ns         9886 ns        70800 bytes_per_second=33.9551M/s
bench_kyber::decode<11>                       10190 ns        10190 ns        68691 bytes_per_second=32.9429M/s
bench_kyber::encode<12>                        9292 ns         9292 ns        75315 bytes_per_second=39.4128M/s
bench_kyber::decode<12>                        8126 ns         8126 ns        86144 bytes_per_second=45.0686M/s
bench_kyber::compress<1>                       6.96 ns         6.96 ns    100557254
bench_kyber::decompress<1>                     8.27 ns         8.27 ns     84681524
bench_kyber::compress<4>                       6.96 ns         6.96 ns    100553735
bench_kyber::decompress<4>                     8.27 ns         8.27 ns     84681057
bench_kyber::compress<5>                       6.96 ns         6.96 ns    100556023
bench_kyber::decompress<5>                     8.27 ns         8.27 ns     84671887
bench_kyber::compress<10>                      6.96 ns         6.96 ns    100554099
bench_kyber::decompress<10>                    8.27 ns         8.27 ns     84673042
bench_kyber::compress<11>                      6.96 ns         6.96 ns    100556890
bench_kyber::decompress<11>                    8.27 ns         8.27 ns     84680479
bench_kyber::ntt                               5277 ns         5277 ns       132758
bench_kyber::intt                              5173 ns         5173 ns       135091
bench_kyber::polymul                            864 ns          864 ns       810502
bench_kyber::random_data<32>                   4580 ns         4580 ns       152851
bench_kyber::pke_keygen<2, 3>                128902 ns       128894 ns         5445 items_per_second=7.75831k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>         125555 ns       125553 ns         5564 items_per_second=7.96479k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          52269 ns        52268 ns        13377 items_per_second=19.1322k/s
bench_kyber::pke_keygen<3, 2>                198871 ns       198868 ns         3520 items_per_second=5.02845k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         202628 ns       202620 ns         3455 items_per_second=4.93534k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          73318 ns        73314 ns         9545 items_per_second=13.6399k/s
bench_kyber::pke_keygen<4, 2>                298013 ns       298008 ns         2349 items_per_second=3.35561k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         308343 ns       308333 ns         2273 items_per_second=3.24325k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>         108994 ns       108993 ns         6421 items_per_second=9.17488k/s
bench_kyber::kem_keygen<2, 3>                143694 ns       143690 ns         4871 items_per_second=6.95942k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>     158518 ns       158517 ns         4415 items_per_second=6.30847k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     193171 ns       193161 ns         3622 items_per_second=5.17703k/s
bench_kyber::kem_keygen<3, 2>                218857 ns       218852 ns         3198 items_per_second=4.5693k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     242635 ns       242628 ns         2887 items_per_second=4.12153k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     294937 ns       294936 ns         2367 items_per_second=3.39057k/s
bench_kyber::kem_keygen<4, 2>                323647 ns       323638 ns         2163 items_per_second=3.08987k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     360859 ns       360855 ns         1939 items_per_second=2.7712k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     444195 ns       444181 ns         1580 items_per_second=2.25133k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-09-30T10:22:06+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.15, 0.03, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.17 ns         6.17 ns    113392478
bench_kyber::ff_compound_add                   2.31 ns         2.31 ns    302830795
bench_kyber::ff_sub                            6.20 ns         6.20 ns    113198222
bench_kyber::ff_compound_sub                   2.31 ns         2.31 ns    302881677
bench_kyber::ff_neg                            1.33 ns         1.33 ns    533425034
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82596904
bench_kyber::ff_compound_mul                   4.62 ns         4.62 ns    151400454
bench_kyber::ff_inv                            36.6 ns         36.6 ns     20358533
bench_kyber::ff_div                            37.8 ns         37.8 ns     21502050
bench_kyber::ff_exp                             583 ns          583 ns      1163559
bench_kyber::encode<1>                          283 ns          283 ns      2468457 bytes_per_second=107.653M/s
bench_kyber::decode<1>                          179 ns          179 ns      3907377 bytes_per_second=170.248M/s
bench_kyber::encode<4>                         1306 ns         1306 ns       535911 bytes_per_second=93.4533M/s
bench_kyber::decode<4>                         1429 ns         1429 ns       489676 bytes_per_second=85.4341M/s
bench_kyber::encode<5>                         2371 ns         2371 ns       295222 bytes_per_second=64.3667M/s
bench_kyber::decode<5>                         3055 ns         3055 ns       229046 bytes_per_second=49.9412M/s
bench_kyber::encode<10>                        4729 ns         4729 ns       148012 bytes_per_second=64.5293M/s
bench_kyber::decode<10>                        5379 ns         5379 ns       130131 bytes_per_second=56.7359M/s
bench_kyber::encode<11>                        6375 ns         6374 ns       109830 bytes_per_second=52.6631M/s
bench_kyber::decode<11>                        6103 ns         6103 ns       114634 bytes_per_second=55.0022M/s
bench_kyber::encode<12>                        5677 ns         5676 ns       123318 bytes_per_second=64.5143M/s
bench_kyber::decode<12>                        6048 ns         6048 ns       115675 bytes_per_second=60.5486M/s
bench_kyber::compress<1>                       5.47 ns         5.46 ns    127956045
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113567873
bench_kyber::compress<4>                       5.47 ns         5.47 ns    127829247
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113565028
bench_kyber::compress<5>                       5.47 ns         5.47 ns    128097327
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113570437
bench_kyber::compress<10>                      5.47 ns         5.47 ns    128274544
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113562725
bench_kyber::compress<11>                      5.47 ns         5.47 ns    128326081
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113575792
bench_kyber::ntt                               2037 ns         2037 ns       343688
bench_kyber::intt                              2131 ns         2131 ns       328471
bench_kyber::polymul                            296 ns          296 ns      2366128
bench_kyber::random_data<32>                   1667 ns         1667 ns       419026
bench_kyber::pke_keygen<2, 3>                 59393 ns        59390 ns        11801 items_per_second=16.8378k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          60174 ns        60172 ns        11641 items_per_second=16.6189k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          31684 ns        31683 ns        22103 items_per_second=31.5627k/s
bench_kyber::pke_keygen<3, 2>                 91784 ns        91782 ns         7620 items_per_second=10.8953k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          96102 ns        96100 ns         7379 items_per_second=10.4058k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          45671 ns        45669 ns        15307 items_per_second=21.8965k/s
bench_kyber::pke_keygen<4, 2>                137057 ns       137054 ns         5107 items_per_second=7.29638k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         146427 ns       146422 ns         4787 items_per_second=6.82957k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          63820 ns        63818 ns        10957 items_per_second=15.6694k/s
bench_kyber::kem_keygen<2, 3>                 65004 ns        65001 ns        10796 items_per_second=15.3844k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      73261 ns        73259 ns         9550 items_per_second=13.6501k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      97735 ns        97733 ns         7160 items_per_second=10.232k/s
bench_kyber::kem_keygen<3, 2>                 99780 ns        99777 ns         7036 items_per_second=10.0223k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     110666 ns       110664 ns         6322 items_per_second=9.03634k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     151169 ns       151163 ns         4598 items_per_second=6.61536k/s
bench_kyber::kem_keygen<4, 2>                146998 ns       146995 ns         4761 items_per_second=6.80296k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     166235 ns       166231 ns         4217 items_per_second=6.01572k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     219358 ns       219353 ns         3193 items_per_second=4.55886k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-09-30T10:26:18+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.11, 0.13, 0.07
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            5.78 ns         5.78 ns    121091250
bench_kyber::ff_compound_add                   3.47 ns         3.47 ns    201838264
bench_kyber::ff_sub                            6.16 ns         6.16 ns    113562603
bench_kyber::ff_compound_sub                   3.85 ns         3.85 ns    181689918
bench_kyber::ff_neg                           0.891 ns        0.891 ns    786106215
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82600260
bench_kyber::ff_compound_mul                   6.16 ns         6.16 ns    113560415
bench_kyber::ff_inv                            43.0 ns         43.0 ns     18435618
bench_kyber::ff_div                            32.2 ns         32.2 ns     37858435
bench_kyber::ff_exp                             482 ns          482 ns      1438597
bench_kyber::encode<1>                          305 ns          305 ns      2296436 bytes_per_second=100.112M/s
bench_kyber::decode<1>                          224 ns          224 ns      3106310 bytes_per_second=136.073M/s
bench_kyber::encode<4>                         1405 ns         1405 ns       498212 bytes_per_second=86.8947M/s
bench_kyber::decode<4>                         1444 ns         1444 ns       484896 bytes_per_second=84.5568M/s
bench_kyber::encode<5>                         2283 ns         2283 ns       306593 bytes_per_second=66.843M/s
bench_kyber::decode<5>                         2331 ns         2331 ns       300100 bytes_per_second=65.4735M/s
bench_kyber::encode<10>                        4122 ns         4122 ns       169832 bytes_per_second=74.0397M/s
bench_kyber::decode<10>                        4206 ns         4206 ns       166431 bytes_per_second=72.5535M/s
bench_kyber::encode<11>                        4537 ns         4537 ns       154341 bytes_per_second=73.9935M/s
bench_kyber::decode<11>                        4635 ns         4635 ns       151027 bytes_per_second=72.4251M/s
bench_kyber::encode<12>                        4945 ns         4944 ns       141583 bytes_per_second=74.0643M/s
bench_kyber::decode<12>                        5062 ns         5062 ns       138315 bytes_per_second=72.3488M/s
bench_kyber::compress<1>                       6.55 ns         6.55 ns    106895713
bench_kyber::decompress<1>                     6.16 ns         6.16 ns    113568518
bench_kyber::compress<4>                       6.55 ns         6.55 ns    106876487
bench_kyber::decompress<4>                     6.16 ns         6.16 ns    113571337
bench_kyber::compress<5>                       6.55 ns         6.55 ns    106875450
bench_kyber::decompress<5>                     6.16 ns         6.16 ns    113569435
bench_kyber::compress<10>                      6.55 ns         6.55 ns    106880245
bench_kyber::decompress<10>                    6.16 ns         6.16 ns    113566517
bench_kyber::compress<11>                      6.55 ns         6.55 ns    106881815
bench_kyber::decompress<11>                    6.16 ns         6.16 ns    113560245
bench_kyber::ntt                               1718 ns         1718 ns       407465
bench_kyber::intt                              1990 ns         1990 ns       351691
bench_kyber::polymul                            386 ns          386 ns      1814750
bench_kyber::random_data<32>                   1731 ns         1731 ns       404285
bench_kyber::pke_keygen<2, 3>                 48541 ns        48540 ns        14428 items_per_second=20.6018k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          50133 ns        50132 ns        13959 items_per_second=19.9472k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          27981 ns        27980 ns        24998 items_per_second=35.7392k/s
bench_kyber::pke_keygen<3, 2>                 74034 ns        74032 ns         9457 items_per_second=13.5076k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          77820 ns        77818 ns         9005 items_per_second=12.8505k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          39851 ns        39850 ns        17571 items_per_second=25.094k/s
bench_kyber::pke_keygen<4, 2>                109324 ns       109322 ns         6402 items_per_second=9.14731k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         116363 ns       116359 ns         6028 items_per_second=8.59408k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          54278 ns        54277 ns        12888 items_per_second=18.424k/s
bench_kyber::kem_keygen<2, 3>                 53162 ns        53161 ns        13169 items_per_second=18.8109k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      58841 ns        58840 ns        11876 items_per_second=16.9953k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      82004 ns        82002 ns         8530 items_per_second=12.1948k/s
bench_kyber::kem_keygen<3, 2>                 80238 ns        80236 ns         8723 items_per_second=12.4633k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      89241 ns        89239 ns         7788 items_per_second=11.2059k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     123071 ns       123068 ns         5681 items_per_second=8.12557k/s
bench_kyber::kem_keygen<4, 2>                116837 ns       116835 ns         5987 items_per_second=8.55909k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     130569 ns       130567 ns         5365 items_per_second=7.65893k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     177034 ns       177030 ns         3948 items_per_second=5.64875k/s
```

## Usage

`kyber` is written to be a zero-dependency, header-only C++ library such that it's pretty easy-to-use in your project. All you need to do

- Clone `kyber` repository
- Initialize and update git submodule

```bash
cd kyber
git submodule update --init
```

- Previous step is required for importing `sha3`, which is the only dependency of `kyber`

> Find more about my project `sha3`: https://github.com/itzmeanjan/sha3

- Write your program while including [kyber.hpp](./include/kyber.hpp), which includes declarations ( and definitions ) of all required routines in various namespaces
- When compiling your program, let your compiler know where it can find `kyber` and `sha3` headers, which includes their definitions too

```bash
g++ -std=c++20 -I./kyber/include -I./kyber/sha3/include main.cpp
```

Kyber | Namespace | Header
--- | --- | ---
IND-CPA-secure Public Key Encryption | `cpapke::` | [kyber.hpp](./include/kyber.hpp)
IND-CCA-secure Key Encapsulation Mechanism | `ccakem::` | [kyber.hpp](./include/kyber.hpp)

---
---

### Kyber Public Key Encryption (PKE)

- Select Kyber parameter set from table 1 of Kyber [specification](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Kyber-Round3.zip)

- Generate PKE key pair using `cpapke::keygen<k, η1>(...)`
- Encrypt fixed length 32 -bytes message using `cpapke::encrypt<k, η1, η2, du, dv>(...)`
> **Sender uses receiver's public key**
- Decrypt cipher text to extract 32 -bytes message using `cpapke::decrypt<k, du, dv>(...)`
> **Receiver uses its own secret key**

In [pke.cpp](./example/pke.cpp), I show how to use Kyber PKE API, while sticking to Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/pke.cpp && ./a.out

pubkey : f3f36ef3231384482b2bebb806a06aab0b65474b8676c34031312f00ea665b477753ea608d1bac4c104df1ea61136562f3647ffaf1505f260d56846646fa5788e0c91a0461f7f8c9250c080476a400f541f9f308ecd31329c5af0a9a0d713837be1785d49056bb85a156b341a9801a5160a5f1b2b942eb462885257d3b01c904ce86a955adf116f57c8a150a586d0879b9e68478c1bd8043129ed711f504321240272b293a3179c11ac8c310c683fcd7310ca871062b6797368eeb197c34902aec60b07db072d2dc49b2a81156a3917bf0a9b21a56dbdc0346283cdc23885675a94280168e271fd1467180e701f2a20ace2564b3d0449e12250ea388c8932c8b481f56f85f85898bba744f84653c6cc4472d92cd436a4cb85209e4191560a57c5a54aa9e6943302196cde058384c415c3090a1d7c04bdb71d284b6eeb896bc6836a2c189f3fc8bcd7b2aced24bdcb47912aa123c092f38eba877751d4e495c440743bb655761d9ca9d8124a3d72ed59590a806d0410b4ae3a888eb6a91cdf903544c8e58eb3604c10a0c75094adac1d071a5acda805c5b2b0d2535e1d939eab96b14f3b4775c5c651b2182a2723fc3cbd32015a9d98e787ba5cf7b9269f5ac6e55382f9a98d93a6be0c16ee67c6aa3314e5c666f16d45e963cb3525456f2fa69448ca2e2db02c8d458ee2967640771bc9741fb0c342f709c00ba8c4c273712a8cc2e8b8fcdc26e2361663f4c98cbcbb09d730303c118cbf34bf49b8e0c7bb9a49231365a5dc15c4b719bbacbd138a52c3dd0d98f20ecb56ee12008373f4a1b2be55821de4b043142b3bd12c4a2a382cc38cc4e92ca3e96499243be9de89bd1001fec8982f9015cfe4cce4b19317c17c7e591170590a44ed0758d8732c50b4e111c05ab67278d9945a0139d81ab6ff10059ae956ac69244384c82f1238f01ecc567858155aa4e6e1a1b6f2ba840c2c708fc0f73d51f0f92b870c11934a4b5c254147ec804f014bdcbfbc069527255e2c726d29c4503a22c0aba8ed218c7fc240c76bf4400cad2a03e50d53a80e5ba646b452b8b51169025bce7c621804f86d7076a4fbc30b22f5c78ec29381c2d86bc7b477bd6b96ad0e98b3710cefcc5d3a1de

seckey : 14b16127e5a5229271138bb2f31c29b0ab46ead2aded679ea3017132835eefdaac8c3234e6ec8a9f3b269326112c2a62c0374d76d629eb449d0819912c9a11ef0255454526c6893f59fb1229f3289c652fb6398dbb5ba255da9410e4ac8387a8eb271e1467bf4dd37d7ba74e89dab7444c71330744edd5bf12b3668fc48a46d8c5900b0c4d3b0b45f65ad7e30b2d766574194bf109448511297caa1ec46a1105e323e90436b5e853812c2f2bb56e65a083a850473416847aebbcd651ba1487a7da64c1e9e84e19c4bc7da590738b04cc5800deb51851604c17835c40e6a59734076527a153865521ca93ac9a053d2452cd1a081c427647d1add0597e5ee40ebb7814ce0028b8488058b2ce3bd5ab477901463c12ebd14a9b482e7e3ab83c0370b8883dd81a27baf8bca4b7887b77a8a2399f1af5cd9b19a169061a85a98422e884b5416eb5b4176b0b763534807450490864b7cae732bad99cf99a78af13735755b748db7c47d20c8f2cbdbf834673300b0fcb0faee6906da436ee7cba6ac5a73f4b3d2aba1ee84b9a8d74567bb69590c3af25da9ca0006b71e092900978f71a7346c64c90c49f1e4858f4ba406de1abfa09a100807a4a4612ea581e273c29185a1b68d40f0c3b326d690fee871510679f07ba4f8e5064cbe9a2ff22c8e5190436259a3b699178cb4a43c4caac9bb1d89867a7335c5b74010c1019ab61cde6f057eb93af45778012da04c59a28b5d19197c51d07bc4cb43c0217f552be3cca98440a26e84766b643bfca5068a08b28ab44fbda2520688863c9405759b8dee03546f06079d55c58d846b0219eb719a9240c84a71593bca8cda7e4acc060914da436450633ec5c6af8a101149c551ac266c5a96662126869a47cae0abd62318e9ad161bf404b94a9b9a1d55156c0adef86ac9ff93fe216255f1097aaeb0b4845559a07a6a60242d24ba9304c661f0700ff075f9412959f186547dc6b2f900580a08a9c141376540618667cc2dcb4f10672bc69681ef1512e462c7d2c727c38a7f2cbcc09076bb9c66315957692506c1e2855cfc7beef8b7e370bcc81b750357700

plain text : 6ab93cb42d2076c8d52d4675bea257a4bbaeecee13d5a432f53eeabf5af9d6a7
cipher     : 48cab603da063bcecd266f30c024e2139dfda7d67515071018dfe9b483556662
decrypted  : 6ab93cb42d2076c8d52d4675bea257a4bbaeecee13d5a432f53eeabf5af9d6a7
```

### Kyber Key Encapsulation Mechanism (KEM)

- Select which Kyber parameter set to work with
- Generate KEM key pair using `ccakem::keygen<k, η1>(...)`
- Encapsulate shared secret into cipher text using `ccakem::encapsulate<k, η1, η2, du, dv>(...)` 
> **Peer0 uses Peer1's public key**
- Decapsulate shared secret from cipher message using `ccakem::decapsulate<k, η1, η2, du, dv>(...)`
> **Peer1 uses its secret key**

After a round of communication ( i.e. encapsulation and decapsulation ) both communicating parties reach to a key derivation function ( KDF ) seeded with same secret, which is SHAKE256 in this context.

This allows two parties to share a secret value ( of arbitrary byte length ) over insecure channel, which can be used ( in next phase of communication ) as secret key of some symmetric key primitives ( say AES ) which are much efficient & faster for secure communication.

In [kem.cpp](./example/kem.cpp), I keep an example demonstrating usage of Kyber KEM API, while sticking to Kyber512 parameters.

```bash
g++ -std=c++20 -Wall -O3 -I ./include -I ./sha3/include example/kem.cpp && ./a.out

pubkey : 7cd9bea51c20bf654c0239054ef7197da716010294f57b05561882b62ba535fb3ee78792fdbb4bcf0a0058b01df214aa1619862034a94485bc5bdc741a105e0c620eca5696e151147be4c7a92282889866b90ab832335531c471f109945cb66c0fa87270dc579c85301f3665b9db3ad7f02a7ec87c25f6332e58c6ddc6c842daaeede2065e92c59f0c97dac77e68a2262e546ec91704af029ba6e12d76b72d411279690a1233352668050ebc7398d22275b6cca335e11a4bd23f22f9b588036cb8e0ccb4db68c8c5bf52a35c5ae961be8b6084875a1e5721f1a9c95d2c1c7bb7cb04081686b10d10a8c209872ed7e1898db86fd1bc98c5226ee0e4b00822ae0bd4752e4a6bfb54a33aa137266c60c32bbbb4083dfbb62f6874a4147a14b2270e6ad11f872b34f5887e2bfc5acd32cd39323c59d395d6a72eda57365fec37724a69b784828ae7ab858242fdc60737b30b6f025236b4be539818d9e218fb19183cf261e298a1e10737825ba091595be83a510b5609f2b88c75950d6a85b3c49ba781a43ca71a01d8a030d87b5b5528cd8d00cc8758128b90a1727bb815c4be8ce9ce7527c5464b3db0524fff7bb589eca8e392898f342772039d7d8136384034fcb68331b89059fc6682aaad6ad8a080db3f52a4c625771bcd8163c6008802d1528ac45066bca73b0c9b5652ad0ae9cdda4b2e36299c50e868b5200e6534aab924879e5570cc84a80b6b91d5317d792c9ae84c558fa706538880366163d962b25bc41261c57fa8fbc6c4377329ea852f7a179ff8b229a665b7112490d3a45c07b3914c9c804743732bba512562eee6a673034e7c2a2da047b1b68345b0032397648d60693e7d358d0dd2299236a9bf074ccb55a1a17c1af9f333e0425da6164822c2094a39132b561cf632663b7c5de912aca41751589616a6ba919efb06ec55ba2399654054c80f9a0fac1704a7690425b30eb3756ff659c8200739b3ba0024b4758e6b9126a36882691fde3a6157448e3022a413d165fac222895ac5d97654f454b32b0bba708247e3b66bf0226e7136c2a2a62bf3054d842c9b6c83463d84072bc23dfe5cd0bae03f17942ce5c739a07462203548fc755fa216a04696e36018

seckey : 742ac2c8d9b767529caa83a79f279c475680c55b5c78e384c29bb79086bb839b0b96b25976dc9e1e72a478c82e9926c61086760a152cea280d515c3500d920988cc246cab59794627ae6849fd5b3f3022b3ef8b39385074094b06dea6ec8468374a2211a216a2e76cc17645f1594cc81eb066105a78e09c466da0242040d51288a84f913d39ab22c259a9ed0844ba62a95e9a87e082c8fa4c9412667934c0a462a9003027252166a5702c943c31011e2c9d7f945cde1aa7a46cabc007613637dd099afb6e3bf7c810a1cb4b78af0540625521ce093550a927bf42a801171ac563dda933b6fc4ccdc9953c5f7354067810439652eb56ffd785c5e4795a598397ae8a9322660e44cbff44cbc850c6c1a032d684306aea953be750c951284d8676245873ba68613ec156a2d4102fe08555f056b8263c5c6e92acf4603b99aa72525170c9ba2fed00713899a5ca9cf3d6563bf4245bba6986960be52681b0be94901a8915f9ac35b546a78858210da7122f13d998b93b6423fe8c78dc489590de886d984a43e513066b597ffa49022c66cb244cd587539cc369e2f237fd5a198fa9890493493b5fa97169c3956681316b712f6f9b2969cb90bb83618da4d9f653ca10b3a16d045d24409c90076a6598d039a673749b7c522069e3135752506d6a65e6e4c5fc37922095a8bfb6a7fcb20b76d2601c12b06809995c2759fbf329a90a30be2710d9e5391c9d2cd2087a7aa340ffa71ada320b346e9a3a293487294481beb9e447aaf428744cf6458e324359e7a3dfc64bcd737896a721726c1110396026b655feb87ac1634314cf4387ecc6c0b4c8fc4a61148631048ac0afc3ac6b8408bcc5950ade7c573a0b153a620163439bf8386a2a9bb882c35034699c67a480d637fc4f847c4370892e2aee41c30a15307e5b810014215700731f268349521635440caa107bcf2e9a57012ad6f8b29bb312e04d3c2d2340369694080075d09ba617ff9c87d1565b5550c9fbb2f839193be8358737a05f204046023b9dbb735f5104c259c05fb667e6c71599631873ae72d433a84c7f29b33819ba4c371f9f6047cd9bea51c20bf654c0239054ef7197da716010294f57b05561882b62ba535fb3ee78792fdbb4bcf0a0058b01df214aa1619862034a94485bc5bdc741a105e0c620eca5696e151147be4c7a92282889866b90ab832335531c471f109945cb66c0fa87270dc579c85301f3665b9db3ad7f02a7ec87c25f6332e58c6ddc6c842daaeede2065e92c59f0c97dac77e68a2262e546ec91704af029ba6e12d76b72d411279690a1233352668050ebc7398d22275b6cca335e11a4bd23f22f9b588036cb8e0ccb4db68c8c5bf52a35c5ae961be8b6084875a1e5721f1a9c95d2c1c7bb7cb04081686b10d10a8c209872ed7e1898db86fd1bc98c5226ee0e4b00822ae0bd4752e4a6bfb54a33aa137266c60c32bbbb4083dfbb62f6874a4147a14b2270e6ad11f872b34f5887e2bfc5acd32cd39323c59d395d6a72eda57365fec37724a69b784828ae7ab858242fdc60737b30b6f025236b4be539818d9e218fb19183cf261e298a1e10737825ba091595be83a510b5609f2b88c75950d6a85b3c49ba781a43ca71a01d8a030d87b5b5528cd8d00cc8758128b90a1727bb815c4be8ce9ce7527c5464b3db0524fff7bb589eca8e392898f342772039d7d8136384034fcb68331b89059fc6682aaad6ad8a080db3f52a4c625771bcd8163c6008802d1528ac45066bca73b0c9b5652ad0ae9cdda4b2e36299c50e868b5200e6534aab924879e5570cc84a80b6b91d5317d792c9ae84c558fa706538880366163d962b25bc41261c57fa8fbc6c4377329ea852f7a179ff8b229a665b7112490d3a45c07b3914c9c804743732bba512562eee6a673034e7c2a2da047b1b68345b0032397648d60693e7d358d0dd2299236a9bf074ccb55a1a17c1af9f333e0425da6164822c2094a39132b561cf632663b7c5de912aca41751589616a6ba919efb06ec55ba2399654054c80f9a0fac1704a7690425b30eb3756ff659c8200739b3ba0024b4758e6b9126a36882691fde3a6157448e3022a413d165fac222895ac5d97654f454b32b0bba708247e3b66bf0226e7136c2a2a62bf3054d842c9b6c83463d84072bc23dfe5cd0bae03f17942ce5c739a07462203548fc755fa216a04696e36018d0f7d7bc0e71c5f79c61e48fe92255e4c8da0db44a9f0a6caf354513650250362fa33e2ba8008204ac7f85ee15d8a7826431cb9ec5e4405471ebf3331e1e3f8e

shared secret 0 : 9d6ff376e3ca0d89e5a3e464c7116d5a276f4cde8efe7765e9e32871b588246d
shared secret 1 : 9d6ff376e3ca0d89e5a3e464c7116d5a276f4cde8efe7765e9e32871b588246d
```
