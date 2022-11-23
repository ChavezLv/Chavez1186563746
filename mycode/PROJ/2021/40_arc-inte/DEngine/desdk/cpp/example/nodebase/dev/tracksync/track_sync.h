#ifndef TRACK_SYNC_H_
#define TRACK_SYNC_H_
#include <core/de_ndarray.h>
#include <core/de_thread.h>
#include <de_arctern_node.h>
#include <node/de_video_decoder_node.h>
#include <de_cv_api.h>

namespace de
{
namespace arctern
{
const int kTrackAlgKCF = 0;
const int kTrackAlgSLK = 1;
class TrackSync : public de::Thread
{
public:
    TrackSync();
    ~TrackSync();
    void UpdatePara(void) override;
    void Proc(void* rx_task, int32_t task_type, POutType pout_type) override;
    void Stop(void) override;

    int32_t face_track_enable_ = true;
    int32_t human_track_enable_ = true;
    int32_t car_track_enable_ = true;
    int32_t bike_track_enable_ = true;
    int32_t license_track_enable_ = true;
	int32_t face_use_combine_model_ = true;
	
	int32_t det_bypass_enable_ = false;
	
    int32_t face_track_max_num_ = 40;
    int32_t human_track_max_num_ = 40;
    int32_t car_track_max_num_ = 20;
    int32_t bike_track_max_num_ = 20;

	
    int32_t face_qual_batch_num_ = 40;
    int32_t human_qual_batch_num_ = 40;
    int32_t car_qual_batch_num_ = 20;
    int32_t bike_qual_batch_num_ = 20;
    int32_t license_qual_batch_num_ = 20;
	
    int32_t sync_period_frm_ = 4; ///< 同步帧数
    int32_t track_period_frm_ = 1; ///< 跟踪周期
    int32_t interplot_track_enable_ = true; ///< 如果非连续跟踪（跟踪周期不是1），那么使能后支持插值上报未跟踪帧的框信息
    int32_t luma_calc_enable_ = true; ///< 是否计算亮度，不计算亮度上报为0
    int32_t tracker_alg_ = kTrackAlgKCF; ///< track 算法选择
	static int32_t debug_level_;
private:
    static constexpr int32_t kFrmMaxSize = 128;
    void* tracker_ = nullptr;

	int64_t last_pts_ = -1; ///< pts of frame 0	
	int32_t last_frm_id_ = 0;
	int32_t recvd_face_det_frm_ = -1;///<最近接收到的face检测结果
	int32_t recvd_multi_det_frm_ = -1;///<最近接收到的multi检测结果
	int32_t img_width_ = 1920;///< 视频帧宽
	int32_t img_height_ = 1080;///< 视频帧高
	struct Syncer;
	Syncer* face_syncer_ = nullptr;
	Syncer* human_syncer_ = nullptr;
	Syncer* car_syncer_ = nullptr;
	Syncer* bike_syncer_ = nullptr;
	
    struct Frame;
    std::vector<Frame> db {kFrmMaxSize};  //使用128个buffer循环使用，下标对应frame_id 
    void Proc(de::arctern::DetRslt* face_det_result);
    void Proc(de::arctern::MultiDetRslt* multi_det_result);
    void Proc(de::MediaTask* frame);

#ifdef __CSKY__
	void UpdatePts(int64_t pts);
#endif
    int32_t Pts2Frm(int64_t pts);
    void DetectedRsltUpdate(int32_t frm_id,de::arctern::DetRslt* face_rslt);	
    void DetectedRsltUpdate(int32_t frm_id,de::arctern::MultiDetRslt* multi_det_result);
	void VideoFrameUpdate(int32_t frm_id,de::MediaTask* video_frm);
	void UpdateTrackFlag(Frame& frm);

    bool CheckTrack(Frame& frm_id);
    bool CheckTrack(int32_t frm_id);
    bool CheckSync(Frame& frm_id);
    bool CheckSync(int32_t frm_id);
    bool CheckRsltRpt(Frame& frm);
    inline bool CheckDetectDone(Frame& frm);
    void Track(Frame& frm);
    void TrackInit(Frame& frm);
    void Sync(Frame& frm);
    void ResultRpt(Frame& frm);
    void Run(int32_t frm_id);
	void SetProfileInfo(Frame& frm);
	void UpdateFrameFormat(Frame& frm);
	void CalcLumas(Frame& frm);
};
};
}; // namespace


#endif
