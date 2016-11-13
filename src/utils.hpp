#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <vector>
#include <cstdint>  // std::uint8_t
#include <bitset>

#include "linear.h"

namespace hash {

// extern const int kTrainImages = 10000;
// extern const int kTestImages = 2000;
// extern const int kDimFeatures = 960;
// extern const int kNumLabels = 10;
extern const int kTrainImages;
extern const int kTestImages;
extern const int kDimFeatures;
extern const int kNumLabels;

// uniform distribution [0,1]
double GetUniform();

// sample without replacement
template <typename T>
std::vector<T> Sample(std::vector<T> population, int sample_size);
std::vector<int> Sample(int population_size, int sample_size);
// input
void ReadTrainData();
void ReadTestData();

// codes
std::vector<std::uint8_t> operator^(std::vector<std::uint8_t> lhs, const std::vector<std::uint8_t>& rhs);
std::size_t GetHammingDistance(const std::vector<std::uint8_t>& x, const std::vector<std::uint8_t>& y);
std::vector<std::vector<std::uint8_t>> GenerateLabelsCodes();
int ConvertLabelsCode2Int(std::vector<std::uint8_t>);

// sort
template<class ForwardIterator>
std::vector<std::size_t> SortIndexes(ForwardIterator first, ForwardIterator last);

// train
model* TrainLinear(const std::vector<std::uint8_t>& labels_code, const std::vector<int>& index, int C);
std::vector<model*> Train(const std::vector<std::vector<std::uint8_t>>& labels_codes, std::vector<int> samples, int C);

// test
double Test(int num_samples, const std::vector<std::vector<std::uint8_t>>& labels_codes, int K, int C);

// debug
void DebugGetHammingDistance();
void DebugGenerateLabelsCodes();
void DebugConvertLabelsCode2Int();
}
#endif