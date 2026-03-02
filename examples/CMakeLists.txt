cmake_minimum_required(VERSION 3.30)
project(ml-kem-example LANGUAGES CXX)

# --- Fetch ml-kem ---
# If ml-kem is installed on the system, find_package will find it.
# Otherwise, fetch it from the parent directory (or a remote repository).
find_package(ml-kem QUIET)

if(NOT ml-kem_FOUND)
  message(STATUS "ml-kem not found via find_package, using FetchContent...")
  include(FetchContent)
  FetchContent_Declare(
    ml-kem
    # Option A: For local development, point to the parent directory:
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
    # Option B: For production use, point to the GitHub repository:
    #
    # GIT_REPOSITORY https://github.com/itzmeanjan/ml-kem.git
    # GIT_TAG master
    # GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(ml-kem)
endif()

# --- Example Executable ---
add_executable(ml_kem_768_example ml_kem_768.cpp)
target_link_libraries(ml_kem_768_example PRIVATE ml-kem)
target_compile_features(ml_kem_768_example PRIVATE cxx_std_20)
