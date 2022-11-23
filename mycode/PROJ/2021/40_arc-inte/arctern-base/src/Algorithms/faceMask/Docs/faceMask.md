## FaceMaskRunParameter

*类描述*

人脸口罩运行参数类


## FaceMaskRunParameter::FaceMaskRunParameter

*方法*
```c++
FaceMaskRunParameter()
```
*功能描述*

构造函数


## FaceMaskRunParameter::~FaceMaskRunParameter

*方法*
```c++
~FaceMaskRunParameter()
```
*功能描述*

析构函数




### MaskStatus
*枚举描述*

佩戴口罩状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|MASK|0|  佩戴口罩|
|NOMASK|1| 无佩戴口罩|


### SingleFaceMask

*结构体描述*

单人脸佩戴口罩信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|MaskStatus |maskStatus|  口罩状态|
|float |statusScore|  口罩状态可信度得分|
## FaceMaskResult

*类描述*

人脸口罩结果返回类，存储算法的运行结果


## FaceMaskResult::FaceMaskResult

*方法*
```c++
FaceMaskResult()
```
*功能描述*

构造函数


## FaceMaskResult::~FaceMaskResult

*方法*
```c++
~FaceMaskResult() override
```
*功能描述*

析构函数


## FaceMaskResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceMaskResult::resize

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


## FaceMaskResult::FaceMasks_

*说明*

结果容器

## FaceMask

*类描述*

人脸口罩算法类


## FaceMask::FaceMask

*方法*
```c++
FaceMask()
```
*功能描述*

构造函数


## FaceMask::~FaceMask

*方法*
```c++
~FaceMask() override
```
*功能描述*

析构函数


## FaceMask::Init

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



## FaceMask::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceMask::Process

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



## FaceMask::SetParameter

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



## FaceMask::GetParameter

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



## FaceMask::*impl_

*说明*

算法实现类

