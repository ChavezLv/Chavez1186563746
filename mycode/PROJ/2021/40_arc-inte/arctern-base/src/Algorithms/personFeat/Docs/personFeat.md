## PersonFeatRunParameter

*类描述*

人体特征运行参数类


## PersonFeatRunParameter::PersonFeatRunParameter

*方法*
```c++
PersonFeatRunParameter()
```
*功能描述*

构造函数


## PersonFeatRunParameter::~PersonFeatRunParameter

*方法*
```c++
~PersonFeatRunParameter()
```
*功能描述*

析构函数


## PersonFeatResult

*类描述*

人体特征结果返回类，存储算法的运行结果


## PersonFeatResult::PersonFeatResult

*方法*
```c++
PersonFeatResult()
```
*功能描述*

构造函数


## PersonFeatResult::~PersonFeatResult

*方法*
```c++
~PersonFeatResult() override
```
*功能描述*

析构函数


## PersonFeatResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## PersonFeatResult::resize

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


## PersonFeatResult::features_

*说明*

结果容器

## PersonFeat

*类描述*

人体特征提取算法类


## PersonFeat::PersonFeat

*方法*
```c++
PersonFeat()
```
*功能描述*

构造函数


## PersonFeat::~PersonFeat

*方法*
```c++
~PersonFeat() override
```
*功能描述*

析构函数


## PersonFeat::Init

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



## PersonFeat::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## PersonFeat::Process

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



## PersonFeat::SetParameter

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



## PersonFeat::GetParameter

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



## PersonFeat::*impl_

*说明*

算法实现类

