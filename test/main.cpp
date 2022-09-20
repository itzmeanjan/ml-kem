#include "test_ff.hpp"
#include <iostream>

int
main()
{
  test_ff::test_field_ops();
  std::cout << "[test] Kyber Prime Field Operations" << std::endl;

  return EXIT_SUCCESS;
}
