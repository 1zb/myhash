#include <iostream>
#include <numeric>
// #include <initializer_list>
#include <chrono>

#include "utils.hpp"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << argv[0] << " num_samples C" << std::endl;
    exit(-1);
  }

  int num_samples = std::atoi(argv[1]);
  int C = std::atoi(argv[2]);

  hash::ReadTrainData();
  std::vector<int> samples(num_samples);
  std::iota(samples.begin(), samples.end(), 0);
  // std::vector<std::vector<std::uint8_t>> labels_codes{{1,0,0,0,1,0,0,0,0,0}};
  auto labels_codes = hash::GenerateLabelsCodes();
  hash::Train(labels_codes, samples, C);
  return 0;
}
