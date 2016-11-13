#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <iterator>

#include "utils.hpp"

int main(int argc, char* argv[]) {
  hash::ReadTestData();
  // std::vector<std::vector<std::uint8_t>> labels_codes{{1,0,0,0,1,0,0,0,0,0}};
  // auto all_labels_codes = hash::GenerateLabelsCodes();
  // auto labels_codes = hash::Sample(all_labels_codes, std::atoi(argv[3]));
  std::vector<std::vector<std::uint8_t>> labels_codes;
  std::vector<std::uint8_t> init{0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
  do {
    labels_codes.push_back(init);
  } while (std::next_permutation(init.begin(), init.end()));
  // for (std::size_t i = 0; i < labels_codes.size(); i++) {
  //   std::cout << i;
  //   std::copy(labels_codes[i].begin(), labels_codes[i].end(), std::ostream_iterator<int>(std::cout, " "));
  //   std::cout << std::endl;
  // }
  std::cout << hash::Test(std::atoi(argv[1]), labels_codes, 100, std::atoi(argv[2])) << std::endl;
  return 0;
}