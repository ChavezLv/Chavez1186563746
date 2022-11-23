## FacePoseRunParameter

*类描述*

人脸姿态运行参数类


## FacePoseRunParameter::FacePoseRunParameter

*方法*
```c++
FacePoseRunParameter()
```
*功能描述*

构造函数


## FacePoseRunParameter::~FacePoseRunParameter

*方法*
```c++
~FacePoseRunParameter() override
```
*功能描述*

析构函数




### FacePoseLevel
*枚举描述*

 人脸姿态等级

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NEGATIVE_0_15|0| 角度-(0~15)|
|POSITIVE_0_15|1| 角度+(0~15)|
|NEGATIVE_15_30|2| 角度-(15~30)|
|POSITIVE_15_30|3| 角度+(15~30)|
|NEGATIVE_30_45|4| 角度-(30~45)|
|POSITIVE_30_45|5| 角度+(30~45)|
|NEGATIVE_45_60|6| 角度-(45~60)|
|POSITIVE_45_60|7| 角度+(45~60)|
|NEGATIVE_60_90|8| 角度-(60~90)|
|POSITIVE_60_90|9| 角度+(60~90)|
|OTHERANGLE|10| 角度others|


### SingleFacePose

*结构体描述*

单人脸姿态信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|FacePoseLevel |pitch|  俯仰角|
|FacePoseLevel |yaw|  偏航角|
|FacePoseLevel |roll|  翻滚角|
## FacePoseResult

*类描述*

人脸姿态结果返回类，存储算法的运行结果


## FacePoseResult::FacePoseResult

*方法*
```c++
FacePoseResult()
```
*功能描述*

构造函数


## FacePoseResult::~FacePoseResult

*方法*
```c++
~FacePoseResult() override
```
*功能描述*

析构函数


## FacePoseResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FacePoseResult::resize

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


## FacePoseResult::facePose_

*说明*

结果容器

## FacePose

*类描述*

人脸姿态算法类


## FacePose::FacePose

*方法*
```c++
FacePose()
```
*功能描述*

构造函数


## FacePose::~FacePose

*方法*
```c++
~FacePose() override
```
*功能描述*

析构函数


## FacePose::Init

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



## FacePose::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FacePose::Process

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



## FacePose::SetParameter

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



## FacePose::GetParameter

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



## FacePose::*impl_

*说明*

算法实现类

