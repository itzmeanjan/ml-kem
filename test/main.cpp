#include "test_ff.hpp"
#include "test_ntt.hpp"
#include <iostream>

int
main()
{
  test_ff::test_field_ops();
  std::cout << "[test] Kyber prime field operations" << std::endl;

  test_ntt::test_ntt_intt();
  std::cout << "[test] (i)NTT over degree-255 polynomial R_q | q = 3329"
            << std::endl;

  return EXIT_SUCCESS;
}
