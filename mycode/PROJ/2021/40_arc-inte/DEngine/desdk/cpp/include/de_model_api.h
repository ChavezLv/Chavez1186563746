/**
 * @file de_model_api.h
 * @brief 模型管理接口
 * @author Desdk Group
 * @version 0.1
 * @date 2020-03-31
 */

#ifndef DE_MODEL_API_H_
#define DE_MODEL_API_H_

#include <core/de_c_type.h>
#include <string>
#include <functional>

namespace de
{

/**
 * @brief  model info 
 */
typedef struct {
  std::string name;  ///<模型名字
  uint64_t id;   ///<模型id
  const char* model_addr; ///<模型地址
  uint32_t model_len;  ///<模型长度
  const char* para_addr; ///<参数地址
  uint32_t para_len;  ///<参数长度
  uint32_t ddr_size;///< 中间结果DDR大小，batch=1
}ModelInfo;

/**
 * @brief  model decrypt callback function
 * @param [in]  std::string  the model file. if model file in host, add 'host:' prefix.
 * @param [in]  std::string  the para file, if model file in host, add 'host:' prefix.
 * @param [out] uint8_t** decrepted net model addr in memory
 * @param [out] int*   decrepted net model size
 * @param [out] uint8_t** decrepted para addr in memory
 * @param [out] int*   decrepted para size 
 * @note
 */
using DecryptCallback = std::function<void(std::string, std::string, uint8_t**, int*, uint8_t**, int*)>;

/**
 * @brief  model memfree callback function
 * @param [in] uint8_t* decrepted model addr in memory
 * @param [in] uint8_t* decrepted para addr in memory 
 * @note
 */
using MemFreeCallback = std::function<void(uint8_t*, uint8_t*)>;

class DE_DLL ModelManager
{
 public:
  /**
   * @brief  construct ModelManager object
   * @param dev_id  the device which  model will lod to  .
   * @note if construct ModelManager object in device side , the dev_id  must be -1.
   */
  ModelManager(int dev_id = -1): dev_id_(dev_id) {};
  ~ModelManager(){};

 /**
   * @brief set model decrypt func
   * @param decrypt_callback   usr defined decrypt func name, which is call DE_REGISTER registered,
   * @param memfree_callback   model mem free func name, which is call DE_REGISTER registered.
   * @return 
   * @note  should be called before Load func.
   */
  void SetDecryptCallback(DecryptCallback decrypt_callback, MemFreeCallback memfree_callback);

  /**
   * @brief load model
   * @param model_file   model file full name,
   * @param para_file    model para file full name . Only need in host side.
   * @param is_encrypt   if value is true  indicate model is encrpt, need  decrypt in load proc.Only need in host side.
   * @return nullptr in Host, model info in Device
   * @note  Load and Unload  must  call  in pairs.
   */
  ModelInfo* Load(const std::string& model_file, const std::string& para_file = "", bool is_encrypt = false);

  /**
   * @brief unload model
   * @param model_file model file full name
   * @note  Load and Unload  must  call  in pairs.
   */
  void Unload(const std::string& model_file);

 private:
  int32_t dev_id_;
  DecryptCallback decrypt_callback_;
  MemFreeCallback memfree_callback_;

};

}
#endif
