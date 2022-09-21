#pragma once
#include <cstddef>
#include <cstdint>
#include <random>
#include <type_traits>

// Generates N -many random values of type T | N >= 0
template<typename T>
static inline void
random_data(T* const data, const size_t len) requires(std::is_unsigned_v<T>)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}
