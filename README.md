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
