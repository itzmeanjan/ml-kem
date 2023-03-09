#include "test/test_kyber.hpp"
#include <iostream>

int
main()
{
  test_kyber::test_field_ops();
  std::cout << "[test] Kyber prime field operations" << std::endl;

  test_kyber::test_ntt_intt();
  std::cout << "[test] (i)NTT over degree-255 polynomial" << std::endl;

  test_kyber::test_serialization<12>();
  test_kyber::test_serialization<11>();
  test_kyber::test_serialization<10>();
  test_kyber::test_serialization<5>();
  test_kyber::test_serialization<4>();
  test_kyber::test_serialization<1>();
  std::cout << "[test] Polynomial serialization/ deserialization" << std::endl;

  test_kyber::test_compression<11>();
  test_kyber::test_compression<10>();
  test_kyber::test_compression<5>();
  test_kyber::test_compression<4>();
  test_kyber::test_compression<1>();
  std::cout << "[test] Coefficient compression/ decompression" << std::endl;

  test_kyber::test_kyber_cpa_pke<2, 3, 2, 10, 4>(); // kyber-512
  test_kyber::test_kyber_cpa_pke<3, 2, 2, 10, 4>(); // kyber-768
  test_kyber::test_kyber_cpa_pke<4, 2, 2, 11, 5>(); // kyber-1024
  std::cout << "[test] INDCPA-secure Public Key Encryption" << std::endl;

  test_kyber::test_kyber_cca_kem<2, 3, 2, 10, 4, 32>(); // kyber-512, 32B -key
  test_kyber::test_kyber_cca_kem<3, 2, 2, 10, 4, 32>(); // kyber-768, 32B -key
  test_kyber::test_kyber_cca_kem<4, 2, 2, 11, 5, 32>(); // kyber-1024, 32B -key
  test_kyber::test_kyber_cca_kem<2, 3, 2, 10, 4, 48>(); // kyber-512, 48B -key
  test_kyber::test_kyber_cca_kem<3, 2, 2, 10, 4, 48>(); // kyber-768, 48B -key
  test_kyber::test_kyber_cca_kem<4, 2, 2, 11, 5, 48>(); // kyber-1024, 48B -key
  test_kyber::test_kyber_cca_kem<2, 3, 2, 10, 4, 64>(); // kyber-512, 64B -key
  test_kyber::test_kyber_cca_kem<3, 2, 2, 10, 4, 64>(); // kyber-768, 64B -key
  test_kyber::test_kyber_cca_kem<4, 2, 2, 11, 5, 64>(); // kyber-1024, 64B -key
  std::cout << "[test] INDCCA2-secure Key Encapsulation Mechanism" << std::endl;

  return EXIT_SUCCESS;
}
