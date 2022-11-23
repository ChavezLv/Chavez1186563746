## FaceHatRunParameter

*类描述*

人脸帽子运行参数类


## FaceHatRunParameter::FaceHatRunParameter

*方法*
```c++
FaceHatRunParameter()
```
*功能描述*

构造函数


## FaceHatRunParameter::~FaceHatRunParameter

*方法*
```c++
~FaceHatRunParameter()
```
*功能描述*

析构函数




### HatStatus
*枚举描述*

佩戴帽子状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOHat|0| 不戴帽子|
|Hat|1| 带帽子|


### SingleFaceHat

*结构体描述*

单人脸戴帽子信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|HatStatus |hatStatus| 状态|
|float |statusScore| 状态得分|
## FaceHatResult

*类描述*

人脸帽子结果返回类，存储算法的运行结果


## FaceHatResult::FaceHatResult

*方法*
```c++
FaceHatResult()
```
*功能描述*

构造函数


## FaceHatResult::~FaceHatResult

*方法*
```c++
~FaceHatResult() override
```
*功能描述*

析构函数


## FaceHatResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceHatResult::resize

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


## FaceHatResult::FaceHats_

*说明*

结果容器

## FaceHat

*类描述*

人脸帽子算法类


## FaceHat::FaceHat

*方法*
```c++
FaceHat()
```
*功能描述*

构造函数


## FaceHat::~FaceHat

*方法*
```c++
~FaceHat() override
```
*功能描述*

析构函数


## FaceHat::Init

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



## FaceHat::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceHat::Process

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



## FaceHat::SetParameter

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



## FaceHat::GetParameter

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



## FaceHat::*impl_

*说明*

算法实现类

