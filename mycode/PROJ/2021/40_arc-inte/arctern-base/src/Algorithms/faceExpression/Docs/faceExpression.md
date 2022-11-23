## FaceExpressionRunParameter

*类描述*

人脸表情运行参数类


## FaceExpressionRunParameter::FaceExpressionRunParameter

*方法*
```c++
FaceExpressionRunParameter()
```
*功能描述*

构造函数


## FaceExpressionRunParameter::~FaceExpressionRunParameter

*方法*
```c++
~FaceExpressionRunParameter() override
```
*功能描述*

析构函数




### ExpressionType
*枚举描述*

表情类型

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|ANGRY|0| 愤怒|
|DISGUST|1| 厌恶|
|FEAR|2| 害怕|
|HAPPY|3| 高兴|
|SAD|4| 悲伤|
|SURPRISE|5| 惊讶|
|NEUTRAL_EXP|6| 中性|


### SingleFaceExpression

*结构体描述*

单人脸表情信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|ExpressionType |type|  表情类型|
|float |score|  可信度得分|
## FaceExpressionResult

*类描述*

表情结果返回类，存储算法的运行结果


## FaceExpressionResult::FaceExpressionResult

*方法*
```c++
FaceExpressionResult()
```
*功能描述*

构造函数


## FaceExpressionResult::~FaceExpressionResult

*方法*
```c++
~FaceExpressionResult() override
```
*功能描述*

析构函数


## FaceExpressionResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceExpressionResult::resize

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


## FaceExpressionResult::faceExpressions_

*说明*

结果容器

## FaceExpression

*类描述*

表情算法类


## FaceExpression::FaceExpression

*方法*
```c++
FaceExpression()
```
*功能描述*

构造函数


## FaceExpression::~FaceExpression

*方法*
```c++
~FaceExpression() override
```
*功能描述*

析构函数


## FaceExpression::Init

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



## FaceExpression::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceExpression::Process

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



## FaceExpression::SetParameter

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



## FaceExpression::GetParameter

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



## FaceExpression::*impl_

*说明*

算法实现类

