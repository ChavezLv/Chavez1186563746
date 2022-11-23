## FaceQualityPoseRunParameter

*类描述*

人脸姿态质量运行参数类


## FaceQualityPoseRunParameter::FaceQualityPoseRunParameter

*方法*
```c++
FaceQualityPoseRunParameter()
```
*功能描述*

构造函数


## FaceQualityPoseRunParameter::~FaceQualityPoseRunParameter

*方法*
```c++
~FaceQualityPoseRunParameter() override
```
*功能描述*

析构函数




### SingleFaceQualityPose

*结构体描述*

单人脸姿态和质量

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|float |pitch|  俯仰角|
|float |yaw|  偏航角|
|float |roll|  翻滚角|
|float |quality|  人脸质量得分|
## FaceQualityPoseResult

*类描述*

脸姿态质量结果返回类，存储算法的运行结果


## FaceQualityPoseResult::FaceQualityPoseResult

*方法*
```c++
FaceQualityPoseResult()
```
*功能描述*

构造函数


## FaceQualityPoseResult::~FaceQualityPoseResult

*方法*
```c++
~FaceQualityPoseResult() override
```
*功能描述*

析构函数


## FaceQualityPoseResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceQualityPoseResult::resize

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


## FaceQualityPoseResult::faceQualityPose_

*说明*

结果容器

## FaceQualityPose

*类描述*

脸姿态质量算法类


## FaceQualityPose::FaceQualityPose

*方法*
```c++
FaceQualityPose()
```
*功能描述*

构造函数


## FaceQualityPose::~FaceQualityPose

*方法*
```c++
~FaceQualityPose() override
```
*功能描述*

析构函数


## FaceQualityPose::Init

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



## FaceQualityPose::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceQualityPose::Process

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



## FaceQualityPose::SetParameter

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



## FaceQualityPose::GetParameter

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



## FaceQualityPose::*impl_

*说明*

算法实现类

