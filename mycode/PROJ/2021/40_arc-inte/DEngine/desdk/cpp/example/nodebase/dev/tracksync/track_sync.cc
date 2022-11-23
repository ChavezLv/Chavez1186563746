#include "track_sync.h"
#include <core/de_node.h>
#include <de_arctern_utils.h>
#include <de_cv_api.h>
namespace de
{
namespace arctern
{
#undef LOG_LEVEL
#define LOG_LEVEL (TrackSync::debug_level_)


	int32_t TrackSync::debug_level_ = WARNING;
/**@brief object syncer, sync detect & track rect
*/
	struct TrackSync::Syncer{

	struct ObjectInfo {		
		int32_t trackId;
		deRect rect;	  ///< rect to report
		deRect minRect; ///< rect to sync and track!
		int64_t updateTime;
		int64_t detectTime;
		bool aged;
		std::map<int32_t,de::PairIntRect> assoRects;//关联目标信息，PairIntRect < cat_id, rect >
	};	
	
	typedef enum {
		OBJ_DELETE = 0, //人脸删除
		OBJ_DEFAULT,	// default
		OBJ_ADDED,	// 新增人脸
		OBJ_AMENDDED	// 人脸修正
	}OBJ_STATUS;
	//private:
		uint32_t max_objects_num_;
		struct ObjectInfo;
		std::vector<ObjectInfo> sync_db_;
		int64_t start_time_ = -1;
		int32_t track_id_count_ = 0;
		int32_t cat_id_ = de::arctern::FACE_CAT_ID;
		int32_t track_period_frm_ = 1;
	//public:
		//de::Prop attr_;
		
		int32_t trkSyncAddTh_ = 48; //sync参数,48
		int32_t trkSyncAdjTh_ = 32;//sync参数,32
		int32_t trackUnDetAgeTime_ = 16;//undet time
		double trkSyncBorderAddTh_ = 0.6f;//sync参数,0.6
		double trkSyncBorderAdjTh_ = 0.5f;//sync参数,0.5
		double trkSyncOverlapAdjTh_ = 0.3f;//sync参数,0.3
		double trkSyncOverlapDelTh_ = 0.5f;//sync参数,0.5

		// rect shrink parameter
		double trkRectLeftShrinkScale_ = 0.0f;//左边界缩小比例
		double trkRectRightShrinkScale_ = 0.0f;//右边界缩小比例
		double trkRectTopShrinkScale_ = 0.0f;//上边界缩小比例
		double trkRectBottomShrinkScale_ = 0.0f;//下边界缩小比例
		
		
#if 1	
	/* detect ==> track sync thresh:
	 *	step1. 人脸修正门限:
	 *		  TRACK_SYNC_AMEND_BORDER_RATE_THRES: 边界比率判断门限,越小越有优势
	 *		  TRACK_SYNC_AMEND_SIZE_THRES: 最小尺寸判断, 不能低于该尺寸
	 *		  TRACk_SYNC_AMEND_QUAL_THRES: 质量门限,越小越有优势
	 *		  TRACK_SYNC_AMEND_OVERLAP_RATE_THRES: 交叠区域比率,越大越有优势
	 *	step2. 人脸新增门限:
	  * 	   TRACK_SYNC_ADD_BORDER_RATE_THRES: 边界比率判断门限,越小越有优势
	 *		  TRACK_SYNC_ADD_SIZE_THRES: 最小尺寸判断, 不能低于该尺寸
	 *		  TRACk_SYNC_ADD_QUAL_THRES: 质量门限,越小越有优势
	 *		  TRACK_SYNC_ADD_OVERLAP_RATE_THRES: 交叠区域比率,越大越有优势
	 *	step3. 人脸移出门限:
	 *		   TRACK_SYNC_REMOVE_OVERLAP_DEDUP_THRES: 交叠面积区域比率, 大于门限需要移出
	  */
	Syncer(){};

	Syncer(uint32_t max_objects_num,int32_t cat_id,int32_t track_period_frm) 
		: max_objects_num_(max_objects_num),cat_id_(cat_id),track_period_frm_(track_period_frm){

	};

	void UpdatePara(de::Prop& attr){		
		double undet_period = 2.0f;
		int sync_period = 4;
		attr.Get("sync.period.frm",sync_period);
		
		switch (cat_id_)
		{
			case de::arctern::FACE_CAT_ID:
			{
				attr.Get("face.add.size.thres", trkSyncAddTh_);
				attr.Get("face.amend.size.thres",trkSyncAdjTh_);
				attr.Get("face.add.boder.rate.thres",trkSyncBorderAddTh_);
				attr.Get("face.amend.boder.rate.thres",trkSyncBorderAdjTh_);
				attr.Get("face.amend.overlap.rate.thres",trkSyncOverlapAdjTh_);
				attr.Get("face.remove.overlap.dedup.thres",trkSyncOverlapDelTh_);
				attr.Get("face.undet.aged.period",undet_period);
				trackUnDetAgeTime_ = undet_period*sync_period;
				break;
			}
			case de::arctern::HUMAN_CAT_ID:
			{				
				attr.Get("human.add.size.thres", trkSyncAddTh_);
				attr.Get("human.amend.size.thres",trkSyncAdjTh_);
				attr.Get("human.add.boder.rate.thres",trkSyncBorderAddTh_);
				attr.Get("human.amend.boder.rate.thres",trkSyncBorderAdjTh_);
				attr.Get("human.amend.overlap.rate.thres",trkSyncOverlapAdjTh_);
				attr.Get("human.remove.overlap.dedup.thres",trkSyncOverlapDelTh_);
				attr.Get("human.undet.aged.period",undet_period);
				trackUnDetAgeTime_ = undet_period*sync_period;

				attr.Get("human.rect.shrink.left",trkRectLeftShrinkScale_);
				attr.Get("human.rect.shrink.right",trkRectRightShrinkScale_);
				attr.Get("human.rect.shrink.top",trkRectTopShrinkScale_);
				attr.Get("human.rect.shrink.bottom",trkRectBottomShrinkScale_);
				CHECK(trkRectLeftShrinkScale_ + trkRectRightShrinkScale_ < 1.0f 
					&& trkRectTopShrinkScale_ + trkRectBottomShrinkScale_ < 1.0f)
					<<"wrong rect shrink scale, left:"<<trkRectLeftShrinkScale_
					<<",right:"<<trkRectRightShrinkScale_<<",top:"<<trkRectTopShrinkScale_<<",bottom:"<<trkRectBottomShrinkScale_;
				break;
			}
			case de::arctern::CAR_CAT_ID:
			{
				attr.Get("car.add.size.thres", trkSyncAddTh_);
				attr.Get("car.amend.size.thres",trkSyncAdjTh_);
				attr.Get("car.add.boder.rate.thres",trkSyncBorderAddTh_);
				attr.Get("car.amend.boder.rate.thres",trkSyncBorderAdjTh_);
				attr.Get("car.amend.overlap.rate.thres",trkSyncOverlapAdjTh_);
				attr.Get("car.remove.overlap.dedup.thres",trkSyncOverlapDelTh_);
				attr.Get("car.undet.aged.period",undet_period);
				trackUnDetAgeTime_ = undet_period*sync_period;
			
				break;
			}
			case de::arctern::BIKE_CAT_ID:
			{				
				attr.Get("bike.add.size.thres", trkSyncAddTh_);
				attr.Get("bike.amend.size.thres",trkSyncAdjTh_);
				attr.Get("bike.add.boder.rate.thres",trkSyncBorderAddTh_);
				attr.Get("bike.amend.boder.rate.thres",trkSyncBorderAdjTh_);
				attr.Get("bike.amend.overlap.rate.thres",trkSyncOverlapAdjTh_);
				attr.Get("bike.remove.overlap.dedup.thres",trkSyncOverlapDelTh_);
				attr.Get("bike.undet.aged.period",undet_period);
				trackUnDetAgeTime_ = undet_period*sync_period;
				break;
			}
			case de::arctern::LICENSE_CAT_ID:
			{			
				attr.Get("license.add.size.thres", trkSyncAddTh_);
				attr.Get("license.amend.size.thres",trkSyncAdjTh_);
				attr.Get("license.add.boder.rate.thres",trkSyncBorderAddTh_);
				attr.Get("license.amend.boder.rate.thres",trkSyncBorderAdjTh_);
				attr.Get("license.amend.overlap.rate.thres",trkSyncOverlapAdjTh_);
				attr.Get("license.remove.overlap.dedup.thres",trkSyncOverlapDelTh_);
				attr.Get("license.undet.aged.period",undet_period);
				trackUnDetAgeTime_ = undet_period*sync_period;
				break;
			}				
		}
	};

	~Syncer(){

	};
	/*@brief 更新KCF Track出来的rects到sync db 
	*/
	std::vector<de::PairIntRect> UpdateTrackBoxes(std::vector<std::pair<bool, deRect >>& trackBoxes,uint32_t& update_size, int64_t pts, int32_t imgWidth, int32_t imgHeight, int64_t& agedFlag){		
		CHECK(trackBoxes.size() >= sync_db_.size() + update_size)<<"track result error, tracked num:"<<trackBoxes.size()<<",sync_db_ num:"<<sync_db_.size();

		//step 0: update track result
		for(uint32_t i = 0;i < sync_db_.size();i++) {
			uint32_t update_index = i + update_size;
			if(!(trackBoxes[update_index].second.x >= 0 && trackBoxes[update_index].second.x <= imgWidth && trackBoxes[update_index].second.w >= 0 && trackBoxes[update_index].second.x + trackBoxes[update_index].second.w <= imgWidth
				&& trackBoxes[update_index].second.y >= 0 && trackBoxes[update_index].second.y <= imgHeight && trackBoxes[update_index].second.h >= 0 && trackBoxes[update_index].second.y + trackBoxes[update_index].second.h <= imgHeight))
			{
				LOG(WARNING)<<"illegal track rect! cat id:"<<cat_id_<<",rect:"<<trackBoxes[update_index].second<<",top:"<<trkRectTopShrinkScale_<<",bottom:"<<trkRectBottomShrinkScale_<<",left:"<<trkRectLeftShrinkScale_<<",right:"<<trkRectRightShrinkScale_<<",img width:"<<imgWidth<<",height:"<<imgHeight<<",pts:"<< pts<<",flag:"<<trackBoxes[update_index].first;
			}
			if(trackBoxes[update_index].first) {
				sync_db_[i].minRect = trackBoxes[update_index].second;
				sync_db_[i].rect = GetOrigRect(trackBoxes[update_index].second, imgWidth, imgHeight);
			}
		}
		update_size += sync_db_.size();

		std::vector<de::PairIntRect> synced;
		agedFlag = 0;
		int32_t i = 0;
		for(auto it = sync_db_.begin();it != sync_db_.end();it++,i++) {
			synced.push_back(std::make_pair(it->trackId,it->rect));
			agedFlag |= (it->aged ? (1<<i):0);
		}
		return synced;
	};

	/**@brief get track init boxes
	*/
	std::vector<deRect> GetTrackInitBoxes(){
		std::vector<deRect> ret;
		for(auto it = sync_db_.begin(); it != sync_db_.end(); it++) {
			ret.push_back(it->minRect);
		}
		return ret;
	}


	
#ifndef MAX2
#define MAX2( a, b ) ((a)>(b) ? (a) : (b))
#endif

#ifndef MIN2
#define MIN2( a, b ) ((a)<(b) ? (a) : (b))
#endif


		/*	2个Rect距离的计算 */
#define DISTANCE_RECT(rectA, rectB)  ( \
		(rectA.x*2+rectA.w-rectB.x*2-rectB.w)  * (rectA.x*2+rectA.w-rectB.x*2-rectB.w)	\
			+ (rectA.y*2+rectA.h-rectB.y*2-rectB.h)  * (rectA.y*2+rectA.h-rectB.y*2-rectB.h)  )
		
		/* Rect面积的计算 */
#define AREA_RECT(rect) (rect.w*rect.h)
		
		/* Rect边界率的计算 */
#define BORDER_RATE_RECT(rect) (  \
			(	(rect.h>rect.w)?rect.h-rect.w:rect.w-rect.h)/   \
			MAX2(rect.h, rect.w))
		
		/* Rect交叠的比率计算 */
#define OVERLAP_RATE_CALC_AMEND(distance, areaA, areaB)    ((float) (areaA+areaB-distance)/(areaA+areaB+distance))
		
		/* 2个RECT OVERLAP 计算 */
#define OVERLAP_AREA_CALC(rectA, rectB)  (\
				MAX2((MIN2(rectB.x+rectB.w + 1, rectA.x+rectA.w + 1)-MAX2(rectB.x, rectA.x) + 1), 0)* \
				MAX2((MIN2(rectB.y+rectB.h + 1, rectA.y+rectA.h + 1)-MAX2(rectB.y, rectA.y) + 1), 0) )
		
		/* Rect交叠区域比率计算 */
#define OVERLAP_AREA_RATE_CALC(overlap, areaA, areaB)	((float)overlap/MIN2(areaA,areaB))

	/**@brief 交叠比率计算，公共安全算法
	 * 
	 */
	float OverlapMinRect(const deRect& ra,const deRect& rb){
		deRect ra_min = {
			(ra.w < ra.h ? ra.x : ra.x + (ra.w - ra.h)/2),
			(ra.w < ra.h ? ra.y + (ra.h - ra.w)/2 : ra.y),
			(ra.w < ra.h ? ra.w : ra.h),
			(ra.w < ra.h ? ra.w : ra.h)
		};

		deRect rb_min = {
			
			(rb.w < rb.h ? rb.x : rb.x + (rb.w - rb.h)/2),
			(rb.w < rb.h ? rb.y + (rb.h - rb.w)/2 : rb.y),
			(rb.w < rb.h ? rb.w : rb.h),
			(rb.w < rb.h ? rb.w : rb.h)
		};

		int ra_min_ltx = ra_min.x;
		int ra_min_lty = ra_min.y;
		int ra_min_rbx = ra_min.x + ra_min.w;
		int ra_min_rby = ra_min.y + ra_min.h;

		int rb_min_ltx = rb_min.x;
		int rb_min_lty = rb_min.y;
		int rb_min_rbx = rb_min.x + rb_min.w;
		int rb_min_rby = rb_min.y + rb_min.h;

		float overlap = 0.0f;
		
		int min_ltx = ra_min_ltx > rb_min_ltx ? ra_min_ltx : rb_min_ltx;
		int min_lty = ra_min_lty > rb_min_lty ? ra_min_lty : rb_min_lty;
		int min_rbx = ra_min_rbx > rb_min_rbx ? rb_min_rbx : ra_min_rbx;
		int min_rby = ra_min_rby > rb_min_rby ? rb_min_rby : ra_min_rby;

		if(min_ltx < min_rbx && min_lty < min_rby) {
			overlap = (min_rbx - min_ltx)*(min_rby - min_lty)*1.0f / (ra_min.w < rb_min.w ? ra_min.w*ra_min.w : rb_min.w*rb_min.w) ;			
		}

		LOG(INFO)<<" ra_min "<<ra_min<<", rb_min "<<rb_min<<", overlap: "<<overlap;
		return overlap;
	}

	/**@brief calc min rect from orgin rect
	*/
	deRect GetMinRect(const deRect& rect) {
		deRect ret;
		ret.x = rect.x + rect.w*trkRectLeftShrinkScale_;
		ret.y = rect.y + rect.h*trkRectTopShrinkScale_;
		ret.w = rect.w - rect.w*(trkRectLeftShrinkScale_ + trkRectRightShrinkScale_);
		ret.h = rect.h - rect.h*(trkRectTopShrinkScale_ + trkRectBottomShrinkScale_);
		return ret;
	}
	/**@brief calc orig rect from min rect
	*/
	deRect GetOrigRect(const deRect& minRect,const int32_t imgWidth,const int32_t imgHeight) {
		deRect ret;
		ret.w = minRect.w/(1.0f - trkRectLeftShrinkScale_ - trkRectRightShrinkScale_);
		ret.h = minRect.h/(1.0f - trkRectTopShrinkScale_ - trkRectBottomShrinkScale_);
		ret.x = (minRect.x - ret.w*trkRectLeftShrinkScale_ < 0 ? 0 : minRect.x - ret.w*trkRectLeftShrinkScale_);
		ret.y = (minRect.y - ret.h*trkRectTopShrinkScale_ < 0 ? 0 : minRect.y - ret.h*trkRectTopShrinkScale_);
		ret.w = (ret.x + ret.w >= imgWidth) ? (imgWidth - ret.x) : ret.w;
		ret.h = (ret.y + ret.h >= imgHeight) ? (imgHeight - ret.y) : ret.h;
		CHECK(ret.x >= 0 && ret.x <= imgWidth && ret.w >= 0 && ret.x + ret.w <= imgWidth
			&& ret.y >= 0 && ret.y <= imgHeight && ret.h >= 0 && ret.y + ret.h <= imgHeight)<<"illegal rect! cat id:"<<cat_id_<<",rect:"<<ret<<",min:"<<minRect<<",top:"<<trkRectTopShrinkScale_<<",bottom:"<<trkRectBottomShrinkScale_<<",left:"<<trkRectLeftShrinkScale_<<",right:"<<trkRectRightShrinkScale_<<",img width:"<<imgWidth<<",height:"<<imgHeight	;
		return ret;
	}

	/**@brief 同步检测框
	*/
	std::vector<de::PairIntRect> Sync(std::vector<deRect>& detectBoxes,std::vector<int32_t>& objectMissed,int64_t pts, int64_t& agedFlag){
		int32_t trkSyncAddTh = trkSyncAddTh_; //sync参数,48
		int32_t trkSyncAdjTh = trkSyncAdjTh_;//sync参数,32
		int32_t trackUnDetAgeTime = trackUnDetAgeTime_;//undet time
		double trkSyncBorderAddTh = trkSyncBorderAddTh_;//sync参数,0.6
		double trkSyncBorderAdjTh = trkSyncBorderAdjTh_;//sync参数,0.5
		double trkSyncOverlapAdjTh = trkSyncOverlapAdjTh_;//sync参数,0.3
		double trkSyncOverlapDelTh = trkSyncOverlapDelTh_;//sync参数,0.5

		
			
		uint32_t detObjNum = detectBoxes.size();
		OBJ_STATUS* trk_st = new OBJ_STATUS[max_objects_num_];
		OBJ_STATUS *det_st = new OBJ_STATUS[detObjNum + 1];
		//std::vector<uint32_t> objectMissed;
#if 0
		LOG(INFO)<<"detect boxes:";
		for(auto it = detectBoxes.begin();it != detectBoxes.end();it++) {
			LOG(INFO)<<"rect:"<<*it;
		}
		LOG(INFO)<<"track boxes:";
		for(auto it = sync_db_.begin();it != sync_db_.end();it++) {
			LOG(INFO)<<"trackId:"<<it->trackId<<",rect:"<<it->rect;
		}
#endif

		// step 1: check and update whether the faces are same or not
		for(uint32_t i = 0; i < detObjNum; i++ ) {
			det_st[i] = OBJ_DEFAULT;
		}
		for(uint32_t i = 0; i < sync_db_.size(); i++){
			trk_st[i] = OBJ_DEFAULT;
			sync_db_[i].aged = false;
			int detect_min_i = -1, detect_min_j = -1;
			deRect detect_min_face,  b;
			int distance_min = 0x7fffFFFF;

			for(uint32_t j = 0; j < detObjNum; j++){
				deRect dectMinRect =  GetMinRect(detectBoxes[j]);
				int distance = DISTANCE_RECT(sync_db_[i].minRect,dectMinRect);
				detect_min_i++;
				if( distance < distance_min) {
					detect_min_j = detect_min_i;
					detect_min_face = dectMinRect;
					distance_min = distance;
				}
			}

			// calculate & compare
			if(detect_min_j != -1) {
				b = GetMinRect(detectBoxes[detect_min_j]);
				int a_area = sync_db_[i].minRect.w * sync_db_[i].minRect.h;
				int b_area = b.w * b.h;
				short size_b = MIN2(b.w, b.h);
				if((BORDER_RATE_RECT(b) < trkSyncBorderAdjTh)
					&&(size_b >trkSyncAdjTh)
					&&(OVERLAP_RATE_CALC_AMEND(distance_min,a_area,b_area) > trkSyncOverlapAdjTh))
//				&&(OverlapMinRect(detectBoxes[detect_min_j],sync_db_[i].rect) > trkSyncOverlapAdjTh))

				{
					//detectList[detect_min_j].trackId = faceInfo[i].trackId;
					// update tracking face roi and score
					sync_db_[i].updateTime = pts;
					sync_db_[i].minRect = detect_min_face;
					sync_db_[i].rect = detectBoxes[detect_min_j];
					det_st[detect_min_j] = OBJ_AMENDDED;
					trk_st[i] = OBJ_AMENDDED;
				}
			}
		}

		// step 2: add new track face
		for(uint32_t i = 0; i < detObjNum; i++){
			if(det_st[i] != OBJ_AMENDDED){
				deRect dectMinRect =  GetMinRect(detectBoxes[i]);
				if((BORDER_RATE_RECT(dectMinRect) < trkSyncBorderAddTh)
					&& (MIN2(dectMinRect.w,dectMinRect.h)>trkSyncAddTh))
				{
					if(sync_db_.size() < max_objects_num_)
					{
						ObjectInfo obj;
						obj.trackId = track_id_count_++;
						obj.rect = detectBoxes[i];
						obj.minRect = dectMinRect;
						obj.updateTime = pts;
						obj.detectTime = pts;
						obj.aged = false;
						trk_st[sync_db_.size()] = OBJ_ADDED;						
						sync_db_.push_back(obj);
						det_st[i] = OBJ_ADDED;
						LOG(INFO)<<"Add Object at time:"<<pts<<",trackId:"<<obj.trackId;
					}
				}
				else
				{
					det_st[i] = OBJ_DELETE;
				}
			}
		}

		//step 3: remove the faces timeout by the latest detection frame and overlap face
		for(uint32_t track_i = 0; track_i < sync_db_.size(); track_i++){
			ObjectInfo a, b;
			if(sync_db_[track_i].trackId !=-1){
				a = sync_db_[track_i];
				for(uint32_t track_j = track_i+1; track_j < sync_db_.size(); track_j++){
					if((sync_db_[track_j].trackId !=-1) && (sync_db_[track_i].trackId !=-1)){
						b = sync_db_[track_j];
						int area_a = a.minRect.w * a.minRect.h;
						int area_b = b.minRect.w * b.minRect.h;
						int area_overlap = OVERLAP_AREA_CALC(a.minRect, b.minRect);// = OVERLAP_CALC(a.box[0],a.box[1],a.box[2],a.box[3],b.box[0],b.box[1],b.box[2],b.box[3]);
						if(OVERLAP_AREA_RATE_CALC(area_overlap,area_a,area_b) > trkSyncOverlapDelTh){
						//if(overlap_min_rect(a.rect,b.rect) > trkSyncOverlapDelTh){						
							// same status, remove the small one
							if(trk_st[track_i] == trk_st[track_j]){
								if(area_a >= area_b){
									objectMissed.push_back(sync_db_[track_j].trackId);
									sync_db_[track_j].trackId = -1;
								}else{									
									objectMissed.push_back(sync_db_[track_i].trackId);
									sync_db_[track_i].trackId = -1;
								}

							}
							// amended faces better than added faces, added faces better than default faces
							else if(trk_st[track_i] > trk_st[track_j]){
								objectMissed.push_back(sync_db_[track_j].trackId);
								sync_db_[track_j].trackId = -1;
							}else{
								objectMissed.push_back(sync_db_[track_i].trackId);
								sync_db_[track_i].trackId = -1;
							}
						}
					}
				}

				if(((pts - sync_db_[track_i].updateTime) >= (trackUnDetAgeTime)) 
				&& (sync_db_[track_i].trackId != -1))
				{
					objectMissed.push_back(sync_db_[track_i].trackId);
					sync_db_[track_i].trackId = -1;
				}
				else if ((sync_db_[track_i].trackId != -1) && (trk_st[track_i] == OBJ_DEFAULT)) {
					sync_db_[track_i].aged = true;
				}
				trk_st[track_i] = OBJ_DEFAULT;
			}

		}

		for(auto it = sync_db_.begin();it != sync_db_.end();){
			if(it->trackId == -1) {
				it = sync_db_.erase(it);
			}
			else {
				it++;
			}
		}

		for(auto it = objectMissed.begin(); it != objectMissed.end();it ++) {
			LOG(INFO)<<" Delete object trackId = "<<(*it);
		}

		if (det_st) delete []det_st;
		if (trk_st) delete []trk_st;

		std::vector<de::PairIntRect> syncd;
		agedFlag = 0;
		int32_t i = 0;
		for(auto it = sync_db_.begin();it != sync_db_.end();it++,i++) {
			syncd.push_back(std::make_pair(it->trackId,it->rect));
			agedFlag |= (it->aged?(1<<i):0);
		}
		return syncd;

	};
	
	std::vector<de::PairIntRect> Interpolate(std::vector<de::PairIntRect>& boxesA,int64_t timeA,std::vector<de::PairIntRect>& boxesB,int64_t timeB,int64_t timeNow){
		//return std::vector<de::PairIntRect>();
		
		/*	有上一次跟踪结果，那么需要插值，每帧都上报结果
		 * 假设第N帧： 跟踪人脸集合为A,B,C， 丢失人脸集合为D；
		 * 第N+3帧：跟踪人脸集合为B,C,E，丢失人脸集合为A；
		 * 那么第N+1帧和第N+2帧人脸上报策略如下：
		 * 1. 对于丢失人脸A：认为第N+3帧时刻人脸A丢失，第N+1帧和第N+2帧的位置为第N帧的位置；
		 * 2. 对于新增人脸E：认为第N+3帧时刻人脸才出现；
		 * 3. 对于连续跟踪的人脸B,C：第N+1帧和第N+2帧的位置为第N帧和第N+3帧的插值位置；
		 */
		constexpr int kScale = 128;
		int trkPeriodFrm = track_period_frm_;
		CHECK(trkPeriodFrm > 1 && (timeA + trkPeriodFrm) == timeB && timeA < timeNow && timeNow < timeB)<<"track interpolate para error! timeA:"<<timeA<<",timeB:"<<timeB<<",timeNow:"<<timeNow<<",track priod:"<<trkPeriodFrm;

		std::vector<de::PairIntRect> interpolated;
		for(auto itA = boxesA.begin();itA != boxesA.end();itA++) {
			auto itFound = boxesB.end();
			for(auto itB = boxesB.begin();itB != boxesB.end();itB++) {
				if(itA->first == itB->first) {
					//found!
					itFound = itB;
					break;
				}
			}
			if(itFound != boxesB.end()) {
				//not miss,linear interpolate,scale is 128
				int ratio =  kScale*(timeNow - timeA)/(timeB - timeA);
				int xLT = itA->second.x + ratio*(itFound->second.x - itA->second.x)/kScale;
				int yLT = itA->second.y + ratio*(itFound->second.y - itA->second.y)/kScale;
				int xRB = itA->second.x + itA->second.w + ratio*(itFound->second.x + itFound->second.w - itA->second.x - itA->second.w)/kScale;
				int yRB = itA->second.y + itA->second.h + ratio*(itFound->second.y + itFound->second.h - itA->second.y - itA->second.h)/kScale;
				interpolated.push_back(std::make_pair(itA->first,deRect{xLT,yLT,(xRB - xLT),(yRB - yLT)}));
			}
			else {
				interpolated.push_back(*itA);
			}
		}
		return interpolated;

	};

	using AssoRectFunc = std::function<bool (deRect rect1,deRect rect2)>;

	/**@brief 判断rect1是否在rect2内部
	*/
	static bool IsInternalRect(deRect rect1,deRect rect2){
		int32_t xLT1 = rect1.x;
		int32_t yLT1 = rect1.y;
		int32_t xRB1 = rect1.x + rect1.w;
		int32_t yRB1 = rect1.y + rect1.h;
		
		int32_t xLT2 = rect2.x;
		int32_t yLT2 = rect2.y;
		int32_t xRB2 = rect2.x + rect2.w;
		int32_t yRB2 = rect2.y + rect2.h;

		return ((xLT1 >= xLT2 && xRB1 <= xRB2)&&(yLT1 >= yLT2 && yRB1 <= yRB2));

	}
	/**@brief 关联信息同步
	* 
	*/
	static std::vector<de::PairIntRect> SyncAssoRect(Syncer* who,std::vector<deRect>& detectBoxes,std::vector<int32_t>& objectMissed,int32_t catId, AssoRectFunc func = IsInternalRect){
		std::vector<ObjectInfo>& db = who->sync_db_;
		std::vector<de::PairIntRect> syncd;
		for(auto it = detectBoxes.begin();it != detectBoxes.end();it++){
			bool match = false;
			for(auto itDb = db.begin(); itDb != db.end(); itDb++) {
				if(func(*it,itDb->rect)) {
					//update
					itDb->assoRects[catId] = std::make_pair(itDb->trackId,*it);
					syncd.push_back(itDb->assoRects[catId]);
					match = true;
					break;
				}
			}
			if(!match) {
				LOG(DEBUG)<<"rect not match!rect:["<<it->x<<","<<it->y<<","<<it->w<<","<<it->h<<"] will drop!";
			}
		}

		//find missed too old ?
		
		return syncd;
	}

	static std::vector<de::PairIntRect> SyncAssoRect(std::vector<de::PairIntRect>& trackedBoxes,std::vector<deRect>& detectBoxes,AssoRectFunc func = IsInternalRect){
		std::vector<de::PairIntRect> syncd;
		for(auto it = trackedBoxes.begin();it != trackedBoxes.end();it++) {
			auto itDet = detectBoxes.end();
			for(itDet = detectBoxes.begin();itDet != detectBoxes.end();itDet++) {
				if(func(*itDet,it->second)) {
					syncd.push_back(std::make_pair(1,*itDet));
					break;
				}
			}
			if(itDet == detectBoxes.end()){
				deRect rect{0,0,0,0};
				syncd.push_back(std::make_pair(0,rect));
			}
		}
		return syncd;
	}
	
#endif 
};
/**
 * @brief 检测帧信息
 */
struct TrackSync::Frame
{
    int32_t frame_id{-1}; ///< 帧号
    int64_t pts{-1};
	int64_t dts{-1};
    bool valid_frm{false}; ///< 当前帧数据是否有效
    de::NDArray img;
	int32_t stream_id{-1};
    // 检测帧结果
    bool detected {false} ; ///< 是否为检测帧, 取决于同步周期参数设置，线程运行前设置
    std::vector<deRect> det_face_boxes;
    std::vector<deRect> det_human_boxes;
    std::vector<deRect> det_car_boxes;
    std::vector<deRect> det_bike_boxes;
	std::vector<deRect> det_license_boxes;
    bool valid_face_det{false}; ///< 如果当前帧是检测帧，那么是否收到检测结果
    bool valid_human_det{false};
    bool valid_car_det{false};
    bool valid_bike_det{false};
	bool valid_license_det{false};


    // 跟踪帧结果
    bool tracked {false}; ///< 是否为跟踪帧
    std::vector<de::PairIntRect> track_face_boxes;
    std::vector<de::PairIntRect> track_human_boxes;
    std::vector<de::PairIntRect> track_car_boxes;
    std::vector<de::PairIntRect> track_bike_boxes;
	std::vector<de::PairIntRect> track_license_boxes;
	std::vector<de::PairIntInt> face_lumas;
	std::vector<de::PairIntInt> human_lumas;
	std::vector<de::PairIntInt> car_lumas;
	std::vector<de::PairIntInt> bike_lumas;
	int64_t face_aged_flag{0};
	int64_t human_aged_flag{0};
	int64_t car_aged_flag{0};
	int64_t bike_aged_flag{0};

	std::vector<int32_t> face_missed;
	std::vector<int32_t> human_missed;
	std::vector<int32_t> car_missed;
	std::vector<int32_t> bike_missed;
	std::vector<int32_t> license_missed;
	bool track_done{false}; ///< 是否完成跟踪
	bool rpt_done{false}; ///< 是否上报
};

/**
 * @brief
 */
TrackSync::TrackSync()
{
	pin.SetTypeInfo(0, "de::arctern::DetRslt", de::TaskDeSerialize<de::arctern::DetRslt>,de::TaskDeleter<de::arctern::DetRslt>);
	pin.SetTypeInfo(1, "de::arctern::MultiDetRslt", de::TaskDeSerialize<de::arctern::MultiDetRslt>,de::TaskDeleter<de::arctern::MultiDetRslt>);
	pin.SetTypeInfo(2, "de::MediaTask", de::TaskDeSerialize<de::MediaTask>,de::TaskDeleter<de::MediaTask>);

	pouts_[0]->SetTypeInfo("de::arctern::ObjQualReq", de::TaskSerialize<de::arctern::ObjQualReq>);	//face
	pouts_[1]->SetTypeInfo("de::arctern::ObjQualReq", de::TaskSerialize<de::arctern::ObjQualReq>);	//body
	pouts_[2]->SetTypeInfo("de::arctern::ObjQualReq", de::TaskSerialize<de::arctern::ObjQualReq>); //car
	pouts_[3]->SetTypeInfo("de::arctern::ObjQualReq", de::TaskSerialize<de::arctern::ObjQualReq>);	//bike
	// pouts_[4]->SetTypeInfo("de::arctern::CarQualReq",de::TaskSerialize<de::arctern::CarQualReq>);//car with license
	
	pouts_[5]->SetTypeInfo("de::arctern::MultiTrackResult", de::TaskSerialize<de::arctern::MultiTrackResult>);	//report boxes
	pouts_[6]->SetTypeInfo("de::arctern::TrackMissedResult", de::TaskSerialize<de::arctern::TrackMissedResult>);//report misses

	pouts_[7]->SetTypeInfo("de::arctern::MultiTrackResultImg", de::TaskSerialize<de::arctern::MultiTrackResultImg>);//debug

	attr_.SetDefault("sync.period.frm", 4, 1, 128);
	attr_.SetDefault("track.period.frm",1,1,2);
	attr_.SetDefault("pts.per.frame",3600);

	attr_.SetDefault("det_bypass",det_bypass_enable_,0,1);
	attr_.SetDefault("face.track.enable",face_track_enable_,0,1);
	attr_.SetDefault("body.track.enable",human_track_enable_,0,1);
	attr_.SetDefault("car.track.enable",car_track_enable_,0,1);
	attr_.SetDefault("bike.track.enable",bike_track_enable_,0,1);
	attr_.SetDefault("license.track.enable",license_track_enable_,0,1);
	attr_.SetDefault("debug.level",WARNING);
	attr_.SetDefault("track.interpolate.enable",interplot_track_enable_,0,1);
	attr_.SetDefault("luma.calc.enable",luma_calc_enable_,0,1);
	attr_.SetDefault("track.alg",tracker_alg_,kTrackAlgKCF,kTrackAlgSLK);
	attr_.SetDefault("face.use.alg.combine",face_use_combine_model_,0,1);
	//sync parameter
	
	attr_.SetDefault("face.amend.boder.rate.thres", 0.3f);
	attr_.SetDefault("face.amend.size.thres",0);
	attr_.SetDefault("face.amend.overlap.rate.thres",0.4f);
	
	attr_.SetDefault("face.add.boder.rate.thres",0.2f);
	attr_.SetDefault("face.add.size.thres",24);
	attr_.SetDefault("face.remove.overlap.dedup.thres",0.56f);
	attr_.SetDefault("face.undet.aged.period",2.0f);

	
	attr_.SetDefault("human.amend.boder.rate.thres", 0.4f);
	attr_.SetDefault("human.amend.size.thres",0);
	attr_.SetDefault("human.amend.overlap.rate.thres",0.4f);
	
	attr_.SetDefault("human.add.boder.rate.thres",0.2f);
	attr_.SetDefault("human.add.size.thres",32);
	attr_.SetDefault("human.remove.overlap.dedup.thres",0.85f);
	attr_.SetDefault("human.undet.aged.period",2.0f);

	attr_.SetDefault("car.amend.boder.rate.thres", 0.4f);
	attr_.SetDefault("car.amend.size.thres",0);
	attr_.SetDefault("car.amend.overlap.rate.thres",0.46f);
	
	attr_.SetDefault("car.add.boder.rate.thres",0.2f);
	attr_.SetDefault("car.add.size.thres",48);
	attr_.SetDefault("car.remove.overlap.dedup.thres",0.9f);
	attr_.SetDefault("car.undet.aged.period",2.0f);

	attr_.SetDefault("bike.amend.boder.rate.thres", 0.4f);
	attr_.SetDefault("bike.amend.size.thres",0);
	attr_.SetDefault("bike.amend.overlap.rate.thres",0.4f);
	
	attr_.SetDefault("bike.add.boder.rate.thres",0.2f);
	attr_.SetDefault("bike.add.size.thres",32);
	attr_.SetDefault("bike.remove.overlap.dedup.thres",0.9f);
	attr_.SetDefault("bike.undet.aged.period",2.0f);

	
	attr_.SetDefault("license.amend.boder.rate.thres", 0.6f);
	attr_.SetDefault("license.amend.size.thres",0);
	attr_.SetDefault("license.amend.overlap.rate.thres",0.4f);
	
	attr_.SetDefault("license.add.boder.rate.thres",0.4f);
	attr_.SetDefault("license.add.size.thres",24);
	attr_.SetDefault("license.remove.overlap.dedup.thres",0.56f);
	attr_.SetDefault("license.undet.aged.period",2.0f);

	//max track num
	attr_.SetDefault("face.track.max.num",40,1,128);
	attr_.SetDefault("human.track.max.num",40,1,128);
	attr_.SetDefault("car.track.max.num",20,1,128);
	attr_.SetDefault("bike.track.max.num",20,1,128);

	
	attr_.SetDefault("face.qual.batch.num",40,1,40);
	attr_.SetDefault("human.qual.batch.num",40,1,40);
	attr_.SetDefault("car.qual.batch.num",20,1,20);
	attr_.SetDefault("bike.qual.batch.num",20,1,20);
	attr_.SetDefault("license.qual.batch.num",20,1,20);

	attr_.SetDefault("human.rect.shrink.left",0.0f,0.0f,1.0f);
	attr_.SetDefault("human.rect.shrink.right",0.0f,0.0f,1.0f);
	attr_.SetDefault("human.rect.shrink.top",0.0f,0.0f,1.0f);
	attr_.SetDefault("human.rect.shrink.bottom",0.0f,0.0f,1.0f);
	

    name_ = "HikTrackSync";
}

/**
 * @brief
 */
TrackSync::~TrackSync()
{
	if(tracker_) {
		if(tracker_alg_ == kTrackAlgKCF){
			delete static_cast<de::cv::KCFMultiTracker*>(tracker_); 
		}
		else {		
			LOG(FATAL)<<"track alg: "<<tracker_alg_<<" NOT support!!";
		}		
	}
	
	if(face_syncer_) {
		delete face_syncer_; 
	}
	if(human_syncer_){
		delete human_syncer_ ;
	}
	if(car_syncer_){
		delete car_syncer_ ;
	}
	if(bike_syncer_) {
		delete bike_syncer_ ;
	}
}


/**
 * @brief 检测是否完成检测
 * @param frm
 * @return 
 */
bool TrackSync::CheckDetectDone(Frame& frm){
	return (
	( (face_track_enable_ && frm.valid_face_det) || (!face_track_enable_ )) &&
	( (human_track_enable_ && frm.valid_human_det) || (!human_track_enable_ )) &&
	( (car_track_enable_ && frm.valid_car_det) || (!car_track_enable_ )) &&
	( (bike_track_enable_ && frm.valid_bike_det) || (!bike_track_enable_ )) &&
	( (license_track_enable_ && frm.valid_license_det) || (!license_track_enable_))
	);

}


/**
 * @brief 检测是否需要跟踪
 *
 * @param frm
 *
 * @return
 */
bool TrackSync::CheckTrack(Frame& frm)
{
    LOG(INFO)<<"CheckTrack: fram_id:"<<frm.frame_id<<",tracked:"<<frm.tracked<<",vaild:"<<frm.valid_frm;

    // 如果当前帧不需要跟踪，那么退出
    if(!frm.tracked) return false;

    // 如果没有收到图像帧，那么等待退出
    if(!frm.valid_frm) return false;

	// 第一帧不用做
	if(frm.frame_id == 0) return false;

	//	因为等到detect结果收齐，才设置frm.tracked标志，因此肯定能做

    //std::vector<Frame> db {128};  //使用128个buffer循环使用，下标对应frame_id % 128
    return true;
}


/**
 * @brief 
 *
 * @param frm
 */
void TrackSync::TrackInit(Frame& frm)
{	
	LOG(INFO)<<"TrackInit frame_id:"<<frm.frame_id<<",faces:"<<frm.track_face_boxes.size()
		<<",bodys:"<<frm.track_human_boxes.size()<<",cars:"<<frm.track_car_boxes.size()<<",bikes:"<<frm.track_bike_boxes.size();
	std::vector<deRect> init_boxes;
	if(face_track_enable_) {
		std::vector<deRect> rects  = face_syncer_->GetTrackInitBoxes();
		init_boxes.insert(init_boxes.end(),rects.begin(),rects.end());
	}
	if(human_track_enable_){
		std::vector<deRect> rects = human_syncer_->GetTrackInitBoxes();
		init_boxes.insert(init_boxes.end(),rects.begin(),rects.end());
	}
	if(car_track_enable_){
		std::vector<deRect> rects = car_syncer_->GetTrackInitBoxes();
		init_boxes.insert(init_boxes.end(),rects.begin(),rects.end());
	}
	if(bike_track_enable_){
		std::vector<deRect> rects = bike_syncer_->GetTrackInitBoxes();
		init_boxes.insert(init_boxes.end(),rects.begin(),rects.end());
	}
	if(tracker_alg_ == kTrackAlgKCF) {
		static_cast<de::cv::KCFMultiTracker*>(tracker_)->Init(frm.img, init_boxes);
	}
	else {
		LOG(FATAL)<<"track alg: "<<tracker_alg_<<" NOT support!!";			
	}
}
/**
 * @brief 跟踪
 *
 * @param frm
 */
void TrackSync::Track(Frame& frm)
{	
	LOG(INFO)<<"Track frame_id:"<<frm.frame_id<<",faces:"<<frm.track_face_boxes.size()
		<<",bodys:"<<frm.track_human_boxes.size()<<",cars:"<<frm.track_car_boxes.size()<<",bikes:"<<frm.track_bike_boxes.size()
		<<",img size:"<<frm.img.GetTensorDataSize();
	std::vector<std::pair<bool,deRect>> update_boxes;
	if(tracker_alg_ == kTrackAlgKCF) {
		static_cast<de::cv::KCFMultiTracker*>(tracker_)->Update(frm.img,update_boxes);;
	}
	else {
		LOG(FATAL)<<"track alg: "<<tracker_alg_<<" NOT support!!";
	}
	uint32_t update_size = 0;
	if(face_track_enable_){
		frm.track_face_boxes = face_syncer_->UpdateTrackBoxes(update_boxes, update_size, frm.frame_id, img_width_, img_height_,frm.face_aged_flag);
		LOG(INFO)<<"face track done! num:"<<frm.track_face_boxes.size();
	}
	else  //lt
	{
		if (det_bypass_enable_){
			
			for (int i=0;i<frm.det_face_boxes.size();i++)
				//trackid ,derect
				frm.track_face_boxes.push_back(std::make_pair(i,frm.det_face_boxes[i]));		
		}
	}
	if(human_track_enable_){
		frm.track_human_boxes = human_syncer_->UpdateTrackBoxes(update_boxes, update_size, frm.frame_id, img_width_, img_height_,frm.human_aged_flag);	
		LOG(INFO)<<"body track done! num:"<<frm.track_human_boxes.size();
	}
	else
	{
		if (det_bypass_enable_){
			for (int i=0;i<frm.det_human_boxes.size();i++) frm.track_human_boxes.push_back(std::make_pair(i,frm.det_human_boxes[i]));		
		}
	}
	if(car_track_enable_){
		frm.track_car_boxes = car_syncer_->UpdateTrackBoxes(update_boxes, update_size, frm.frame_id, img_width_, img_height_,frm.car_aged_flag);	
		if(license_track_enable_ && !frm.detected) {
			std::vector<deRect> rects;//empty to generate all 0 result
			frm.track_license_boxes = Syncer::SyncAssoRect(frm.track_car_boxes,rects);
		}
		LOG(INFO)<<"car track done! num:"<<frm.track_car_boxes.size();
	}
	else
	{
		if (det_bypass_enable_){
			for (int i=0;i<frm.det_car_boxes.size();i++) frm.track_car_boxes.push_back(std::make_pair(i,frm.det_car_boxes[i]));		
		}
	}
	if(bike_track_enable_){
		frm.track_bike_boxes = bike_syncer_->UpdateTrackBoxes(update_boxes, update_size, frm.frame_id, img_width_, img_height_,frm.bike_aged_flag);		
		LOG(INFO)<<"bike track done! num:"<<frm.track_bike_boxes.size();
	}
	else
	{
		if (det_bypass_enable_){
			for (int i=0;i<frm.det_bike_boxes.size();i++) frm.track_bike_boxes.push_back(std::make_pair(i,frm.det_bike_boxes[i]));		
		}
	}
	CHECK(update_size == update_boxes.size())<<"update size : "<<update_size <<" != track boxes size: "<<update_boxes.size();

	frm.track_done = true;
}


/**
 * @brief 检测是否需要同步
 *
 * @param frm
 *
 * @return
 */
bool TrackSync::CheckSync(Frame& frm)
{
	//check 是检测帧并且检测结果收齐
	LOG(DEBUG)<<"CheckSync: frame_id:"<<frm.frame_id<<",detectd:"<<frm.detected
	<<",face_detected:"<<frm.valid_face_det<<",body_detected:"<<frm.valid_human_det
	<<",car_detected:"<<frm.valid_car_det<<",bike_detected:"<<frm.valid_bike_det<<",frm.valid:"<<frm.valid_frm<<",data.size: "<<frm.img.GetTensorDataSize()<<std::endl;
	if(frm.detected && CheckDetectDone(frm) && frm.valid_frm) return true;
    return false;
}

void WriteImgToDir(de::NDArray& img,const int32_t frm_id, const int32_t cat_id, const std::vector<deRect>& rects, const std::string dir_path)
{
#if USE_X86_PLATFORM
    static  std::map<int32_t, std::string> cat_name = { {de::arctern::FACE_CAT_ID, "face"},
        {de::arctern::HUMAN_CAT_ID, "human"},
        {de::arctern::CAR_CAT_ID, "car"},
        {de::arctern::BIKE_CAT_ID, "bike"}
    };
    std::string name = "detect_" + cat_name[cat_id];
    std::string file = dir_path + de::FormatString("/%s-frm%04d.jpg", name.c_str(), frm_id);
    de::PicDraw canvas(img);
    //std::string header = string_format("",);
    for(size_t i = 0; i < rects.size(); i++)
    {
        auto track_id = i;
        deRect rect = rects[i];
        auto labels = { de::FormatString("det:%d", track_id)
                      };
        canvas.AddRectText(rect, labels);
    }
    canvas.Save(file);
#endif
};

/**
 * @brief 检测结果和跟踪结果同步
 *
 * @param frm
 */
void TrackSync::Sync(Frame& frm)
{
#if 0
	WriteImgToDir(frm.img,frm.frame_id,de::arctern::HUMAN_CAT_ID,frm.det_human_boxes,"/workspace/desdkhik/output/tmp");
#endif
	if(face_track_enable_){
		frm.track_face_boxes = face_syncer_->Sync(frm.det_face_boxes, frm.face_missed, frm.frame_id,frm.face_aged_flag);
	}
	if(human_track_enable_){
		frm.track_human_boxes = human_syncer_->Sync(frm.det_human_boxes,frm.human_missed,frm.frame_id,frm.human_aged_flag);
	}
	if(car_track_enable_){
		frm.track_car_boxes = car_syncer_->Sync(frm.det_car_boxes,frm.car_missed,frm.frame_id,frm.car_aged_flag);
		if(license_track_enable_){
			frm.track_license_boxes = Syncer::SyncAssoRect(frm.track_car_boxes,frm.det_license_boxes);
		}
	}
	if(bike_track_enable_){
		frm.track_bike_boxes = bike_syncer_->Sync(frm.det_bike_boxes,frm.bike_missed,frm.frame_id,frm.bike_aged_flag);
	}
	
	if(frm.frame_id == 0) frm.track_done = true;//第一帧无需track,sync后认为track done
}


/**
 * @brief 检测帧的结果是否可以上报
 *
 * @param frm
 *
 * @return
 */
bool TrackSync::CheckRsltRpt(Frame & frm)
{
	LOG(INFO)<<"CheckRsltRpt:frame_id:"<<frm.frame_id<<",tracked:"<<frm.tracked<<",trackdone:"<<frm.track_done<<",rptdone:"<<frm.rpt_done;
	if(frm.rpt_done) return false;
	if(!frm.tracked) {
		//need interpolate!
		int32_t frameA_id = -1;
		int32_t frameB_id =  -1;
		int32_t frame_now = frm.frame_id;
		for(int i = 1;i < track_period_frm_;i++ ) {
			//find A & B
			if(frameA_id == -1 && (frame_now -i >= 0) && db[(frame_now - i)%kFrmMaxSize].track_done && db[(frame_now - i)%kFrmMaxSize].frame_id < frame_now) 
				frameA_id = frame_now - i;
			if(frameB_id == -1 && db[(frame_now + i)%kFrmMaxSize].track_done && db[(frame_now + i)%kFrmMaxSize].frame_id > frame_now)
				frameB_id = frame_now + i;

			if(frameA_id != -1 && frameB_id != -1)
				break;
		}

		if(frameA_id == -1 || frameB_id == -1 ) //not ready 
			return false;


		if(interplot_track_enable_) {
			
			Frame& frmA = db[frameA_id%kFrmMaxSize];
			Frame& frmB = db[frameB_id%kFrmMaxSize];
			if(face_track_enable_){
				frm.track_face_boxes = face_syncer_->Interpolate(frmA.track_face_boxes,frmA.frame_id,frmB.track_face_boxes,frmB.frame_id,frm.frame_id);
				frm.face_lumas = frmA.face_lumas;
			}
			if(human_track_enable_){
				frm.track_human_boxes = human_syncer_->Interpolate(frmA.track_human_boxes,frmA.frame_id,frmB.track_human_boxes,frmB.frame_id,frm.frame_id);
				frm.human_lumas = frmA.human_lumas;
			}
			if(car_track_enable_){
				frm.track_car_boxes = car_syncer_->Interpolate(frmA.track_car_boxes,frmA.frame_id,frmB.track_car_boxes,frmB.frame_id,frm.frame_id);
				frm.car_lumas = frmA.car_lumas;
			}
			if(bike_track_enable_){
				frm.track_bike_boxes = bike_syncer_->Interpolate(frmA.track_bike_boxes,frmA.frame_id,frmB.track_bike_boxes,frmB.frame_id,frm.frame_id);
				frm.bike_lumas = frmA.bike_lumas;
			}

		}

		return true;

	}
	else{
		if(frm.track_done) CalcLumas(frm);
		return frm.track_done;
	}
}

/**
* @brief set profile info
*
*/
void TrackSync::SetProfileInfo(Frame& frm){
	if(profile_enable_ && frm.tracked) {
//	if(debug_level_ == WARNING && frm.tracked) {
		static int32_t total_ms = 0;
		static int32_t max_ms = 0;
		static int32_t count = 0;
		static int32_t max_ms_handle = 0;
		static int32_t total_ms_handle = 0;
		int32_t priod_frm = profile_stat_period_/40;//25fps,40ms per frm
		if(frm.frame_id % priod_frm == 0) {			
			LOG(WARNING)<<"Profile name:"<<name_<<",frm_id:"<<frm.frame_id<<",count:"<<count<<",max_ms:"<<max_ms<<",ave_ms:"<<(count == 0? 0 : total_ms/count)<<",max_handle:"<<max_ms_handle<<",ave_handle:"<<(count == 0? 0 : total_ms_handle/count);
			total_ms = 0;
			max_ms = 0;
			count = 0;
			total_ms_handle = 0;
			max_ms_handle = 0;
		}
		count++;
		
		struct DecBareData baredata = de::GetDataFromBearArea<DecBareData>(frm.img);
		auto to_epoch = std::chrono::steady_clock::now().time_since_epoch();
		auto time_now = std::chrono::duration_cast<std::chrono::milliseconds>(to_epoch);
		auto time_consume = (time_now.count() - baredata.dts);
		if(time_consume > max_ms){
			max_ms = time_consume;		
			LOG(DEBUG)<<"Profile name:"<<name_<<",frm_id:"<<frm.frame_id<<",dts:"<<baredata.dts<<",now:"<<time_now.count()<<",max:"<<max_ms;
		}
		total_ms += time_consume;
		auto time_consume_handle = (time_now.count() - frm.dts);
		if(time_consume_handle > max_ms_handle) {
			max_ms_handle = time_consume_handle;
		}
		total_ms_handle += time_consume_handle;
		//todo:set profile.node
		frame_no = frm.frame_id;
		proc_average_time = total_ms/count;
		proc_max_time = max_ms;
	}

}
/**
* @brief 更新帧格式
*
*/
void TrackSync::UpdateFrameFormat(Frame& frm){
	int32_t fmt = 0;
	frm.img.GetImgInfo(img_height_,img_width_,fmt);
}

/**
* @brief 计算亮度值
*
*/
void TrackSync::CalcLumas(Frame & frm){
	if(luma_calc_enable_) {
		std::vector<deRect> rois;
		std::vector<uint8_t> lumas;

		//step1: get all rois
		for(auto it = frm.track_face_boxes.begin();it != frm.track_face_boxes.end();it++) {
			rois.push_back(it->second);
		}
		for(auto it = frm.track_human_boxes.begin();it != frm.track_human_boxes.end();it++) {
			rois.push_back(it->second);
		}
		for(auto it = frm.track_car_boxes.begin();it != frm.track_car_boxes.end();it++) {
			rois.push_back(it->second);
		}
		for(auto it = frm.track_bike_boxes.begin();it != frm.track_bike_boxes.end();it++) {
			rois.push_back(it->second);
		}
		if(rois.size() == 0) return;
		LOG(DEBUG)<<"frm id:"<<frm.frame_id<<",detected:"<<frm.detected<<",tracked:"<<frm.tracked<<",trackdone:"<<frm.track_done<<",rptdone:"<<frm.rpt_done
			<<",recv det face:"<<recvd_face_det_frm_<<",recv det multi:"<<recvd_multi_det_frm_<<",roi.size:"<<rois.size()
			<<",frm.img:"<<frm.img.defined()<<",tensor:"<<std::hex<<frm.img.GetTensorData()<<std::dec;
		//step2: calc luma

		de::cv::Luma(frm.img,rois,lumas);

		CHECK(rois.size() == lumas.size())<<",luma unmatch req.size:"<<rois.size()<<",luma.size:"<<lumas.size();
		//step3: save result
		auto itLuma = lumas.begin();
		for(auto it = frm.track_face_boxes.begin(); it != frm.track_face_boxes.end(); it++,itLuma++) {
			frm.face_lumas.push_back(std::make_pair(it->first,*itLuma));
		}
		for(auto it = frm.track_human_boxes.begin(); it != frm.track_human_boxes.end(); it++,itLuma++) {
			frm.human_lumas.push_back(std::make_pair(it->first,*itLuma));
		}
		for(auto it = frm.track_car_boxes.begin(); it != frm.track_car_boxes.end(); it++,itLuma++) {
			frm.car_lumas.push_back(std::make_pair(it->first,*itLuma));
		}
		for(auto it = frm.track_bike_boxes.begin(); it != frm.track_bike_boxes.end(); it++,itLuma++) {
			frm.bike_lumas.push_back(std::make_pair(it->first,*itLuma));
		}
	}
}


/**
 * @brief 帧结果上报
 *
 * @param frm_id
 */
void TrackSync::ResultRpt(Frame& frm)
{
    /// 如果非连续跟踪，那么需要插值上报框信息, interplot_track_enable_
    SetProfileInfo(frm);
    if(pouts_[0]->IsLinked() && frm.track_face_boxes.size()>0 && frm.tracked)
    {
    	auto it = frm.track_face_boxes.begin();
		auto split_num = (frm.track_face_boxes.size() + face_qual_batch_num_ - 1)/face_qual_batch_num_;
		for(auto i = 0;i < (int32_t)split_num;i++) {
			auto task = new de::arctern::ObjQualReq;			
			task->stream_id = frm.stream_id;
			task->img_id = frm.frame_id;
			task->img = frm.img;
			if(i == (int32_t)(split_num -1)) {
				task->rects.assign(it,frm.track_face_boxes.end());
			}
			else{
				task->rects.assign(it,it+face_qual_batch_num_);
				it += face_qual_batch_num_;
			}
			pouts_[0]->SendTask(task);		
		}
    }
    if(pouts_[1]->IsLinked() && frm.track_human_boxes.size()>0 && frm.tracked)
    {
        // pin1: arctern::ObjQualReq (人体)
        
    	auto it = frm.track_human_boxes.begin();
		auto split_num = (frm.track_human_boxes.size() + human_qual_batch_num_ - 1)/human_qual_batch_num_;
		for(auto i = 0;i < (int32_t)split_num;i++) {
			auto task = new de::arctern::ObjQualReq;			
			task->stream_id = frm.stream_id;
			task->img_id = frm.frame_id;
			task->img = frm.img;
			if(i == (int32_t)(split_num -1)) {
				task->rects.assign(it,frm.track_human_boxes.end());
			}
			else{
				task->rects.assign(it,it+human_qual_batch_num_);
				it += human_qual_batch_num_;
			}
			pouts_[1]->SendTask(task);		
		}		
    }
    if(pouts_[2]->IsLinked() && frm.track_car_boxes.size()>0 && frm.tracked)
    {
        // pin2: arctern::ObjQualReq (机动车) 		
    	auto it = frm.track_car_boxes.begin();
		auto split_num = (frm.track_car_boxes.size() + car_qual_batch_num_ - 1)/car_qual_batch_num_;
		for(auto i = 0;i < (int32_t)split_num;i++) {
			auto task = new de::arctern::ObjQualReq;			
			task->stream_id = frm.stream_id;
			task->img_id = frm.frame_id;
			task->img = frm.img;
			if(i == (int32_t)(split_num -1)) {
				task->rects.assign(it,frm.track_car_boxes.end());
			}
			else{
				task->rects.assign(it,it+car_qual_batch_num_);
				it += car_qual_batch_num_;
			}
			pouts_[2]->SendTask(task); 	
		}		
    }
    if(pouts_[3]->IsLinked() && frm.track_bike_boxes.size()>0 && frm.tracked)
    {
        // pin3: arctern::ObjQualReq (非机动车)       
        auto it = frm.track_bike_boxes.begin();		
		auto split_num = (frm.track_bike_boxes.size() + bike_qual_batch_num_ - 1)/bike_qual_batch_num_;
		for(auto i = 0;i < (int32_t)split_num;i++) {
			auto task = new de::arctern::ObjQualReq;			
			task->stream_id = frm.stream_id;
			task->img_id = frm.frame_id;
			task->img = frm.img;
			if(i == (int32_t)(split_num -1)) {
				task->rects.assign(it,frm.track_bike_boxes.end());
			}
			else{
				task->rects.assign(it,it+bike_qual_batch_num_);
				it += bike_qual_batch_num_;
			}
			pouts_[3]->SendTask(task); 	
		}		
    }
	
	if(pouts_[4]->IsLinked() && frm.track_car_boxes.size()>0 && frm.tracked)
	{
		// pin4: de::arctern::CarQualReq (机动车&车牌)       
        auto it = frm.track_car_boxes.begin();	
		auto itLic = frm.track_license_boxes.begin();
		CHECK(frm.track_car_boxes.size() == frm.track_license_boxes.size());
		auto split_num = (frm.track_car_boxes.size() + car_qual_batch_num_ - 1)/car_qual_batch_num_;
		for(auto i = 0;i < (int32_t)split_num;i++) {
			// auto task = new de::arctern::CarQualReq;			
			// task->stream_id = frm.stream_id;
			// task->img_id = frm.frame_id;
			// task->img = frm.img;
			// if(i == (int32_t)( split_num -1)) {
				// task->rects.assign(it,frm.track_car_boxes.end());
				// task->license_rects.assign(itLic,frm.track_license_boxes.end());
			// }
			// else{
				// task->rects.assign(it,it+car_qual_batch_num_);
				// task->license_rects.assign(itLic,itLic+car_qual_batch_num_);
				// it += car_qual_batch_num_;
				// itLic += car_qual_batch_num_;
			// }
			// pouts_[4]->SendTask(task); 	
		}		
	}
	
    if(pouts_[5]->IsLinked())
    {
        // pin4 :MultiTrackResult
        auto task = new de::arctern::MultiTrackResult;
		task->stream_id = frm.stream_id;
		task->pts = frm.pts;
		task->frm_id = frm.frame_id;
		task->obj_boxes[de::arctern::FACE_CAT_ID] = frm.track_face_boxes;
		task->obj_boxes[de::arctern::HUMAN_CAT_ID] = frm.track_human_boxes;
		task->obj_boxes[de::arctern::CAR_CAT_ID] = frm.track_car_boxes;
		task->obj_boxes[de::arctern::BIKE_CAT_ID] = frm.track_bike_boxes;
		//task->obj_boxes[de::arctern::LICENSE_CAT_ID] = frm.track_license_boxes;
		task->obj_lumas[de::arctern::FACE_CAT_ID] = frm.face_lumas;
		task->obj_lumas[de::arctern::HUMAN_CAT_ID] = frm.human_lumas;
		task->obj_lumas[de::arctern::CAR_CAT_ID] = frm.car_lumas;
		task->obj_lumas[de::arctern::BIKE_CAT_ID] = frm.bike_lumas;
		pouts_[5]->SendTask(task);
		
    }
	if(pouts_[6]->IsLinked())
	{
		if(frm.face_missed.size() ||frm.human_missed.size() || frm.car_missed.size() || frm.bike_missed.size() || frm.license_missed.size()) {
			auto task = new de::arctern::TrackMissedResult;
			task->stream_id = frm.stream_id;
			task->pts = frm.pts;
			task->frm_id = frm.frame_id;
			if(frm.face_missed.size()) {
				task->obj_missed[de::arctern::FACE_CAT_ID] = frm.face_missed;
			}
			if(frm.human_missed.size()) {
				task->obj_missed[de::arctern::HUMAN_CAT_ID] = frm.human_missed;
			}
			if(frm.car_missed.size()) {
				task->obj_missed[de::arctern::CAR_CAT_ID] = frm.car_missed;
			}
			if(frm.bike_missed.size()) {
				task->obj_missed[de::arctern::BIKE_CAT_ID] = frm.bike_missed;
			}
			//if(frm.license_missed.size()) {
			//	task->obj_missed[de::arctern::LICENSE_CAT_ID] = frm.license_missed;
			//}

			pouts_[6]->SendTask(task);
		}
	}
	if(pouts_[7]->IsLinked() && frm.tracked)
	{
		//插值帧未存图,不上报
		auto task = new de::arctern::MultiTrackResultImg;
		task->stream_id = frm.stream_id;
		task->pts = frm.pts;
		task->frm_id = frm.frame_id;
		task->img = frm.img;
		task->obj_boxes[de::arctern::FACE_CAT_ID] = frm.track_face_boxes;
		task->obj_boxes[de::arctern::HUMAN_CAT_ID] = frm.track_human_boxes;
		task->obj_boxes[de::arctern::CAR_CAT_ID] = frm.track_car_boxes;
		task->obj_boxes[de::arctern::BIKE_CAT_ID] = frm.track_bike_boxes;
		//task->obj_boxes[de::arctern::LICENSE_CAT_ID] = frm.track_license_boxes;
		
		task->obj_lumas[de::arctern::FACE_CAT_ID] = frm.face_lumas;
		task->obj_lumas[de::arctern::HUMAN_CAT_ID] = frm.human_lumas;
		task->obj_lumas[de::arctern::CAR_CAT_ID] = frm.car_lumas;
		task->obj_lumas[de::arctern::BIKE_CAT_ID] = frm.bike_lumas;
		pouts_[7]->SendTask(task);
	}
    // 删除当前上报帧
    frm.img = de::NDArray();
	frm.valid_frm = false;
	frm.rpt_done = true;
}


/**
 * @brief 跟踪触发调度
 *
 * @param frm_id
 */
void TrackSync::Run(int32_t frm_id)
{
/*
*因为检测帧结果可能在收到后续视频帧之后，所以check到下一检测帧
*/
	for(int i = 0; i < sync_period_frm_;i++) {
	    Frame& frm = db[(frm_id + i) % kFrmMaxSize];		
		if((i != 0 && frm.detected) || (frm.frame_id < frm_id)) break;//
		
	    if(CheckTrack(frm))
	    {
			static uint64_t sum=0;
			static int count=0;
			auto start = std::chrono::steady_clock::now();
	        Track(frm);
			// uint64_t cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			// sum+=cost++;
			// if (count % 30 == 0){
				// printf("track cost %llu ms\n", sum/30);
				// sum =0;
				// count=0;
			// }
	    }

	    if(CheckSync(frm))
	    {
			static uint64_t sum=0;
			static int count=0;
			auto start = std::chrono::steady_clock::now();
	        Sync(frm);
	        TrackInit(frm);
			// uint64_t cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			// sum+=cost++;
			// if (count % 30 == 0){
				// printf("sync cost %llu ms\n", sum/30);
				// sum =0;
				// count=0;
			// }
	    }
	}
	/*
	*因为track可能插值，做完track后再判断能否上报
	*/
	
	/**
	*因为插值，当前帧时跟踪帧时，需要check前一个跟踪周期能否上报
	*/
	
	if(track_period_frm_ > 1 && db[frm_id%kFrmMaxSize].track_done && frm_id != 0) {
		for(int i = track_period_frm_ - 1; i > 0; i--) {
			Frame& frm = db[(frm_id + kFrmMaxSize -i)%kFrmMaxSize];
			if(CheckRsltRpt(frm)){
				ResultRpt(frm);				
			}
		}
	}

	for(int i = 0; i < sync_period_frm_;i++) {		
		Frame& frm = db[(frm_id + i) % kFrmMaxSize];		
		if((i != 0 && frm.detected) || (frm.frame_id < frm_id) ) break;//
	    if(CheckRsltRpt(frm))
	    {
	        ResultRpt(frm);
	    }
		
	}

}
/**@brief
* update track flag
*/
void TrackSync::UpdateTrackFlag(Frame& frm){	
	int32_t frm_id = frm.frame_id;
	if(frm.detected && CheckDetectDone(frm))
	{
		for(int i = 0;i < sync_period_frm_;i++) {
			Frame& f = db[(frm_id + i)%kFrmMaxSize];
			if( ((frm_id + i) % track_period_frm_)	== 0 || f.detected) {
				f.tracked = true;
			}
			else {
				f.tracked = false;				
				f.img = de::NDArray();//插值帧无需大图
				f.valid_frm = false;
			}
		}
	}
}



/**
 * @brief 检测结果更新frame数据库
 *
 * @param frm_id
 */
void TrackSync::DetectedRsltUpdate(int32_t frm_id,de::arctern::DetRslt* face_rslt)
{
    // Check 只允许同步帧收到检测结果
    // 如果所有的检测结果收到，那么更新下一个sync周期所有的track flag, 比如sync周期为4，在第100帧收齐
    // 所有的检测结果，那么在第101帧，第101、102、103、104帧设置track flag为true
    Frame& frm = db[frm_id % kFrmMaxSize ];
	//if(!(frm.detected == true || frm_id%sync_period_frm_ == 0)){
		CHECK(frm.detected == true || frm_id%sync_period_frm_ == 0)<<"not detect frame, frame_id:"<<frm_id<<",sync period:"<<sync_period_frm_;
	//}
	CHECK(!face_use_combine_model_)<<"rcvd face result! but face use combine model is: "<<face_use_combine_model_;
	recvd_face_det_frm_ = frm_id;
	frm.det_face_boxes = face_rslt->rects;
	frm.valid_face_det = true;	
	UpdateTrackFlag(frm);
	
	LOG(INFO)<<"DetectedRsltUpdate frm_id: "<<frm_id<<",detected: "<<frm.detected<<",tracked:"<<frm.tracked<<",trackdone:"<<frm.track_done;
}
void TrackSync::DetectedRsltUpdate(int32_t frm_id,de::arctern::MultiDetRslt* multi_det_result){
	
	// Check 只允许同步帧收到检测结果
	// 如果所有的检测结果收到，那么更新下一个sync周期所有的track flag, 比如sync周期为4，在第100帧收齐
	// 所有的检测结果，那么在第101帧，第101、102、103、104帧设置track flag为true
	Frame& frm = db[frm_id % kFrmMaxSize ];
	//if(!(frm.detected == true || frm_id%sync_period_frm_ == 0)){
		CHECK(frm.detected == true || frm_id%sync_period_frm_ == 0)<<"not detect frame, frame_id:"<<frm_id<<",sync period:"<<sync_period_frm_;
	//}
	recvd_multi_det_frm_ = frm_id;
	//clear detect boxes
	frm.det_human_boxes.clear();
	frm.det_car_boxes.clear();
	frm.det_bike_boxes.clear();
	frm.det_license_boxes.clear();
	if(face_use_combine_model_){
		recvd_face_det_frm_ = frm_id;
		frm.det_face_boxes.clear();
		frm.valid_face_det = true;
	}

	for(auto it = multi_det_result->rects.begin();it != multi_det_result->rects.end();it++) {
		switch(it->type) {
			case de::arctern::HUMAN_CAT_ID://eg. person
				frm.det_human_boxes.push_back(it->box);
				break;
			case de::arctern::CAR_CAT_ID://eg. car
				frm.det_car_boxes.push_back(it->box);
				break;
			case de::arctern::BIKE_CAT_ID://eg.bike
				frm.det_bike_boxes.push_back(it->box);
				break;
			case de::arctern::LICENSE_CAT_ID:
				frm.det_license_boxes.push_back(it->box);
				break;
			case de::arctern::FACE_CAT_ID:
				CHECK(face_use_combine_model_)<<"rcvd multi face rslt! but face use combine model is: "<<face_use_combine_model_;
				frm.det_face_boxes.push_back(it->box);
				break;
			default:
				LOG(ERROR)<<"unkown type detect result type:"<<it->type;
				break;
		}
	}
	/* 有可能某一类别没检测到，因此收到Multi检测结果后认为收到所有类别结果
	*/
	frm.valid_human_det = true;	
	frm.valid_car_det = true;	
	frm.valid_bike_det = true;
	frm.valid_license_det = true;
		
	UpdateTrackFlag(frm);
	LOG(INFO)<<"MultiDetectedRsltUpdate frm_id: "<<frm_id<<",detected: "<<frm.detected<<",tracked:"<<frm.tracked<<",trackdone:"<<frm.track_done;

}

/**@brief 更新视频帧
*/
void TrackSync::VideoFrameUpdate(int32_t frm_id,de::MediaTask* video_frm){
	Frame& frm = db[frm_id%kFrmMaxSize];
	frm.stream_id = video_frm->streamid;
	frm.pts = video_frm->pts;
	frm.dts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	frm.img = video_frm->array;
	frm.frame_id = frm_id;
	//检测结果清除
	if(frm_id > recvd_face_det_frm_){		
		frm.det_face_boxes.clear();		
		frm.valid_face_det = false;
	}
	else {
		LOG(WARNING)<<"recv video frame:"<< frm.frame_id <<" after face detect result frame:"<<recvd_face_det_frm_;
	}
	if(frm_id > recvd_multi_det_frm_){
		frm.det_bike_boxes.clear();
		frm.det_car_boxes.clear();
		frm.det_human_boxes.clear();
		frm.det_license_boxes.clear();

		frm.valid_bike_det = false;
		frm.valid_human_det = false;
		frm.valid_car_det = false;
		frm.valid_license_det = false;
	}
	else {
		LOG(WARNING)<<"recv video frame:"<< frm.frame_id <<" after multi detect result frame:"<<recvd_multi_det_frm_;
	}

	frm.track_bike_boxes.clear();
	frm.track_human_boxes.clear();
	frm.track_car_boxes.clear();
	frm.track_face_boxes.clear();
	frm.track_license_boxes.clear();
	frm.face_lumas.clear();
	frm.human_lumas.clear();
	frm.car_lumas.clear();
	frm.bike_lumas.clear();
	
	frm.face_missed.clear();
	frm.human_missed.clear();
	frm.car_missed.clear();
	frm.bike_missed.clear();
	frm.license_missed.clear();

	//set detect&track flag
	if(frm.frame_id % sync_period_frm_ == 0){
		frm.detected = true;
		//check if already have detect result
		frm.tracked = false;
		frm.track_done = false;
		for(int i = 1; i < sync_period_frm_;i++) {
			Frame& f = db[(frm_id+i)%kFrmMaxSize];
			f.detected = false;		
			f.tracked = false;
			f.track_done = false;
		}
	}
	frm.valid_frm = true;
	//check if detect done 
	if(frm.frame_id <= recvd_face_det_frm_
		&& frm.frame_id <= recvd_multi_det_frm_) {
		UpdateTrackFlag(frm);
	}
	//if(frm.detected) {
	//	for(int i = 0 ;i < sync_period_frm_;i++){
	//		//clear next sync perios track flag
	//		Frame& f = db[(frm_id+i)%kFrmMaxSize];
	//		f.tracked = false;
	//		f.track_done = false;
	//	}
	//}
	frm.rpt_done = false;
	if(frm.frame_id == 0) UpdateFrameFormat(frm);
	LOG(INFO)<<"VideoFrameUpdate: frm_id: "<<frm_id<<",detected:"<<frm.detected<<",tracked:"<<frm.tracked<<",trackdone:"<<frm.track_done;
}


/**
 * @brief 更新结果
 */
void TrackSync::UpdatePara(void)
{
	attr_.Get("det_bypass",det_bypass_enable_);
    attr_.Get("sync.period.frm", sync_period_frm_);
    attr_.Get("track.period.frm", track_period_frm_);
	attr_.Get("debug.level",debug_level_);
	attr_.Get("track.interpolate.enable",interplot_track_enable_);
	attr_.Get("luma.calc.enable",luma_calc_enable_);
	attr_.Get("track.alg",tracker_alg_);
	attr_.Get("face.use.alg.combine",face_use_combine_model_);

	CHECK(track_period_frm_ <=  sync_period_frm_ )<<" NOT support! sync_period:"<<sync_period_frm_<<",track_period:"<<track_period_frm_;
/*
	// 设置检测帧标志
    for(auto i = 0; i < kFrmMaxSize; i++)
    {
        if(i % sync_period_frm_ == 0)
        {
            db[i].detected = true;
        }

		
    }
*/
	attr_.Get("face.track.enable",face_track_enable_);
	attr_.Get("body.track.enable",human_track_enable_);
	attr_.Get("car.track.enable",car_track_enable_);
	attr_.Get("bike.track.enable",bike_track_enable_);
	attr_.Get("license.track.enable",license_track_enable_);

	
	attr_.Get("face.track.max.num",face_track_max_num_);
	attr_.Get("human.track.max.num",human_track_max_num_);
	attr_.Get("car.track.max.num",car_track_max_num_);
	attr_.Get("bike.track.max.num",bike_track_max_num_);

	
	attr_.Get("face.qual.batch.num",face_qual_batch_num_);
	attr_.Get("human.qual.batch.num",human_qual_batch_num_);
	attr_.Get("car.qual.batch.num",car_qual_batch_num_);
	attr_.Get("bike.qual.batch.num",bike_qual_batch_num_);
	attr_.Get("license.qual.batch.num",license_qual_batch_num_);

	int32_t max_track_objects = 0;
	if(face_track_enable_ ) {
		CHECK(!tracker_ && !face_syncer_)<<"face tracker not nullptr!";
		max_track_objects += face_track_max_num_;
		face_syncer_ = new Syncer(face_track_max_num_,de::arctern::FACE_CAT_ID,track_period_frm_);
		face_syncer_->UpdatePara(attr_);
    }
    if(human_track_enable_){
		CHECK(!tracker_ && !human_syncer_)<<"human tracker not nullptr!";
		max_track_objects += human_track_max_num_;
		human_syncer_ = new Syncer(human_track_max_num_,de::arctern::HUMAN_CAT_ID,track_period_frm_);
		human_syncer_->UpdatePara(attr_);
    }
	if(car_track_enable_ ){
		CHECK(!tracker_ && !car_syncer_)<<"car tracker not nullptr!";
		max_track_objects += car_track_max_num_;
    	car_syncer_ = new Syncer(car_track_max_num_,de::arctern::CAR_CAT_ID,track_period_frm_);
		car_syncer_->UpdatePara(attr_);
	}
	if(bike_track_enable_ ) {
		CHECK(!tracker_ && !bike_syncer_)<<"bike tracker not nullptr!";
		max_track_objects += bike_track_max_num_;
		bike_syncer_ = new Syncer(bike_track_max_num_,de::arctern::BIKE_CAT_ID,track_period_frm_);
		bike_syncer_->UpdatePara(attr_);
	}
	if(tracker_alg_ == kTrackAlgKCF) {
		tracker_ = new de::cv::KCFMultiTracker(max_track_objects);
	}
	else{
		LOG(FATAL)<<"track alg: "<<tracker_alg_<<" NOT support!!";
	}
	LOG(DEBUG)<<"update para done! sync_period:"<<sync_period_frm_<<",track_period:"<<track_period_frm_<<",face_enable:"<<face_track_enable_<<",body_enable:"<<human_track_enable_
	<<",car_enable:"<<car_track_enable_<<",bike_enable:"<<bike_track_enable_<<",license_enable:"<<license_track_enable_;
}


/**
 * @brief 资源释放
 */
void TrackSync::Stop(void)
{
	pin.DestroyQueue();
}

#if 0

void TrackSync::UpdatePts(int64_t pts){
	int32_t pts_per_frame = 1 ;
	attr_.Get("pts.per.frame", pts_per_frame);
	if(last_pts_ == -1){
		last_frm_id_ = 0;
		last_pts_ = pts;
	}
	else if(pts > last_pts_) {
		last_frm_id_ +=  (pts - last_pts_) / pts_per_frame;		
		last_pts_ = pts;
	}
	else {
		//回绕
		int64_t pts_tmp = pts + ((int64_t)0x100000000);
		last_frm_id_ += (pts_tmp - last_pts_) / pts_per_frame;
		last_pts_ = pts;
	}
}


/**
 * @brief pts时间转为帧
 * @NOTE: 需要和检测Node保持一致 && pts不能循环，目前rtsp时间戳是32位会循环，需要志国支持修改
 * @param pts 
 *
 * @return
 */
int32_t TrackSync::Pts2Frm(int64_t pts)
{
	int32_t frame_id = 0;
	int32_t pts_per_frame = 1 ;
	attr_.Get("pts.per.frame", pts_per_frame);
	if(last_pts_ == -1) {
		frame_id = 0;
		last_pts_ = pts;
	}
	else {	
		#ifdef __CSKY__
		frame_id = ((int32_t)pts - (int32_t)last_pts_ )/pts_per_frame + last_frm_id_;
		#else
		frame_id = (pts - last_pts_) / pts_per_frame + last_frm_id_;
		#endif
	}
    return frame_id;
}
#endif

/**
 * @brief 人脸检测结果处理
 *
 * @param face_det_result
 */
void TrackSync::Proc(de::arctern::DetRslt* face_det_result)
{
    int32_t frm_id = face_det_result->id;//Pts2Frm(face_det_result->id);
    // 检测结果存到数据库
	LOG(INFO)<<"receiv face det result,frm_id:"<<frm_id<<",pts:"<<face_det_result->id;
    DetectedRsltUpdate(frm_id,face_det_result);
    Run(frm_id);
}


/**
 * @brief 人体/机动车/非机动车/检测结果处理
 *
 * @param multi_det_result
 */
void TrackSync::Proc(de::arctern::MultiDetRslt* multi_det_result)
{
    int32_t frm_id = multi_det_result->id;//Pts2Frm(multi_det_result->id);
    // 检测结果存到数据库
	LOG(INFO)<<"receiv multi det result,frm_id:"<<frm_id<<",pts:"<<multi_det_result->id;
    DetectedRsltUpdate(frm_id,multi_det_result);
    Run(frm_id);
}



/**
 * @brief 视频帧数据处理
 *
 * @param frame
 */
void TrackSync::Proc(de::MediaTask* frame)
{	
#if 0//def __CSKY__
	UpdatePts(frame->pts);
#endif
    int32_t frm_id = frame->frameid;//Pts2Frm(frame->pts);

    // 视频帧保存
	LOG(INFO)<<"rcv video frame,frm_id:"<<frm_id<<",pts:"<<frame->pts;
    VideoFrameUpdate(frm_id, frame);
    Run(frm_id);
}


/**
 * @brief 线程处理
 *
 * @param rx_task
 * @param task_type
 */
void TrackSync::Proc(void* rx_task, int32_t task_type,POutType pout_type)
{
	LOG(INFO)<<"receive task:"<<task_type;
    switch(task_type)
    {
    case 0:
        Proc(static_cast<de::arctern::DetRslt*>(rx_task));
        break;
    case 1:
        Proc(static_cast<de::arctern::MultiDetRslt*>(rx_task));
        break;
    case 2:
        Proc(static_cast<de::MediaTask*>(rx_task));
        break;
    default:
        LOG(ERROR) << "type not recoginzed!";
        break;
    }
	//delete rx_task
	pin.DelTask(rx_task, task_type);
}

};
};

namespace de{
	DE_CLASS_REGISTER("de::arctern::TrackSyncNode", de::arctern::TrackSync);
}
