## FaceGlassRunParameter

*类描述*

眼镜运行参数类


## FaceGlassRunParameter::FaceGlassRunParameter

*方法*
```c++
FaceGlassRunParameter()
```
*功能描述*

构造函数


## FaceGlassRunParameter::~FaceGlassRunParameter

*方法*
```c++
~FaceGlassRunParameter()
```
*功能描述*

析构函数




### GlassesStatus
*枚举描述*

带眼镜状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOGLASSES|0| 不戴眼镜|
|GLASSES|1| 戴普通眼镜|
|SUNGLASSES|2| 戴太阳镜|


### SingleFaceGlass

*结构体描述*

单人脸带眼镜状态

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|GlassesStatus |glassesStatus| 状态|
|float |statusScore| 可信度得分|
## FaceGlassResult

*类描述*

眼镜结果返回类，存储算法的运行结果


## FaceGlassResult::FaceGlassResult

*方法*
```c++
FaceGlassResult()
```
*功能描述*

构造函数


## FaceGlassResult::~FaceGlassResult

*方法*
```c++
~FaceGlassResult() override
```
*功能描述*

析构函数


## FaceGlassResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceGlassResult::resize

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


## FaceGlassResult::FaceGlasss_

*说明*

结果容器

## FaceGlass

*类描述*

眼镜算法类


## FaceGlass::FaceGlass

*方法*
```c++
FaceGlass()
```
*功能描述*

构造函数


## FaceGlass::~FaceGlass

*方法*
```c++
~FaceGlass() override
```
*功能描述*

析构函数


## FaceGlass::Init

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



## FaceGlass::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceGlass::Process

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



## FaceGlass::SetParameter

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



## FaceGlass::GetParameter

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



## FaceGlass::*impl_

*说明*

算法实现类

