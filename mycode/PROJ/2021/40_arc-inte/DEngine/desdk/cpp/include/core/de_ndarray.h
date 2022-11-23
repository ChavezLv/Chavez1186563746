/**
 * @file de_ndarray.h
 * @brief Declare function interface of general NDArray
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_NDARRAY_H_
#define DE_NDARRAY_H_

#include "de_c_type.h"
#include "de_base.h"
#include "de_logging.h"
#include "de_serializer.h"
#include <atomic>
#include <vector>
#include <utility>


#define EXTEND_TRANSPORT_BLOCK_SIZE    24  // 传输信息扩展
#define EXTEND_TENSOR_INFO_BLOCK_SIZE  64  // tensor 控制信息
#define EXTEND_BEAR_BLOCK_SIZE        256  // 用户业务扩展

/**
 * @brief notice: the total data size
*/
#define EXTEND_TOTAL_SIZE (EXTEND_TRANSPORT_BLOCK_SIZE + EXTEND_TENSOR_INFO_BLOCK_SIZE)

namespace de
{

class Stream;

/*!
 * @brief Managed NDArray.
 *  The array is backed by reference counted blocks.
 */
class NDArray
{
public:

    // internal container type
    class Container;

    //------------------------------------------------------
    //
    //		1. 拷贝构造赋值
    //
    //------------------------------------------------------
    /*! @brief default constructor */
    NDArray()
    {
    }

    /*!
     * @brief cosntruct a NDArray that refers to data
     * @param data The data this NDArray refers to
     */
    explicit inline NDArray(Container *data);


    /*!
     * @brief copy constructor.
     *
     * It does not make a copy, but the reference count of the input NDArray is incremented
     *
     * @param other NDArray that shares internal data with the input NDArray.
     */
    inline NDArray(const NDArray &other);  // NOLINT(*)


    /*!
     * @brief move constructor
     * @param other The value to be moved
     */
    NDArray(NDArray &&other) // NOLINT(*)
        :
        data_(other.data_)
    {
        other.data_ = nullptr;
    }


    /*! @brief destructor */
    ~NDArray()
    {
        this->reset();
    }

    /*!
     * @brief copy assignmemt
     * @param other The value to be assigned.
     * @return reference to self.
     */
    NDArray& operator=(const NDArray &other)    // NOLINT(*)
    {
        // copy-and-swap idiom
        NDArray(other).swap(*this);  // NOLINT(*)
        return *this;
    }
    /*!
     * @brief move assignmemt
     * @param other The value to be assigned.
     * @return reference to self.
     */
    NDArray& operator=(NDArray &&other)    // NOLINT(*)
    {
        // copy-and-swap idiom
        NDArray(std::move(other)).swap(*this); // NOLINT(*)
        return *this;
    }


    //------------------------------------------------------
    //
    //		2. NDArray创建和释放
    //
    //------------------------------------------------------
    /**
     * @brief Create new ndarray
     *
     * @param shapes tensor shape
     * @param data 外部数据指针（拷贝到NDArray内部）
     *
     * @return
     */
    DE_DLL static NDArray Create(std::vector<int64_t> shapes, void* data = nullptr);

    /**
     * @brief 根据宽、高以及格式，生成NDArray
     *
     * @param width 
     * @param height
     * @param format
     * @param data 外部数据指针（拷贝到NDArray内部）
     *
     * @return
     */
    DE_DLL static NDArray Create(int32_t width, int32_t height, int format = dePixelFormat::DE_PIX_FMT_NV12, const deRect& rect = {0,0,0,0}, void* data = nullptr);

    /**
     * @brief 根据原NDArray信息，创建新的NDArray
     *
     * @param 
     *
     * @return
     */
    DE_DLL NDArray Clone();
    
    /**
     * @brief 从当前NDArray的传输域创建新的NDArray View
     *
     * @return
     * @note 用于P2P传输和序列化
     * 传输域中缓存的是新NDArray的控制域
     */
    DE_DLL NDArray CreateViewFromTransport(void) const;

    /**
     * @brief 从当前NDArray的传输域创建NDArray list View
     *
     * @param nds
     *
     * @return
     * @note 用于P2P
     */
    DE_DLL bool CreateViewFromTransport(std::vector<NDArray>& nds) const;

    /**
    * @brief 从数据指针(原NDArray)创建新的NDArray view
    *
    * @param data 数据指针(原NDArray)
    *
    * @return
    * @note 和SaveTensorPtr配套使用
    */
    DE_DLL static NDArray CreateViewFromTensorPtr(void* data);

    /**
    * @brief 根据Rect在现有NDArray上创建新的NDArray View
    *
    * @param rect 
    * @param format
    *
    * @return
    */
    DE_DLL NDArray CreateView(const deRect* rect, int32_t format = -1) const;

    /**
    * @brief 根据user configure,在现有NDArray上创建新的NDArray View
    *
    * @param shape        
    * @param dtype
    * @param info 
    * @param data_offset
    *
    * @return
    */
    DE_DLL NDArray CreateView(std::vector<int64_t> shape, DLDataType dtype, ExtInfo info, int32_t data_offset) const;

    /*!
    * @brief only dec reference cnt
    * @note 慎用！和SaveTensorPtr配套使用。
    */
    DE_DLL void Release();

    /*!
     * @brief Create an empty NDArray.
     * @param shape The shape of the new array.
     * @param dtype The data type of the new array.
     * @param ctx The context of the Array.
     * @param info The ExtInfo of the Array.
     * @return The created Array
     */
    DE_DLL  static NDArray Empty(std::vector<int64_t> shape,
                                DLDataType dtype = {DLDataTypeCode::kDLUInt, 8, 1}, 
                                DLContext ctx = {DLDeviceType::kDLCPU, 0},
                                ExtInfo info = {dePixelFormat::DE_PIX_FMT_NONE, deShapeCode::HW, { 0, 0, 0, 0 }, 0});
    /*!
     * @brief Create an empty NDArray.
     * @param max_size array's tensor data size.
     * @param bear_data_size array's bear exten data size for user.
     * @return The created Array
     */
    DE_DLL  static NDArray Empty(size_t max_size, size_t bear_data_size = 0);


    /*!
     * @brief Create an empty NDArray.
     * @param shape The shape of the new array.
     * @param dtype DLDataType
     * @param ctx   DLContext
     * @param info  ExtInfo
     * @param user_deleter user defined deleter. if nullptr, ndarray use DefaultDeleter.
     * @param managed_ctx user managed memory ctx
     * @param user_data user data ptr
     * @return The created Array
     */
    typedef void (*USER_DELETER)(Container *self);
    DE_DLL  static NDArray UserEmpty(std::vector<int64_t> shape,
                                     DLDataType dtype = { kDLInt, 8, 1 },
                                     DLContext ctx = { kDLCPU, 0 }, ExtInfo info = { dePixelFormat::DE_PIX_FMT_NONE, deShapeCode::HW, { 0, 0, 0, 0 } },
                                     USER_DELETER user_deleter = nullptr,
                                     void *managed_ctx = nullptr,
                                     void *user_data = nullptr);

    //------------------------------------------------------
    //
    //		3. 读取NDArray常量信息
    //
    //------------------------------------------------------
    /*! @return If NDArray is defined */
    inline bool defined() const
    {
        return data_ != nullptr;
    }

    /*! @return Pointer to content of DLTensor */
    inline const DLTensor* operator->() const;

    /**
     * @brief 检查data ptr是否为有效的NDArray的data指针
     *
     * @param date_ptr
     *
     * @return
     * @note 如果data_ptr是通过SaveTensorPtr获取，那么为有效的指针
     */
    DE_DLL  static bool CheckValidDataPtr(void* date_ptr);

    /*!
    *@brief get ndarray memory size.
    */
    DE_DLL  size_t TotalSize() const;

    DE_DLL  size_t GetTensorDataSize() const;

    /*!
    * @brief get transport data size
    */
    DE_DLL  size_t GetTransportDataCap() const;

    /*!
    * @brief get user data area allocated size
    */
    DE_DLL  size_t GetBearDataCap() const;

    /*!
    * @brief get user data used size
    */
    DE_DLL  size_t GetBearDataSize() const;

    /*!
    * @brief get DLTensor
    */
    DE_DLL const DLTensor* GetTensor() const;

    /*!
     * @brief get ext info.
     */
    DE_DLL  const ExtInfo* GetExtInfo() const;

    /*!
     * @brief get img info.
     * @note if ndarray is create from img,  
     */
    DE_DLL void GetImgInfo(int32_t& height, int32_t& width, int32_t& format) const;

    /*!
     * @return the reference counter
     * @note this number is approximate in multi-threaded setting.
     */
    inline int use_count() const;

    /**
     * @brief Calculate CRC
     * @param[in] crc_use_tensor_data Whether to use tensor data when calculating CRC, 0 by default
     * @return CRC result
     * @note Transport data, tensor control data and bear data are always used to calc CRC.\n
     *		 Whether to use tensor data to calc CRC is configured by users.
     */
    DE_DLL  int32_t CalcCrc(int crc_use_tensor_data = 0) const;


    //------------------------------------------------------
    //
    //		4. 数据域读写以及修改
    //
    //------------------------------------------------------
    /*!
    * @brief 获取传输域指针，用于数据读写
    *
    * @return
    * @note 用于P2P传输
    */
    DE_DLL	void* GetTransportData() const;

    /*!
    * @brief get tensor data addr
    */
    DE_DLL  void* GetTensorData() const;

    /*!
    * @brief get Traffic data addr
    */
    DE_DLL  void* GetBearData() const;

    /*!
    * @brief Shrink an NDArray memory size.
    * @param shapes array's shapes to be replace.
    * @param bear_data_len array's bear exten data size to be replace.
    * @return
    */
    DE_DLL  void Shrink(std::vector<int64_t> &shapes, size_t bear_data_len = -1);


    //------------------------------------------------------
    //
    //		5. 序列化以及跨平台、跨框架传输、数据域操作
    //
    //------------------------------------------------------

    /*!
     * @brief 把tensor的控制域写入到指定码流地址ptr
     *
     * @param ptr 如果为nullptr，那么写入到当前NDArray的transport区域
     *
     * @return
     */
    DE_DLL	void SaveTensorInfo(uint8_t* ptr = nullptr) const;

    /*!
     * @brief Copy data content from another array.
     * @param other The source array to be copied from.
     * @note The copy may happen asynchrously if it involves a GPU context.
     *       TVMSynchronize is necessary.
     */
    inline void CopyFrom(DLTensor *other);
    inline void CopyFrom(const NDArray &other);

    /*!
    * @brief Copy data content into another array.
    * @param other The source array to be copied from.
    * @note The copy may happen asynchrously if it involves a GPU context.
    *       TVMSynchronize is necessary.
    */
    inline void CopyTo(DLTensor *other) const;
    inline void CopyTo(const NDArray &other) const;

    /*!
     * @brief Copy the data to another context.
     * @param ctx The target context.
     * @return The array under another context.
     */
    inline NDArray CopyTo(const DLContext &ctx) const;

    /*!
    * @brief Function to copy data from one array to another.
    * @param from The source array.
    * @param to The target array.
    * @param stream The stream used in copy.
    */
    DE_DLL  static void CopyFromTo(DLTensor *from, DLTensor *to, DEStreamHandle stream = nullptr);

    /**
     * @brief CopyToBytes
     * @param[in] data data ptr where tensor data copy to
     * @param[in] nbytes tensor data size
     * @return result 0 success, -1 fail
     * @note
     */
    DE_DLL  int CopyToBytes(void* data, size_t nbytes);

    /**
     * @brief CopyFromBytes
     * @param[in] data data ptr where tensor data copy from
     * @param[in] nbytes tensor data size
     * @return result 0 success, -1 fail
     * @note
     */
    DE_DLL  int CopyFromBytes(void* data, size_t nbytes);

    /*!
    * @brief Load NDArray from stream
    * @param stream The input data stream
    * @return Whether load is successful
    */
    inline bool Load(de::Stream *stream);
    /*!
     * @brief Save NDArray to stream
     * @param stream The output data stream
     */
    inline void Save(de::Stream *stream) const;

    /**
    * @brief 存储NDArray的DLTensor指针
    *
    * @return 数据域指针
    * @note 内部引用计数加1
    */
   DE_DLL  void* SaveTensorPtr();

    /*!
    * @brief Create a reference view of NDArray that
    *  represents as DLManagedTensor.
    * @return A DLManagedTensor
    */
    DE_DLL  DLManagedTensor* ToDLPack() const;

    /*!
    * @brief Create a NDArray backed by a dlpack tensor.
    *
    * This allows us to create a NDArray using the memory
    * allocated by an external deep learning framework
    * that is DLPack compatible.
    *
    * The memory is retained until the NDArray went out of scope.
    * @param tensor The DLPack tensor to copy from.
    * @return The created NDArray view.
    */
    DE_DLL  static NDArray FromDLPack(DLManagedTensor *tensor);

    //------------------------------------------------------
    //
    //		6. 文件读写
    //
    //------------------------------------------------------

    /**
     * @brief load file data to ndarray
     * @param[in]  file_name  the file ndarray load from
     * @return
     * @note
     */
    DE_DLL static NDArray LoadFromFile(const std::string& file_name);

    /**
     * @brief save ndarray to file
     * @param[in]   file_name  the file ndarray save to
     * @return      -1  save error
     * @note
     */
    DE_DLL  int SaveToFile(const std::string& file_name);

    /**
     * @brief save ndarray to jpeg file
     * @param[in]   file_name  the file ndarray save to
     * @return  -1  save error
     * @note
     */
    DE_DLL int SaveToJpegFile(const std::string& file_name);

    /**
    * @brief load jpeg file to ndarray
    * @param[in]  file_name  the file ndarray load from
    * @return
    * @note
    */
    DE_DLL static NDArray LoadFromJpegFile(const std::string& file_name, int beardata_size = 0);

    // internal namespace
    struct Internal;
    // bear data allignment size
    static constexpr int kBearDataAlignment = 4;

private:
    void RecoverTensorInfo();
    /*!
     * @brief Swap this array with another NDArray
     * @param other The other NDArray
     */
    void swap(NDArray &other)    // NOLINT(*)
    {
        std::swap(data_, other.data_);
    }

    /*! @return If both NDArray reference the same container */
    bool same_as(const NDArray &other) const
    {
        return data_ == other.data_;
    }
    /*! @brief reset the content of NDArray to be nullptr */
    inline void reset();

protected:
    /*! @brief Internal Data content */
    Container *data_ { nullptr };
    // enable internal functions
    friend struct Internal;
    friend class DEPODValue_;
    friend class DEArgValue;
    friend class DERetValue;
    friend class DEArgsSetter;
};

/*!
 * @brief The type trait indicates subclass of TVM's NDArray.
 *  For irrelavant classes, code = -1.
 *  For TVM NDArray itself, code = 0.
 *  All subclasses of NDArray should override code > 0.
 */
template<typename T>
struct array_type_info
{
    /*! @brief the value of the traits */
    static const int code = -1;
};

template<>
/**< Overrides the type trait for tvm's NDArray. */
struct array_type_info<NDArray>
{
    static const int code = 0;
};


template<>
/**< Overrides the serial trait for tvm's NDArray. */
struct has_saveload<NDArray>
{
    static const bool value = true;
};

/*!
 * @brief Reference counted Container object used to back NDArray.
 *
 *  This object is DLTensor compatible:
 *    the pointer to the NDArrayContainer can be directly
 *    interpreted as a DLTensor*
 *
 * @note do not use this function directly, use NDArray.
 */
class NDArray::Container
{
public:
    // NOTE: the first part of this structure is the same as
    // DLManagedTensor, note that, however, the deleter
    // is only called when the reference counter goes to 0
    /*!
     * @brief The corresponding dl_tensor field.
     * @note it is important that the first field is DLTensor
     *  So that this data structure is DLTensor compatible.
     *  The head ptr of this struct can be viewed as DLTensor*.
     */
    DLTensor dl_tensor;
    /*!
     * @brief addtional context, reserved for recycling
     * @note We can attach additional content here
     *  which the current container depend on
     *  (e.g. reference to original memory when creating views).
     */
    void *manager_ctx { nullptr };
    /*!
     * @brief Customized deleter
     *
     * @note The customized deleter is helpful to enable
     *  different ways of memory allocator that are not
     *  currently defined by the system.
     */
    void (*deleter)(Container *self) = nullptr;
    /*
     *@brief user managed ctx, memory managed by user
     */
    void *user_managed_ctx { nullptr };

protected:
    friend class NDArray;
    friend class DEPODValue_;
    friend class DEArgValue;
    friend class DERetValue;
    friend class RPCWrappedFunc;
    /*!
     * @brief Type flag used to indicate subclass.
     *  Default value 0 means normal NDArray::Conatainer.
     *
     *  We can extend a more specialized NDArray::Container
     *  and use the array_type_code_ to indicate
     *  the specific array subclass.
     */
    int32_t array_type_code_ {0};
    /*! @brief The internal reference counter */
    std::atomic<int> ref_counter_ {0};
    /*!
     * @brief The shape container,
     *  can be used used for shape data.
     */
    std::vector<int64_t> shape_;
    /*
     * @brief Tensor ext info
     */
    ExtInfo ext_info {0};
    /*
     * @brief record tensor data len
     */
    int32_t data_len {0};
    /*
     * @brief user bear data allocated size
     */
    int32_t bear_data_cap {0};
    /*
     * @brief record user bear data used len
     */
    int32_t bear_data_len {0};
    /*
    * @brief  bear data is in use
    */
    int32_t bear_data_in_use_{0};


public:
    /*! @brief default constructor */
    Container()
    {
        dl_tensor.data = nullptr;
        dl_tensor.ndim = 0;
        dl_tensor.shape = nullptr;
        dl_tensor.strides = nullptr;
        dl_tensor.byte_offset = 0;
    }

    /*! @brief default constructor
    * @param  info extension info
    */
    Container(ExtInfo info)
    {
        dl_tensor.data = nullptr;
        dl_tensor.ndim = 0;
        dl_tensor.shape = nullptr;
        dl_tensor.strides = nullptr;
        dl_tensor.byte_offset = 0;
        ext_info = info;
    }
    /*! @brief developer function, increases reference counter */
    void IncRef()
    {
        ref_counter_.fetch_add(1, std::memory_order_relaxed);
    }
    /*! @brief developer function, decrease reference counter */
    void DecRef()
    {
        if (ref_counter_.fetch_sub(1, std::memory_order_release) == 1)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            if (this->deleter != nullptr)
            {
                (*this->deleter)(this);
            }
        }
    }
    /*! @brief developer function, get ext_info*/
    ExtInfo* GetExtInfo() const
    {
        CHECK(this != nullptr);
        return (ExtInfo*) & (this->ext_info);
    }
};

// implementations of inline functions
// the usages of functions are documented in place.
inline NDArray::NDArray(Container *data) :
    data_(data)
{
    if (data != nullptr)
    {
        data_->IncRef();
    }
}

inline NDArray::NDArray(const NDArray &other) :
    data_(other.data_)
{
    if (data_ != nullptr)
    {
        data_->IncRef();
    }
}

inline void NDArray::reset()
{
    if (data_ != nullptr)
    {
        data_->DecRef();
        data_ = nullptr;
    }
}

/*! @brief return the size of data the DLTensor hold, in term of number of bytes
 *
 *  @param arr the input DLTensor
 *
 *  @return number of  bytes of data in the DLTensor.
 */
inline size_t GetDataSize(const DLTensor &arr)
{
    size_t size = 1;
    for (de_index_t i = 0; i < arr.ndim; ++i)
    {
        size *= static_cast<size_t>(arr.shape[i]);
    }
    size *= (arr.dtype.bits * arr.dtype.lanes + 7) / 8;
    return size;
}

inline void NDArray::CopyFrom(DLTensor *other)
{
    CHECK(data_ != nullptr);
    CopyFromTo(other, &(data_->dl_tensor));
}

inline void NDArray::CopyFrom(const NDArray &other)
{
    CHECK(data_ != nullptr);
    CHECK(other.data_ != nullptr);
    CopyFromTo(&(other.data_->dl_tensor), &(data_->dl_tensor));
}

inline void NDArray::CopyTo(DLTensor *other) const
{
    CHECK(data_ != nullptr);
    CopyFromTo(&(data_->dl_tensor), other);
}

inline void NDArray::CopyTo(const NDArray &other) const
{
    CHECK(data_ != nullptr);
    CHECK(other.data_ != nullptr);
    CopyFromTo(&(data_->dl_tensor), &(other.data_->dl_tensor));
}

inline NDArray NDArray::CopyTo(const DLContext &ctx) const
{
    CHECK(data_ != nullptr);
    const DLTensor *dptr = operator->();
    NDArray ret = Empty(
                      std::vector < int64_t > (dptr->shape, dptr->shape + dptr->ndim),
                      dptr->dtype, ctx);
    this->CopyTo(ret);
    return ret;
}

inline int NDArray::use_count() const
{
    if (data_ == nullptr)
        return 0;
    return data_->ref_counter_.load(std::memory_order_relaxed);
}

inline const DLTensor* NDArray::operator->() const
{
    return &(data_->dl_tensor);
}

/*! @brief Magic number for NDArray file */
constexpr uint32_t kDENDArrayMagic = 0xDD5E403F;

inline void NDArray::Save(de::Stream *strm) const
{
    std::vector<uint8_t> tensor_info(EXTEND_TOTAL_SIZE + 4);

    int32_t *ptrTrans = reinterpret_cast<int32_t*>(&tensor_info[0]);
    ptrTrans[1] = static_cast<int32_t>(TotalSize());

    SaveTensorInfo(static_cast<uint8_t*>(&tensor_info[EXTEND_TOTAL_SIZE]));
    // writer externd hearder to strm
    strm->WriteArray(static_cast<uint8_t*>(&tensor_info[0]), static_cast<size_t>(EXTEND_TOTAL_SIZE)); 
    // write data to strm
    strm->WriteArray(static_cast<uint8_t*>(GetTensorData()), static_cast<size_t>(TotalSize() - EXTEND_TOTAL_SIZE));
}

inline bool NDArray::Load(de::Stream *strm)
{
    int header;
    int packet_len;

    strm->Read(&header);
    strm->Read(&packet_len);

    std::vector < int64_t > shapes { packet_len - EXTEND_TOTAL_SIZE };
    NDArray array = Empty(shapes);
    strm->ReadArray((char*) array.GetTransportData() + 2 * sizeof(int),
                    packet_len - 2 * sizeof(int));

    int *ptr = static_cast<int*>(array.GetTransportData());
    ptr[0] = header;
    ptr[1] = packet_len;

    *this = array.CreateViewFromTransport();

    return true;
}
   
inline std::ostream& operator<<(std::ostream& os, const NDArray& obj) {
    os << "tensor info : use_count "<< obj.use_count()<<", shape[" ;
    for (auto idx = 0; idx < obj->ndim; idx++) {
        os << obj->shape[idx] << ", ";
    }
    os << "]";
    int32_t width = 0, height = 0, format = -1;
    obj.GetImgInfo(height, width, format);
    os << ", dltype[ " << static_cast<int32_t>(obj->dtype.code) << ", " << static_cast<int32_t>(obj->dtype.bits) << ", " << obj->dtype.lanes << "]";
    //os << ", context[dev_id : " << obj->ctx.device_id << ", dev_type: " << obj->ctx.device_type << " ]";
    os << ", data len " << obj.GetTensorDataSize() << ", bear_data_cap " << obj.GetBearDataCap()<< ", bear_data_len " << obj.GetBearDataSize();
    os << ", img info: width " << width << ", height "<< height << ", format "<< format;
    os << ", roi["<< obj.GetExtInfo()->roi_rect.x << ", "<< obj.GetExtInfo()->roi_rect.y << ", "<< obj.GetExtInfo()->roi_rect.w << ", "<< obj.GetExtInfo()->roi_rect.h<< "]" << std::endl; 
    
    return os;
}

template<typename T>
bool WriteDataToBearArea(NDArray& array, T data) 
{
  CHECK(array.defined());
  de::UserDataBinaryInStream stream = std::move(de::toByteArray(data));
  DEByteArray byte_array = stream;
  CHECK_GT(array.GetBearDataCap(),  byte_array.size) << "serialize data size = " << byte_array.size << " larger than " << "bear data (size = )." <<  array.GetBearDataCap();
  memcpy((char*)array.GetBearData(), byte_array.data, byte_array.size);
  std::vector<int64_t> shapes;
  array.Shrink(shapes, byte_array.size);
  return true;
}

template<typename T>
T GetDataFromBearArea(NDArray& array)
{
  CHECK(array.defined());
  CHECK(array.GetBearData() != nullptr)<< "bear data ptr is null";
  std::string str_data(reinterpret_cast<char*>(array.GetBearData()), array.GetBearDataSize());
  return  de::fromByteArray<T>(str_data);
}

}  // namespace de
#endif  // DE_NDARRAY_H_
