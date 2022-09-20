#include "test_kyber.hpp"
#include <iostream>

int
main()
{
  test_kyber::test_field_ops();
  std::cout << "[test] Kyber prime field operations" << std::endl;

  test_kyber::test_ntt_intt();
  std::cout << "[test] (i)NTT over degree-255 polynomial R_q | q = 3329"
            << std::endl;

  return EXIT_SUCCESS;
}
