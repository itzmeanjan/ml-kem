cmake_minimum_required(VERSION 3.30)

project(ml-kem VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Options ---
option(ML_KEM_BUILD_TESTS "Build tests" OFF)
option(ML_KEM_BUILD_EXAMPLES "Build examples" OFF)
option(ML_KEM_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(ML_KEM_BUILD_FUZZERS "Build fuzzers (requires clang)" OFF)
option(ML_KEM_ENABLE_LTO "Enable Interprocedural Optimization (LTO)" ON)
option(ML_KEM_NATIVE_OPT "Enable -march=native (not suitable for cross-compilation)" OFF)
option(ML_KEM_ASAN "Enable AddressSanitizer" OFF)
option(ML_KEM_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ML_KEM_FETCH_DEPS "Fetch missing dependencies (GTest, Benchmark)" OFF)

# --- Tools ---
find_program(CLANG_TIDY_EXE clang-tidy)
find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_TIDY_EXE)
  file(GLOB_RECURSE TIDY_SOURCES 
       "include/**/*.hpp" 
       "examples/*.cpp")
  
  add_custom_target(tidy
    COMMAND ${CLANG_TIDY_EXE} --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy --header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/ml_kem/.* ${TIDY_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-tidy"
  )
endif()

if(CLANG_FORMAT_EXE)
  file(GLOB_RECURSE FORMAT_SOURCES 
       "include/**/*.hpp" 
       "examples/*.cpp" 
       "benchmarks/*.cpp" 
       "tests/*.cpp")

  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-format"
  )
endif()

# Utility target: Sync ACVP KAT vectors
add_custom_target(sync_acvp_kats
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/kats/scripts/sync_acvp_kats.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/kats/scripts
  COMMENT "Syncing ACVP KAT vectors"
)

# --- Standard settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Compiler Warnings ---
if(MSVC)
  set(ML_KEM_WARNING_FLAGS /W3)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
else()
  set(ML_KEM_WARNING_FLAGS -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Werror)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND ML_KEM_WARNING_FLAGS -Wno-pass-failed)
  endif()
endif()

# --- Sanitizers ---
if(ML_KEM_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
  add_link_options(-fsanitize=address)
endif()

if(ML_KEM_UBSAN)
  add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-sanitize-recover=undefined)
  add_link_options(-fsanitize=undefined)
endif()

# --- Native Optimization ---
if(ML_KEM_NATIVE_OPT)
  add_compile_options(-march=native)
  message(STATUS "Enabled -march=native (machine-specific optimization)")
endif()

# --- Library ---
add_library(ml-kem INTERFACE)
target_include_directories(ml-kem INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sha3/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/subtle/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/RandomShake/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(ml-kem INTERFACE cxx_std_20)

# --- LTO ---
if(ML_KEM_ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)
  if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "LTO is not supported: ${output}")
  endif()
endif()

# --- Submodule based dependencies ---
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(WARNING "git submodule update --init --recursive failed.")
    endif()
  endif()
endif()

# --- Tests ---
if(ML_KEM_BUILD_TESTS)
  enable_testing()
  find_package(GTest QUIET)

  if(NOT GTest_FOUND)
    if(ML_KEM_FETCH_DEPS)
      message(STATUS "GTest not found locally. Fetching...")
      include(FetchContent)
      FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      )
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(googletest)
    else()
      message(FATAL_ERROR "GTest not found. Install it locally or set ML_KEM_FETCH_DEPS=ON")
    endif()
  endif()

  file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
       "tests/kat/*.cpp" 
       "tests/prop/*.cpp"
  )
  
  add_executable(ml_kem_tests ${TEST_SOURCES})
  target_link_libraries(ml_kem_tests PRIVATE ml-kem GTest::gtest_main)
  target_include_directories(ml_kem_tests PRIVATE tests)
  target_compile_options(ml_kem_tests PRIVATE ${ML_KEM_WARNING_FLAGS})
  
  include(GoogleTest)
  gtest_discover_tests(ml_kem_tests)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/kats" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# --- Benchmarks ---
if(ML_KEM_BUILD_BENCHMARKS)
  find_package(benchmark QUIET)

  if(NOT benchmark_FOUND)
    if(ML_KEM_FETCH_DEPS)
      message(STATUS "Google Benchmark not found locally. Fetching...")
      include(FetchContent)
      FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      )
      set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
      set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(benchmark)
    else()
      message(FATAL_ERROR "Google Benchmark not found. Install it locally or set ML_KEM_FETCH_DEPS=ON")
    endif()
  endif()

  file(GLOB BENCHMARK_SOURCES CONFIGURE_DEPENDS "benchmarks/*.cpp")
  find_library(LIBPFM pfm)

  add_executable(ml_kem_benchmarks ${BENCHMARK_SOURCES})
  target_link_libraries(ml_kem_benchmarks PRIVATE ml-kem benchmark::benchmark_main)
  
  if(LIBPFM)
    target_link_libraries(ml_kem_benchmarks PRIVATE ${LIBPFM})
    message(STATUS "Found libpfm: ${LIBPFM} - linking it to benchmarks")
  endif()

  target_include_directories(ml_kem_benchmarks PRIVATE benchmarks)
  target_compile_options(ml_kem_benchmarks PRIVATE ${ML_KEM_WARNING_FLAGS})
endif()

# --- Fuzzers ---
if(ML_KEM_BUILD_FUZZERS)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Fuzzers require Clang compiler")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=undefined")

  set(VARIANTS 512 768 1024)
  set(OPS keygen encaps decaps)

  foreach(variant ${VARIANTS})
    foreach(op ${OPS})
      add_executable(ml_kem_${variant}_${op}_fuzzer tests/fuzz/ml_kem_${op}.cpp)
      target_compile_definitions(ml_kem_${variant}_${op}_fuzzer PRIVATE ML_KEM_${variant})
      target_link_libraries(ml_kem_${variant}_${op}_fuzzer PRIVATE ml-kem)
      target_include_directories(ml_kem_${variant}_${op}_fuzzer PRIVATE tests)
      target_compile_options(ml_kem_${variant}_${op}_fuzzer PRIVATE ${ML_KEM_WARNING_FLAGS})
    endforeach()
  endforeach()

  foreach(fuzzer field_arithmetic poly_ntt poly_serialize poly_compression poly_sampling)
    add_executable(${fuzzer}_fuzzer tests/fuzz/${fuzzer}.cpp)
    target_link_libraries(${fuzzer}_fuzzer PRIVATE ml-kem)
    target_include_directories(${fuzzer}_fuzzer PRIVATE tests)
    target_compile_options(${fuzzer}_fuzzer PRIVATE ${ML_KEM_WARNING_FLAGS})
  endforeach()
endif()

# --- Examples ---
if(ML_KEM_BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "examples/*.cpp")
  foreach(ex_src ${EXAMPLE_SOURCES})
    get_filename_component(ex_name ${ex_src} NAME_WE)
    add_executable(${ex_name} ${ex_src})
    target_link_libraries(${ex_name} PRIVATE ml-kem)
    target_compile_options(${ex_name} PRIVATE ${ML_KEM_WARNING_FLAGS})
  endforeach()
endif()

# --- Install ---
include(GNUInstallDirs)
install(TARGETS ml-kem EXPORT ml-kem-config INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY sha3/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY RandomShake/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY subtle/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ml-kem-config NAMESPACE ml-kem:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ml-kem)
