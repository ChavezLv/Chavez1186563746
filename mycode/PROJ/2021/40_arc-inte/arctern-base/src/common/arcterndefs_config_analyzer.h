/****************************************************************************
 *  Filename:       arcterndefs_config_analyzer.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.11
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_ARCTERNDEFS_CONFIG_ANALYZER_H_
#define SRC_COMMON_ARCTERNDEFS_CONFIG_ANALYZER_H_
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "./arcterndefs_generated.h"
#include "common/common.h"


inline bool try_get_value(const arctern::MxnetModel &model, const std::string &key, std::string &value) {
    auto configs = model.configs();
    if (configs == NULL) { return false; }

    auto config = configs->LookupByKey(key.c_str());

    if (config == NULL) { return false; }

    value = config->value()->str();
    return true;
}


template<typename T>
inline bool try_get_value(const arctern::MxnetModel& model, const std::string &key, T &value) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::stringstream ss;
  ss << str_value;
  ss >> value;

  return true;
}

template<typename T>
inline bool try_get_value(const arctern::MxnetModel &model, const std::string &key, std::vector<T> &values) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::vector<std::string> chunks = split_string(str_value, ',');
  for (const std::string &chunk : chunks) {
    T value;
    std::stringstream ss;
    ss << chunk;
    ss >> value;
    values.push_back(value);
  }

  return true;
}


inline bool try_get_value(const arctern::MxnetModel &model, const std::string &key,
                                                    std::vector<std::string> &values) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::vector<std::string> chunks = split_string(str_value, ',');
  for (const std::string &chunk : chunks) {
    values.push_back(chunk);
  }

  return true;
}

/// get opt data for key
/// @params models, arctern::MxnetModel or arctern::TrtModel
/// @params key
/// @params p_data where data for opt will be stored, you will call delete when it's not used
/// @params data_size size of the data
/// @returns if found opt, returns true, otherwise returns false
template<typename T>
inline bool try_get_opt(const T &models, const std::string &key, char **p_data, size_t &data_size) {
  auto opts = models.opts();
  if (opts == NULL) {
    std::cout << "opts is null" << std::endl;
    return false;
  }

  auto opt = opts->LookupByKey(key.c_str());
  if (opt == NULL) {
    std::cout << "can not find opt" << std::endl;
    return false;
  }

  size_t size = opt->bytes()->size();
  *p_data = new char[size + 1];
  memset(*p_data,0,size);
  std::memcpy(*p_data, opt->bytes()->data(), size);
  data_size = size;
  return true;
}

# ifndef WITH_NNIE
template<typename T>
inline bool try_get_value(const arctern::TrtModel &model, const std::string &key, T &value) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::stringstream ss;
  ss << str_value;
  ss >> value;

  return true;
}

template<>
inline bool try_get_value<std::string>(const arctern::TrtModel &model, const std::string &key, std::string &value) {
  auto configs = model.configs();
  if (configs == NULL) { return false; }

  auto config = configs->LookupByKey(key.c_str());
  if (config == NULL) { return false; }

  value = config->value()->str();
  return true;
}

template<typename T>
inline bool try_get_value(const arctern::TrtModel &model, const std::string &key, std::vector<T> &values) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::vector<std::string> chunks = split_string(str_value, ',');
  for (const std::string &chunk : chunks) {
    T value;
    std::stringstream ss;
    ss << chunk;
    ss >> value;
    values.push_back(value);
  }

  return true;
}

template<>
inline bool try_get_value<std::vector<std::string>>(const arctern::TrtModel &model, const std::string &key,
                                                    std::vector<std::string> &values) {
  std::string str_value;
  bool str_value_result = try_get_value(model, key, str_value);
  if (!str_value_result) { return false; }

  std::vector<std::string> chunks = split_string(str_value, ',');
  for (const std::string &chunk : chunks) {
    values.push_back(chunk);
  }

  return true;
}
# endif  // if not def WITH_NNIE

#endif  // SRC_COMMON_ARCTERNDEFS_CONFIG_ANALYZER_H_
