/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <sstream>
#include <chrono>
#include <fstream>
#include <list>
#include "src/Algorithms/memoryDB/memoryDB.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
    const int test_feature_length = 512 * 4 + 16;
    auto max_dist = [](const std::vector<float>& vec) {
      return std::distance(vec.cbegin(),
                           std::max_element(vec.cbegin(), vec.cend()));
    };

    const std::string FEATURE_BIN_FILE = "./Data/memoryDB/features.bin";
    //const int NUM_BLOCK = 5;

    std::ifstream ifs(FEATURE_BIN_FILE, std::ios::binary);
    if (!ifs.is_open()) {
      return 0;
    }
    std::vector<uint8_t> feature[10];
    std::vector<int64_t> id(10);
    for (size_t i = 0; i < 10; ++i) {
      id[i] = i * 100;
      feature[i].resize(test_feature_length);
      ifs.read((char*)feature[i].data(), test_feature_length);
    }

    std::vector<uint8_t>& aimfeat1 = feature[3];
    std::vector<uint8_t>& aimfeat2 = feature[8];
    std::list<std::vector<uint8_t>> vecfeat{aimfeat1, aimfeat2};
    
    arctern::MemoryDB mdb;
    
    float sim = arctern::compute_similarity(aimfeat1, aimfeat2);
    std::cout<< "similarity between aimfeat1 and aimfeat2 : "<< sim <<std::endl;

    {
    mdb.Init(20000, 512); // block number , feature dim
    auto ret_insert = mdb.feature_memdb_insert(id, feature);
    for (size_t i = 0; i < id.size(); ++i) {
      auto ret_erase = mdb.feature_memdb_erase(id[i]);
      size_t ret_size = mdb.feature_memdb_size();
      std::cout<<"ret_erase: "<< ret_erase <<std::endl;
      std::cout<<"ret_size, true size: "<< ret_size << ", " 
          << id.size() - i - 1 << std::endl;
    }
    std::cout<<"memoryDB is empty? : "<< mdb.feature_memdb_empty() <<std::endl;
    std::cout<<"-----------------"<<std::endl;
    for (size_t i = 0; i < id.size(); ++i) {
      mdb.feature_memdb_insert(id[i], feature[i]);
      std::cout<<"current db-size: "<< mdb.feature_memdb_size() <<std::endl;
    }
    std::vector<float> sim1, sim2;
    auto ret_comp1 = mdb.feature_memdb_compare(aimfeat1, sim1);
    int64_t id1 = mdb.feature_memdb_at(max_dist(sim1));
    auto ret_comp2 = mdb.feature_memdb_compare(aimfeat2, sim2);
    int64_t id2 = mdb.feature_memdb_at(max_dist(sim2));
    std::cout<< "aimfeat1:  id:" << id1 << ", "
        << "similarity:"<< sim1[3] <<std::endl;
    std::cout<< "aimfeat2:  id:" << id2 << ", "
        << "similarity:"<< sim2[8] <<std::endl;
    std::list<std::vector<float>> vecsim;
    auto ret_comp = mdb.feature_memdb_compare(vecfeat, vecsim);
    std::cout<< "vecsim id : " 
        << mdb.feature_memdb_at(max_dist(vecsim.front())) << ", "
        << mdb.feature_memdb_at(max_dist(vecsim.back())) << std::endl;
    }
    


  return 0;
}
