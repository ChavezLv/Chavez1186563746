## FaceBangsRunParameter

*类描述*

刘海运行参数类


## FaceBangsRunParameter::FaceBangsRunParameter

*方法*
```c++
FaceBangsRunParameter()
```
*功能描述*

构造函数


## FaceBangsRunParameter::~FaceBangsRunParameter

*方法*
```c++
~FaceBangsRunParameter()
```
*功能描述*

析构函数




### BangsStatus
*枚举描述*

人脸刘海状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOBANGS|0| 无刘海|
|BANGS|1| 有刘海|


### SingleFaceBangs

*结构体描述*

单人脸刘海状态信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|BangsStatus |status| 状态|
|float |statusScore|  状态可信度得分|
## FaceBangsResult

*类描述*

刘海结果返回类，存储算法的运行结果


## FaceBangsResult::FaceBangsResult

*方法*
```c++
FaceBangsResult()
```
*功能描述*

构造函数


## FaceBangsResult::~FaceBangsResult

*方法*
```c++
~FaceBangsResult() override
```
*功能描述*

析构函数


## FaceBangsResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FaceBangsResult::resize

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


## FaceBangsResult::faceBangss_

*说明*

结果容器

## FaceBangs

*类描述*

刘海算法类


## FaceBangs::FaceBangs

*方法*
```c++
FaceBangs()
```
*功能描述*

构造函数


## FaceBangs::~FaceBangs

*方法*
```c++
~FaceBangs() override
```
*功能描述*

析构函数


## FaceBangs::Init

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



## FaceBangs::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FaceBangs::Process

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



## FaceBangs::SetParameter

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



## FaceBangs::GetParameter

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



## FaceBangs::*impl_

*说明*

算法实现类

