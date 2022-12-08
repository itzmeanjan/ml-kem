#pragma once

#if defined __x86_64__

#include <x86intrin.h>

// x86_64 architecture specific compiler built-in routine for computing CPU
// cycles spent executing some instructions. For example
//
// start = cpu_cycles()
// {
// ... bunch of instructions ...
// }
// end = cpu_cycles()
//
// CPU cycles spent executing code block = end - start
//
// Taken from
// https://github.com/itzmeanjan/sphincs/blob/44ac97c515261c9db75036eabd3d517efe5fed97/include/bench/x86_64_cpu_cycles.hpp
inline static uint64_t
cpu_cycles()
{
  return __rdtsc();
}

#endif
