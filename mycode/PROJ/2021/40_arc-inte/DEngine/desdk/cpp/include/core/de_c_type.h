/**
 * @file de_c_type.h
 * @brief Define general data c type in DeSdk
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_C_TYPE_H_
#define DE_C_TYPE_H_

#ifndef DE_DLL
#ifdef _WIN32
#ifdef DE_EXPORTS
#define DE_DLL __declspec(dllexport)
#else
#define DE_DLL __declspec(dllimport)
#endif
#else
#define DE_DLL __attribute__((visibility("default")))
#endif
#endif

#define DE_API DE_DLL

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

#ifdef __cplusplus
extern "C" {
#endif

/**@struct Rect
* @brief rect
*/
typedef struct {
  int x;
  int y;
  int w;
  int h;
} deRect;

/**@enum ShapeCode
* @brief tensor shape type
*/
typedef enum {
  NDHW = 0,
  DHW,
  HW,
  W
}deShapeCode;

/**@brief image size in pixel
*/
typedef struct{
  int width;    ///< width
  int height;   ///< height
} deSize;

/**@enum PixelFormat
* @brief pixel format
*/
enum dePixelFormat {
  DE_PIX_FMT_NONE = -1,                ///<
  DE_PIX_FMT_NV12 = 0,                 ///< YYYY... UVUVUV... 
  DE_PIX_FMT_NV21 = 1,                 ///< YYYY... VUVUVU... 
  //DE_PIX_FMT_GREY = 2,               ///< only Y 
  DE_PIX_FMT_YUV422_YUYV = 3,          ///< YUYVYUYV...  
  DE_PIX_FMT_YUV422_UYVY = 4,          ///< UYVYUYVY... 
  //DE_PIX_FMT_YUV422_UYVY_GREY = 5,   ///< only Y 
  DE_PIX_FMT_BGR888_PLANE = 6,         ///< 24-bit BGR 32bpp, plane: b...g...r... 
  DE_PIX_FMT_RGB888_PLANE = 7,         ///< 24-bit RGB 32bpp, plane: r...g...b...

  //
  DE_PIX_FMT_GRAY8 = 8,            ///< packed GRAY 8, 8bpp 
  DE_PIX_FMT_NV16 = 9,                 ///< YUV  4:2:2 16bpp 
  DE_PIX_FMT_YUV420P = 10,             ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) 
  DE_PIX_FMT_RGB565 = 11,              ///< 16-bit RGB 16bpp 
  DE_PIX_FMT_BGR565 = 12,              ///< 16-bit RGB 16bpp 
  DE_PIX_FMT_RGB555 = 13,              ///< 15-bit RGB 16bpp 
  DE_PIX_FMT_BGR555 = 14,              ///< 15-bit RGB 16bpp 
  DE_PIX_FMT_RGB444 = 15,              ///< 12-bit RGB 16bpp 
  DE_PIX_FMT_BGR444 = 16,              ///< 12-bit RGB 16bpp 
  DE_PIX_FMT_RGB101010 = 17,           ///< 30-bit RGB 32bpp 
  DE_PIX_FMT_BGR101010 = 18,           ///< 30-bit RGB 32bpp 

  DE_PIX_FMT_H264 = 30 + 1,
  DE_PIX_FMT_H265 = 32,
  DE_PIX_FMT_MJPEG = 33,
  DE_PIX_FMT_JPEG = 34,

  DE_PIX_FMT_RGB888 = 40,              ///< 24-bit RGB 32bpp 
  DE_PIX_FMT_BGR888 = 41,              ///< 24-bit BGR 32bpp 

  DE_PIX_FMT_YUV422P = 42,             ///< planar YUV 4:2:2,
};

/**@brief decoder bare data in output ndarray*/
struct DecBareData{
	int64_t dts;    ///< decode timestamp, unit:ms
};

typedef enum  {
	DE_NODE_INIT,			//´´½¨Ïß³ÌÎ´Æô¶¯
	DE_NODE_RUN,			//Ïß³Ìstart
	DE_NODE_FINISH,			//Ïß³ÌÍê³ÉÍË³ö

	DE_NODE_EOS,			//Ïß³ÌÈÎÎñÒì³£½áÊø£¨¶ÏÁ÷£¬ÎÄ¼þ¶ÁÈ¡½áÊø£©

	//ÖÐ¼ä×´Ì¬/Òì³£×´Ì¬
	DE_NODE_HW_ERROR, 		//Ïà¹ØÓ²¼þ¼ÓËÙÆ÷Òì³£
	DE_NODE_OOM_ERROR, 		//out of memoryÒì³£
	DE_NODE_TIMEOUT_ERROR, 		//³¬Ê±Òì³£
	DE_NODE_ARG_ERROR,		//²»Ö§³ÖµÄ²ÎÊýÒì³£
}DeNodeState;

/**
* @brief deep1000 extension data
*/
typedef	struct {
  /*!
  * \brief Img format
  */
  int32_t format;
  /*!
  * \brief indicator Img layout sequence in shape.reference ShapeCode 
  */
  int32_t shape_indicator;
  /*!
  * \brief roi rect
  */
  deRect roi_rect;
  /*!
  * \brief user extend bear data size
  */
  int32_t bear_data_len;
  /*!
  * \brief image time stamp
  */
  uint64_t pts;
} deExtInfo;

/*!
 * \brief The data type the tensor can hold.
 *
 *  Examples
 *   - float: type_code = 2, bits = 32, lanes=1
 *   - float4(vectorized 4 float): type_code = 2, bits = 32, lanes=4
 *   - int8: type_code = 0, bits = 8, lanes=1
 */
typedef struct {
  /*!
  * \brief Type code of base types.
  * We keep it uint8_t instead of DLDataTypeCode for minimal memory
  * footprint, but the value should be one of DLDataTypeCode enum values.
  * */
  uint8_t code;
  /*!
  * \brief Number of bits, common choices are 8, 16, 32.
  */
  uint8_t bits;
  /*! \brief Number of lanes in the type, used for vector types. */
  uint16_t lanes;
} DLDataType;

/*!
 * \brief The type code options DLDataType.
 */
typedef enum {
  kDLInt = 0U,
  kDLUInt = 1U,
  kDLFloat = 2U,
} DLDataTypeCode;

/*!
 * \brief The device type in DLContext.
 */
typedef enum {
  /*! \brief CPU device */
  kDLCPU = 1,
  /*! \brief CUDA GPU device */
  kDLGPU = 2,
  /*!
   * \brief Pinned CUDA GPU device by cudaMallocHost
   * \note kDLCPUPinned = kDLCPU | kDLGPU
   */
  kDLCPUPinned = 3,
  /*! \brief OpenCL devices. */
  kDLOpenCL = 4,
  /*! \brief Vulkan buffer for next generation graphics. */
  kDLVulkan = 7,
  /*! \brief Metal for Apple GPU. */
  kDLMetal = 8,
  /*! \brief Verilog simulator buffer */
  kDLVPI = 9,
  /*! \brief ROCm GPUs for AMD GPUs */
  kDLROCM = 10,
  /*!
   * \brief Reserved extension device type,
   * used for quickly test extension device
   * The semantics can differ depending on the implementation.
   */
  kDLExtDev = 12,
} DLDeviceType;

/*!
 * \brief A Device context for Tensor and operator.
 */
typedef struct {
  /*! \brief The device type used in the device. */
  DLDeviceType device_type;
  /*! \brief The device index */
  int device_id;
} DLContext;

/**
* @brief deep1000 extension data
*/
typedef	struct {
  /*!
  * \brief Img format
  */
  int32_t format;
  /*!
  * \brief indicator Img layout sequence in shape.reference ShapeCode
  */
  int32_t shape_indicator;
  /*!
  * \brief roi rect
  */
  deRect roi_rect;
  /*!
  * \brief user extend bear data size
  */
  int32_t bear_data_len;
  /*!
  * \brief image time stamp
  */
  uint64_t pts;
}ExtInfo;

/*!
 * \brief Plain C Tensor object, does not manage memory.
 */
typedef struct {
  /*!
   * \brief The opaque data pointer points to the allocated data. This will be
   * CUDA device pointer or cl_mem handle in OpenCL. This pointer is always
   * aligns to 256 bytes as in CUDA.
   *
   * For given DLTensor, the size of memory required to store the contents of
   * data is calculated as follows:
   *
   * \code{.c}
   * static inline size_t GetDataSize(const DLTensor* t) {
   *   size_t size = 1;
   *   for (tvm_index_t i = 0; i < t->ndim; ++i) {
   *     size *= t->shape[i];
   *   }
   *   size *= (t->dtype.bits * t->dtype.lanes + 7) / 8;
   *   return size;
   * }
   * \endcode
   */
  void* data;
  /*! \brief The device context of the tensor */
  DLContext ctx;
  /*! \brief Number of dimensions */
  int ndim;
  /*! \brief The data type of the pointer*/
  DLDataType dtype;
  /*! \brief The shape of the tensor */
  int64_t* shape;
  /*!
   * \brief strides of the tensor,
   *  can be NULL, indicating tensor is compact.
   */
  int64_t* strides;
  /*! \brief The offset in bytes to the beginning pointer to data */
  uint64_t byte_offset;
} DLTensor;

/*!
 * \brief C Tensor object, manage memory of DLTensor. This data structure is
 *  intended to faciliate the borrowing of DLTensor by another framework. It is
 *  not meant to transfer the tensor. When the borrowing framework doesn't need
 *  the tensor, it should call the deleter to notify the host that the resource
 *  is no longer needed.
 */
typedef struct DLManagedTensor {
  /*! \brief DLTensor which is being memory managed */
  DLTensor dl_tensor;
  /*! \brief the context of the original host framework of DLManagedTensor in
   *   which DLManagedTensor is used in the framework. It can also be NULL.
   */
  void * manager_ctx;
  /*! \brief Destructor signature void (*)(void*) - this should be called
   *   to destruct manager_ctx which holds the DLManagedTensor. It can be NULL
   *   if there is no way for the caller to provide a reasonable destructor.
   */
  void (*deleter)(struct DLManagedTensor * self);
} DLManagedTensor;

/*!
 * \brief The data type used in DE Runtime.
 *
 *  Examples
 *   - float: type_code = 2, bits = 32, lanes=1
 *   - float4(vectorized 4 float): type_code = 2, bits = 32, lanes=4
 *   - int8: type_code = 0, bits = 8, lanes=1
 *
 * \note Arguments DE API function always takes bits=64 and lanes=1
 */
typedef DLDataType DEType;

/*!
 * \brief The Device information, abstract away common device types.
 */
typedef DLContext DEContext;

/*!
 * \brief Union type of values
 *  being passed through API and function calls.
 */
typedef union {
  int64_t v_int64;
  double v_float64;
  void* v_handle;
  const char* v_str;
  DEType v_type;
  DEContext v_ctx;
} DEValue;

/*! \brief type of array index. */
typedef int64_t de_index_t;

/*! \brief Extension device types in DE */
typedef enum {
  kDLAOCL = 5,
  kDLSDAccel = 6,
  kOpenGL = 11,
  // AddExtraDEType which is not in DLPack here
} DEDeviceExtType;

/*!
 * \brief The type code in DEType
 * \note DEType is used in two places.
 */
typedef enum {
  // The type code of other types are compatible with DLPack.
  // The next few fields are extension types
  // that is used by DE API calls.
  kHandle = 3U,
  kNull = 4U,
  kDEType = 5U,
  kDEContext = 6U,
  kArrayHandle = 7U,
  kNodeHandle = 8U,
  kModuleHandle = 9U,
  kFuncHandle = 10U,
  kStr = 11U,
  kBytes = 12U,
  kNDArrayContainer = 13U,
  // Extension codes for other frameworks to integrate DE PackedFunc.
  // To make sure each framework's id do not conflict, use first and
  // last sections to mark ranges.
  // Open an issue at the repo if you need a section of code.
  kExtBegin = 15U,
  kNNVMFirst = 16U,
  kNNVMLast = 20U,
  //ndarray addition code 
  kNDArrayAdditionInfo = 22U,
  // The following section of code is used for non-reserved types.
  kExtReserveEnd = 64U,
  kExtEnd = 128U
} DETypeCode;


/*!
 * \brief The tensor array stucture to DE API.
 */
typedef DLTensor DEArray;

/*! \brief the array handle */
typedef DEArray* DEArrayHandle;

typedef ExtInfo* ExtInfoHandle;

#ifdef __cplusplus
} // 
#endif

#endif

