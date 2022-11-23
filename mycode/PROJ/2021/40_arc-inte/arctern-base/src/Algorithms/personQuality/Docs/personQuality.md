## PersonQualityRunParameter

*类描述*

人体质量运行参数类


## PersonQualityRunParameter::PersonQualityRunParameter

*方法*
```c++
PersonQualityRunParameter()
```
*功能描述*

构造函数


## PersonQualityRunParameter::~PersonQualityRunParameter

*方法*
```c++
~PersonQualityRunParameter() override
```
*功能描述*

析构函数


## 人体质量得分
using qualityScore = float;


*类描述*

人体质量结果返回类，存储算法的运行结果


## PersonQualityResult::PersonQualityResult

*方法*
```c++
PersonQualityResult()
```
*功能描述*

构造函数


## PersonQualityResult::~PersonQualityResult

*方法*
```c++
~PersonQualityResult() override
```
*功能描述*

析构函数


## PersonQualityResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## PersonQualityResult::resize

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


*说明*

结果容器

## PersonQuality

*类描述*

人体质量算法类


## PersonQuality::PersonQuality

*方法*
```c++
PersonQuality()
```
*功能描述*

构造函数


## PersonQuality::~PersonQuality

*方法*
```c++
~PersonQuality() override
```
*功能描述*

析构函数


## PersonQuality::Init

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



## PersonQuality::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## PersonQuality::Process

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



## PersonQuality::SetParameter

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



## PersonQuality::GetParameter

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



## PersonQuality::*impl_

*说明*

算法实现类

