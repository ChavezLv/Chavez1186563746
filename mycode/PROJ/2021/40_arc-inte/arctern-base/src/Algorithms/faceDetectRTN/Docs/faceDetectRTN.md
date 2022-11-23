

### FACE_DETECT_RTN_INFO

*结构体描述*

 单人脸检测信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|ArcternRect |rect|  人脸框|
|ArcternPoint []|landmarks|  人脸关键点|
|float |score|  人脸框可信度|
|float |quality|  人脸质量|
## FaceDetectRTNConfigParameter

*类描述*

人脸检测配置参数类


## FaceDetectRTNConfigParameter::FaceDetectRTNConfigParameter

*方法*
```c++
FaceDetectRTNConfigParameter()
```
*功能描述*

构造函数


## FaceDetectRTNConfigParameter::~FaceDetectRTNConfigParameter

*方法*
```c++
~FaceDetectRTNConfigParameter() override
```
*功能描述*

析构函数


## FaceDetectRTNConfigParameter::confThresh_

*说明*

可信度阈值，低于此阈值的人脸框会被过滤

## FaceDetectRTNResult

*类描述*

人脸检测结果返回类，存储算法的运行结果


## FaceDetectRTNResult::FaceDetectRTNResult

*方法*
```c++
FaceDetectRTNResult()
```
*功能描述*

构造函数


## FaceDetectRTNResult::~FaceDetectRTNResult

*方法*
```c++
~FaceDetectRTNResult() override
```
*功能描述*

析构函数


## FaceDetectRTNResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceDetectRTNResult::resize

*方法*
```c++
void resize(int n) override
```
*功能描述*

调整容器的大小。上层可以不关注此函数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|n|int|容器尺寸|


## FaceDetectRTNResult::detInfos_

*说明*

检测信息

## FaceDetectRTN

*类描述*

人脸检测算法类


## FaceDetectRTN::FaceDetectRTN

*方法*
```c++
FaceDetectRTN()
```
*功能描述*

构造函数


## FaceDetectRTN::~FaceDetectRTN

*方法*
```c++
~FaceDetectRTN() override
```
*功能描述*

析构函数


## FaceDetectRTN::Init

*方法*
```c++
ErrorType Init(const InitParameter *p) override
```
*功能描述*

算法初始化


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|p|InitParameter*|初始化参数|

*返回值* 

错误码 



## FaceDetectRTN::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceDetectRTN::Process

*方法*
```c++
const Result *Process(const RunParameter *p) override
```
*功能描述*

执行算法


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|p|RunParameter*|运行参数|

*返回值* 

算法结果 



## FaceDetectRTN::SetParameter

*方法*
```c++
ErrorType SetParameter(ConfigParameter *p) override
```
*功能描述*

配置参数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|p|ConfigParameter*|参数信息,|

*返回值* 

错误码 



## FaceDetectRTN::GetParameter

*方法*
```c++
const StatusParameter *GetParameter(StatusParameter *p) override
```
*功能描述*

查询参数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|p|StatusParameter*|参数|

*返回值* 

参数状态 



## FaceDetectRTN::*impl_

*说明*

算法实现类

