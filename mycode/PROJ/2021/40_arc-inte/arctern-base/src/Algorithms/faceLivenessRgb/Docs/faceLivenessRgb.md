## FaceLivenessRgbRunParameter

*类描述*

rgb活体检测运行参数类


## FaceLivenessRgbRunParameter::FaceLivenessRgbRunParameter

*方法*
```c++
FaceLivenessRgbRunParameter()
```
*功能描述*

构造函数


## FaceLivenessRgbRunParameter::~FaceLivenessRgbRunParameter

*方法*
```c++
~FaceLivenessRgbRunParameter() override
```
*功能描述*

析构函数


## FaceLivenessRgbResult

*类描述*

rgb活体检测结果返回类，存储算法的运行结果


## FaceLivenessRgbResult::FaceLivenessRgbResult

*方法*
```c++
FaceLivenessRgbResult()
```
*功能描述*

构造函数


## FaceLivenessRgbResult::~FaceLivenessRgbResult

*方法*
```c++
~FaceLivenessRgbResult() override
```
*功能描述*

析构函数


## FaceLivenessRgbResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceLivenessRgbResult::resize

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


## FaceLivenessRgbResult::faceLiveness_

*说明*

结果容器

## FaceLivenessRgb

*类描述*

rgb活体检测算法类


## FaceLivenessRgb::FaceLivenessRgb

*方法*
```c++
FaceLivenessRgb()
```
*功能描述*

构造函数


## FaceLivenessRgb::~FaceLivenessRgb

*方法*
```c++
~FaceLivenessRgb() override
```
*功能描述*

析构函数


## FaceLivenessRgb::Init

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



## FaceLivenessRgb::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceLivenessRgb::Process

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



## FaceLivenessRgb::SetParameter

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



## FaceLivenessRgb::GetParameter

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



## FaceLivenessRgb::*impl_

*说明*

算法实现类

