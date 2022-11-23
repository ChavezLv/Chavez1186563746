## FaceAbnomalLightRunParameter

*类描述*

人脸光照质量运行参数类


## FaceAbnomalLightRunParameter::FaceAbnomalLightRunParameter

*方法*
```c++
FaceAbnomalLightRunParameter()
```
*功能描述*

构造函数


## FaceAbnomalLightRunParameter::~FaceAbnomalLightRunParameter

*方法*
```c++
~FaceAbnomalLightRunParameter()
```
*功能描述*

析构函数




### AbnomalLightType
*枚举描述*

人脸光照质量

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|BACK|0| 逆光|
|FRONT|1| 过曝光|
|DARK|2| 过暗|
|SIDE|3| 侧光阴阳脸|
|NORMAL_LIGHT|4| 正常|


### SingleFaceAbnomalLight

*结构体描述*

单人脸人脸光照质量

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|AbnomalLightType |type| 人脸光照质量类型|
|float |score| 人脸光照质量可信度|
## FaceAbnomalLightResult

*类描述*

人脸光照质量结果返回类，存储算法的运行结果


## FaceAbnomalLightResult::FaceAbnomalLightResult

*方法*
```c++
FaceAbnomalLightResult()
```
*功能描述*

构造函数


## FaceAbnomalLightResult::~FaceAbnomalLightResult

*方法*
```c++
~FaceAbnomalLightResult() override
```
*功能描述*

析构函数


## FaceAbnomalLightResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceAbnomalLightResult::resize

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


## FaceAbnomalLightResult::faceAbnomalLights_

*说明*

结果容器

## FaceAbnomalLight

*类描述*

人脸光照质量算法类


## FaceAbnomalLight::FaceAbnomalLight

*方法*
```c++
FaceAbnomalLight()
```
*功能描述*

构造函数


## FaceAbnomalLight::~FaceAbnomalLight

*方法*
```c++
~FaceAbnomalLight() override
```
*功能描述*

析构函数


## FaceAbnomalLight::Init

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



## FaceAbnomalLight::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceAbnomalLight::Process

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



## FaceAbnomalLight::SetParameter

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



## FaceAbnomalLight::GetParameter

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



## FaceAbnomalLight::*impl_

*说明*

算法实现类

