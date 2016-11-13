#include "utils.hpp"

#include <iostream>
#include <random>  // std::mt19937
#include <fstream>  // std::ifstream
#include <algorithm>  // std::count()
#include <iterator>  // std::ostream_iterator()
#include <chrono>
#include <numeric>  // std::iota

namespace hash {

const int kTrainImages = 50000;
const int kTestImages = 2000;
const int kDimFeatures = 320;
const int kNumLabels = 10;

static std::random_device rd;
static std::mt19937 re(rd());

float train_data[kTrainImages][kDimFeatures];
int train_label[kTrainImages];

float test_data[kTestImages][kDimFeatures];
int test_label[kTestImages];

std::uint8_t dist[kTestImages][kTestImages];

void ReadTrainData() {
  // std::ifstream train_ifs("data/cifar-10-batches-bin/data_batch_1.bin.gist",
  //                         std::ios::binary | std::ios::in);
  std::ifstream train_ifs("data/cifar-10-batches-bin/data_all_in_one.gist",
                          std::ios::binary | std::ios::in);
  if (!train_ifs.is_open()) {
    std::cout << "open training data error" << std::endl;
    exit(-1);
  }
  char label;
  for (std::size_t i = 0; i < kTrainImages; i++) {
    train_ifs.read(&label, sizeof(char));
    train_label[i] = static_cast<int>(label);
    train_ifs.read(reinterpret_cast<char*>(train_data[i]),
                   sizeof(float)*kDimFeatures);
  }
}

void ReadTestData() {
  std::ifstream test_ifs("data/cifar-10-batches-bin/test_batch.bin.gist",
                         std::ios::binary | std::ios::in);
  if (!test_ifs.is_open()) {
    std::cout << "open test data error" << std::endl;
    exit(-1);
  }
  char label;
  for (std::size_t i = 0; i < kTestImages; i++) {
    test_ifs.read(&label, sizeof(char));
    test_label[i] = static_cast<int>(label);
    test_ifs.read(reinterpret_cast<char*>(test_data[i]),
                  sizeof(float)*kDimFeatures);
  }
}

std::vector<std::uint8_t> operator^(std::vector<std::uint8_t> lhs, const std::vector<std::uint8_t>& rhs) {
  if (lhs.size() != rhs.size()) {
    std::cout << "differently sized bitwise operands" << std::endl;
    exit(-1);
  }
  std::vector<std::uint8_t>::iterator iter_lhs = lhs.begin();
  std::vector<std::uint8_t>::const_iterator iter_rhs = rhs.begin();
  while (iter_lhs < lhs.end()) {
    // *(iter_lhs._M_p ++) |= *(iter_rhs._M_p ++); // word-at-a-time bitwise operation
    // *iter_lhs ^= *iter_rhs;
    *iter_lhs = (*iter_lhs) ^ (*iter_rhs);
    ++iter_lhs;
    ++iter_rhs;
  }
  return lhs;
}

std::size_t GetHammingDistance(const std::vector<std::uint8_t>& x, const std::vector<std::uint8_t>& y) {
  auto val = x ^ y;
  return std::count(val.begin(), val.end(), 1);
}

void DebugGetHammingDistance() {
  std::vector<std::uint8_t> x = {1,0,1,0,1,0};
  std::vector<std::uint8_t> y = {1,0,0,1,0,0};
  std::cout << "Hamming Distance of ";
  std::copy(x.begin(), x.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << "to " ;
  std::copy(y.begin(), y.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << "= ";
  std::cout << GetHammingDistance(x, y) << std::endl;
}

template<class ForwardIterator>
std::vector<std::size_t> SortIndexes(ForwardIterator first, ForwardIterator last) {
  std::vector<std::size_t> idx(last - first);
  std::iota(idx.begin(), idx.end(), 0);

  std::sort(idx.begin(), idx.end(),
       [&first](std::size_t i1, std::size_t i2) {return *(first+i1) < *(first+i2);});
  return idx;
}

void PrintNothing(const char *s) {}

model* TrainLinear(const std::vector<std::uint8_t>& labels_code, const std::vector<int>& index, int C) {
  if (labels_code.size() != kNumLabels) {
    std::cout << "fatal error" << std::endl;
    exit(-1);
  }
  problem p;
  p.l = static_cast<int>(index.size());
  p.n = kDimFeatures + 1;
#ifdef _LINEAR2
  p.y = new double[p.l];
#elif _LINEAR1
  p.y = new int[p.l];
#endif
  p.x = new feature_node*[p.l];

  for (int i = 0; i < p.l; i++) {
    // p.y[i] = static_cast<int>(code[train_label[i]]);
#ifdef _LINEAR2
    p.y[i] = static_cast<double>(labels_code[train_label[i]]);
#elif _LINEAR1
    p.y[i] = static_cast<int>(labels_code[train_label[i]]);
#endif
    p.x[i] = new feature_node[p.n + 1];
    for (int j = 0; j < p.n - 1; j++) {
      p.x[i][j].value = train_data[index[i]][j];
      p.x[i][j].index = j + 1;
    }
    p.x[i][p.n - 1].value = 1;
    p.x[i][p.n - 1].index = p.n;
    p.x[i][p.n].index = -1;
  }

  parameter param;
  param.solver_type = 1;
  param.C = C;
  param.eps = 0.1;
#ifdef _LINEAR2
  param.p = 0.1;
#endif
  param.nr_weight = 0;
  param.weight_label = NULL;
  param.weight = NULL;

  set_print_string_function(&PrintNothing);

  auto m = train(&p, &param);

  for (int i = 0; i < p.l; i++) {
    delete[] p.x[i];
  }
  delete[] p.x;
  delete[] p.y;

  return m;
}

std::vector<model*> Train(const std::vector<std::vector<std::uint8_t>>& labels_codes, std::vector<int> samples, int C) {
  // std::vector<int> samples(1000);
  // std::iota(samples.begin(), samples.end(), 0);
  std::vector<model*> models;
  for (std::size_t i = 0 ; i < labels_codes.size(); i++) {
    auto labels_code = labels_codes[i];

    std::cout << i << "th training step. Labels: ";
    std::copy(labels_code.begin(), labels_code.end(), std::ostream_iterator<int>(std::cout, " "));

    auto t1 = std::chrono::high_resolution_clock::now();
    // samples = Sample(kTrainImages, static_cast<int>(samples.size()));

    auto m = TrainLinear(labels_code, samples, C);
    models.push_back(m);

    char filename[100];
#ifdef _WIN64
    _snprintf(filename, sizeof(filename), "data/cifar-10-batches-bin/svm/%d_C%d/%d_%.4i", samples.size(), C, labels_codes.size(), i);
#elif __linux__
    std::snprintf(filename, sizeof(filename), "data/cifar-10-batches-bin/svm/%zd_C%d/%zd_%.4zi", samples.size(), C, labels_codes.size(), i);
#endif
    save_model(filename, m);
    free_and_destroy_model(&m);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << std::endl << "SVM Step: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "\tmilliseconds\n";
  }
  return models;
}

std::vector<std::vector<std::uint8_t>> GenerateLabelsCodes() {
  std::vector<std::bitset<kNumLabels>> labels_codes(1<<kNumLabels);
  std::iota(labels_codes.begin(), labels_codes.end(), 0);
  std::vector<std::vector<std::uint8_t>> ret(labels_codes.size(), std::vector<std::uint8_t>(kNumLabels));
  for (std::size_t i = 0; i < labels_codes.size(); i++) {
    for (std::size_t j = 0; j < kNumLabels; j++) {
      ret[i][j] = static_cast<std::uint8_t>(labels_codes[i][kNumLabels - j - 1]);
    }
  }
  return ret;
}

void DebugGenerateLabelsCodes() {
  auto labels_codes = GenerateLabelsCodes();
  for (const auto& labels_code: labels_codes) {
    std::copy(labels_code.begin(), labels_code.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }
}

int ConvertLabelsCode2Int(std::vector<std::uint8_t> labels_code) {
  if (labels_code.size() != kNumLabels) {
    std::cout << "wrong labels_code" << std::endl;
    exit(-1);
  }
  std::bitset<kNumLabels> tmp;
  for (std::size_t j = 0; j < kNumLabels; j++) {
    if (labels_code[j] == 1) {
      tmp.set(kNumLabels - j - 1);
    }
  }
  return static_cast<int>(tmp.to_ulong());
}

void DebugConvertLabelsCode2Int() {
  std::vector<std::uint8_t> labels_code{0, 0, 0, 0, 1, 0, 0, 1, 0, 1};
  std::cout << ConvertLabelsCode2Int(labels_code);
}

double Test(int num_samples, const std::vector<std::vector<std::uint8_t>>& labels_codes, int K, int C) {
  std::vector<model*> models;
  for (const auto& labels_code: labels_codes) {
    int i = ConvertLabelsCode2Int(labels_code);
    char filename[100];
#ifdef _WIN64
    _snprintf(filename, sizeof(filename), "data/cifar-10-batches-bin/svm/%d_C%d/%d_%.4i", num_samples, C, 1<<kNumLabels, i);
#elif __linux__
    std::snprintf(filename, sizeof(filename), "data/cifar-10-batches-bin/svm/%d_C%d/%d_%.4i", num_samples, C, 1<<kNumLabels, i);
#endif
    auto m = load_model(filename);
    if (m == NULL) {
      std::cout << filename << " io wrong!" << std::endl;
      exit(-1);
    }
    models.push_back(m);
  }

  feature_node* feature = new feature_node[kDimFeatures + 1];
  std::vector<std::vector<std::uint8_t>> test_codes(kTestImages, std::vector<std::uint8_t>(labels_codes.size(), 0));
  for (std::size_t i = 0; i < kTestImages; i++) {
    for (std::size_t j = 0; j < kDimFeatures; j++) {
      feature[j].value = test_data[i][j];
      feature[j].index = static_cast<int>(j) + 1;
    }
    feature[kDimFeatures].value = 1;
    feature[kDimFeatures].index = kDimFeatures + 1;
    feature[kDimFeatures + 1].index = -1;

    for (std::size_t j = 0; j < models.size(); j++) {
      int n = predict(models[j], feature);
      test_codes[i][j] = static_cast<std::uint8_t>(n);
    }
    // std::copy(test_codes[i].begin(), test_codes[i].end(), std::ostream_iterator<int>(std::cout, " "));
    // std::cout << std::endl;
  }

  for (std::size_t i = 0; i < kTestImages; i++) {
    for (std::size_t j = 0; j < kTestImages; j++) {
      // if (i == j) continue;
      dist[i][j] = static_cast<std::uint8_t>(GetHammingDistance(test_codes[i], test_codes[j]));
    }
  }

  double precision_at_k = 0.0;
  double mean_average_precision = 0.0;
  for (std::size_t i = 0; i < kTestImages; i++) {
    auto ind = SortIndexes(dist[i], dist[i] + kTestImages);
    int tp = 0;
    for (int j = 0; j < K+1; j++) {
      // if (ind[j] == i) continue;
      if (test_label[ind[j]] == test_label[i]) ++tp;
    }
    precision_at_k += 1.0 * tp / K;

    double average_precision = 0.0;
    int count = 0;
    for (std::size_t j = 0; j < 500; j++) {
      if (test_label[ind[j]] == test_label[i]) {
        count++;
        average_precision += 1.0 * count / (static_cast<double>(j) + 1);
        // std::cout << i << ' ' << j << ' ' << count << ' ' << average_precision << std::endl;
      }
    }
    average_precision /= count;
    mean_average_precision += average_precision;
  }
  mean_average_precision /= kTestImages;
  // std::cout << "precision at " << K << ": " << precision_at_k / kTestImages << std::endl;
  // std::cout << "mean average precision: " << mean_average_precision << std::endl;
  return mean_average_precision;
}

double GetUniform() {
  // static std::mt19937 re;
  static std::uniform_real_distribution<double> dist(0, 1);
  return dist(re);
}

template <typename T>
std::vector<T> Sample(std::vector<T> population, int sample_size) {
  std::vector<T> samples;
  int& n = sample_size;
  // int& N = population_size;
  int N = static_cast<int>(population.size());

  int t = 0;
  int m = 0;
  double u;
  while (m < n) {
    u = GetUniform();
    if ((N-t)*u >= n-m) {
      t++;
    } else {
      samples.push_back(population[t]);
      t++;
      m++;
    }
  }
  return samples;
}

template
std::vector<std::vector<std::uint8_t>> Sample(std::vector<std::vector<std::uint8_t>> population, int sample_size);

template
std::vector<int> Sample(std::vector<int> population, int sample_size);

std::vector<int> Sample(int population_size, int sample_size) {
  std::vector<int> population(population_size);
  std::iota(population.begin(), population.end(), 0);
  auto ret = Sample(population, sample_size);
  return ret;
}

}
