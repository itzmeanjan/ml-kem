# ML-KEM Example

A standalone example project demonstrating how to use the [ml-kem](https://github.com/itzmeanjan/ml-kem) library.

## Quick Start

```bash
cd examples

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/ml_kem_768
```

## Using `ml-kem` in Your Own Project

The simplest way to integrate `ml-kem` into your CMake project is via `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.30)
project(my_app LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  ml-kem
  GIT_REPOSITORY https://github.com/itzmeanjan/ml-kem.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ml-kem)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ml-kem)
target_compile_features(my_app PRIVATE cxx_std_20)
```

Alternatively, install `ml-kem` system-wide and use `find_package`:

```bash
# From the ml-kem root directory
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

```cmake
find_package(ml-kem REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ml-kem)
```
