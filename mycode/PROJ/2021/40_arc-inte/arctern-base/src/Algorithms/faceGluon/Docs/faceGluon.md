## FaceGluonRunParameter

*类描述*

人脸特征提取运行参数类


## FaceGluonRunParameter::FaceGluonRunParameter

*方法*
```c++
FaceGluonRunParameter()
```
*功能描述*

构造函数


## FaceGluonRunParameter::~FaceGluonRunParameter

*方法*
```c++
~FaceGluonRunParameter() override
```
*功能描述*

析构函数


## FaceGluonRunParameter::faceLandmarksV_

*说明*

人脸关键点，部分识别算法需要人脸关键点。是否需要请查看运行log

## FaceGluonResult

*类描述*

人脸特征提取结果返回类，存储算法的运行结果


## FaceGluonResult::FaceGluonResult

*方法*
```c++
FaceGluonResult()
```
*功能描述*

构造函数


## FaceGluonResult::~FaceGluonResult

*方法*
```c++
~FaceGluonResult() override
```
*功能描述*

析构函数


## FaceGluonResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceGluonResult::resize

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


## FaceGluonResult::features_

*说明*

结果容器

## FaceGluon

*类描述*

人脸特征提取算法类


## FaceGluon::FaceGluon

*方法*
```c++
FaceGluon()
```
*功能描述*

构造函数


## FaceGluon::~FaceGluon

*方法*
```c++
~FaceGluon() override
```
*功能描述*

析构函数


## FaceGluon::Init

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



## FaceGluon::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceGluon::Process

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



## FaceGluon::SetParameter

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



## FaceGluon::GetParameter

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



## FaceGluon::*impl_

*说明*

算法实现类

