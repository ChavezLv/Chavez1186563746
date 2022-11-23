## FacialLandmarkRunParameter

*类描述*

人脸关键点运行参数类


## FacialLandmarkRunParameter::FacialLandmarkRunParameter

*方法*
```c++
FacialLandmarkRunParameter()
```
*功能描述*

构造函数


## FacialLandmarkRunParameter::~FacialLandmarkRunParameter

*方法*
```c++
~FacialLandmarkRunParameter() override
```
*功能描述*

析构函数


## FacialLandmarkResult

*类描述*

人脸关键点结果返回类，存储算法的运行结果


## FacialLandmarkResult::FacialLandmarkResult

*方法*
```c++
FacialLandmarkResult()
```
*功能描述*

构造函数


## FacialLandmarkResult::~FacialLandmarkResult

*方法*
```c++
~FacialLandmarkResult() override
```
*功能描述*

析构函数


## FacialLandmarkResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FacialLandmarkResult::resize

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


## FacialLandmarkResult::landmarks_

*说明*

结果容器

## FacialLandmark

*类描述*

人脸关键点算法类


## FacialLandmark::FacialLandmark

*方法*
```c++
FacialLandmark()
```
*功能描述*

构造函数


## FacialLandmark::~FacialLandmark

*方法*
```c++
~FacialLandmark() override
```
*功能描述*

析构函数


## FacialLandmark::Init

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



## FacialLandmark::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FacialLandmark::Process

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



## FacialLandmark::SetParameter

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



## FacialLandmark::GetParameter

*方法*
```c++
const StatusParameter *GetParameter(StatusParameter * p) override
```
*功能描述*

查询参数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|p|StatusParameter *|参数|

*返回值* 

参数状态 



## FacialLandmark::*impl_

*说明*

算法实现类

