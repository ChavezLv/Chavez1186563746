## FaceHeadwearRunParameter

*类描述*

头戴物运行参数类


## FaceHeadwearRunParameter::FaceHeadwearRunParameter

*方法*
```c++
FaceHeadwearRunParameter()
```
*功能描述*

构造函数


## FaceHeadwearRunParameter::~FaceHeadwearRunParameter

*方法*
```c++
~FaceHeadwearRunParameter() override
```
*功能描述*

析构函数




### HeadwearColor
*枚举描述*

头戴物颜色

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOHEADWEARCOLOR|0| 无头戴物|
|BLACK|1| 黑色|
|WHITE|2| 白色|
|GRAY_BROWN|3| 灰褐色|
|RED|4| 红色|
|PINK|5| 粉色|
|ORANGE|6| 橙色|
|BLUE|7| 蓝色|
|YELLOW|8| 黄色|
|OTHERCOLOR|9| 其他颜色|


### HeadwearType
*枚举描述*

头戴物类型

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOHEADWEARTYPE|0|  无头戴物|
|HAT|1|  帽子|
|BUILDER|2|  builder|
|SPORT|3|  sport|
|CYCLING|4|  cycling|
|OTHERTYPE|5|  other|


### SingleFaceHeadwear

*结构体描述*

单人脸头戴物信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|HeadwearColor |color|  头戴物颜色|
|float |colorScore|  头戴物颜色可信度|
|HeadwearType |type|  头戴物类型|
|float |typeScore|  头戴物类型可信度|
## FaceHeadwearResult

*类描述*

头戴物结果返回类，存储算法的运行结果


## FaceHeadwearResult::FaceHeadwearResult

*方法*
```c++
FaceHeadwearResult()
```
*功能描述*

构造函数


## FaceHeadwearResult::~FaceHeadwearResult

*方法*
```c++
~FaceHeadwearResult() override
```
*功能描述*

析构函数


## FaceHeadwearResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceHeadwearResult::resize

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


## FaceHeadwearResult::faceHeadwears_

*说明*

结果容器

## FaceHeadwear

*类描述*

头戴物算法类


## FaceHeadwear::FaceHeadwear

*方法*
```c++
FaceHeadwear()
```
*功能描述*

构造函数


## FaceHeadwear::~FaceHeadwear

*方法*
```c++
~FaceHeadwear() override
```
*功能描述*

析构函数


## FaceHeadwear::Init

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



## FaceHeadwear::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceHeadwear::Process

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



## FaceHeadwear::SetParameter

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



## FaceHeadwear::GetParameter

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



## FaceHeadwear::*impl_

*说明*

算法实现类

