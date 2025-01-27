## FaceLivenessIrRunParameter

*类描述*

ir活体检测运行参数类


## FaceLivenessIrRunParameter::FaceLivenessIrRunParameter

*方法*
```c++
FaceLivenessIrRunParameter()
```
*功能描述*

构造函数


## FaceLivenessIrRunParameter::~FaceLivenessIrRunParameter

*方法*
```c++
~FaceLivenessIrRunParameter() override
```
*功能描述*

析构函数


## FaceLivenessIrResult

*类描述*

ir活体检测结果返回类，存储算法的运行结果


## FaceLivenessIrResult::FaceLivenessIrResult

*方法*
```c++
FaceLivenessIrResult()
```
*功能描述*

构造函数


## FaceLivenessIrResult::~FaceLivenessIrResult

*方法*
```c++
~FaceLivenessIrResult() override
```
*功能描述*

析构函数


## FaceLivenessIrResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceLivenessIrResult::resize

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


## FaceLivenessIrResult::faceLiveness_

*说明*

结果容器

## FaceLivenessIr

*类描述*

ir活体检测算法类


## FaceLivenessIr::FaceLivenessIr

*方法*
```c++
FaceLivenessIr()
```
*功能描述*

构造函数


## FaceLivenessIr::~FaceLivenessIr

*方法*
```c++
~FaceLivenessIr() override
```
*功能描述*

析构函数


## FaceLivenessIr::Init

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



## FaceLivenessIr::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceLivenessIr::Process

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



## FaceLivenessIr::SetParameter

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



## FaceLivenessIr::GetParameter

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



## FaceLivenessIr::*impl_

*说明*

算法实现类

