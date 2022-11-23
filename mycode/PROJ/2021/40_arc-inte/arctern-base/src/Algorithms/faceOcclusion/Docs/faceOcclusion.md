## FaceOcclusionRunParameter

*类描述*

人脸遮挡运行参数类


## FaceOcclusionRunParameter::FaceOcclusionRunParameter

*方法*
```c++
FaceOcclusionRunParameter()
```
*功能描述*

构造函数


## FaceOcclusionRunParameter::~FaceOcclusionRunParameter

*方法*
```c++
~FaceOcclusionRunParameter()
```
*功能描述*

析构函数




### SingleFaceOcclusion

*结构体描述*

 单人脸人脸遮挡信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|occlusionStatus |leftEyeOpen| 左眼睁开|
|occlusionStatus |rightEyeOpen| 右眼睁开|
|occlusionStatus |leftEyeOcc| 左眼遮挡|
|occlusionStatus |rightEyeOcc| 右眼遮挡|
|occlusionStatus |leftCheekOcc| 左脸颊遮挡|
|occlusionStatus |rightCheekOcc| 右脸颊遮挡|
|occlusionStatus |mouthOcc| 鼻子遮挡|
|occlusionStatus |noseOcc| 嘴巴遮挡|
|occlusionStatus |chincontourOcc| 下巴遮挡|
## FaceOcclusionResult

*类描述*

人脸遮挡结果返回类，存储算法的运行结果


## FaceOcclusionResult::FaceOcclusionResult

*方法*
```c++
FaceOcclusionResult()
```
*功能描述*

构造函数


## FaceOcclusionResult::~FaceOcclusionResult

*方法*
```c++
~FaceOcclusionResult() override
```
*功能描述*

析构函数


## FaceOcclusionResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceOcclusionResult::resize

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


## FaceOcclusionResult::faceOcclusion_

*说明*

结果容器

## FaceOcclusion

*类描述*

人脸遮挡算法类


## FaceOcclusion::FaceOcclusion

*方法*
```c++
FaceOcclusion()
```
*功能描述*

构造函数


## FaceOcclusion::~FaceOcclusion

*方法*
```c++
~FaceOcclusion() override
```
*功能描述*

析构函数


## FaceOcclusion::Init

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



## FaceOcclusion::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceOcclusion::Process

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



## FaceOcclusion::SetParameter

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



## FaceOcclusion::GetParameter

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



## FaceOcclusion::*impl_

*说明*

算法实现类

