## PersonFeatRunParameter

*类描述*

人体完整度运行参数类


## PersonCompletenessRunParameter::PersonCompletenessRunParameter

*方法*
```c++
PersonCompletenessRunParameter()
```
*功能描述*

构造函数


## PersonCompletenessRunParameter::~PersonCompletenessRunParameter

*方法*
```c++
~PersonCompletenessRunParameter()
```
*功能描述*

析构函数


## PersonCompletenessResult

*类描述*

人体完整度结果返回类，存储算法的运行结果


## PersonCompletenessResult::PersonCompletenessResult

*方法*
```c++
PersonCompletenessResult()
```
*功能描述*

构造函数


## PersonCompletenessResult::~PersonCompletenessResult

*方法*
```c++
~PersonCompletenessResult() override
```
*功能描述*

析构函数


## PersonCompletenessResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## PersonCompletenessResult::resize

*方法*
```c++
void resize(int n) override
```
*功能描述*

调整容器的大小。上层可以不关注此函数


*说明*

结果容器

## PersonCompleteness

*类描述*

人体特征提取算法类


## PersonCompleteness::PersonCompleteness

*方法*
```c++
PersonCompleteness()
```
*功能描述*

构造函数


## PersonCompleteness::~PersonCompleteness

*方法*
```c++
~PersonCompleteness() override
```
*功能描述*

析构函数


## PersonCompleteness::Init

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



## PersonCompleteness::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## PersonCompleteness::Process

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



## PersonCompleteness::SetParameter

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



## PersonCompleteness::GetParameter

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



## PersonCompleteness::*impl_

*说明*

算法实现类

