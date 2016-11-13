
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

extern "C" {
  #include "gist.h"
}

const static int kHeight = 32;
const static int kWidth = 32;
const static int kChannelSize = 1024;
const static int kNumImagesPerBatch = 10000;
const static std::vector<std::string> filenames = {
  "data/cifar-10-batches-bin/data_batch_1.bin",
  "data/cifar-10-batches-bin/data_batch_2.bin",
  "data/cifar-10-batches-bin/data_batch_3.bin",
  "data/cifar-10-batches-bin/data_batch_4.bin",
  "data/cifar-10-batches-bin/data_batch_5.bin",
  "data/cifar-10-batches-bin/test_batch.bin",
};

float R = 0.2989f;
float G = 0.5870f;
float B = 0.1140f;

int nblocks=4;
int n_scale=3;
int orientations_per_scale[50]={8,8,4};

int main() {
  std::uint8_t* image = new std::uint8_t[1 + kChannelSize * 3];
  image_t *grey_image = image_new(kWidth, kHeight);

  std::ofstream ofs_aio("data/cifar-10-batches-bin/data_all_in_one.gist", std::ios::binary | std::ios::out);
  for (const auto& filename : filenames) {
    std::ifstream ifs(filename, std::ios::binary | std::ios::in);
    std::ofstream ofs(filename + ".gist", std::ios::binary | std::ios::out);
    if (!ifs.is_open() || !ofs.is_open()) {
      std::cout << "open file " << filename << " wrong" << std::endl;
      exit(-1);
    }
    std::cout << "processing " << filename << std::endl;
    for (int i = 0; i < kNumImagesPerBatch; i++) {
      // std::cout << ifs.tellg() << std::endl;
      ifs.read(reinterpret_cast<char*>(image), sizeof(std::uint8_t) * (1 + kChannelSize * 3));
      ofs.write(reinterpret_cast<char*>(image), sizeof(std::uint8_t));
      ofs_aio.write(reinterpret_cast<char*>(image), sizeof(std::uint8_t));
      for (int j = 0; j < kChannelSize; j++) {
        grey_image->data[j] = R * image[j + 1] +
                              G * image[j + 1 + kChannelSize] +
                              B * image[j + 1 + kChannelSize * 2];
        // std::cout << grey_image->data[j] << ' ' ;
      }
      float *desc = bw_gist_scaletab(grey_image, nblocks, n_scale, orientations_per_scale);
      int descsize = 0;
      for (int j = 0; j < n_scale; j++) {
        descsize += nblocks * nblocks * orientations_per_scale[j];
      }
      ofs.write(reinterpret_cast<char*>(desc), (sizeof(float) * descsize));
      ofs_aio.write(reinterpret_cast<char*>(desc), (sizeof(float) * descsize));
      // std::copy(desc, desc+10, std::ostream_iterator<float>(std::cout, " "));
    }
  }
  return 0;
}

  // for (auto filename : filenames) {
  //   std::ifstream ifs(filename, std::ios::binary | std::ios::in);
  //   std::ofstream ofs(filename + ".gist", std::ios::binary | std::ios::out);
  //   if (!ifs.is_open() || !ofs.is_open()) {
  //     std::cout << "open file " << filename << " wrong" << std::endl;
  //     exit(-1);
  //   }
  //   color_image_t *im = color_image_new(width,height);
  //   for (std::size_t i = 0; i < num_images_batch; i++) {
  //     std::uint8_t label;
  //     ifs.read(reinterpret_cast<char*>(&label), sizeof(std::uint8_t));
  //     ofs.write(reinterpret_cast<char*>(&label), sizeof(std::uint8_t));
  //     std::uint8_t intensity;
  //     for (int j = 0; j < channel_size; j++) {
  //       ifs.read(reinterpret_cast<char*>(&intensity), sizeof(std::uint8_t));
  //       im->c1[j] = intensity;
  //     }
  //     for (int j = 0; j < channel_size; j++) {
  //       ifs.read(reinterpret_cast<char*>(&intensity), sizeof(std::uint8_t));
  //       im->c2[j] = intensity;
  //     }
  //     for (int j = 0; j < channel_size; j++) {
  //       ifs.read(reinterpret_cast<char*>(&intensity), sizeof(std::uint8_t));
  //       im->c3[j] = intensity;
  //     }

  //     float *desc=color_gist_scaletab(im,nblocks,n_scale,orientations_per_scale);
  //     int descsize=0;
  //     /* compute descriptor size */
  //     for(int j=0;j<n_scale;j++) 
  //       descsize+=nblocks*nblocks*orientations_per_scale[j];
  //     descsize*=3;

  //     ofs.write(reinterpret_cast<char*>(desc), (sizeof(float) * descsize));
  //     // std::cout << sizeof(float) * descsize << ' ';
  //     // for (int k = 0; k < 5; k++) {
  //     //   std::cout << desc[k] << ' ' ;
  //     // }
  //     // std::cout << std::endl;
  //     // std::ifstream tmp_ifs(filename+".gist", std::ios::binary | std::ios::out);
  //     // // tmp_ifs.seekg(1, std::ios::beg);
  //     // std::uint8_t tmp_label;
  //     // tmp_ifs.read(reinterpret_cast<char*>(tmp_label), sizeof(uint8_t));
  //     // std::cout << int(tmp_label) << ' ';
  //     // float* tmp_gist = new float[5];
  //     // tmp_ifs.read(reinterpret_cast<char*>(tmp_gist), sizeof(float) * 5);
  //     // for (int k = 0; k < 5; k++) {
  //     //   std::cout << tmp_gist[k] << ' ';
  //     // }
  //     // std::cout << std::endl;
  //   }
  //   ifs.close();
  //   ofs.close();
  // }