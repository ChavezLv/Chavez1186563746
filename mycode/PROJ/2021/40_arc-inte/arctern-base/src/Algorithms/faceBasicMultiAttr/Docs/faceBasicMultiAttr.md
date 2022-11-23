## FaceBasicMultiAttrRunParameter

*类描述*

人脸基础属性(5种)运行参数类


## FaceBasicMultiAttrRunParameter::FaceBasicMultiAttrRunParameter

*方法*
```c++
FaceBasicMultiAttrRunParameter()
```
*功能描述*

构造函数


## FaceBasicMultiAttrRunParameter::~FaceBasicMultiAttrRunParameter

*方法*
```c++
~FaceBasicMultiAttrRunParameter() override
```
*功能描述*

析构函数




### BasicGender
*枚举描述*

性别

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|FAMALE|0| 女性|
|MALE|1| 男性|


### BasicGlasses
*枚举描述*

眼镜

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOGLASSES|0| 没戴眼镜|
|GLASSES|1| 普通眼镜|
|SUNGLASSES|2| 太阳眼镜/墨镜|


### BasicMask
*枚举描述*

口罩

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOMASK|0| 没戴口罩|
|MALE|1| 戴了口罩|


### BasicHeadwear
*枚举描述*

头戴物类型

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOHEADWEAR|0|  无头戴物|
|HAT|1|  帽子|
|SPORT|2|  sport|
|CYCLING|3|  cycling|
|BUILDER|4|  builder|
|OTHERTYPE|5|  other|


### SingleFaceBasicMultiAttr

*结构体描述*

单人脸基础属性信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|BasicGender   |gender|  性别|
|float         |genderScore|  性别可信度|
|int           |age|  年龄|
|float         |ageScore|  年龄可信度|
|BasicGlasses  |glasses|  眼镜类型|
|float         |glassesScore|  眼镜类型可信度|
|BasicMask     |mask|  口罩|
|float         |maskScore|  口罩可信度|
|BasicHeadwear |headwear|  头戴物类型|
|float         |headwearScore|  头戴物类型可信度|
## FaceBasicMultiAttrResult

*类描述*

人脸基础属性返回类，存储算法的运行结果


## FaceBasicMultiAttrResult::FaceBasicMultiAttrResult

*方法*
```c++
FaceBasicMultiAttrResult()
```
*功能描述*

构造函数


## FaceBasicMultiAttrResult::~FaceBasicMultiAttrResult

*方法*
```c++
~FaceBasicMultiAttrResult() override
```
*功能描述*

析构函数


## FaceBasicMultiAttrResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceBasicMultiAttrResult::resize

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


## FaceBasicMultiAttrResult::faceBasicMultiAttrs_

*说明*

结果容器

## FaceBasicMultiAttr

*类描述*

人脸基础属性算法类


## FaceBasicMultiAttr::FaceBasicMultiAttr

*方法*
```c++
FaceBasicMultiAttr()
```
*功能描述*

构造函数


## FaceBasicMultiAttr::~FaceBasicMultiAttr

*方法*
```c++
~FaceBasicMultiAttr() override
```
*功能描述*

析构函数


## FaceBasicMultiAttr::Init

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



## FaceBasicMultiAttr::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceBasicMultiAttr::Process

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



## FaceBasicMultiAttr::SetParameter

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



## FaceBasicMultiAttr::GetParameter

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



## FaceBasicMultiAttr::*impl_

*说明*

算法实现类

