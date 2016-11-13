#include <iostream>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <map>
#include "utils.hpp"

std::vector<int> bits_set{1, 8, 16, 32, 64, 128, 256};
// std::vector<int> num_iters{100, 100, 50, 20, 10, 10, 10};
std::map<int, int> num_iters{{1, 200}, {8, 100}, {16, 50}, {32, 25}, {64, 10}, {128, 5}, {256, 3}};

std::vector<int> num_samples_set{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
// std::vector<int> num_samples_set{1000, 2000};
auto all_labels_codes = hash::GenerateLabelsCodes();

double GetMAP(int num_samples, int bits) {
  // int num_iters = 10;
  double map = 0.0;
  for (int i = 0; i < num_iters[bits]; i++) {
    double current = hash::Test(num_samples, hash::Sample(all_labels_codes, bits), 100, 1);
    if (current > map) {
      map = current;
      std::cout << num_samples << ' ' << bits << ' ' << map << std::endl;
    }
  }
  return map;
}
int main(int argc, char* argv[]) {
  hash::ReadTestData();
  
  std::ofstream ofs("figure/results.csv");
  if (!ofs.is_open()) {
    std::cout << "open file error" << std::endl;
    exit(-1);
  }
  ofs << "Number of Training Samples," << *bits_set.begin();
  for (auto it = ++(bits_set.begin()); it != bits_set.end(); ++it) {
    ofs << "," << *it;
  }
  ofs << std::endl;

  for (auto num_samples : num_samples_set) {
    ofs << num_samples << ",";
    // ofs << hash::Test(num_samples, hash::Sample(all_labels_codes, *bits_set.begin()), 100, 1);
    ofs << GetMAP(num_samples, *bits_set.begin());
    for (auto it = ++(bits_set.begin()); it != bits_set.end(); ++it) {
      // ofs << "," << hash::Test(num_samples, hash::Sample(all_labels_codes, *it), 100, 1);
      ofs << "," << GetMAP(num_samples, *it);
    }
    ofs << std::endl;
  }
  return 0;
}

// #include <iostream>
// #include <initializer_list>

// #include "utils.hpp"

// int main(int argc, char* argv[]) {
//   if (argc < 5) {
//     std::cout << argv[0] << " num_samples C bits test_times" << std::endl;
//     exit(-1);
//   }

//   int num_samples = std::atoi(argv[1]);
//   int C = std::atoi(argv[2]);
//   int bits = std::atoi(argv[3]);
//   int test_times = std::atoi(argv[4]);
//   hash::ReadTestData();
//   // std::vector<std::vector<std::uint8_t>> labels_codes{{1,0,0,0,1,0,0,0,0,0}};
//   auto all_labels_codes = hash::GenerateLabelsCodes();
//   auto labels_codes = hash::Sample(all_labels_codes, bits);

//   double mean_average_precision = 0.0;
//   for (int i = 0; i < test_times; i++) {
//     mean_average_precision += hash::Test(num_samples, labels_codes, 100, C);
//   }
//   mean_average_precision /= test_times;
//   std::cout << "mean average precision: " << mean_average_precision << std::endl;
//   return 0;
// }