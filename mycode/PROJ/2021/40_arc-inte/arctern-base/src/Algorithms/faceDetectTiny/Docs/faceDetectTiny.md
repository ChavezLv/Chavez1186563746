

### FACE_DETECT_TINY_INFO

*结构体描述*

 单人脸检测信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|ArcternRect |rect| 人脸框|
|float |score| 人脸可信度得分|
## FaceDetectTinyConfigParameter

*类描述*

人脸检测配置参数类


## FaceDetectTinyConfigParameter::FaceDetectTinyConfigParameter

*方法*
```c++
FaceDetectTinyConfigParameter()
```
*功能描述*

构造函数


## FaceDetectTinyConfigParameter::~FaceDetectTinyConfigParameter

*方法*
```c++
~FaceDetectTinyConfigParameter() override
```
*功能描述*

析构函数


## FaceDetectTinyConfigParameter::confThresh_

*说明*

人脸可信度阈值，低于此阈值，人脸框会被过滤

## FaceDetectTinyResult

*类描述*

人脸检测结果返回类，存储算法的运行结果


## FaceDetectTinyResult::FaceDetectTinyResult

*方法*
```c++
FaceDetectTinyResult()
```
*功能描述*

构造函数


## FaceDetectTinyResult::~FaceDetectTinyResult

*方法*
```c++
~FaceDetectTinyResult() override
```
*功能描述*

析构函数


## FaceDetectTinyResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceDetectTinyResult::resize

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


## FaceDetectTinyResult::detInfos_

*说明*

检测信息

## FaceDetectTiny

*类描述*

人脸检测算法类


## FaceDetectTiny::FaceDetectTiny

*方法*
```c++
FaceDetectTiny()
```
*功能描述*

构造函数


## FaceDetectTiny::~FaceDetectTiny

*方法*
```c++
~FaceDetectTiny() override
```
*功能描述*

析构函数


## FaceDetectTiny::Init

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



## FaceDetectTiny::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceDetectTiny::Process

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



## FaceDetectTiny::SetParameter

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



## FaceDetectTiny::GetParameter

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



## FaceDetectTiny::*impl_

*说明*

算法实现类

