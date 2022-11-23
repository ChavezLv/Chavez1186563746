

### DeviceType
*枚举描述*

设备类型

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|DT_CPU|0| CPU|
|DT_GPU|1| GPU|
|DT_DSP|2| DSP|
|DT_IVE|3| hist IVE|


### TrackerState
*枚举描述*

跟踪状态

|枚举名      |    枚举值 | 描述  |
| :-------- | --------:| :--: |
|NEW|0| 开始跟踪|
|TRACKED|1| 跟踪中|
|LOST|2| 跟踪丢失|
|REMOVED|3| todo|
|REPLACED|4| todo|


### TrackerInfo

*结构体描述*

追踪信息

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|cv::Rect |rect| 当前位置|
|std::vector<cv::Rect> |rect_history| 历史位置|
|int |id| 物体ID,每一个不同的物体具有不同的ID|
|bool |from_det| 是否是在当前帧被检测到|
|TrackerState |state| 跟踪状态|
|int |frame_index| 帧编号|


### TrackResult

*结构体描述*

追踪结果

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|int |tracked_count| 追踪过，但是在当前帧消失的物体数量|
|std::vector<TrackerInfo> |tracked_infos| 追踪过，但是在当前帧消失的物体信息|
|std::vector<TrackerInfo> |tracking_infos| 正在追踪的物体信息|


### ObjectTrackerParameter

*结构体描述*

物体跟踪参数配置

| 类型      |    变量 | 描述  |
| :-------- | --------:| :--: |
|int detect_period = |5| 检测间隔，为0表示每帧都检测，为5表示每5帧检测一次|
|int detect_id = |0| 开始检测的帧号|
|double min_ap_dist = |0.4| 相似度阈值（下确界）|
|float min_approved_overlap_ratio = |0.0| min overlap ratio between tracked rect and kalman predicted rect.|
|int max_time_lost = |0| 跟踪丢失最长时间阈值，超过此时间，则认为跟踪结束|
|bool use_tracking = |true| 在检测帧也运行追踪|
|std::string single_tracker_type = |"SLK"| 跟踪类型，包括（"KCF"，"SLK"）, KCF类型仅仅支持海思3516设备|
|bool use_refind = |true| 在追踪时，重新查找已经消失的物体|
|float gating_thres = |200000| distance threshold between matched rect and predicted rect|
|int choose_size = |48| todo|
|bool accu_tracked_result = |true| always return tracked_map_ in an accumulated way, if set to false, only returns thos objects no longer tracked at last detection frame|
|int channel_idx = |0| todo|
|DeviceType |device_type| todo|
|std::string |dsp_bin_root| todo|
|unsigned int ive_input_img_width = |1280| size of input pic of ive, smaller size, faster speed|
|unsigned int ive_input_img_height = |720| the width and height of the ive input pic must be multiple of 8|
|float bbox_resize_scale = |0.7| the center area of bbox, when tracking face, the area may be bigger, setting this to 0.7 ,when tracking body, the area may be smaller, setting this to 0.3|
|float OF_calc_threshold = |0.3| larger than which, the OF result will be treated as wrong|
|unsigned int OF_point_num_per_row = |5| row num of OF pts grid|
|unsigned int OF_point_num_per_col = |5| col num of OF pts grid|
|short max_level = |2| layers num of lk pic pyramid|
|int min_obj_size = |32| todo|
|float min_iou_threshold = |0.65| iou threshold, the greater this value is, the easier that two rects to match|
## IDetectRecogSink

*类描述*

interface of callback


## IDetectRecogSink::~IDetectRecogSink

*方法*
```c++
virtual ~IDetectRecogSink() = default
```
*功能描述*

析构函数


## IDetectRecogSink::OnDetect

*方法*
```c++
virtual std::vector<cv::Rect> OnDetect(cv::Mat &frameColor) = 0
```
*功能描述*

人脸检测回调函数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|frameColor|cv::Mat&|BGR格式视频桢|

*返回值* 

人脸框 



## IDetectRecogSink::OnCalcFeatures

*方法*
```c++
virtual std::vector<std::vector<uint8>> OnCalcFeatures(cv::Mat &frameColor, std::vector<cv::Rect> rects) = 0
```
*功能描述*

人脸特征提取回调


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|frameColor|cv::Mat&|BGR格式视频桢|
|rects|std::vector<cv::Rect>|人脸框|

*返回值* 

特征值 



## FaceTracker

*类描述*

人脸跟踪算法类


## FaceTracker::FaceTracker

*方法*
```c++
explicit FaceTracker(const ObjectTrackerParameter &tracker_params)
```
*功能描述*

构造函数


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|tracker_params|const ObjectTrackerParameter&|跟踪参数|


## FaceTracker::~FaceTracker

*方法*
```c++
~FaceTracker()
```
*功能描述*

析构函数


## FaceTracker::initDetectRecogSink

*方法*
```c++
void initDetectRecogSink(IDetectRecogSink *sink)
```
*功能描述*

注册回调函数对象


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|sink|IDetectRecogSink*|回调对象指针|


## FaceTracker::trackFace

*方法*
```c++
TrackResult trackFace(arctern::ArcternImage image)
```
*功能描述*

跟踪人脸


*参数说明*

| 参数      |    类型 | 描述  |
| :-------- | --------:| :--: |
|image|arctern::ArcternImage|图像桢|

*返回值* 

跟踪结果 



## FaceTracker::*impl_

*说明*

算法实现类

