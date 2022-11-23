## FaceNoiseMotionblurRunParameter

*类描述*

人脸运动质量运行参数类


## FaceNoiseMotionblurRunParameter::FaceNoiseMotionblurRunParameter

*方法*
```c++
FaceNoiseMotionblurRunParameter()
```
*功能描述*

构造函数


## FaceNoiseMotionblurRunParameter::~FaceNoiseMotionblurRunParameter

*方法*
```c++
~FaceNoiseMotionblurRunParameter()
```
*功能描述*

析构函数




### NoiseType
*枚举描述*

 人脸运动质量类型

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|MOTIONBLUR|0| 运动模糊|
|NOISY|1| 有噪点|
|NORMAL|2| 正常|


### SingleFaceNoiseMotionblur

*结构体描述*

单人脸运动质量信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|NoiseType |type|  人脸运动质量类型|
|float |score|  人脸运动质量类型得分|
## FaceNoiseMotionblurResult

*类描述*

人脸运动质量结果返回类，存储算法的运行结果


## FaceNoiseMotionblurResult::FaceNoiseMotionblurResult

*方法*
```c++
FaceNoiseMotionblurResult()
```
*功能描述*

构造函数


## FaceNoiseMotionblurResult::~FaceNoiseMotionblurResult

*方法*
```c++
~FaceNoiseMotionblurResult() override
```
*功能描述*

析构函数


## FaceNoiseMotionblurResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceNoiseMotionblurResult::resize

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


## FaceNoiseMotionblurResult::faceNoiseMotionblurs_

*说明*

结果容器

## FaceNoiseMotionblur

*类描述*

人脸运动质量算法类


## FaceNoiseMotionblur::FaceNoiseMotionblur

*方法*
```c++
FaceNoiseMotionblur()
```
*功能描述*

构造函数


## FaceNoiseMotionblur::~FaceNoiseMotionblur

*方法*
```c++
~FaceNoiseMotionblur() override
```
*功能描述*

析构函数


## FaceNoiseMotionblur::Init

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



## FaceNoiseMotionblur::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceNoiseMotionblur::Process

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



## FaceNoiseMotionblur::SetParameter

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



## FaceNoiseMotionblur::GetParameter

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



## FaceNoiseMotionblur::*impl_

*说明*

算法实现类

