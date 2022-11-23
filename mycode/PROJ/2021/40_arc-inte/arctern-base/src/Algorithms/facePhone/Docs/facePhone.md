## FacePhoneRunParameter

*类描述*

打手机运行参数类


## FacePhoneRunParameter::FacePhoneRunParameter

*方法*
```c++
FacePhoneRunParameter()
```
*功能描述*

构造函数


## FacePhoneRunParameter::~FacePhoneRunParameter

*方法*
```c++
~FacePhoneRunParameter() override
```
*功能描述*

析构函数




### PhoneStatus
*枚举描述*

 打手机状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOPhone|0| 没有在打手机|
|Phone|1| 在打手机|


### SingleFacePhone

*结构体描述*

单人脸打手机信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|PhoneStatus |phoneStatus|  打手机状态|
|float |statusScore|  打手机状态可信度得分|
## FacePhoneResult

*类描述*

打手机结果返回类，存储算法的运行结果


## FacePhoneResult::FacePhoneResult

*方法*
```c++
FacePhoneResult()
```
*功能描述*

构造函数


## FacePhoneResult::~FacePhoneResult

*方法*
```c++
~FacePhoneResult() override
```
*功能描述*

析构函数


## FacePhoneResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FacePhoneResult::resize

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


## FacePhoneResult::facePhones_

*说明*

结果容器

## FacePhone

*类描述*

打手机算法类


## FacePhone::FacePhone

*方法*
```c++
FacePhone()
```
*功能描述*

构造函数


## FacePhone::~FacePhone

*方法*
```c++
~FacePhone() override
```
*功能描述*

析构函数


## FacePhone::Init

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



## FacePhone::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FacePhone::Process

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



## FacePhone::SetParameter

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



## FacePhoneRunParameter

*类描述*

打手机运行参数类


## FacePhoneRunParameter::FacePhoneRunParameter

*方法*
```c++
FacePhoneRunParameter()
```
*功能描述*

构造函数


## FacePhoneRunParameter::~FacePhoneRunParameter

*方法*
```c++
~FacePhoneRunParameter() override
```
*功能描述*

析构函数




### PhoneStatus
*枚举描述*

 打手机状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NOPhone|0| 没有在打手机|
|Phone|1| 在打手机|


### SingleFacePhone

*结构体描述*

单人脸打手机信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|PhoneStatus |phoneStatus|  打手机状态|
|float |statusScore|  打手机状态可信度得分|
## FacePhoneResult

*类描述*

打手机结果返回类，存储算法的运行结果


## FacePhoneResult::FacePhoneResult

*方法*
```c++
FacePhoneResult()
```
*功能描述*

构造函数


## FacePhoneResult::~FacePhoneResult

*方法*
```c++
~FacePhoneResult() override
```
*功能描述*

析构函数


## FacePhoneResult::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源，内部调用此函数释放资源，上层可以不关注此函数


## FacePhoneResult::resize

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


## FacePhoneResult::facePhones_

*说明*

结果容器

## FacePhone

*类描述*

打手机算法类


## FacePhone::FacePhone

*方法*
```c++
FacePhone()
```
*功能描述*

构造函数


## FacePhone::~FacePhone

*方法*
```c++
~FacePhone() override
```
*功能描述*

析构函数


## FacePhone::Init

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



## FacePhone::Release

*方法*
```c++
void Release() override
```
*功能描述*

释放资源


## FacePhone::Process

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



## FacePhone::SetParameter

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



## FacePhone::GetParameter

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



## FacePhone::*impl_

*说明*

算法实现类

