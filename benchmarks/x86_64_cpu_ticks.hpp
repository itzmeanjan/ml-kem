#pragma once

#ifdef __x86_64__

#include <cstdint>
#include <emmintrin.h>
#include <x86intrin.h>

// Issues x86_64 architecture specific intrinsic for obtaining CPU ticks passed by, while executing a set of instructions. For example
//
// start = cpu_ticks()
// {
// ... bunch
// ... of
// ... instructions
// }
// end = cpu_ticks()
//
// CPU ticks passed by executing above code block = end - start
static inline uint64_t
cpu_ticks()
{
  _mm_mfence();
  return __rdtsc();
}

#endif
