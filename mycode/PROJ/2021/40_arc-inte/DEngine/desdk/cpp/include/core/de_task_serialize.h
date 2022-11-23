/**
 * @file de_task_serialize.h
 * @brief define the templete of task serialize and deserialize.
 * @author desdk@intellif.com
 * @date 2020/03/18
 * @copyright Intellif
 * @note 
 */

#ifndef DE_TASK_SERIALIZE_H_
#define DE_TASK_SERIALIZE_H_

#include <vector>
#include <string>
#include "de_serializer.h"
#include "de_ndarray.h"
#include "de_thread.h"

namespace de{

template<typename T>
NDArray TaskSerialize(void* task) {
    T* task_in = reinterpret_cast<T*>(task);
    de::UserDataBinaryInStream stream = std::move(de::toByteArray(*task_in));
    DEByteArray byte_array = stream;

    std::vector<int64_t> shape;
    shape.emplace_back(byte_array.size);

    NDArray array = NDArray::Create(shape, const_cast<char*>(byte_array.data));

    delete task_in;

    return array;
}

template<typename T>
void* TaskDeSerialize(NDArray data) {
    std::string str_data(reinterpret_cast<char*>(data.GetTensorData()), data.GetTensorDataSize());
    //T task = de::fromByteArray<T>(str_data);
    //T* task_out = new T(task);
    T* task_out = new T();
    *task_out = de::fromByteArray<T>(str_data);

    return reinterpret_cast<void*>(task_out);
}

template<typename T>
void TaskDeleter(TaskQueue::Task job) {
    delete static_cast<T*>(job.task);
}

/*data transfer no copy Task*/
template<typename T>
struct DataTask {
    T task;
    NDArray data;

    DataTask() {
    }

    ~DataTask() {
    }

    inline void Load() {
        std::string str_data(reinterpret_cast<char*>(data.GetBearData()), data.GetBearDataSize());
        task = de::fromByteArray<T>(str_data);
    }

    inline void Save() {
        de::UserDataBinaryInStream stream = std::move(de::toByteArray(task));
        DEByteArray byte_array = stream;
        CHECK_GT(data.GetBearDataCap(),  byte_array.size) << "serialize data size = " << byte_array.size << " larger than " << "bear data (size = )." <<  data.GetBearDataCap();
        memcpy((char*)data.GetBearData(), byte_array.data, byte_array.size);
        std::vector<int64_t> shapes;
        data.Shrink(shapes, byte_array.size);
        data.SaveTensorInfo(static_cast<uint8_t*>(data.GetTensorData()));
    }
};

/*T indicate task control info*/
template<typename T>
NDArray DataTaskSerialize(void* task) {
	DataTask<T> * task_in = static_cast<DataTask<T> *>(task);
    task_in->Save();
    NDArray data = task_in->data;
    delete task_in;

    return data;
}

template<typename T>
void* DataTaskDeSerialize(NDArray data) {
	DataTask<T> *  task_out = new DataTask<T> ();
	task_out->data = data.CreateViewFromTransport();
	task_out->Load();

	return static_cast<void *>(task_out);
}

template<typename T>
void DataTaskDeleter(TaskQueue::Task job) {
    delete static_cast<DataTask<T> *>(job.task);
}


/* 
*  需要用户自己实现序列化、反序列化接口，将控制信息写在NDArray 的 beardata区域
*/
template<typename T>
NDArray TaskSerializeCustom(void* task) {
    T* task_in = static_cast<T *>(task); 
    NDArray data = task_in->Serialize();
    delete task_in;
    
    return data;
}


template<typename T>
void* TaskDeSerializeCustom(NDArray data) {
    T* task_out = new T(); 
    task_out->DeSerialize(data);
    return static_cast<void *>(task_out);
}

// template<typename T>
// void TaskDeleterNoCustom(TaskQueue::Task job) {
//     delete static_cast<T *>(job.task);
// }

} // namespace de 


#endif

