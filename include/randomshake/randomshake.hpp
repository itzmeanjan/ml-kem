#pragma once

// Address MSVC assembly incompatibility in RandomShake dependency without touching submodule code.
// See: https://github.com/itzmeanjan/ml-kem/issues (Windows CI Fix)

#ifdef _MSC_VER
#define asm(...)
#define volatile
#endif

// Include the actual submodule header using a relative path to bypass the interceptor.
#include "../../RandomShake/include/randomshake/randomshake.hpp"

#ifdef _MSC_VER
#undef asm
#undef volatile
#endif
