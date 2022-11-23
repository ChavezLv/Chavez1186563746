/**
 * @file de_serializer.h
 * @brief Serializer template class that helps serialization
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 * @note This file do not need to be directly used by mostr user.
 */

#ifndef DE_SERIALIZER_H_
#define DE_SERIALIZER_H_

#include <vector>
#include <string>
#include <string.h>
#include "de_base.h"
#include "de_logging.h"
#include <algorithm>
  
namespace de {

/**
 * @brief Provides general de-serialization support for user-defined struct/class
 */
class UserDataBinaryInStream : public Stream {
public:
	using Stream::Write;
	using Stream::WriteArray;
	static const int kInitCapacity = 4 << 10;
	UserDataBinaryInStream()
		: buffer_(kInitCapacity) {
		curr_ptr_ = 0;
		buffer_size_ = kInitCapacity;
	}
	virtual size_t Read(void *ptr, size_t size) {
		LOG(FATAL) << "UserDataBinaryInStream::Read  is not supported";
		return 0;
	}
	virtual void Write(const void *ptr, size_t size) {
		if (size == 0) return;
		size_t req_size = curr_ptr_ + size;
		if (req_size > buffer_size_) {
			size_t new_size = static_cast<size_t>(buffer_size_ * 1.2);
			new_size = std::max(new_size, req_size + 1);
			buffer_.resize(new_size);
			buffer_size_ = new_size;
		}
		memcpy(buffer_.data() + curr_ptr_, ptr, size);
		curr_ptr_ += size;
	}

	operator DEByteArray() const {
		DEByteArray arr_;
		arr_.data = buffer_.data();
		arr_.size = curr_ptr_;
		return std::move(arr_);
	}

private:
	/*! \brief current pointer */
	size_t curr_ptr_;
	size_t buffer_size_;
	std::vector<char> buffer_;
};

/**
 * @brief Provides general serialization support for user-defined struct/class
 */
class UserDataBinaryOutStream : public Stream {
public:
	using Stream::Read;
	using Stream::ReadArray;
	UserDataBinaryOutStream(std::string& str) : string_(str) {
		buffer_size_ = string_.size();
	}

	virtual size_t Read(void *ptr, size_t size) {
		CHECK(curr_ptr_ + size <= buffer_size_);
		size_t nread = std::min(buffer_size_ - curr_ptr_, size);
		if (nread != 0) memcpy(ptr, string_.data() + curr_ptr_, nread);
		curr_ptr_ += nread;
		return nread;
	}
	virtual void Write(const void *ptr, size_t size) {
		LOG(FATAL) << "UserDataBinaryInStream::Write  is not supported";
	}

private:
	/*! \brief current pointer */
	size_t curr_ptr_{ 0 };
	size_t buffer_size_;
	std::string& string_;
};

template<typename T>
inline UserDataBinaryInStream toByteArray(const T &data) {
	UserDataBinaryInStream strm;
	strm.Write(data);
	return std::move(strm);
}

template<typename T>
inline T fromByteArray(std::string& _str) {
	T data;
	UserDataBinaryOutStream strm(_str);
	strm.Read(&data);
	return std::move(data);
}

/*! \brief internal namespace for serializers */
namespace serializer {

#define STRUCT_SERIALIZE_START(Type)	\
template<>  struct Handler<Type> {

#define STRUCT_SERIALIZE_WRITE(Type)	\
  inline static void Write(Stream *strm, const Type& data) {

#define STRUCT_SERIALIZE_WR_FIELD(ftype, field)	\
  Handler<ftype>::Write(strm, data.field);

#define STRUCT_SERIALIZE_READ(Type)	\
  } ; inline static bool Read(Stream *strm, Type* data) {

#define STRUCT_SERIALIZE_RD_FIELD(ftype, field)	\
  if (!Handler<ftype>::Read(strm, &(data->field))) return false;

#define STRUCT_SERIALIZE_END()		\
return true;}};


#define STRUCT_SERIALIZE_1(_struct, _ftype0, _field0)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_2(_struct, _ftype0, _field0, _ftype1, _field1)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_3(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2 )	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_END()

#define STRUCT_SERIALIZE_4(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3 )	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_END()

#define STRUCT_SERIALIZE_5(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_END()

#define STRUCT_SERIALIZE_6(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_7(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_8(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_9(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_10(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_11(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_12(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_END()

#define STRUCT_SERIALIZE_13(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11 , _ftype12, _field12)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_14(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11 , _ftype12, _field12, _ftype13, _field13)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_15(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11 , _ftype12, _field12, _ftype13, _field13, _ftype14, _field14)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_END()

#define STRUCT_SERIALIZE_16(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11 , _ftype12, _field12, _ftype13, _field13, _ftype14, _field14, _ftype15, _field15)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype15, _field15)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype15, _field15)\
  STRUCT_SERIALIZE_END()


#define STRUCT_SERIALIZE_17(_struct, _ftype0, _field0, _ftype1, _field1, _ftype2, _field2, _ftype3, _field3, _ftype4, _field4, _ftype5, _field5, _ftype6, _field6, _ftype7, _field7, _ftype8, _field8, _ftype9, _field9, _ftype10, _field10 , _ftype11, _field11 , _ftype12, _field12, _ftype13, _field13, _ftype14, _field14, _ftype15, _field15 , _ftype16, _field16)	\
  STRUCT_SERIALIZE_START(_struct)	\
  STRUCT_SERIALIZE_WRITE(_struct)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype15, _field15)\
  STRUCT_SERIALIZE_WR_FIELD(_ftype16, _field16)\
  STRUCT_SERIALIZE_READ(_struct)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype0, _field0)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype1, _field1)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype2, _field2)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype3, _field3)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype4, _field4)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype5, _field5)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype6, _field6)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype7, _field7)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype8, _field8)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype9, _field9)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype10, _field10)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype11, _field11)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype12, _field12)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype13, _field13)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype14, _field14)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype15, _field15)\
  STRUCT_SERIALIZE_RD_FIELD(_ftype16, _field16)\
  STRUCT_SERIALIZE_END()
} // namespace serial

}  // namespace de
#endif  // DE_SERIALIZER_H_
