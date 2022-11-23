## FaceSmokeRunParameter

*类描述*

抽烟运行参数类


## FaceSmokeRunParameter::FaceSmokeRunParameter

*方法*
```c++
FaceSmokeRunParameter()
```
*功能描述*

构造函数


## FaceSmokeRunParameter::~FaceSmokeRunParameter

*方法*
```c++
~FaceSmokeRunParameter() override
```
*功能描述*

析构函数




### SmokeStatus
*枚举描述*

 抽烟状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOSmoke|0| 没有抽烟|
|Smoke|1|  正在抽烟|


### SingleFaceSmoke

*结构体描述*

 单人脸抽烟信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|SmokeStatus |smokeStatus|  抽烟状态|
|float |statusScore|  抽烟状态可信度得分|
## FaceSmokeResult

*类描述*

抽烟结果返回类，存储算法的运行结果


## FaceSmokeResult::FaceSmokeResult

*方法*
```c++
FaceSmokeResult()
```
*功能描述*

构造函数


## FaceSmokeResult::~FaceSmokeResult

*方法*
```c++
~FaceSmokeResult() override
```
*功能描述*

析构函数


## FaceSmokeResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceSmokeResult::resize

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


## FaceSmokeResult::faceSmokes_

*说明*

结果容器

## FaceSmoke

*类描述*

抽烟算法类


## FaceSmoke::FaceSmoke

*方法*
```c++
FaceSmoke()
```
*功能描述*

构造函数


## FaceSmoke::~FaceSmoke

*方法*
```c++
~FaceSmoke() override
```
*功能描述*

析构函数


## FaceSmoke::Init

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



## FaceSmoke::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceSmoke::Process

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



## FaceSmoke::SetParameter

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



## FaceSmoke::GetParameter

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



## FaceSmoke::*impl_

*说明*

算法实现类

