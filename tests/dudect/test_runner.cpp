#include "test_kem.hpp"
#include <cstdlib>

int
main()
{
  if (test_kyber512_keygen() != DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
