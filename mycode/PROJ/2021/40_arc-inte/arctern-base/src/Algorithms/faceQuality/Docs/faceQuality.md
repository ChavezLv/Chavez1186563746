## FaceQualityRunParameter

*类描述*

人脸质量运行参数类


## FaceQualityRunParameter::FaceQualityRunParameter

*方法*
```c++
FaceQualityRunParameter()
```
*功能描述*

构造函数


## FaceQualityRunParameter::~FaceQualityRunParameter

*方法*
```c++
~FaceQualityRunParameter() override
```
*功能描述*

析构函数


## FaceQualityResult

*类描述*

人脸质量结果返回类，存储算法的运行结果


## FaceQualityResult::FaceQualityResult

*方法*
```c++
FaceQualityResult()
```
*功能描述*

构造函数


## FaceQualityResult::~FaceQualityResult

*方法*
```c++
~FaceQualityResult() override
```
*功能描述*

析构函数


## FaceQualityResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceQualityResult::resize

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


## FaceQualityResult::faceQuality_

*说明*

结果容器

## FaceQuality

*类描述*

人脸质量算法类


## FaceQuality::FaceQuality

*方法*
```c++
FaceQuality()
```
*功能描述*

构造函数


## FaceQuality::~FaceQuality

*方法*
```c++
~FaceQuality() override
```
*功能描述*

析构函数


## FaceQuality::Init

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



## FaceQuality::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceQuality::Process

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



## FaceQuality::SetParameter

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



## FaceQuality::GetParameter

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



## FaceQuality::*impl_

*说明*

算法实现类

