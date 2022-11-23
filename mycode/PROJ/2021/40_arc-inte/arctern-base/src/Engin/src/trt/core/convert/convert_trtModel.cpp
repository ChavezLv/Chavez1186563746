//
// Created by gpu on 19-9-1.
//

#include "convert_trtModel.h"
#include <iostream>
#include <sstream>
#include "flatbuffers/util.h"
#include <common/aes_cryptor.h>
#include "common/release_utils.h"
namespace arctern {
    namespace convert {

        std::vector<std::string> split_string(const std::string &str, char delimiter) {
          std::vector<std::string> result;
          if (str.size() > 0) {
            std::stringstream ss(str);
            std::string item;
            while (std::getline(ss, item, delimiter)) {
              result.push_back(item);
            }
          }
          return result;
        }

        std::vector<flatbuffers::Offset<arctern::Config>> get_configs(const std::string &configs_str, flatbuffers::FlatBufferBuilder &builder) {
          std::vector<flatbuffers::Offset<arctern::Config>> configs_vector;
          std::vector<std::string> chunks_vector = split_string(configs_str, ':');
          for (const std::string chunk: chunks_vector) {
            if (chunk.size() == 0) { continue; }
            std::vector<std::string> kv = split_string(chunk, '=');
            if (kv.size() != 2) {
              std::cout << "[get_configs] ignore kv: " << kv[0] << std::endl;
              continue;
            }

            auto key = builder.CreateString(kv[0]);
            auto value = builder.CreateString(kv[1]);
            auto config = arctern::CreateConfig(builder, key, value);

            configs_vector.push_back(config);
          }
          return configs_vector;
        }

        std::vector<flatbuffers::Offset<arctern::Optional>> get_opts(flatbuffers::FlatBufferBuilder &builder,
                std::vector<std::pair<std::string, std::string>> &kvOpts) {
          std::vector<flatbuffers::Offset<arctern::Optional>> opts_vector;

          for(auto& kv: kvOpts){
            auto key = builder.CreateString(kv.first);
            char *opt_data = const_cast<char*>(kv.second.c_str());
            size_t opt_data_len = kv.second.size();
            auto data = builder.CreateVector(reinterpret_cast<int8_t *>(opt_data), opt_data_len);
            auto opt = arctern::CreateOptional(builder, key, data);
            opts_vector.push_back(opt);
          }

          return opts_vector;
        }


        void convertTrt2Bin(char *trtData, int trtDataSize, std::string &version, int &batchSize,
                            const std::string &configs_str, std::vector<std::pair<std::string, std::string>> &options,
                            const std::string &savePath){

          /*flatbuffers::FlatBufferBuilder builder;
          auto version_str = builder.CreateString(version);

          auto params = builder.CreateVector(reinterpret_cast<int8_t*>(trtData), trtDataSize);

          auto configs_vector = get_configs(configs_str, builder);
          auto configs = builder.CreateVectorOfSortedTables(&configs_vector);
          auto model = arctern::CreateTrtModel(builder, params, version_str, batchSize, configs);

          std::vector<flatbuffers::Offset<arctern::TrtModel>> models_vector;
          models_vector.push_back(model);
          auto models = builder.CreateVector(models_vector);

          auto opts_vector = get_opts(builder, options);
          auto opts = builder.CreateVectorOfSortedTables(&opts_vector);
          auto orc = CreateTrtModels(builder, models, version_str, 0, opts);
          builder.Finish(orc);

          uint8_t* buf = builder.GetBufferPointer();
          FILE* fpout = fopen(savePath.c_str(), "wb");
          fwrite(buf, sizeof(uint8_t), builder.GetSize(), fpout);
          fclose(fpout);
          */

          flatbuffers::FlatBufferBuilder builder;
          auto versionOffset = builder.CreateString(version);
          auto paramsOffset = builder.CreateVector(reinterpret_cast<int8_t*>(trtData),trtDataSize);
          auto vConfigs = get_configs(configs_str,builder);
          auto configOffset = builder.CreateVectorOfSortedTables(&vConfigs);
          auto modelOffset = arctern::CreateMxnetModel(builder,0,0,0,paramsOffset,versionOffset,configOffset,0);

          std::vector<flatbuffers::Offset<arctern::MxnetModel>> vModelOffset;
          vModelOffset.push_back(modelOffset);
          auto modelsOffset = builder.CreateVector(vModelOffset);
          auto vOption = get_opts(builder,options);
          auto optionOffset = builder.CreateVectorOfSortedTables(&vOption);

          auto trtModels = CreateMxnetModels(builder,modelsOffset,versionOffset,optionOffset);
          builder.Finish(trtModels);

#ifdef PLAIN_MODEL
          flatbuffers::SaveFile(savePath.c_str(),reinterpret_cast<char*>(builder.GetBufferPointer()),builder.GetSize(),true);
#else
          /// encrypt
          uint8_t* buf = builder.GetBufferPointer();
          GENERATE_RELEASE_KEYS_BLOCK;
          AESCryptor aes_cryptor(RELEASE_KEY_STRING);
          std::string cryptData;
          std::string planData;
          planData.assign((char*) buf,(int) builder.GetSize());
          aes_cryptor.encrypt_string(planData,cryptData);

          FILE* fpout = fopen(savePath.c_str(), "wb");
          fwrite(cryptData.c_str(), sizeof(char), cryptData.size(), fpout);
          fclose(fpout);
#endif


        }

    }/// namespace convert
}/// namespace arctern