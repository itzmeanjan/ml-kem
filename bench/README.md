## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber implementation ( along with its components ) on CPU systems, you need to issue

```bash
make benchmark
```

> **Warning**

> Because most of the CPUs employ dynamic frequency boosting technique, when benchmarking routines, you may want to disable CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2022-11-01T12:58:37+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.53, 1.82, 2.22
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            3.98 ns         3.98 ns    173441627
bench_kyber::ff_compound_add                   2.09 ns         2.07 ns    358886018
bench_kyber::ff_sub                            4.36 ns         4.35 ns    158630880
bench_kyber::ff_compound_sub                   2.20 ns         2.18 ns    326427069
bench_kyber::ff_neg                           0.806 ns        0.805 ns    854826104
bench_kyber::ff_mul                            6.75 ns         6.74 ns    102879146
bench_kyber::ff_compound_mul                   4.48 ns         4.47 ns    157113422
bench_kyber::ff_inv                             100 ns          100 ns     10000000
bench_kyber::ff_div                            51.1 ns         51.1 ns     10092709
bench_kyber::ff_exp                             410 ns          410 ns      1734696
bench_kyber::parse                             2162 ns         2161 ns       314605
bench_kyber::generate_matrix<2, true>          9293 ns         9284 ns        74171
bench_kyber::generate_matrix<2, false>         9205 ns         9198 ns        75258
bench_kyber::generate_matrix<3, true>         20605 ns        20596 ns        33275
bench_kyber::generate_matrix<3, false>        21074 ns        20982 ns        33803
bench_kyber::generate_matrix<4, true>         36987 ns        36936 ns        19007
bench_kyber::generate_matrix<4, false>        37258 ns        37218 ns        19208
bench_kyber::cbd<2>                            39.3 ns         39.2 ns     17917982
bench_kyber::cbd<3>                            97.9 ns         97.8 ns      7007989
bench_kyber::encode<1>                         44.4 ns         44.4 ns     15793404 bytes_per_second=687.902M/s
bench_kyber::decode<1>                         35.7 ns         35.7 ns     19597797 bytes_per_second=855.24M/s
bench_kyber::encode<4>                         18.0 ns         17.9 ns     39057705 bytes_per_second=6.64392G/s
bench_kyber::decode<4>                         15.9 ns         15.9 ns     44094210 bytes_per_second=7.51432G/s
bench_kyber::encode<5>                         78.1 ns         78.1 ns      8905852 bytes_per_second=1.90839G/s
bench_kyber::decode<5>                         62.1 ns         62.1 ns     11123647 bytes_per_second=2.4008G/s
bench_kyber::encode<10>                         399 ns          398 ns      1764513 bytes_per_second=766.292M/s
bench_kyber::decode<10>                        67.4 ns         67.3 ns     10249502 bytes_per_second=4.42792G/s
bench_kyber::encode<11>                         127 ns          127 ns      5497569 bytes_per_second=2.58286G/s
bench_kyber::decode<11>                         121 ns          121 ns      5671139 bytes_per_second=2.7094G/s
bench_kyber::encode<12>                         192 ns          192 ns      3597751 bytes_per_second=1.86614G/s
bench_kyber::decode<12>                        37.9 ns         37.9 ns     18675382 bytes_per_second=9.44575G/s
bench_kyber::compress<1>                       3.48 ns         3.48 ns    199230401
bench_kyber::decompress<1>                     4.23 ns         4.23 ns    164535540
bench_kyber::compress<4>                       3.47 ns         3.47 ns    199662286
bench_kyber::decompress<4>                     4.21 ns         4.21 ns    166740112
bench_kyber::compress<5>                       3.49 ns         3.48 ns    201197990
bench_kyber::decompress<5>                     4.21 ns         4.21 ns    166470469
bench_kyber::compress<10>                      3.50 ns         3.49 ns    200092042
bench_kyber::decompress<10>                    4.21 ns         4.21 ns    166744878
bench_kyber::compress<11>                      3.54 ns         3.53 ns    198303654
bench_kyber::decompress<11>                    4.22 ns         4.21 ns    166788979
bench_kyber::ntt                               1484 ns         1482 ns       468299
bench_kyber::intt                              1425 ns         1425 ns       491438
bench_kyber::polymul                            369 ns          369 ns      1889803
bench_kyber::random_data<32>                  11687 ns        11670 ns        58157
bench_kyber::pke_keygen<2, 3>                 20972 ns        20958 ns        33039 items_per_second=47.7135k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          22719 ns        22705 ns        28724 items_per_second=44.043k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           5671 ns         5663 ns       111028 items_per_second=176.588k/s
bench_kyber::pke_keygen<3, 2>                 37180 ns        37155 ns        18633 items_per_second=26.9144k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          41523 ns        41334 ns        16629 items_per_second=24.193k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           8107 ns         8071 ns        84272 items_per_second=123.904k/s
bench_kyber::pke_keygen<4, 2>                 63764 ns        63625 ns        10917 items_per_second=15.717k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          65369 ns        65201 ns         9475 items_per_second=15.3373k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          10236 ns        10218 ns        67106 items_per_second=97.8644k/s
bench_kyber::kem_keygen<2, 3>                 23048 ns        23025 ns        30009 items_per_second=43.4318k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      27794 ns        27768 ns        25043 items_per_second=36.0129k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      30928 ns        30912 ns        22176 items_per_second=32.3497k/s
bench_kyber::kem_keygen<3, 2>                 40539 ns        40506 ns        17152 items_per_second=24.6877k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      47923 ns        47879 ns        14828 items_per_second=20.8862k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      51955 ns        51902 ns        12725 items_per_second=19.2671k/s
bench_kyber::kem_keygen<4, 2>                 64043 ns        63997 ns        10380 items_per_second=15.6258k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      71999 ns        71930 ns         9489 items_per_second=13.9025k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      77664 ns        77599 ns         8569 items_per_second=12.8868k/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2022-11-01T09:02:23+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.36, 0.09, 0.03
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            7.83 ns         7.83 ns     89351926
bench_kyber::ff_compound_add                   2.61 ns         2.61 ns    268163220
bench_kyber::ff_sub                            7.83 ns         7.83 ns     89387539
bench_kyber::ff_compound_sub                   2.61 ns         2.61 ns    268173304
bench_kyber::ff_neg                            2.25 ns         2.25 ns    310700374
bench_kyber::ff_mul                            11.7 ns         11.7 ns     59591221
bench_kyber::ff_compound_mul                   6.53 ns         6.53 ns    107266950
bench_kyber::ff_inv                            48.4 ns         48.4 ns     12992777
bench_kyber::ff_div                            35.3 ns         35.3 ns     15751668
bench_kyber::ff_exp                             840 ns          840 ns       846324
bench_kyber::parse                             7866 ns         7866 ns        88972
bench_kyber::generate_matrix<2, true>         32264 ns        32264 ns        21847
bench_kyber::generate_matrix<2, false>        32390 ns        32389 ns        21639
bench_kyber::generate_matrix<3, true>         72586 ns        72584 ns         9642
bench_kyber::generate_matrix<3, false>        72703 ns        72701 ns         9647
bench_kyber::generate_matrix<4, true>        129328 ns       129327 ns         5433
bench_kyber::generate_matrix<4, false>       128551 ns       128549 ns         5416
bench_kyber::cbd<2>                             222 ns          222 ns      3147606
bench_kyber::cbd<3>                             328 ns          328 ns      2133918
bench_kyber::encode<1>                         95.5 ns         95.5 ns      7332494 bytes_per_second=319.677M/s
bench_kyber::decode<1>                          344 ns          344 ns      2035004 bytes_per_second=88.7186M/s
bench_kyber::encode<4>                         78.8 ns         78.8 ns      8887744 bytes_per_second=1.51362G/s
bench_kyber::decode<4>                          108 ns          108 ns      6483824 bytes_per_second=1.10426G/s
bench_kyber::encode<5>                          249 ns          249 ns      2812914 bytes_per_second=613.179M/s
bench_kyber::decode<5>                          271 ns          271 ns      2586122 bytes_per_second=563.774M/s
bench_kyber::encode<10>                         279 ns          279 ns      2510103 bytes_per_second=1094.31M/s
bench_kyber::decode<10>                         343 ns          343 ns      2042620 bytes_per_second=890.514M/s
bench_kyber::encode<11>                         325 ns          325 ns      2156674 bytes_per_second=1034.26M/s
bench_kyber::decode<11>                         352 ns          352 ns      1988130 bytes_per_second=953.444M/s
bench_kyber::encode<12>                         149 ns          149 ns      4710053 bytes_per_second=2.40356G/s
bench_kyber::decode<12>                         151 ns          151 ns      4647373 bytes_per_second=2.37431G/s
bench_kyber::compress<1>                       4.79 ns         4.79 ns    146276236
bench_kyber::decompress<1>                     7.40 ns         7.40 ns     94641368
bench_kyber::compress<4>                       4.79 ns         4.79 ns    146276269
bench_kyber::decompress<4>                     7.40 ns         7.40 ns     94648524
bench_kyber::compress<5>                       4.79 ns         4.79 ns    146274376
bench_kyber::decompress<5>                     7.40 ns         7.40 ns     94644731
bench_kyber::compress<10>                      4.79 ns         4.79 ns    146276819
bench_kyber::decompress<10>                    7.40 ns         7.40 ns     94648078
bench_kyber::compress<11>                      4.79 ns         4.79 ns    146275183
bench_kyber::decompress<11>                    7.40 ns         7.40 ns     94645222
bench_kyber::ntt                               5240 ns         5240 ns       133601
bench_kyber::intt                              5181 ns         5181 ns       135115
bench_kyber::polymul                            845 ns          845 ns       828389
bench_kyber::random_data<32>                   4679 ns         4679 ns       149634
bench_kyber::pke_keygen<2, 3>                 75153 ns        75152 ns         9316 items_per_second=13.3063k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          79989 ns        79988 ns         8744 items_per_second=12.5019k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          19280 ns        19280 ns        36311 items_per_second=51.8666k/s
bench_kyber::pke_keygen<3, 2>                126275 ns       126273 ns         5551 items_per_second=7.91936k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         136362 ns       136359 ns         5119 items_per_second=7.3336k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          26013 ns        26013 ns        26907 items_per_second=38.4421k/s
bench_kyber::pke_keygen<4, 2>                204584 ns       204583 ns         3444 items_per_second=4.88799k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         215498 ns       215495 ns         3207 items_per_second=4.64047k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          32809 ns        32809 ns        21332 items_per_second=30.4792k/s
bench_kyber::kem_keygen<2, 3>                 85759 ns        85759 ns         8209 items_per_second=11.6606k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>     107750 ns       107749 ns         6603 items_per_second=9.28083k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     114161 ns       114158 ns         6123 items_per_second=8.75981k/s
bench_kyber::kem_keygen<3, 2>                141852 ns       141849 ns         4935 items_per_second=7.04976k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     172686 ns       172683 ns         4017 items_per_second=5.79095k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     183051 ns       183048 ns         3830 items_per_second=5.46303k/s
bench_kyber::kem_keygen<4, 2>                223604 ns       223603 ns         3127 items_per_second=4.47221k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     261831 ns       261824 ns         2656 items_per_second=3.81935k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     275633 ns       275632 ns         2543 items_per_second=3.62803k/s
```

### On ARM Cortex-A72 ( compiled with Clang )

```bash
2022-11-01T09:10:09+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.22, 0.12, 0.08
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            8.51 ns         8.51 ns     82926178
bench_kyber::ff_compound_add                   5.55 ns         5.55 ns    127026407
bench_kyber::ff_sub                            8.36 ns         8.36 ns     83752358
bench_kyber::ff_compound_sub                   6.00 ns         6.00 ns    117081322
bench_kyber::ff_neg                            1.74 ns         1.74 ns    402243119
bench_kyber::ff_mul                            12.2 ns         12.2 ns     56914817
bench_kyber::ff_compound_mul                   8.31 ns         8.31 ns     84274572
bench_kyber::ff_inv                            52.0 ns         52.0 ns     12897199
bench_kyber::ff_div                            52.4 ns         52.4 ns     21450425
bench_kyber::ff_exp                             770 ns          770 ns      1054975
bench_kyber::parse                             6262 ns         6262 ns       111814
bench_kyber::generate_matrix<2, true>         26198 ns        26197 ns        26728
bench_kyber::generate_matrix<2, false>        26625 ns        26625 ns        26462
bench_kyber::generate_matrix<3, true>         59504 ns        59501 ns        11803
bench_kyber::generate_matrix<3, false>        59554 ns        59553 ns        11724
bench_kyber::generate_matrix<4, true>        105769 ns       105768 ns         6643
bench_kyber::generate_matrix<4, false>       106321 ns       106318 ns         6587
bench_kyber::cbd<2>                             300 ns          300 ns      2331886
bench_kyber::cbd<3>                             368 ns          368 ns      1899560
bench_kyber::encode<1>                          192 ns          192 ns      3651495 bytes_per_second=159.182M/s
bench_kyber::decode<1>                          218 ns          218 ns      3211419 bytes_per_second=140.008M/s
bench_kyber::encode<4>                         31.8 ns         31.8 ns     21997977 bytes_per_second=3.74623G/s
bench_kyber::decode<4>                         58.7 ns         58.7 ns     11916494 bytes_per_second=2.02959G/s
bench_kyber::encode<5>                          149 ns          148 ns      4713605 bytes_per_second=1027.54M/s
bench_kyber::decode<5>                          193 ns          193 ns      3623820 bytes_per_second=789.941M/s
bench_kyber::encode<10>                         185 ns          185 ns      3779281 bytes_per_second=1.60908G/s
bench_kyber::decode<10>                         176 ns          176 ns      3971060 bytes_per_second=1.68947G/s
bench_kyber::encode<11>                         214 ns          214 ns      3263455 bytes_per_second=1.52834G/s
bench_kyber::decode<11>                         259 ns          259 ns      2704093 bytes_per_second=1.2664G/s
bench_kyber::encode<12>                        79.2 ns         79.2 ns      8840033 bytes_per_second=4.51665G/s
bench_kyber::decode<12>                         112 ns          112 ns      6236059 bytes_per_second=3.18607G/s
bench_kyber::compress<1>                       6.56 ns         6.56 ns    107267226
bench_kyber::decompress<1>                     8.20 ns         8.20 ns     85403715
bench_kyber::compress<4>                       6.61 ns         6.61 ns    106035408
bench_kyber::decompress<4>                     8.20 ns         8.20 ns     85581435
bench_kyber::compress<5>                       7.18 ns         7.18 ns     97527972
bench_kyber::decompress<5>                     8.27 ns         8.27 ns     85018249
bench_kyber::compress<10>                      7.14 ns         7.14 ns     98055630
bench_kyber::decompress<10>                    8.20 ns         8.20 ns     86150097
bench_kyber::compress<11>                      7.49 ns         7.48 ns     95828965
bench_kyber::decompress<11>                    8.22 ns         8.22 ns     85506079
bench_kyber::ntt                               5691 ns         5690 ns       123026
bench_kyber::intt                              5086 ns         5086 ns       137613
bench_kyber::polymul                            882 ns          882 ns       793672
bench_kyber::random_data<32>                   4888 ns         4888 ns       143207
bench_kyber::pke_keygen<2, 3>                 68053 ns        68052 ns        10320 items_per_second=14.6947k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          73295 ns        73295 ns         9544 items_per_second=13.6435k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          22265 ns        22265 ns        31437 items_per_second=44.9136k/s
bench_kyber::pke_keygen<3, 2>                115663 ns       115660 ns         6041 items_per_second=8.64603k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         124915 ns       124915 ns         5605 items_per_second=8.00547k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          30083 ns        30083 ns        23270 items_per_second=33.2413k/s
bench_kyber::pke_keygen<4, 2>                186275 ns       186273 ns         3776 items_per_second=5.36848k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         195347 ns       195341 ns         3590 items_per_second=5.11925k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          38405 ns        38405 ns        18227 items_per_second=26.0383k/s
bench_kyber::kem_keygen<2, 3>                 75857 ns        75856 ns         9214 items_per_second=13.1829k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      95223 ns        95221 ns         7495 items_per_second=10.5018k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>     107170 ns       107170 ns         6556 items_per_second=9.33099k/s
bench_kyber::kem_keygen<3, 2>                127567 ns       127567 ns         5489 items_per_second=7.83904k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     153117 ns       153116 ns         4565 items_per_second=6.531k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     170086 ns       170077 ns         4121 items_per_second=5.8797k/s
bench_kyber::kem_keygen<4, 2>                201657 ns       201652 ns         3464 items_per_second=4.95905k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     231690 ns       231689 ns         3024 items_per_second=4.31614k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     253299 ns       253297 ns         2769 items_per_second=3.94793k/s
```

### On AWS Graviton3 ( compiled with GCC )

```bash
2022-11-01T09:05:14+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.18, 0.05, 0.01
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.17 ns         6.17 ns    113365024
bench_kyber::ff_compound_add                   2.31 ns         2.31 ns    302862107
bench_kyber::ff_sub                            6.21 ns         6.21 ns    112795719
bench_kyber::ff_compound_sub                   2.31 ns         2.31 ns    302855803
bench_kyber::ff_neg                            1.32 ns         1.32 ns    532421787
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82597045
bench_kyber::ff_compound_mul                   4.62 ns         4.62 ns    151388544
bench_kyber::ff_inv                            51.4 ns         51.4 ns     10000000
bench_kyber::ff_div                            24.4 ns         24.4 ns     34073839
bench_kyber::ff_exp                             604 ns          604 ns      1160935
bench_kyber::parse                             3681 ns         3681 ns       190065
bench_kyber::generate_matrix<2, true>         14637 ns        14637 ns        47944
bench_kyber::generate_matrix<2, false>        14624 ns        14623 ns        44635
bench_kyber::generate_matrix<3, true>         33281 ns        33281 ns        21064
bench_kyber::generate_matrix<3, false>        33089 ns        33088 ns        21217
bench_kyber::generate_matrix<4, true>         59053 ns        59052 ns        11796
bench_kyber::generate_matrix<4, false>        58838 ns        58837 ns        11889
bench_kyber::cbd<2>                            55.8 ns         55.7 ns     12557006
bench_kyber::cbd<3>                             102 ns          102 ns      6836846
bench_kyber::encode<1>                         46.2 ns         46.2 ns     15146742 bytes_per_second=660.86M/s
bench_kyber::decode<1>                          125 ns          125 ns      5615531 bytes_per_second=244.999M/s
bench_kyber::encode<4>                         19.2 ns         19.2 ns     36360271 bytes_per_second=6.19797G/s
bench_kyber::decode<4>                         28.6 ns         28.6 ns     24501560 bytes_per_second=4.16713G/s
bench_kyber::encode<5>                         81.7 ns         81.7 ns      8567833 bytes_per_second=1.82408G/s
bench_kyber::decode<5>                          110 ns          110 ns      6382373 bytes_per_second=1.35875G/s
bench_kyber::encode<10>                         102 ns          102 ns      6892848 bytes_per_second=2.93169G/s
bench_kyber::decode<10>                         115 ns          115 ns      6095239 bytes_per_second=2.59419G/s
bench_kyber::encode<11>                         115 ns          115 ns      6104813 bytes_per_second=2.85985G/s
bench_kyber::decode<11>                         132 ns          132 ns      5297263 bytes_per_second=2.48012G/s
bench_kyber::encode<12>                        34.5 ns         34.5 ns     20385394 bytes_per_second=10.3753G/s
bench_kyber::decode<12>                        43.8 ns         43.8 ns     15978514 bytes_per_second=8.16941G/s
bench_kyber::compress<1>                       3.85 ns         3.85 ns    181706164
bench_kyber::decompress<1>                     5.39 ns         5.39 ns    129781087
bench_kyber::compress<4>                       3.85 ns         3.85 ns    181716088
bench_kyber::decompress<4>                     5.39 ns         5.39 ns    129795400
bench_kyber::compress<5>                       3.85 ns         3.85 ns    181714344
bench_kyber::decompress<5>                     5.39 ns         5.39 ns    129793812
bench_kyber::compress<10>                      3.85 ns         3.85 ns    181704078
bench_kyber::decompress<10>                    5.39 ns         5.39 ns    129790705
bench_kyber::compress<11>                      3.85 ns         3.85 ns    181714944
bench_kyber::decompress<11>                    5.39 ns         5.39 ns    129780268
bench_kyber::ntt                               2038 ns         2038 ns       343589
bench_kyber::intt                              2135 ns         2135 ns       327980
bench_kyber::polymul                            296 ns          296 ns      2366542
bench_kyber::random_data<32>                   1663 ns         1663 ns       420025
bench_kyber::pke_keygen<2, 3>                 32694 ns        32694 ns        21414 items_per_second=30.5868k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          33318 ns        33317 ns        21015 items_per_second=30.015k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7429 ns         7429 ns        94258 items_per_second=134.615k/s
bench_kyber::pke_keygen<3, 2>                 53423 ns        53422 ns        13084 items_per_second=18.719k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          58078 ns        58076 ns        12197 items_per_second=17.2189k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>           9940 ns         9940 ns        70421 items_per_second=100.603k/s
bench_kyber::pke_keygen<4, 2>                 86506 ns        86503 ns         8103 items_per_second=11.5603k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          91906 ns        91902 ns         7705 items_per_second=10.8811k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          12601 ns        12600 ns        55546 items_per_second=79.3631k/s
bench_kyber::kem_keygen<2, 3>                 35902 ns        35901 ns        19493 items_per_second=27.8541k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      43784 ns        43782 ns        15994 items_per_second=22.8405k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      47195 ns        47194 ns        14848 items_per_second=21.189k/s
bench_kyber::kem_keygen<3, 2>                 60265 ns        60262 ns        11801 items_per_second=16.5941k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      72573 ns        72571 ns         9779 items_per_second=13.7796k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      76683 ns        76682 ns         9121 items_per_second=13.0409k/s
bench_kyber::kem_keygen<4, 2>                 94530 ns        94526 ns         7403 items_per_second=10.5791k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     109032 ns       109030 ns         6417 items_per_second=9.17179k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     114192 ns       114188 ns         6140 items_per_second=8.75749k/s
```

### On AWS Graviton3 ( compiled with Clang )

```bash
2022-11-01T09:07:38+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.32, 0.22, 0.09
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            5.78 ns         5.78 ns    121106956
bench_kyber::ff_compound_add                   3.47 ns         3.47 ns    201837114
bench_kyber::ff_sub                            6.16 ns         6.16 ns    113558171
bench_kyber::ff_compound_sub                   3.85 ns         3.85 ns    181709503
bench_kyber::ff_neg                           0.891 ns        0.890 ns    786295119
bench_kyber::ff_mul                            8.48 ns         8.48 ns     82591275
bench_kyber::ff_compound_mul                   6.16 ns         6.16 ns    113565092
bench_kyber::ff_inv                            25.5 ns         25.5 ns     22759174
bench_kyber::ff_div                            34.4 ns         34.4 ns     20346439
bench_kyber::ff_exp                             489 ns          489 ns      1456322
bench_kyber::parse                             2326 ns         2326 ns       300896
bench_kyber::generate_matrix<2, true>         10140 ns        10139 ns        69301
bench_kyber::generate_matrix<2, false>        10028 ns        10028 ns        69574
bench_kyber::generate_matrix<3, true>         22270 ns        22270 ns        31365
bench_kyber::generate_matrix<3, false>        22399 ns        22399 ns        31353
bench_kyber::generate_matrix<4, true>         42361 ns        42360 ns        16729
bench_kyber::generate_matrix<4, false>        41695 ns        41694 ns        16855
bench_kyber::cbd<2>                             101 ns          101 ns      6926846
bench_kyber::cbd<3>                             153 ns          153 ns      4563846
bench_kyber::encode<1>                         46.5 ns         46.5 ns     15060330 bytes_per_second=656.625M/s
bench_kyber::decode<1>                         44.4 ns         44.4 ns     15735572 bytes_per_second=686.798M/s
bench_kyber::encode<4>                         9.62 ns         9.62 ns     72871904 bytes_per_second=12.394G/s
bench_kyber::decode<4>                         15.9 ns         15.9 ns     44234277 bytes_per_second=7.50922G/s
bench_kyber::encode<5>                         57.5 ns         57.5 ns     12171148 bytes_per_second=2.58982G/s
bench_kyber::decode<5>                         69.7 ns         69.7 ns     10095784 bytes_per_second=2.13925G/s
bench_kyber::encode<10>                        53.3 ns         53.3 ns     13101569 bytes_per_second=5.58896G/s
bench_kyber::decode<10>                        55.4 ns         55.4 ns     12612631 bytes_per_second=5.37706G/s
bench_kyber::encode<11>                        91.9 ns         91.9 ns      7625590 bytes_per_second=3.56817G/s
bench_kyber::decode<11>                         106 ns          106 ns      6607063 bytes_per_second=3.09577G/s
bench_kyber::encode<12>                        24.4 ns         24.4 ns     28727948 bytes_per_second=14.6837G/s
bench_kyber::decode<12>                        32.7 ns         32.7 ns     21441635 bytes_per_second=10.9525G/s
bench_kyber::compress<1>                       5.01 ns         5.01 ns    139762836
bench_kyber::decompress<1>                     4.62 ns         4.62 ns    151433777
bench_kyber::compress<4>                       5.01 ns         5.01 ns    139773332
bench_kyber::decompress<4>                     4.62 ns         4.62 ns    151419827
bench_kyber::compress<5>                       5.01 ns         5.01 ns    139788126
bench_kyber::decompress<5>                     4.62 ns         4.62 ns    151426354
bench_kyber::compress<10>                      5.01 ns         5.01 ns    139785371
bench_kyber::decompress<10>                    4.62 ns         4.62 ns    151426002
bench_kyber::compress<11>                      5.01 ns         5.01 ns    139762898
bench_kyber::decompress<11>                    4.62 ns         4.62 ns    151422419
bench_kyber::ntt                               1724 ns         1724 ns       406703
bench_kyber::intt                              1993 ns         1993 ns       351298
bench_kyber::polymul                            385 ns          385 ns      1816713
bench_kyber::random_data<32>                   1723 ns         1723 ns       406272
bench_kyber::pke_keygen<2, 3>                 23712 ns        23712 ns        29626 items_per_second=42.1735k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          27534 ns        27534 ns        25910 items_per_second=36.3192k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>           7743 ns         7743 ns        90383 items_per_second=129.144k/s
bench_kyber::pke_keygen<3, 2>                 40882 ns        40881 ns        17207 items_per_second=24.4612k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          45974 ns        45973 ns        15226 items_per_second=21.752k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          10395 ns        10395 ns        67328 items_per_second=96.2015k/s
bench_kyber::pke_keygen<4, 2>                 67457 ns        67456 ns        10346 items_per_second=14.8246k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>          73381 ns        73379 ns         9316 items_per_second=13.6279k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          13234 ns        13234 ns        52903 items_per_second=75.5635k/s
bench_kyber::kem_keygen<2, 3>                 26296 ns        26296 ns        26468 items_per_second=38.029k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      34453 ns        34451 ns        20619 items_per_second=29.0266k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      38807 ns        38807 ns        18019 items_per_second=25.7689k/s
bench_kyber::kem_keygen<3, 2>                 45580 ns        45579 ns        15557 items_per_second=21.9399k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      55821 ns        55820 ns        12537 items_per_second=17.9149k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      61482 ns        61480 ns        11269 items_per_second=16.2654k/s
bench_kyber::kem_keygen<4, 2>                 73308 ns        73306 ns         9588 items_per_second=13.6414k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>      86185 ns        86183 ns         8153 items_per_second=11.6032k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>      93007 ns        93005 ns         7474 items_per_second=10.7522k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2022-11-01T09:12:53+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.30, 0.08, 0.02
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.73 ns         6.73 ns    103909403
bench_kyber::ff_compound_add                   2.97 ns         2.97 ns    235666360
bench_kyber::ff_sub                            6.70 ns         6.70 ns    104436966
bench_kyber::ff_compound_sub                   2.97 ns         2.97 ns    235676010
bench_kyber::ff_neg                           0.917 ns        0.917 ns    764622674
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64824143
bench_kyber::ff_compound_mul                   7.06 ns         7.06 ns     99116660
bench_kyber::ff_inv                            59.5 ns         59.4 ns      8608749
bench_kyber::ff_div                             104 ns          104 ns      9850444
bench_kyber::ff_exp                             800 ns          800 ns       893689
bench_kyber::parse                             4749 ns         4749 ns       147400
bench_kyber::generate_matrix<2, true>         20687 ns        20687 ns        32113
bench_kyber::generate_matrix<2, false>        20724 ns        20723 ns        33801
bench_kyber::generate_matrix<3, true>         46667 ns        46667 ns        14920
bench_kyber::generate_matrix<3, false>        46868 ns        46868 ns        15089
bench_kyber::generate_matrix<4, true>         83370 ns        83371 ns         8329
bench_kyber::generate_matrix<4, false>        82678 ns        82677 ns         8470
bench_kyber::cbd<2>                            69.4 ns         69.4 ns     10076878
bench_kyber::cbd<3>                             155 ns          155 ns      4503816
bench_kyber::encode<1>                         54.7 ns         54.6 ns     13055642 bytes_per_second=558.431M/s
bench_kyber::decode<1>                          292 ns          292 ns      2394225 bytes_per_second=104.396M/s
bench_kyber::encode<4>                         32.5 ns         32.5 ns     21504282 bytes_per_second=3.66394G/s
bench_kyber::decode<4>                         29.1 ns         29.1 ns     24059595 bytes_per_second=4.09791G/s
bench_kyber::encode<5>                          171 ns          171 ns      4098817 bytes_per_second=892.796M/s
bench_kyber::decode<5>                          216 ns          216 ns      3232425 bytes_per_second=705.226M/s
bench_kyber::encode<10>                         210 ns          210 ns      3339801 bytes_per_second=1.42123G/s
bench_kyber::decode<10>                         292 ns          292 ns      2396666 bytes_per_second=1045.76M/s
bench_kyber::encode<11>                         277 ns          277 ns      2532126 bytes_per_second=1.18243G/s
bench_kyber::decode<11>                         284 ns          284 ns      2467984 bytes_per_second=1.15557G/s
bench_kyber::encode<12>                        84.6 ns         84.6 ns      8277791 bytes_per_second=4.22556G/s
bench_kyber::decode<12>                        87.4 ns         87.4 ns      8003627 bytes_per_second=4.09043G/s
bench_kyber::compress<1>                       4.83 ns         4.83 ns    145015268
bench_kyber::decompress<1>                     5.94 ns         5.94 ns    117859987
bench_kyber::compress<4>                       4.48 ns         4.48 ns    156321883
bench_kyber::decompress<4>                     5.94 ns         5.94 ns    117859174
bench_kyber::compress<5>                       4.13 ns         4.13 ns    169597992
bench_kyber::decompress<5>                     5.94 ns         5.94 ns    117848945
bench_kyber::compress<10>                      4.47 ns         4.47 ns    156449987
bench_kyber::decompress<10>                    5.94 ns         5.94 ns    117862162
bench_kyber::compress<11>                      4.47 ns         4.47 ns    156462995
bench_kyber::decompress<11>                    5.94 ns         5.94 ns    117750877
bench_kyber::ntt                               3282 ns         3282 ns       213146
bench_kyber::intt                              5207 ns         5207 ns       134493
bench_kyber::polymul                            632 ns          632 ns      1109547
bench_kyber::random_data<32>                   3277 ns         3277 ns       213283
bench_kyber::pke_keygen<2, 3>                 48398 ns        48398 ns        14374 items_per_second=20.6619k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          59286 ns        59285 ns        12036 items_per_second=16.8678k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          14252 ns        14252 ns        48861 items_per_second=70.1672k/s
bench_kyber::pke_keygen<3, 2>                 84221 ns        84217 ns         8299 items_per_second=11.8741k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>         100385 ns       100375 ns         6991 items_per_second=9.96264k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          18649 ns        18648 ns        37581 items_per_second=53.6265k/s
bench_kyber::pke_keygen<4, 2>                136006 ns       135999 ns         5154 items_per_second=7.35297k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         156468 ns       156461 ns         4473 items_per_second=6.39135k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          23433 ns        23432 ns        29886 items_per_second=42.6764k/s
bench_kyber::kem_keygen<2, 3>                 55636 ns        55634 ns        12611 items_per_second=17.9746k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      77838 ns        77833 ns         9145 items_per_second=12.848k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      83294 ns        83293 ns         8396 items_per_second=12.0058k/s
bench_kyber::kem_keygen<3, 2>                 94929 ns        94928 ns         7363 items_per_second=10.5343k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>     125472 ns       125472 ns         5593 items_per_second=7.96993k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>     133954 ns       133950 ns         5236 items_per_second=7.46549k/s
bench_kyber::kem_keygen<4, 2>                149831 ns       149826 ns         4663 items_per_second=6.67439k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     188777 ns       188771 ns         3705 items_per_second=5.29743k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     200172 ns       200156 ns         3495 items_per_second=4.9961k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2022-11-01T09:15:04+00:00
Running ./bench/a.out
Run on (4 X 2300.15 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.51, 0.28, 0.11
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
bench_kyber::ff_add                            6.75 ns         6.74 ns    103753961
bench_kyber::ff_compound_add                   3.35 ns         3.35 ns    208965726
bench_kyber::ff_sub                            7.10 ns         7.10 ns     98520875
bench_kyber::ff_compound_sub                   3.78 ns         3.78 ns    185154523
bench_kyber::ff_neg                            1.11 ns         1.11 ns    628782520
bench_kyber::ff_mul                            10.8 ns         10.8 ns     64990397
bench_kyber::ff_compound_mul                   7.43 ns         7.43 ns     94212483
bench_kyber::ff_inv                            97.3 ns         97.3 ns     11756774
bench_kyber::ff_div                             130 ns          130 ns     10876137
bench_kyber::ff_exp                             627 ns          627 ns      1170213
bench_kyber::parse                             3816 ns         3816 ns       183319
bench_kyber::generate_matrix<2, true>         16706 ns        16705 ns        46910
bench_kyber::generate_matrix<2, false>        17545 ns        17545 ns        43886
bench_kyber::generate_matrix<3, true>         33348 ns        33349 ns        19635
bench_kyber::generate_matrix<3, false>        35701 ns        35700 ns        20427
bench_kyber::generate_matrix<4, true>         63136 ns        63136 ns        11112
bench_kyber::generate_matrix<4, false>        63677 ns        63672 ns        11405
bench_kyber::cbd<2>                            73.3 ns         73.3 ns      9556771
bench_kyber::cbd<3>                             166 ns          166 ns      4214236
bench_kyber::encode<1>                         37.2 ns         37.2 ns     18813398 bytes_per_second=821.368M/s
bench_kyber::decode<1>                         61.4 ns         61.4 ns     11416905 bytes_per_second=497.257M/s
bench_kyber::encode<4>                         47.9 ns         47.9 ns     14597853 bytes_per_second=2.48694G/s
bench_kyber::decode<4>                         35.7 ns         35.7 ns     19603871 bytes_per_second=3.33915G/s
bench_kyber::encode<5>                         72.3 ns         72.3 ns      9718351 bytes_per_second=2.06188G/s
bench_kyber::decode<5>                         83.6 ns         83.6 ns      8371618 bytes_per_second=1.78201G/s
bench_kyber::encode<10>                         217 ns          217 ns      3225364 bytes_per_second=1.37327G/s
bench_kyber::decode<10>                         104 ns          104 ns      6719658 bytes_per_second=2.86156G/s
bench_kyber::encode<11>                         192 ns          192 ns      3634980 bytes_per_second=1.70343G/s
bench_kyber::decode<11>                         182 ns          182 ns      3855317 bytes_per_second=1.80495G/s
bench_kyber::encode<12>                         113 ns          113 ns      6200497 bytes_per_second=3.16732G/s
bench_kyber::decode<12>                        70.8 ns         70.8 ns      9877692 bytes_per_second=5.04965G/s
bench_kyber::compress<1>                       5.96 ns         5.96 ns    117443723
bench_kyber::decompress<1>                     7.06 ns         7.06 ns     99233209
bench_kyber::compress<4>                       5.99 ns         5.99 ns    116991216
bench_kyber::decompress<4>                     7.06 ns         7.06 ns     99145392
bench_kyber::compress<5>                       5.99 ns         5.99 ns    116857105
bench_kyber::decompress<5>                     7.06 ns         7.06 ns     99267713
bench_kyber::compress<10>                      5.99 ns         5.99 ns    116999861
bench_kyber::decompress<10>                    7.06 ns         7.06 ns     99227109
bench_kyber::compress<11>                      5.99 ns         5.99 ns    116992145
bench_kyber::decompress<11>                    7.05 ns         7.05 ns     99236562
bench_kyber::ntt                               2508 ns         2508 ns       279155
bench_kyber::intt                              2465 ns         2465 ns       283828
bench_kyber::polymul                            742 ns          742 ns       943307
bench_kyber::random_data<32>                   3381 ns         3381 ns       206916
bench_kyber::pke_keygen<2, 3>                 37080 ns        37078 ns        19278 items_per_second=26.9701k/s
bench_kyber::encrypt<2, 3, 2, 10, 4>          40018 ns        40017 ns        17657 items_per_second=24.9897k/s
bench_kyber::decrypt<2, 3, 2, 10, 4>          10118 ns        10118 ns        69141 items_per_second=98.8376k/s
bench_kyber::pke_keygen<3, 2>                 64048 ns        64043 ns        10827 items_per_second=15.6144k/s
bench_kyber::encrypt<3, 2, 2, 10, 4>          66479 ns        66474 ns        10458 items_per_second=15.0434k/s
bench_kyber::decrypt<3, 2, 2, 10, 4>          13810 ns        13810 ns        50689 items_per_second=72.4133k/s
bench_kyber::pke_keygen<4, 2>                103526 ns       103524 ns         6823 items_per_second=9.65955k/s
bench_kyber::encrypt<4, 2, 2, 11, 5>         113624 ns       113619 ns         6177 items_per_second=8.80135k/s
bench_kyber::decrypt<4, 2, 2, 11, 5>          17613 ns        17612 ns        39752 items_per_second=56.7802k/s
bench_kyber::kem_keygen<2, 3>                 40115 ns        40114 ns        17703 items_per_second=24.9288k/s
bench_kyber::encapsulate<2, 3, 2, 10, 4>      46529 ns        46528 ns        14956 items_per_second=21.4926k/s
bench_kyber::decapsulate<2, 3, 2, 10, 4>      52913 ns        52912 ns        13329 items_per_second=18.8993k/s
bench_kyber::kem_keygen<3, 2>                 69023 ns        69016 ns        10145 items_per_second=14.4895k/s
bench_kyber::encapsulate<3, 2, 2, 10, 4>      83534 ns        83529 ns         8731 items_per_second=11.9719k/s
bench_kyber::decapsulate<3, 2, 2, 10, 4>      89694 ns        89689 ns         7736 items_per_second=11.1496k/s
bench_kyber::kem_keygen<4, 2>                111698 ns       111695 ns         6391 items_per_second=8.95296k/s
bench_kyber::encapsulate<4, 2, 2, 11, 5>     122291 ns       122290 ns         5606 items_per_second=8.17727k/s
bench_kyber::decapsulate<4, 2, 2, 11, 5>     131389 ns       131389 ns         5076 items_per_second=7.611k/s
```
