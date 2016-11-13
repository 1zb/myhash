#include <iostream>
#include <initializer_list>

#include "utils.hpp"

int main(int argc, char* argv[]) {
  if (argc < 5) {
    std::cout << argv[0] << " num_samples C bits test_times" << std::endl;
    exit(-1);
  }

  int num_samples = std::atoi(argv[1]);
  int C = std::atoi(argv[2]);
  int bits = std::atoi(argv[3]);
  int test_times = std::atoi(argv[4]);
  hash::ReadTestData();
  // std::vector<std::vector<std::uint8_t>> labels_codes{{1,0,0,0,1,0,0,0,0,0}};
  auto all_labels_codes = hash::GenerateLabelsCodes();
  auto labels_codes = hash::Sample(all_labels_codes, bits);

  double mean_average_precision = 0.0;
  for (int i = 0; i < test_times; i++) {
    mean_average_precision += hash::Test(num_samples, labels_codes, 100, C);
  }
  mean_average_precision /= test_times;
  std::cout << "mean average precision: " << mean_average_precision << std::endl;
  return 0;
}