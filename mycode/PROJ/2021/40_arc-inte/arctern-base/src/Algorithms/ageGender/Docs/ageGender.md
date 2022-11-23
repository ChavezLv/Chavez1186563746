## AgeGenderRunParameter

*类描述*

年龄性别运行参数类


## AgeGenderRunParameter::AgeGenderRunParameter

*方法*
```c++
AgeGenderRunParameter()
```
*功能描述*

构造函数


## AgeGenderRunParameter::~AgeGenderRunParameter

*方法*
```c++
~AgeGenderRunParameter() override
```
*功能描述*

析构函数




### ArcternGender
*枚举描述*

枚举类型，性别

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|MALE|0| 男性|
|FEMALE|1| 女性|


### SingleAgeGender

*结构体描述*

单人脸年龄性别信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|int |age| 年龄|
|float |ageScore| 年龄可信度|
|ArcternGender |gender| 性别|
|float |genderScore| 性别可信度|
## AgeGenderResult

*类描述*

年龄性别结果返回类，存储算法的运行结果


## AgeGenderResult::AgeGenderResult

*方法*
```c++
AgeGenderResult()
```
*功能描述*

构造函数


## AgeGenderResult::~AgeGenderResult

*方法*
```c++
~AgeGenderResult() override
```
*功能描述*

析构函数


## AgeGenderResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## AgeGenderResult::resize

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


## AgeGenderResult::ageGenders_

*说明*

结果容器

## AgeGender

*类描述*

年龄性别算法类


## AgeGender::AgeGender

*方法*
```c++
AgeGender()
```
*功能描述*

构造函数


## AgeGender::~AgeGender

*方法*
```c++
~AgeGender() override
```
*功能描述*

析构函数


## AgeGender::Init

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



## AgeGender::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## AgeGender::Process

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



## AgeGender::SetParameter

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



## AgeGender::GetParameter

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



## AgeGender::*impl_

*说明*

算法实现类

