/**
 * @file de_base.h
 * @brief define common configuration macros.
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 * @note configuration macros in this file can be overritten by CMake
 */

#ifndef DE_BASE_H_
#define DE_BASE_H_

#include<vector>
#include<string>
#include<stdexcept>
#include<map>
#include<set>
#include<list>
#include<deque>
#include<unordered_map>
#include<unordered_set>

#ifdef _MSC_VER
//! \cond Doxygen_Suppress
typedef signed char int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned char uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
//! \endcond
#else
#include <inttypes.h>
#endif

// Macros to do weak linking
#ifdef _MSC_VER
#define DE_WEAK __declspec(selectany)
#else
#define DE_WEAK __attribute__((weak))
#endif

/*! \brief helper macro to supress unused warning */
#if defined(__GNUC__)
#define DE_ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define DE_ATTRIBUTE_UNUSED
#endif

#define DE_STR_CONCAT_(__x, __y) __x##__y
#define DE_STR_CONCAT(__x, __y) DE_STR_CONCAT_(__x, __y)

 /*!
  * \brief whether throw de::Error instead of
  *  directly calling abort when FATAL error occured
  *  NOTE: this may still not be perfect.
  *  do not use FATAL and CHECK in destructors
  */
#ifndef DE_LOG_FATAL_THROW
#define DE_LOG_FATAL_THROW 1
#endif

  /*!
   * \brief whether always log a message before throw
   * This can help identify the error that cannot be catched.
   */
#ifndef DE_LOG_BEFORE_THROW
#define DE_LOG_BEFORE_THROW 0
#endif

   /*!
    * \brief Whether to use customized logger,
    * whose output can be decided by other libraries.
    */
#ifndef DE_LOG_CUSTOMIZE
#define DE_LOG_CUSTOMIZE 0
#endif

/*! \brief whether or not use c++11 support */
#ifndef DE_USE_CXX11
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(_MSC_VER)
#define DE_USE_CXX11 1
#else
#define DE_USE_CXX11 (__cplusplus >= 201103L)
#endif
#endif

/*! \brief Whether cxx11 thread local is supported */
#ifndef DE_CXX11_THREAD_LOCAL
#if defined(_MSC_VER)
#define DE_CXX11_THREAD_LOCAL (_MSC_VER >= 1900)
#elif defined(__clang__)
#define DE_CXX11_THREAD_LOCAL (__has_feature(cxx_thread_local))
#else
#define DE_CXX11_THREAD_LOCAL (__cplusplus >= 201103L)
#endif
#endif

#if DE_USE_CXX11
#define DE_THROW_EXCEPTION noexcept(false)
#define DE_NO_EXCEPTION  noexcept(true)
#else
#define DE_THROW_EXCEPTION
#define DE_NO_EXCEPTION
#endif

/* default logic for stack trace */
#if (defined(__GNUC__) && !defined(__MINGW32__)\
     && !defined(__sun) && !defined(__SVR4)\
     && !(defined __MINGW64__) && !(defined __ANDROID__))\
     && !defined(__CYGWIN__) && !defined(__EMSCRIPTEN__)\
     && !defined(__RISCV__) \
     && !defined(USE_HISIV500_PLATFORM)
  #define DE_LOG_STACK_TRACE 1
  #define DE_LOG_STACK_TRACE_SIZE 10
  #define DE_EXECINFO_H <execinfo.h>
#endif

/*!
    * \brief Use little endian for binary serialization
    *  if this is set to 0, use big endian.
    */
#ifndef DE_IO_USE_LITTLE_ENDIAN
#define DE_IO_USE_LITTLE_ENDIAN 1
#endif

#if defined(__APPLE__) || defined(_WIN32) || defined(__CYGWIN__)
#define DE_LITTLE_ENDIAN 1
#elif defined(__GLIBC__) || defined(__ANDROID__) || defined(__RISCV__)
#include <endian.h>
#define DE_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#define DE_LITTLE_ENDIAN (_BYTE_ORDER == _LITTLE_ENDIAN)
#elif defined(__EMSCRIPTEN__)
#define DE_LITTLE_ENDIAN 1
#elif defined(__sun) && defined(__SVR4)
     // Solaris supports x86 (little endian) and SPARC (big endian)
#if defined(__x86_64) || defined(__i386__)
#define DE_LITTLE_ENDIAN 1
#else
#define DE_LITTLE_ENDIAN 0
#endif
#else
#error "Unable to determine endianness of your machine; use CMake to compile"
#endif

/*!
 * \brief The stream that is specific to device
 * can be NULL, which indicates the default one.
 */
typedef void* DEStreamHandle;

/*!
 * \brief Byte array type used to pass in byte array
 *  When kBytes is used as data type.
 */
typedef struct {
  const char* data;
  size_t size;
} DEByteArray;


#include <type_traits>

/*! \brief whether serialize using little endian */
#define DE_IO_NO_ENDIAN_SWAP (DE_LITTLE_ENDIAN == DE_IO_USE_LITTLE_ENDIAN)

/*! \brief  macro to guard beginning and end section of all functions */
#define API_BEGIN() try {
/*! \brief every function starts with API_BEGIN();
     and finishes with API_END() or API_END_HANDLE_ERROR */
#define API_END() } catch(std::runtime_error &_except_) { return DEAPIHandleException(_except_); } return 0;  // NOLINT(*)
/*!
 * \brief every function starts with API_BEGIN();
 *   and finishes with API_END() or API_END_HANDLE_ERROR
 *   The finally clause contains procedure to cleanup states when an error happens.
 */
#define API_END_HANDLE_ERROR(Finalize) } catch(std::runtime_error &_except_) { Finalize; return DEAPIHandleException(_except_); } return 0; // NOLINT(*)

/*!
 * \brief handle exception throwed out
 * \param e the exception
 * \return the return value of API after exception is handled
 */
int DEAPIHandleException(const std::runtime_error &e);

/*! \brief namespace for de */
namespace de {
    /*!
     * \brief safely get the beginning address of a vector
     * \param vec input vector
     * \return beginning address of a vector
     */
    template<typename T>
    inline T *BeginPtr(std::vector<T> &vec) {  // NOLINT(*)
        if (vec.size() == 0) {
            return nullptr;
        }
        else {
            return &vec[0];
        }
    }
    /*!
     * \brief get the beginning address of a const vector
     * \param vec input vector
     * \return beginning address of a vector
     */
    template<typename T>
    inline const T *BeginPtr(const std::vector<T> &vec) {
        if (vec.size() == 0) {
            return nullptr;
        }
        else {
            return &vec[0];
        }
    }
    /*!
     * \brief get the beginning address of a string
     * \param str input string
     * \return beginning address of a string
     */
    inline char* BeginPtr(std::string &str) {  // NOLINT(*)
        if (str.length() == 0) return nullptr;
        return &str[0];
    }
    /*!
     * \brief get the beginning address of a const string
     * \param str input string
     * \return beginning address of a string
     */
    inline const char* BeginPtr(const std::string &str) {
        if (str.length() == 0) return nullptr;
        return &str[0];
    }

    /*!
 * \brief A generic inplace byte swapping function.
 * \param data The data pointer.
 * \param elem_bytes The number of bytes of the data elements
 * \param num_elems Number of elements in the data.
 * \note Always try pass in constant elem_bytes to enable
 *       compiler optimization
 */
    inline void ByteSwap(void* data, size_t elem_bytes, size_t num_elems) {
        for (size_t i = 0; i < num_elems; ++i) {
            uint8_t* bptr = reinterpret_cast<uint8_t*>(data) + elem_bytes * i;
            for (size_t j = 0; j < elem_bytes / 2; ++j) {
                uint8_t v = bptr[elem_bytes - 1 - j];
                bptr[elem_bytes - 1 - j] = bptr[j];
                bptr[j] = v;
            }
        }
    }

    /*!
   * \brief interface of stream I/O for serialization
   */
  class Stream {  // NOLINT(*)
  public:
	/*!
	 * \brief reads data from a stream
	 * \param ptr pointer to a memory buffer
	 * \param size block size
	 * \return the size of data read
	 */
	virtual size_t Read(void *ptr, size_t size) = 0;
	/*!
	 * \brief writes data to a stream
	 * \param ptr pointer to a memory buffer
	 * \param size block size
	 */
	virtual void Write(const void *ptr, size_t size) = 0;
	/*! \brief virtual destructor */
	virtual ~Stream(void) {}
	/*!
	 * \brief generic factory function
	 *  create an stream, the stream will close the underlying files upon deletion
	 *
	 * \param uri the uri of the input currently we support
	 *            hdfs://, s3://, and file:// by default file:// will be used
	 * \param flag can be "w", "r", "a"
	 * \param allow_null whether NULL can be returned, or directly report error
	 * \return the created stream, can be NULL when allow_null == true and file do not exist
	 */
	static Stream *Create(const char *uri,
	  const char* const flag,
	  bool allow_null = false);
	// helper functions to write/read different data structures
	/*!
	 * \brief writes a data to stream.
	 *
	 * de::Stream support Write/Read of most STL composites and base types.
	 * If the data type is not supported, a compile time error will be issued.
	 *
	 * This function is endian-aware,
	 * the output endian defined by DE_IO_USE_LITTLE_ENDIAN
	 *
	 * \param data data to be written
	 * \tparam T the data type to be written
	 */
	template<typename T>
	inline void Write(const T &data);
	/*!
	 * \brief loads a data from stream.
	 *
	 * de::Stream support Write/Read of most STL composites and base types.
	 * If the data type is not supported, a compile time error will be issued.
	 *
	 * This function is endian-aware,
	 * the input endian defined by DE_IO_USE_LITTLE_ENDIAN
	 *
	 * \param out_data place holder of data to be deserialized
	 * \return whether the load was successful
	 */
	template<typename T>
	inline bool Read(T *out_data);
	/*!
	 * \brief Endian aware write array of data.
	 * \param data The data pointer
	 * \param num_elems Number of elements
	 * \tparam T the data type.
	 */
	template<typename T>
	inline void WriteArray(const T* data, size_t num_elems);
	/*!
	 * \brief Endian aware read array of data.
	 * \param data The data pointer
	 * \param num_elems Number of elements
	 * \tparam T the data type.
	 * \return whether the load was successful
	 */
	template<typename T>
	inline bool ReadArray(T* data, size_t num_elems);
  };

  /*!
   * \brief helper class to construct a string that represents type name
   *
   * Specialized this class to defined type name of custom types
   *
   * \tparam T the type to query
   */
  template<typename T>
  struct type_name_helper {
    /*!
     * \return a string of typename.
     */
    static inline std::string value() {
      return "";
    }
  };

  /*!
   * \brief the string representation of type name
   * \tparam T the type to query
   * \return a const string of typename.
   */
  template<typename T>
  inline std::string type_name() {
    return type_name_helper<T>::value();
  }

  /*!
   * \brief whether a type have save/load function
   * \tparam T the type to query
   */
  template<typename T>
  struct has_saveload {
    /*! \brief the value of the traits */
    static const bool value = false;
  };

  /*!
   * \brief template to select type based on condition
   * For example, IfThenElseType<true, int, float>::Type will give int
   * \tparam cond the condition
   * \tparam Then the typename to be returned if cond is true
   * \tparam Else typename to be returned if cond is false
  */
  template<bool cond, typename Then, typename Else>
  struct IfThenElseType;

  /*! \brief macro to quickly declare traits information */
  #define DE_DECLARE_TRAITS(Trait, Type, Value)       \
    template<>                                          \
    struct Trait<Type> {                                \
      static const bool value = Value;                  \
    }

  /*! \brief macro to quickly declare traits information */
  #define DE_DECLARE_TYPE_NAME(Type, Name)            \
    template<>                                          \
    struct type_name_helper<Type> {                     \
      static inline std::string value() {               \
        return Name;                                    \
      }                                                 \
    }

  DE_DECLARE_TYPE_NAME(float, "float");
  DE_DECLARE_TYPE_NAME(double, "double");
  DE_DECLARE_TYPE_NAME(int, "int");
  DE_DECLARE_TYPE_NAME(uint32_t, "int (non-negative)");
  DE_DECLARE_TYPE_NAME(uint64_t, "long (non-negative)");
  DE_DECLARE_TYPE_NAME(std::string, "string");
  DE_DECLARE_TYPE_NAME(bool, "boolean");
  DE_DECLARE_TYPE_NAME(void*, "ptr");

  template<typename Then, typename Else>
  struct IfThenElseType<true, Then, Else> {
    typedef Then Type;
  };

  template<typename Then, typename Else>
  struct IfThenElseType<false, Then, Else> {
    typedef Else Type;
  };
  //! \endcond

  /*! \brief internal namespace for serializers */
  namespace serializer {
  /*!
   * \brief generic serialization handler
   * \tparam T the type to be serialized
   * \tparam need_endian_swap Whether use little endian
   */
  template<typename T>
  struct Handler;

  //! \cond Doxygen_Suppress
  /*!
   * \brief Serializer that redirect calls by condition
   * \tparam cond the condition
   * \tparam Then the serializer used for then condition
   * \tparam Else the serializer used for else condition
   * \tparam Return the type of data the serializer handles
   */
  template<bool cond, typename Then, typename Else, typename Return>
  struct IfThenElse;

  template<typename Then, typename Else, typename T>
  struct IfThenElse<true, Then, Else, T> {
	inline static void Write(de::Stream *strm, const T &data) {
	  Then::Write(strm, data);
	}
	inline static bool Read(de::Stream *strm, T *data) {
	  return Then::Read(strm, data);
	}
  };
  template<typename Then, typename Else, typename T>
  struct IfThenElse<false, Then, Else, T> {
	inline static void Write(de::Stream *strm, const T &data) {
	  Else::Write(strm, data);
	}
	inline static bool Read(de::Stream *strm, T *data) {
	  return Else::Read(strm, data);
	}
  };

  /*! \brief Serializer for POD(plain-old-data) data */
  template<typename T>
  struct NativePODHandler {
	inline static void Write(de::Stream *strm, const T &data) {
	  strm->Write(&data, sizeof(T));
	}
	inline static bool Read(de::Stream *strm, T *dptr) {
	  return strm->Read((void*)dptr, sizeof(T)) == sizeof(T);  // NOLINT(*)
	}
  };

  /*! \brief Serializer for arithmetic data, handle endianness */
  template<typename T>
  struct ArithmeticHandler {
	inline static void Write(de::Stream *strm, const T &data) {
	  if (DE_IO_NO_ENDIAN_SWAP) {
		strm->Write(&data, sizeof(T));
	  } else {
		T copy = data;
		ByteSwap(&copy, sizeof(T), 1);
		strm->Write(&copy, sizeof(T));
	  }
	}
	inline static bool Read(de::Stream *strm, T *dptr) {
	  bool ret = strm->Read((void*)dptr, sizeof(T)) == sizeof(T);  // NOLINT(*)
	  if (!DE_IO_NO_ENDIAN_SWAP) {
		ByteSwap(dptr, sizeof(T), 1);
	  }
	  return ret;
	}
  };

  // serializer for class that have save/load function
  template<typename T>
  struct SaveLoadClassHandler {
	inline static void Write(de::Stream *strm, const T &data) {
	  data.Save(strm);
	}
	inline static bool Read(de::Stream *strm, T *data) {
	  return data->Load(strm);
	}
  };

  /*!
   * \brief dummy class for undefined serialization.
   *   This is used to generate error message when user tries to
   *   serialize something that is not supported.
   * \tparam T the type to be serialized
   */
  template<typename T>
  struct UndefinedSerializerFor {
  };

  /*!
   * \brief Serializer handler for std::vector<T> where T is POD type.
   * \tparam T element type
   */
  template<typename T>
  struct NativePODVectorHandler {
	inline static void Write(de::Stream *strm, const std::vector<T> &vec) {
	  uint64_t sz = static_cast<uint64_t>(vec.size());
	  strm->Write<uint64_t>(sz);
	  if (sz != 0) {
		strm->Write(&vec[0], sizeof(T) * vec.size());
	  }
	}
	inline static bool Read(de::Stream *strm, std::vector<T> *out_vec) {
	  uint64_t sz;
	  if (!strm->Read<uint64_t>(&sz)) return false;
	  size_t size = static_cast<size_t>(sz);
	  out_vec->resize(size);
	  if (sz != 0) {
		size_t nbytes = sizeof(T) * size;
		return strm->Read(&(*out_vec)[0], nbytes) == nbytes;
	  }
	  return true;
	}
  };

  /*!
   * \brief Serializer handler for std::vector<T> where T can be composed type
   * \tparam T element type
   */
  template<typename T>
  struct ComposeVectorHandler {
	inline static void Write(de::Stream *strm, const std::vector<T> &vec) {
	  uint64_t sz = static_cast<uint64_t>(vec.size());
	  strm->Write<uint64_t>(sz);
	  strm->WriteArray(de::BeginPtr(vec), vec.size());
	}
	inline static bool Read(de::Stream *strm, std::vector<T> *out_vec) {
	  uint64_t sz;
	  if (!strm->Read<uint64_t>(&sz)) return false;
	  size_t size = static_cast<size_t>(sz);
	  out_vec->resize(size);
	  return strm->ReadArray(de::BeginPtr(*out_vec), size);
	}
  };

  /*!
   * \brief Serializer handler for std::basic_string<T> where T is POD type.
   * \tparam T element type
   */
  template<typename T>
  struct NativePODStringHandler {
	inline static void Write(de::Stream *strm, const std::basic_string<T> &vec) {
	  uint64_t sz = static_cast<uint64_t>(vec.length());
	  strm->Write<uint64_t>(sz);
	  if (sz != 0) {
		strm->Write(&vec[0], sizeof(T) * vec.length());
	  }
	}
	inline static bool Read(de::Stream *strm, std::basic_string<T> *out_vec) {
	  uint64_t sz;
	  if (!strm->Read<uint64_t>(&sz)) return false;
	  size_t size = static_cast<size_t>(sz);
	  out_vec->resize(size);
	  if (sz != 0) {
		size_t nbytes = sizeof(T) * size;
		return strm->Read(&(*out_vec)[0], nbytes) == nbytes;
	  }
	  return true;
	}
  };

  /*! \brief Serializer for std::pair */
  template<typename TA, typename TB>
  struct PairHandler {
	inline static void Write(de::Stream *strm, const std::pair<TA, TB> &data) {
	  Handler<TA>::Write(strm, data.first);
	  Handler<TB>::Write(strm, data.second);
	}
	inline static bool Read(de::Stream *strm, std::pair<TA, TB> *data) {
	  return Handler<TA>::Read(strm, &(data->first)) &&
		  Handler<TB>::Read(strm, &(data->second));
	}
  };

  // set type handler that can handle most collection type case
  template<typename ContainerType, typename ElemType>
  struct CollectionHandler {
	inline static void Write(de::Stream *strm, const ContainerType &data) {
	  // dump data to vector
	  std::vector<ElemType> vdata(data.begin(), data.end());
	  // serialize the vector
	  Handler<std::vector<ElemType> >::Write(strm, vdata);
	}
	inline static bool Read(de::Stream *strm, ContainerType *data) {
	  std::vector<ElemType> vdata;
	  if (!Handler<std::vector<ElemType> >::Read(strm, &vdata)) return false;
	  data->clear();
	  data->insert(vdata.begin(), vdata.end());
	  return true;
	}
  };


  // handler that can handle most list type case
  // this type insert function takes additional iterator
  template<typename ListType>
  struct ListHandler {
	inline static void Write(de::Stream *strm, const ListType &data) {
	  typedef typename ListType::value_type ElemType;
	  // dump data to vector
	  std::vector<ElemType> vdata(data.begin(), data.end());
	  // serialize the vector
	  Handler<std::vector<ElemType> >::Write(strm, vdata);
	}
	inline static bool Read(de::Stream *strm, ListType *data) {
	  typedef typename ListType::value_type ElemType;
	  std::vector<ElemType> vdata;
	  if (!Handler<std::vector<ElemType> >::Read(strm, &vdata)) return false;
	  data->clear();
	  data->insert(data->begin(), vdata.begin(), vdata.end());
	  return true;
	}
  };

  //! \endcond

  /*!
   * \brief generic serialization handler for type T
   *
   *  User can define specialization of this class to support
   *  composite serialization of their own class.
   *
   * \tparam T the type to be serialized
   */
  template<typename T>
  struct Handler {
	/*!
	 * \brief write data to stream
	 * \param strm the stream we write the data.
	 * \param data the data obeject to be serialized
	 */
	inline static void Write(de::Stream *strm, const T &data) {
	  IfThenElse<std::is_arithmetic<T>::value,
				 ArithmeticHandler<T>,
				 IfThenElse<std::is_pod<T>::value && DE_IO_NO_ENDIAN_SWAP,
							NativePODHandler<T>,
							IfThenElse<de::has_saveload<T>::value,
									   SaveLoadClassHandler<T>,
									   UndefinedSerializerFor<T>, T>,
							T>,
				 T>
		  ::Write(strm, data);
	}
	/*!
	 * \brief read data to stream
	 * \param strm the stream to read the data.
	 * \param data the pointer to the data obeject to read
	 * \return whether the read is successful
	 */
	inline static bool Read(de::Stream *strm, T *data) {
	  return
	  IfThenElse<std::is_arithmetic<T>::value,
				 ArithmeticHandler<T>,
				 IfThenElse<std::is_pod<T>::value && DE_IO_NO_ENDIAN_SWAP,
							NativePODHandler<T>,
							IfThenElse<de::has_saveload<T>::value,
									   SaveLoadClassHandler<T>,
									   UndefinedSerializerFor<T>, T>,
							T>,
				 T>
	  ::Read(strm, data);
	}
  };

  //! \cond Doxygen_Suppress
  template<typename T>
  struct Handler<std::vector<T> > {
	inline static void Write(de::Stream *strm, const std::vector<T> &data) {
	  IfThenElse<std::is_pod<T>::value && DE_IO_NO_ENDIAN_SWAP,
				 NativePODVectorHandler<T>,
				 ComposeVectorHandler<T>, std::vector<T> >
	  ::Write(strm, data);
	}
	inline static bool Read(de::Stream *strm, std::vector<T> *data) {
	  return IfThenElse<std::is_pod<T>::value && DE_IO_NO_ENDIAN_SWAP,
						NativePODVectorHandler<T>,
						ComposeVectorHandler<T>,
						std::vector<T> >
	  ::Read(strm, data);
	}
  };

  template<typename T>
  struct Handler<std::basic_string<T> > {
	inline static void Write(de::Stream *strm, const std::basic_string<T> &data) {
	  IfThenElse<std::is_pod<T>::value && (DE_IO_NO_ENDIAN_SWAP || sizeof(T) == 1),
				 NativePODStringHandler<T>,
				 UndefinedSerializerFor<T>,
				 std::basic_string<T> >
	  ::Write(strm, data);
	}
	inline static bool Read(de::Stream *strm, std::basic_string<T> *data) {
	  return IfThenElse<std::is_pod<T>::value && (DE_IO_NO_ENDIAN_SWAP || sizeof(T) == 1),
						NativePODStringHandler<T>,
						UndefinedSerializerFor<T>,
						std::basic_string<T> >
	  ::Read(strm, data);
	}
  };

  template<typename TA, typename TB>
  struct Handler<std::pair<TA, TB> > {
	inline static void Write(de::Stream *strm, const std::pair<TA, TB> &data) {
	  IfThenElse<std::is_pod<TA>::value &&
				 std::is_pod<TB>::value &&
				 DE_IO_NO_ENDIAN_SWAP,
				 NativePODHandler<std::pair<TA, TB> >,
				 PairHandler<TA, TB>,
				 std::pair<TA, TB> >
	  ::Write(strm, data);
	}
	inline static bool Read(de::Stream *strm, std::pair<TA, TB> *data) {
	  return IfThenElse<std::is_pod<TA>::value &&
						std::is_pod<TB>::value &&
						DE_IO_NO_ENDIAN_SWAP,
						NativePODHandler<std::pair<TA, TB> >,
						PairHandler<TA, TB>,
						std::pair<TA, TB> >
	  ::Read(strm, data);
	}
  };

  template<typename K, typename V>
  struct Handler<std::map<K, V> >
	  : public CollectionHandler<std::map<K, V>, std::pair<K, V> > {
  };

  template<typename K, typename V>
  struct Handler<std::multimap<K, V> >
	  : public CollectionHandler<std::multimap<K, V>, std::pair<K, V> > {
  };

  template<typename T>
  struct Handler<std::set<T> >
	  : public CollectionHandler<std::set<T>, T> {
  };

  template<typename T>
  struct Handler<std::multiset<T> >
	  : public CollectionHandler<std::multiset<T>, T> {
  };

  template<typename T>
  struct Handler<std::list<T> >
	  : public ListHandler<std::list<T> > {
  };

  template<typename T>
  struct Handler<std::deque<T> >
	  : public ListHandler<std::deque<T> > {
  };


  template<typename K, typename V>
  struct Handler<std::unordered_map<K, V> >
	  : public CollectionHandler<std::unordered_map<K, V>, std::pair<K, V> > {
  };

  template<typename K, typename V>
  struct Handler<std::unordered_multimap<K, V> >
	  : public CollectionHandler<std::unordered_multimap<K, V>, std::pair<K, V> > {
  };

  template<typename T>
  struct Handler<std::unordered_set<T> >
	  : public CollectionHandler<std::unordered_set<T>, T> {
  };

  template<typename T>
  struct Handler<std::unordered_multiset<T> >
	  : public CollectionHandler<std::unordered_multiset<T>, T> {
  };

  //! \endcond
  }  // namespace serializer

  // implementations of inline functions
	template<typename T>
	inline void Stream::Write(const T &data) {
	  de::serializer::Handler<T>::Write(this, data);
	}
	template<typename T>
	inline bool Stream::Read(T *out_data) {
	  return de::serializer::Handler<T>::Read(this, out_data);
	}

	template<typename T>
	inline void Stream::WriteArray(const T* data, size_t num_elems) {
	  for (size_t i = 0; i < num_elems; ++i) {
		this->Write<T>(data[i]);
	  }
	}

	template<typename T>
	inline bool Stream::ReadArray(T* data, size_t num_elems) {
	  for (size_t i = 0; i < num_elems; ++i) {
		if (!this->Read<T>(data + i)) return false;
	  }
	  return true;
	}
}

#endif  // DE_BASE_H_

