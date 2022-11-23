/****************************************************************************
 *  Filename:       arcterndefs_generated.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_ARCTERNDEFS_GENERATED_H_
#define SRC_COMMON_ARCTERNDEFS_GENERATED_H_
#include <vector>
#include "flatbuffers/flatbuffers.h"


namespace arctern {

struct Config;

struct MxnetModel;

struct Optional;

struct MxnetModels;

struct Config FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_KEY = 4,
    VT_VALUE = 6
  };
  const flatbuffers::String *key() const {
    return GetPointer<const flatbuffers::String *>(VT_KEY);
  }
  bool KeyCompareLessThan(const Config *o) const {
    return *key() < *o->key();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(key()->c_str(), val);
  }
  const flatbuffers::String *value() const {
    return GetPointer<const flatbuffers::String *>(VT_VALUE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_KEY) &&
           verifier.Verify(key()) &&
           VerifyOffset(verifier, VT_VALUE) &&
           verifier.Verify(value()) &&
           verifier.EndTable();
  }
};

struct ConfigBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(flatbuffers::Offset<flatbuffers::String> key) {
    fbb_.AddOffset(Config::VT_KEY, key);
  }
  void add_value(flatbuffers::Offset<flatbuffers::String> value) {
    fbb_.AddOffset(Config::VT_VALUE, value);
  }
  explicit ConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ConfigBuilder &operator=(const ConfigBuilder &);
  flatbuffers::Offset<Config> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Config>(end);
    fbb_.Required(o, Config::VT_KEY);
    return o;
  }
};

inline flatbuffers::Offset<Config> CreateConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> key = 0,
    flatbuffers::Offset<flatbuffers::String> value = 0) {
  ConfigBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_key(key);
  return builder_.Finish();
}

inline flatbuffers::Offset<Config> CreateConfigDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *key = nullptr,
    const char *value = nullptr) {
  return arctern::CreateConfig(
      _fbb,
      key ? _fbb.CreateString(key) : 0,
      value ? _fbb.CreateString(value) : 0);
}

struct MxnetModel FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_INPUT_SHAPE = 4,
    VT_NUM_OUTPUT = 6,
    VT_JSON = 8,
    VT_PARAMS = 10,
    VT_VERSION = 12,
    VT_CONFIGS = 14,
    VT_INPUT_SHAPE_IDXES = 16
  };
  const flatbuffers::Vector<int32_t> *input_shape() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_INPUT_SHAPE);
  }
  int32_t num_output() const {
    return GetField<int32_t>(VT_NUM_OUTPUT, 0);
  }
  const flatbuffers::String *json() const {
    return GetPointer<const flatbuffers::String *>(VT_JSON);
  }
  const flatbuffers::Vector<int8_t> *params() const {
    return GetPointer<const flatbuffers::Vector<int8_t> *>(VT_PARAMS);
  }
  const flatbuffers::String *version() const {
    return GetPointer<const flatbuffers::String *>(VT_VERSION);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Config>> *configs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Config>> *>(VT_CONFIGS);
  }
  const flatbuffers::Vector<int32_t> *input_shape_idxes() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_INPUT_SHAPE_IDXES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INPUT_SHAPE) &&
           verifier.Verify(input_shape()) &&
           VerifyField<int32_t>(verifier, VT_NUM_OUTPUT) &&
           VerifyOffset(verifier, VT_JSON) &&
           verifier.Verify(json()) &&
           VerifyOffset(verifier, VT_PARAMS) &&
           verifier.Verify(params()) &&
           VerifyOffset(verifier, VT_VERSION) &&
           verifier.Verify(version()) &&
           VerifyOffset(verifier, VT_CONFIGS) &&
           verifier.Verify(configs()) &&
           verifier.VerifyVectorOfTables(configs()) &&
           VerifyOffset(verifier, VT_INPUT_SHAPE_IDXES) &&
           verifier.Verify(input_shape_idxes()) &&
           verifier.EndTable();
  }
};

struct MxnetModelBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_input_shape(flatbuffers::Offset<flatbuffers::Vector<int32_t>> input_shape) {
    fbb_.AddOffset(MxnetModel::VT_INPUT_SHAPE, input_shape);
  }
  void add_num_output(int32_t num_output) {
    fbb_.AddElement<int32_t>(MxnetModel::VT_NUM_OUTPUT, num_output, 0);
  }
  void add_json(flatbuffers::Offset<flatbuffers::String> json) {
    fbb_.AddOffset(MxnetModel::VT_JSON, json);
  }
  void add_params(flatbuffers::Offset<flatbuffers::Vector<int8_t>> params) {
    fbb_.AddOffset(MxnetModel::VT_PARAMS, params);
  }
  void add_version(flatbuffers::Offset<flatbuffers::String> version) {
    fbb_.AddOffset(MxnetModel::VT_VERSION, version);
  }
  void add_configs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Config>>> configs) {
    fbb_.AddOffset(MxnetModel::VT_CONFIGS, configs);
  }
  void add_input_shape_idxes(flatbuffers::Offset<flatbuffers::Vector<int32_t>> input_shape_idxes) {
    fbb_.AddOffset(MxnetModel::VT_INPUT_SHAPE_IDXES, input_shape_idxes);
  }
  explicit MxnetModelBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MxnetModelBuilder &operator=(const MxnetModelBuilder &);
  flatbuffers::Offset<MxnetModel> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MxnetModel>(end);
    return o;
  }
};

inline flatbuffers::Offset<MxnetModel> CreateMxnetModel(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> input_shape = 0,
    int32_t num_output = 0,
    flatbuffers::Offset<flatbuffers::String> json = 0,
    flatbuffers::Offset<flatbuffers::Vector<int8_t>> params = 0,
    flatbuffers::Offset<flatbuffers::String> version = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Config>>> configs = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> input_shape_idxes = 0) {
  MxnetModelBuilder builder_(_fbb);
  builder_.add_input_shape_idxes(input_shape_idxes);
  builder_.add_configs(configs);
  builder_.add_version(version);
  builder_.add_params(params);
  builder_.add_json(json);
  builder_.add_num_output(num_output);
  builder_.add_input_shape(input_shape);
  return builder_.Finish();
}

inline flatbuffers::Offset<MxnetModel> CreateMxnetModelDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<int32_t> *input_shape = nullptr,
    int32_t num_output = 0,
    const char *json = nullptr,
    const std::vector<int8_t> *params = nullptr,
    const char *version = nullptr,
    const std::vector<flatbuffers::Offset<Config>> *configs = nullptr,
    const std::vector<int32_t> *input_shape_idxes = nullptr) {
  return arctern::CreateMxnetModel(
      _fbb,
      input_shape ? _fbb.CreateVector<int32_t>(*input_shape) : 0,
      num_output,
      json ? _fbb.CreateString(json) : 0,
      params ? _fbb.CreateVector<int8_t>(*params) : 0,
      version ? _fbb.CreateString(version) : 0,
      configs ? _fbb.CreateVector<flatbuffers::Offset<Config>>(*configs) : 0,
      input_shape_idxes ? _fbb.CreateVector<int32_t>(*input_shape_idxes) : 0);
}

struct Optional FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_KEY = 4,
    VT_BYTES = 6
  };
  const flatbuffers::String *key() const {
    return GetPointer<const flatbuffers::String *>(VT_KEY);
  }
  bool KeyCompareLessThan(const Optional *o) const {
    return *key() < *o->key();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(key()->c_str(), val);
  }
  const flatbuffers::Vector<int8_t> *bytes() const {
    return GetPointer<const flatbuffers::Vector<int8_t> *>(VT_BYTES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_KEY) &&
           verifier.Verify(key()) &&
           VerifyOffset(verifier, VT_BYTES) &&
           verifier.Verify(bytes()) &&
           verifier.EndTable();
  }
};

struct OptionalBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(flatbuffers::Offset<flatbuffers::String> key) {
    fbb_.AddOffset(Optional::VT_KEY, key);
  }
  void add_bytes(flatbuffers::Offset<flatbuffers::Vector<int8_t>> bytes) {
    fbb_.AddOffset(Optional::VT_BYTES, bytes);
  }
  explicit OptionalBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  OptionalBuilder &operator=(const OptionalBuilder &);
  flatbuffers::Offset<Optional> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Optional>(end);
    fbb_.Required(o, Optional::VT_KEY);
    return o;
  }
};

inline flatbuffers::Offset<Optional> CreateOptional(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> key = 0,
    flatbuffers::Offset<flatbuffers::Vector<int8_t>> bytes = 0) {
  OptionalBuilder builder_(_fbb);
  builder_.add_bytes(bytes);
  builder_.add_key(key);
  return builder_.Finish();
}

inline flatbuffers::Offset<Optional> CreateOptionalDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *key = nullptr,
    const std::vector<int8_t> *bytes = nullptr) {
  return arctern::CreateOptional(
      _fbb,
      key ? _fbb.CreateString(key) : 0,
      bytes ? _fbb.CreateVector<int8_t>(*bytes) : 0);
}

struct MxnetModels FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MODELS = 4,
    VT_VERSION = 6,
    VT_OPTS = 8
  };
  const flatbuffers::Vector<flatbuffers::Offset<MxnetModel>> *models() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<MxnetModel>> *>(VT_MODELS);
  }
  const flatbuffers::String *version() const {
    return GetPointer<const flatbuffers::String *>(VT_VERSION);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Optional>> *opts() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Optional>> *>(VT_OPTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MODELS) &&
           verifier.Verify(models()) &&
           verifier.VerifyVectorOfTables(models()) &&
           VerifyOffset(verifier, VT_VERSION) &&
           verifier.Verify(version()) &&
           VerifyOffset(verifier, VT_OPTS) &&
           verifier.Verify(opts()) &&
           verifier.VerifyVectorOfTables(opts()) &&
           verifier.EndTable();
  }
};

struct MxnetModelsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_models(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<MxnetModel>>> models) {
    fbb_.AddOffset(MxnetModels::VT_MODELS, models);
  }
  void add_version(flatbuffers::Offset<flatbuffers::String> version) {
    fbb_.AddOffset(MxnetModels::VT_VERSION, version);
  }
  void add_opts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Optional>>> opts) {
    fbb_.AddOffset(MxnetModels::VT_OPTS, opts);
  }
  explicit MxnetModelsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MxnetModelsBuilder &operator=(const MxnetModelsBuilder &);
  flatbuffers::Offset<MxnetModels> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MxnetModels>(end);
    return o;
  }
};

inline flatbuffers::Offset<MxnetModels> CreateMxnetModels(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<MxnetModel>>> models = 0,
    flatbuffers::Offset<flatbuffers::String> version = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Optional>>> opts = 0) {
  MxnetModelsBuilder builder_(_fbb);
  builder_.add_opts(opts);
  builder_.add_version(version);
  builder_.add_models(models);
  return builder_.Finish();
}

inline flatbuffers::Offset<MxnetModels> CreateMxnetModelsDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<MxnetModel>> *models = nullptr,
    const char *version = nullptr,
    const std::vector<flatbuffers::Offset<Optional>> *opts = nullptr) {
  return arctern::CreateMxnetModels(
      _fbb,
      models ? _fbb.CreateVector<flatbuffers::Offset<MxnetModel>>(*models) : 0,
      version ? _fbb.CreateString(version) : 0,
      opts ? _fbb.CreateVector<flatbuffers::Offset<Optional>>(*opts) : 0);
}

inline const arctern::MxnetModels *GetMxnetModels(const void *buf) {
  return flatbuffers::GetRoot<arctern::MxnetModels>(buf);
}

inline const arctern::MxnetModels *GetSizePrefixedMxnetModels(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<arctern::MxnetModels>(buf);
}

inline bool VerifyMxnetModelsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<arctern::MxnetModels>(nullptr);
}

inline bool VerifySizePrefixedMxnetModelsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<arctern::MxnetModels>(nullptr);
}

inline void FinishMxnetModelsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<arctern::MxnetModels> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMxnetModelsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<arctern::MxnetModels> root) {
  fbb.FinishSizePrefixed(root);
}

// \brief define for converting trt to bin
struct TrtModel;

struct TrtModels;

struct TrtModel FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
    enum {
        VT_PARAMS = 4,
        VT_VERSION = 6,
        VT_BATCHSIZE = 8,
        VT_CONFIGS = 10
    };
    const flatbuffers::Vector<int8_t> *params() const {
      return GetPointer<const flatbuffers::Vector<int8_t> *>(VT_PARAMS);
    }
    const flatbuffers::String *version() const {
      return GetPointer<const flatbuffers::String *>(VT_VERSION);
    }
    int32_t batchSize() const {
      return GetField<int32_t>(VT_BATCHSIZE, 1);
    }
    const flatbuffers::Vector<flatbuffers::Offset<Config>> *configs() const {
      return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Config>> *>(VT_CONFIGS);
    }
    bool Verify(flatbuffers::Verifier &verifier) const {
      return VerifyTableStart(verifier) &&
             VerifyOffset(verifier, VT_PARAMS) &&
             verifier.Verify(params()) &&
             VerifyOffset(verifier, VT_VERSION) &&
             verifier.Verify(version()) &&
             VerifyField<int32_t>(verifier, VT_BATCHSIZE) &&
             VerifyOffset(verifier, VT_CONFIGS) &&
             verifier.Verify(configs()) &&
             verifier.VerifyVectorOfTables(configs()) &&
             verifier.EndTable();
    }
};

struct TrtModelBuilder {
    flatbuffers::FlatBufferBuilder &fbb_;
    flatbuffers::uoffset_t start_;
    void add_params(flatbuffers::Offset<flatbuffers::Vector<int8_t>> params) {
      fbb_.AddOffset(TrtModel::VT_PARAMS, params);
    }
    void add_version(flatbuffers::Offset<flatbuffers::String> version) {
      fbb_.AddOffset(TrtModel::VT_VERSION, version);
    }
    void add_batchSize(int32_t batchSize) {
      fbb_.AddElement<int32_t>(TrtModel::VT_BATCHSIZE, batchSize, 1);
    }
    void add_configs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Config>>> configs) {
      fbb_.AddOffset(TrtModel::VT_CONFIGS, configs);
    }
    explicit TrtModelBuilder(flatbuffers::FlatBufferBuilder &_fbb)
            : fbb_(_fbb) {
      start_ = fbb_.StartTable();
    }
    TrtModelBuilder &operator=(const TrtModelBuilder &);
    flatbuffers::Offset<TrtModel> Finish() {
      const auto end = fbb_.EndTable(start_);
      auto o = flatbuffers::Offset<TrtModel>(end);
      return o;
    }
};

inline flatbuffers::Offset<TrtModel> CreateTrtModel(
        flatbuffers::FlatBufferBuilder &_fbb,
        flatbuffers::Offset<flatbuffers::Vector<int8_t>> params = 0,
        flatbuffers::Offset<flatbuffers::String> version = 0,
        int32_t batchSize = 1,
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Config>>> configs = 0) {
  TrtModelBuilder builder_(_fbb);
  builder_.add_configs(configs);
  builder_.add_batchSize(batchSize);
  builder_.add_version(version);
  builder_.add_params(params);
  return builder_.Finish();
}

inline flatbuffers::Offset<TrtModel> CreateTrtModelDirect(
        flatbuffers::FlatBufferBuilder &_fbb,
        const std::vector<int8_t> *params = nullptr,
        const char *version = nullptr,
        int32_t batchSize = 1,
        const std::vector<flatbuffers::Offset<Config>> *configs = nullptr) {
  return arctern::CreateTrtModel(
          _fbb,
          params ? _fbb.CreateVector<int8_t>(*params) : 0,
          version ? _fbb.CreateString(version) : 0,
          batchSize,
          configs ? _fbb.CreateVector<flatbuffers::Offset<Config>>(*configs) : 0);
}

struct TrtModels FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
    enum {
        VT_MODELS = 4,
        VT_VERSION = 6,
        VT_PNETMODELSNUM = 8,
        VT_OPTS = 10
    };
    const flatbuffers::Vector<flatbuffers::Offset<TrtModel>> *models() const {
      return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<TrtModel>> *>(VT_MODELS);
    }
    const flatbuffers::String *version() const {
      return GetPointer<const flatbuffers::String *>(VT_VERSION);
    }
    int32_t pnetModelsNum() const {
      return GetField<int32_t>(VT_PNETMODELSNUM, 0);
    }
    const flatbuffers::Vector<flatbuffers::Offset<Optional>> *opts() const {
      return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Optional>> *>(VT_OPTS);
    }
    bool Verify(flatbuffers::Verifier &verifier) const {
      return VerifyTableStart(verifier) &&
             VerifyOffset(verifier, VT_MODELS) &&
             verifier.Verify(models()) &&
             verifier.VerifyVectorOfTables(models()) &&
             VerifyOffset(verifier, VT_VERSION) &&
             verifier.Verify(version()) &&
             VerifyField<int32_t>(verifier, VT_PNETMODELSNUM) &&
             VerifyOffset(verifier, VT_OPTS) &&
             verifier.Verify(opts()) &&
             verifier.VerifyVectorOfTables(opts()) &&
             verifier.EndTable();
    }
};

struct TrtModelsBuilder {
    flatbuffers::FlatBufferBuilder &fbb_;
    flatbuffers::uoffset_t start_;
    void add_models(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TrtModel>>> models) {
      fbb_.AddOffset(TrtModels::VT_MODELS, models);
    }
    void add_version(flatbuffers::Offset<flatbuffers::String> version) {
      fbb_.AddOffset(TrtModels::VT_VERSION, version);
    }
    void add_pnetModelsNum(int32_t pnetModelsNum) {
      fbb_.AddElement<int32_t>(TrtModels::VT_PNETMODELSNUM, pnetModelsNum, 0);
    }
    void add_opts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Optional>>> opts) {
      fbb_.AddOffset(TrtModels::VT_OPTS, opts);
    }
    explicit TrtModelsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
            : fbb_(_fbb) {
      start_ = fbb_.StartTable();
    }
    TrtModelsBuilder &operator=(const TrtModelsBuilder &);
    flatbuffers::Offset<TrtModels> Finish() {
      const auto end = fbb_.EndTable(start_);
      auto o = flatbuffers::Offset<TrtModels>(end);
      return o;
    }
};

inline flatbuffers::Offset<TrtModels> CreateTrtModels(
        flatbuffers::FlatBufferBuilder &_fbb,
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TrtModel>>> models = 0,
        flatbuffers::Offset<flatbuffers::String> version = 0,
        int32_t pnetModelsNum = 0,
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Optional>>> opts = 0) {
  TrtModelsBuilder builder_(_fbb);
  builder_.add_opts(opts);
  builder_.add_pnetModelsNum(pnetModelsNum);
  builder_.add_version(version);
  builder_.add_models(models);
  return builder_.Finish();
}

inline flatbuffers::Offset<TrtModels> CreateTrtModelsDirect(
        flatbuffers::FlatBufferBuilder &_fbb,
        const std::vector<flatbuffers::Offset<TrtModel>> *models = nullptr,
        const char *version = nullptr,
        int32_t pnetModelsNum = 0,
        const std::vector<flatbuffers::Offset<Optional>> *opts = nullptr) {
  return arctern::CreateTrtModels(
          _fbb,
          models ? _fbb.CreateVector<flatbuffers::Offset<TrtModel>>(*models) : 0,
          version ? _fbb.CreateString(version) : 0,
          pnetModelsNum,
          opts ? _fbb.CreateVector<flatbuffers::Offset<Optional>>(*opts) : 0);
}

inline const arctern::TrtModels *GetTrtModels(const void *buf) {
  return flatbuffers::GetRoot<arctern::TrtModels>(buf);
}

inline const arctern::TrtModels *GetSizePrefixedTrtModels(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<arctern::TrtModels>(buf);
}

inline bool VerifyTrtModelsBuffer(
        flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<arctern::TrtModels>(nullptr);
}

inline bool VerifySizePrefixedTrtModelsBuffer(
        flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<arctern::TrtModels>(nullptr);
}

inline void FinishTrtModelsBuffer(
        flatbuffers::FlatBufferBuilder &fbb,
        flatbuffers::Offset<arctern::TrtModels> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTrtModelsBuffer(
        flatbuffers::FlatBufferBuilder &fbb,
        flatbuffers::Offset<arctern::TrtModels> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace arctern

#endif  // SRC_COMMON_ARCTERNDEFS_GENERATED_H_
