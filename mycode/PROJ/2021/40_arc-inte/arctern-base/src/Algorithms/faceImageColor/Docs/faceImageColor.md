## FaceImageColorRunParameter

*类描述*

人脸图像颜色运行参数类


## FaceImageColorRunParameter::FaceImageColorRunParameter

*方法*
```c++
FaceImageColorRunParameter()
```
*功能描述*

构造函数


## FaceImageColorRunParameter::~FaceImageColorRunParameter

*方法*
```c++
~FaceImageColorRunParameter() override
```
*功能描述*

析构函数


*类描述*

人脸图像颜结果返回类，存储算法的运行结果


## FaceImageColorResult::FaceImageColorResult

*方法*
```c++
FaceImageColorResult()
```
*功能描述*

构造函数


## FaceImageColorResult::~FaceImageColorResult

*方法*
```c++
~FaceImageColorResult() override
```
*功能描述*

析构函数


## FaceImageColorResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceImageColorResult::resize

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


## FaceImageColorResult::faceImageColors_

*说明*

结果容器, 大于0.5表示为彩色，否则为黑白

## FaceImageColor

*类描述*

人脸图像颜色算法类


## FaceImageColor::FaceImageColor

*方法*
```c++
FaceImageColor()
```
*功能描述*

构造函数


## FaceImageColor::~FaceImageColor

*方法*
```c++
~FaceImageColor() override
```
*功能描述*

析构函数


## FaceImageColor::Init

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



## FaceImageColor::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceImageColor::Process

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



## FaceImageColor::SetParameter

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



## FaceImageColor::GetParameter

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



## FaceImageColor::*impl_

*说明*

算法实现类

