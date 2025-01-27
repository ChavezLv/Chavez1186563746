/**
 * @file de_module.h
 * @brief Container of the functions generated by DeSdk.
 *        This is used to support dynamically link, load and save 
 *        functions from different convention under unified API.
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_MODULE_H_
#define DE_MODULE_H_

#include "de_base.h"
#include "de_c_type.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace de {

// The internal container of module.
class ModuleNode;
class PackedFunc;

/*!
 * \brief Module container of DE.
 */
class Module {
 public:
  Module() {}
  // constructor from container.
  explicit Module(std::shared_ptr<ModuleNode> n)
      : node_(n) {}
  /*!
   * \brief Get packed function from current module by name.
   *
   * \param name The name of the function.
   * \param query_imports Whether also query dependency modules.
   * \return The result function.
   *  This function will return PackedFunc(nullptr) if function do not exist.
   * \note Implemented in packed_func.cc
   */
  inline PackedFunc GetFunction(const std::string& name, bool query_imports = false);
  /*! \return internal container */
  inline ModuleNode* operator->();
  /*! \return internal container */
  inline const ModuleNode* operator->() const;
  // The following functions requires link with runtime.
  /*!
   * \brief Import another module into this module.
   * \param other The module to be imported.
   *
   * \note Cyclic dependency is not allowed among modules,
   *  An error will be thrown when cyclic dependency is detected.
   */
  DE_DLL void Import(Module other);
  /*!
   * \brief Load a module from file.
   * \param file_name The name of the host function module.
   * \param format The format of the file.
   * \note This function won't load the import relationship.
   *  Re-create import relationship by calling Import.
   */
  DE_DLL static Module LoadFromFile(const std::string& file_name,
                                     const std::string& format = "");

 private:
  std::shared_ptr<ModuleNode> node_;
};

/*!
 * \brief Base node container of module.
 *  Do not create this directly, instead use Module.
 */
#if defined(_WIN32)
class ModuleNode {
#else
class DE_DLL ModuleNode {
#endif
 public:
  /*! \brief virtual destructor */
  virtual ~ModuleNode() {}
  /*! \return The module type key */
  virtual const char* type_key() const = 0;
  /*!
   * \brief Get a PackedFunc from module.
   *
   *  The PackedFunc may not be fully initialized,
   *  there might still be first time running overhead when
   *  executing the function on certain devices.
   *  For benchmarking, use prepare to eliminate
   *
   * \param name the name of the function.
   * \param sptr_to_self The shared_ptr that points to this module node.
   *
   * \return PackedFunc(nullptr) when it is not available.
   *
   * \note The function will always remain valid.
   *   If the function need resource from the module(e.g. late linking),
   *   it should capture sptr_to_self.
   */
   virtual PackedFunc GetFunction(
      const std::string& name,
      const std::shared_ptr<ModuleNode>& sptr_to_self) = 0;
  /*!
   * \brief Save the module to file.
   * \param file_name The file to be saved to.
   * \param format The format of the file.
   */
  DE_DLL virtual void SaveToFile(const std::string& file_name,
                                  const std::string& format);
  /*!
   * \brief Save the module to binary stream.
   * \param stream The binary stream to save to.
   * \note It is recommended to implement this for device modules,
   *   but not necessarily host modules.
   *   We can use this to do AOT loading of bundled device functions.
   */
  DE_DLL virtual void SaveToBinary(de::Stream* stream);
  /*!
   * \brief Get the source code of module, when available.
   * \param format Format of the source code, can be empty by default.
   * \return Possible source code when available.
   */
  DE_DLL virtual std::string GetSource(const std::string& format = "");
  /*!
   * \brief Get a function from current environment
   *  The environment includes all the imports as well as Global functions.
   *
   * \param name name of the function.
   * \return The corresponding function.
   */
  DE_DLL const PackedFunc* GetFuncFromEnv(const std::string& name);
  /*! \return The module it imports from */
  const std::vector<Module>& imports() const {
    return imports_;
  }

 protected:
  friend class Module;
  /*! \brief The modules this module depend on */
  std::vector<Module> imports_;

 private:
  /*! \brief Cache used by GetImport */
  std::unordered_map<std::string,
                     std::unique_ptr<PackedFunc> > import_cache_;
};

/*! \brief namespace for constant symbols */
namespace symbol {
/*! \brief Global variable to store module context. */
constexpr const char* de_module_ctx = "__de_module_ctx";
/*! \brief Global variable to store device module blob */
constexpr const char* de_dev_mblob = "__de_dev_mblob";
/*! \brief Number of bytes of device module blob. */
constexpr const char* de_dev_mblob_nbytes = "__de_dev_mblob_nbytes";
/*! \brief global function to set device */
constexpr const char* de_set_device = "__de_set_device";
/*! \brief Auxiliary counter to global barrier. */
constexpr const char* de_global_barrier_state = "__de_global_barrier_state";
/*! \brief Prepare the global barrier before kernels that uses global barrier. */
constexpr const char* de_prepare_global_barrier = "__de_prepare_global_barrier";
/*! \brief Placeholder for the module's entry function. */
constexpr const char* de_module_main = "__de_main__";
}  // namespace symbol

// implementations of inline functions.
inline ModuleNode* Module::operator->() {
  return node_.get();
}

inline const ModuleNode* Module::operator->() const {
  return node_.get();
}

}  // namespace DE

#include "de_packed_func.h"
#endif  // DE_RUNTIME_MODULE_H_
