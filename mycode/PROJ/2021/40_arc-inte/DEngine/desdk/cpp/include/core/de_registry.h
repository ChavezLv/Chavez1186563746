/**
 * @file de_registry.h
 * @brief Defines the DeSdk global function registry.
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 * @note The registered functions will be made available to front-end 
 *       as well as backend uses.\n
 *       The registry stores type-erased functions.\n
 *       Each registered function is automatically exposed to front-end
 *       language(e.g. Python)
 *       Front-end can also pass callbacks as PackedFunc, or register then
 *       into the same global registry in C++.\n
 *       The goal is to mix the front-end language and the DeSdk back-end.\
 *       
 * @code
 *      //register the function as MyAPIFuncName
 *      DE_REGISTER_GLOBAL(MyAPIFuncName)
 *      .set_body([]DEArgs args, DERetValue* rv) {
 *          //my code
 *      });
 * @endcode
 */

#ifndef DE_REGISTRY_H_
#define DE_REGISTRY_H_

#include <string>
#include <vector>
#include "de_packed_func.h"

namespace de {

/*! \brief Registry for global function */
class Registry {
 public:
  /*!
   * \brief set the body of the function to be f
   * \param f The body of the function.
   */
  DE_DLL Registry& set_body(PackedFunc f);  // NOLINT(*)
  /*!
   * \brief set the body of the function to be f
   * \param f The body of the function.
   */
  Registry& set_body(PackedFunc::FType f) {  // NOLINT(*)
    return set_body(PackedFunc(f));
  }
  /*!
   * \brief set the body of the function to be TypedPackedFunc.
   *
   * \code
   *
   * DE_REGISTER_API("addone")
   * .set_body_typed<int(int)>([](int x) { return x + 1; });
   *
   * \endcode
   *
   * \param f The body of the function.
   * \tparam FType the signature of the function.
   * \tparam FLambda The type of f.
   */
  template<typename FType, typename FLambda>
  Registry& set_body_typed(FLambda f) {
    return set_body(TypedPackedFunc<FType>(f).packed());
  }

  /*!
   * \brief set the body of the function to the given function pointer.
   *        Note that this doesn't work with lambdas, you need to
   *        explicitly give a type for those.
   *        Note that this will ignore default arg values and always require all arguments to be provided.
   *
   * \code
   * 
   * int multiply(int x, int y) {
   *   return x * y;
   * }
   *
   * DE_REGISTER_API("multiply")
   * .set_body_typed(multiply); // will have type int(int, int)
   *
   * \endcode
   *
   * \param f The function to forward to.
   * \tparam R the return type of the function (inferred).
   * \tparam Args the argument types of the function (inferred).
   */
  template<typename R, typename ...Args>
  Registry& set_body_typed(R (*f)(Args...)) {
    return set_body(TypedPackedFunc<R(Args...)>(f));
  }

  /*!
   * \brief set the body of the function to be the passed method pointer.
   *        Note that this will ignore default arg values and always require all arguments to be provided.
   *
   * \code
   * 
   * // node subclass:
   * struct Example {
   *    int doThing(int x);
   * }
   * DE_REGISTER_API("Example_doThing")
   * .set_body_method(&Example::doThing); // will have type int(Example, int)
   *
   * \endcode
   *
   * \param f the method pointer to forward to.
   * \tparam T the type containing the method (inferred).
   * \tparam R the return type of the function (inferred).
   * \tparam Args the argument types of the function (inferred).
   */
  template<typename T, typename R, typename ...Args>
  Registry& set_body_method(R (T::*f)(Args...)) {
    return set_body_typed<R(T, Args...)>([f](T target, Args... params) -> R {
      // call method pointer
      return (target.*f)(params...);
    });
  }

  /*!
   * \brief set the body of the function to be the passed method pointer.
   *        Note that this will ignore default arg values and always require all arguments to be provided.
   *
   * \code
   * 
   * // node subclass:
   * struct Example {
   *    int doThing(int x);
   * }
   * DE_REGISTER_API("Example_doThing")
   * .set_body_method(&Example::doThing); // will have type int(Example, int)
   *
   * \endcode
   *
   * \param f the method pointer to forward to.
   * \tparam T the type containing the method (inferred).
   * \tparam R the return type of the function (inferred).
   * \tparam Args the argument types of the function (inferred).
   */
  template<typename T, typename R, typename ...Args>
  Registry& set_body_method(R (T::*f)(Args...) const) {
    return set_body_typed<R(T, Args...)>([f](const T target, Args... params) -> R {
      // call method pointer
      return (target.*f)(params...);
    });
  }

  /*!
   * \brief Register a function with given name
   * \param name The name of the function.
   * \param override Whether allow oveeride existing function.
   * \return Reference to theregistry.
   */
  DE_DLL static Registry& Register(const std::string& name, bool override = false);  // NOLINT(*)
  /*!
   * \brief Register a function with given event id
   * \param event_id The event id of the function.
   * \param override Whether allow oveeride existing function.
   * \return Reference to theregistry.
   */
  DE_DLL static Registry& Register(int event_id, bool override = false);  // NOLINT(*)
  /*!
   * \brief Erase global function from registry, if exist.
   * \param name The name of the function.
   * \return Whether function exist.
   */
  DE_DLL static bool Remove(const std::string& name);
  /*!
   * \brief Erase global function from registry, if exist.
   * \param event_id The event id of the function.
   * \return Whether function exist.
   */
  DE_DLL static bool Remove(int event_id);
  /*!
   * \brief Get the global function by name.
   * \param name The name of the function.
   * \return pointer to the registered function,
   *   nullptr if it does not exist.
   */
  DE_DLL static const PackedFunc* Get(const std::string& name);  // NOLINT(*)
  /*!
   * \brief Get the global function by event id.
   * \param event_id The event id of the function.
   * \return pointer to the registered function,
   *   nullptr if it does not exist.
   */
  DE_DLL static const PackedFunc* Get(int event_id);  // NOLINT(*)
  /*!
   * \brief Get the names of currently registered global function.
   * \return The names
   */
  DE_DLL static std::vector<std::string> ListNames();

  // Internal class.
  struct Manager;

 protected:
  /*! \brief name of the function */
  std::string name_;
  /*! \brief event id of the function */
  int id_;
  /*! \brief internal packed function */
  PackedFunc func_;
  friend struct Manager;
};

/*! \brief helper macro to supress unused warning */
#if defined(__GNUC__)
#define DE_ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define DE_ATTRIBUTE_UNUSED
#endif

#define DE_STR_CONCAT_(__x, __y) __x##__y
#define DE_STR_CONCAT(__x, __y) DE_STR_CONCAT_(__x, __y)

#define DE_FUNC_REG_VAR_DEF                                            \
  static DE_ATTRIBUTE_UNUSED ::de::Registry& __mk_ ## DE

#define DE_TYPE_REG_VAR_DEF                                            \
  static DE_ATTRIBUTE_UNUSED ::de::ExtTypeVTable* __mk_ ## DET

/*!
 * \brief Register a function globally.
 * \code
 *   DE_REGISTER_GLOBAL("MyPrint")
 *   .set_body([](DEArgs args, DERetValue* rv) {
 *   });
 * \endcode
 */
#define DE_REGISTER_GLOBAL(OpName)                              \
  DE_STR_CONCAT(DE_FUNC_REG_VAR_DEF, __COUNTER__) =            \
      ::de::Registry::Register(OpName)

/*!
 * \brief Macro to register extension type.
 *  This must be registered in a cc file
 *  after the trait extension_type_info is defined.
 */
#define DE_REGISTER_EXT_TYPE(T)                                 \
  DE_STR_CONCAT(DE_TYPE_REG_VAR_DEF, __COUNTER__) =            \
      ::de::ExtTypeVTable::Register_<T>()

/*!
 * \brief Register a function globally.
 * \code
 *   DE_REGISTER_EVENT(1)
 *   .set_body([](DEArgs args, DERetValue* rv) {
 *   });
 * \endcode
 */
#define DE_REGISTER_EVENT(EventId)                              \
    DE_STR_CONCAT(DE_FUNC_REG_VAR_DEF, __COUNTER__) =          \
      ::de::Registry::Register(EventId)

}  // namespace de
#endif  // DE_RUNTIME_REGISTRY_H_
