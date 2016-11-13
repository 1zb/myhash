#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <iterator>

#include "utils.hpp"

std::vector<int> num_samples_set{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

int main(int argc, char* argv[]) {
  hash::ReadTestData();
  // std::vector<std::vector<std::uint8_t>> labels_codes{{1,0,0,0,1,0,0,0,0,0}};
  // auto all_labels_codes = hash::GenerateLabelsCodes();
  // auto labels_codes = hash::Sample(all_labels_codes, std::atoi(argv[3]));
  std::vector<std::vector<std::uint8_t>> labels_codes;
  // std::vector<std::uint8_t> init4{0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
  // do {
  //   labels_codes.push_back(init4);
  // } while (std::next_permutation(init4.begin(), init4.end()));
  std::vector<std::vector<std::uint8_t>> init_set{
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
      {0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
  };

  for (auto init : init_set) {
    do {
      labels_codes.push_back(init);
    } while (std::next_permutation(init.begin(), init.end()));
  }


  std::cout << labels_codes.size() << std::endl;
  for (auto num_samples : num_samples_set) {
    std::cout << hash::Test(num_samples, labels_codes, 100, 1) << std::endl;
  }
  return 0;
}