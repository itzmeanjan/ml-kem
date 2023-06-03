#include "test/test_kyber.hpp"
#include <iostream>

int
main()
{
  test_kyber::test_field_ops();
  std::cout << "[test] Kyber prime field operations\n";

  test_kyber::test_ntt_intt();
  std::cout << "[test] (i)NTT over degree-255 polynomial\n";

  test_kyber::test_serialization<12>();
  test_kyber::test_serialization<11>();
  test_kyber::test_serialization<10>();
  test_kyber::test_serialization<5>();
  test_kyber::test_serialization<4>();
  test_kyber::test_serialization<1>();
  std::cout << "[test] Polynomial serialization/ deserialization\n";

  test_kyber::test_compression<11>();
  test_kyber::test_compression<10>();
  test_kyber::test_compression<5>();
  test_kyber::test_compression<4>();
  test_kyber::test_compression<1>();
  std::cout << "[test] Coefficient compression/ decompression\n";

  test_kyber::test_kyber_kem<2, 3, 2, 10, 4, 32>(); // kyber-512, 32B -key
  test_kyber::test_kyber_kem<3, 2, 2, 10, 4, 32>(); // kyber-768, 32B -key
  test_kyber::test_kyber_kem<4, 2, 2, 11, 5, 32>(); // kyber-1024, 32B -key
  std::cout << "[test] INDCCA2-secure Kyber KEM\n";

  test_kyber::test_kyber512_kem_kat();
  test_kyber::test_kyber768_kem_kat();
  test_kyber::test_kyber1024_kem_kat();
  std::cout << "[test] Kyber KEM Known Answer Tests\n";

  return EXIT_SUCCESS;
}
