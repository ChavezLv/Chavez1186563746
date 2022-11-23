import ctypes
import struct
from .basic_struct import StructBasic
from .cv_struct import *
from .pod_struct import *
from .face_struct import FacePose



#struct Video_Config_Para {
#	int mVedioType {0};
#	int mVedioWidth {1920};
#	int mVedioHeight {1080};
#	int mHasTimeStamp {0}; ///< 是否有时间戳，0--没有，1--有
#	int mFrameRate {25}; ///<视频流帧率
#    int mBigImgFlag {1};   ///< 模组是否提供编码的抓拍大图，0--不提供，1--提供。默认0
#    int mRoiImgFlag {1};   ///< 低16位指示模组是否提供编码的抓拍小图，0--不提供，1--提供，2--小图上报同时附大图；高16位指示识别图，0--不提供，1--提供识别小图
#    float mRoiImgEnlargeRate {1.0}; /*抓拍图在人脸roi宽高基础上扩大比例，大于等于1.0，1.0--只报人脸roi原始大小*/
#    int mImgEncLevel {1};  // 抓拍图Jpeg编码等级，0--ENC_QUALITY_LOW, 1--ENC_QUALITY_NORMAL, 2--ENC_QUALITY_HIGH
#	int mFrameSync {1};
#	int mSnapMode {2}; /* 0:无需抓拍选优， 1：抓拍选优无去重， 2：抓拍选优+去重 */
#	int mSnapCalcFe {0}; /* 0:抓拍结果不做FE，1:抓拍结果做FE*/
#	int mRecMode {0};  /* 0:无需识别选优， 1：识别选优（识别成功不再做识别） 3：识别选优 周期 */
#	std::vector<int> mAttrMode; /* 0:不做属性 */
#};  /* 视频流业务配置 */
class VideoCfgPara(StructBasic):
    #_pack_ = 1
    _fields_ = [ ("mVedioType", ctypes.c_int),
                ("mVedioWidth", ctypes.c_int),
               ("mVedioHeight", ctypes.c_int),
               ("mHasTimeStamp", ctypes.c_int),
               ("mFrameRate", ctypes.c_int),
               ("mBigImgFlag", ctypes.c_int),
               ("mRoiImgFlag", ctypes.c_int),
               ("mRoiImgEnlargeRate", ctypes.c_float),
               ("mImgEncLevel", ctypes.c_int),
               ("mFrameSync", ctypes.c_int),
               ("mSnapMode", ctypes.c_int),
               ("mSnapCalcFe", ctypes.c_int),
               ("mRecMode", ctypes.c_int),
              # ("mAttrMode", VecInt),
               ]

    def __init__(self):
        ctypes.Structure.__init__(self)
        self.mVedioType = 0
        self.mVedioWidth = 1920
        self.mVedioHeight = 1080
        self.mHasTimeStamp = 0
        self.mFrameRate = 25
        self.mBigImgFlag = 1
        self.mRoiImgFlag = 1
        self.mRoiImgEnlargeRate = 1.0
        self.mImgEncLevel = 1
        self.mFrameSync = 1
        self.mSnapMode = 0
        self.mSnapCalcFe = 1
        self.mRecMode = 0
        pass

    def __repr__(self):
        _str = 'mVedioType = %d\n' % (self.mVedioType)
        _str += 'mVedioWidth = %d\n' % (self.mVedioWidth)
        _str += 'mVedioHeight = %d\n' % (self.mVedioHeight)
        _str += 'mHasTimeStamp = %d\n' % (self.mHasTimeStamp)
        _str += 'mFrameRate = %d\n' % (self.mFrameRate)
        _str += 'mBigImgFlag = %d\n' % (self.mBigImgFlag)
        _str += 'mRoiImgFlag = %d\n' % (self.mRoiImgFlag)
        _str += 'mRoiImgEnlargeRate = %f\n' % (self.mRoiImgEnlargeRate)
        _str += 'mImgEncLevel = %d\n' % (self.mImgEncLevel)
        _str += 'mFrameSync = %d\n' % (self.mFrameSync)
        _str += 'mSnapMode = %d\n' % (self.mSnapMode)
        _str += 'mSnapCalcFe = %d\n' % (self.mSnapCalcFe)
        _str += 'mRecMode = %d\n' % (self.mRecMode)
        return _str

    def to_bytes(self):
        return struct.pack('7if5i', self.mVedioType, self.mVedioWidth, self.mVedioHeight,
                          self.mHasTimeStamp, self.mFrameRate, self.mBigImgFlag, self.mRoiImgFlag,
                          self.mRoiImgEnlargeRate,
                          self.mImgEncLevel, self.mFrameSync, self.mSnapMode, self.mSnapCalcFe, self.mRecMode)

    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 12 + ctypes.sizeof(ctypes.c_float) 


#struct Detect_Config_Para {
#	int mAlgSel {1};
#	int mDetMinFace {36} ;
#	int mDetMaxFace {600} ;
#	int mDetPeriods {160};
#	std::vector<cv::Rect> mDetReg;
#}; /* 检测配置参数 */

class DetectCfgPara(StructBasic):
    #_pack_ = 1
    _fields_ = [("mAlgSel", ctypes.c_int), 
               ("mDetMinFace", ctypes.c_int),
                ("mDetMaxFace", ctypes.c_int),
                 ("mDetPeriods", ctypes.c_int),
               ("mDetReg", VecRect)
                ]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        mAlgSel = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        mDetMinFace = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        mDetMaxFace = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        mDetPeriods = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        mDetReg = VecRect.from_bytes(_bytes, _offs)
        _offs += mDetReg.bytes_sizeof()

        obj = _cls(mAlgSel, 
                mDetMinFace, 
                mDetMaxFace,
                mDetPeriods,
                mDetReg)
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int)  * 4
        sz += self. mDetReg.bytes_sizeof()
        return sz

    def __init__(self, 
                mAlgSel, 
                mDetMinFace, 
                mDetMaxFace,
                mDetPeriods,
                mDetReg):
        self.mAlgSel = mAlgSel
        self.mDetMinFace = mDetMinFace
        self.mDetMaxFace = mDetMaxFace
        self.mDetPeriods = mDetPeriods
        self.mDetReg =  mDetReg
        pass

    def __repr__(self):        
        str = "mAlgSel = %d \n"   %(self.mAlgSel)
        str += "mDetMinFace = %d \n"   %(self.mDetMinFace)
        str += "mDetMaxFace = %d \n"   %(self.mDetMaxFace)
        str += "mDetPeriods = %d \n"   %(self.mDetPeriods)
        str += "mDetPeriods = %s \n"   %(self.mDetReg)
        return str

    def to_bytes(self):
        return struct.pack('4i', self.mAlgSel, self.mDetMinFace, self.mDetMaxFace,self.mDetPeriods) + self.mDetReg.to_bytes()


#struct Track_Config_Para {
#	int mAlgSel {2};
#	float mTrkUnDetAge {2.0};
#	int mTrkPeriods {0};
#	int mTrkMaxFaceNum {40};
#	int mTrkSyncAddTh {48};
#	int mTrkSyncAdjTh {32};
#	float mTrkSyncBorderAddTh {0.6};
#	float mTrkSyncBorderAdjTh {0.6};
#	float mTrkSyncOverlapAdjTh {0.3};
#	float mTrkSyncOverlapDelTh {0.5};
#	int mTrkOnetAdj {0};
#}; /* 跟踪配置参数 */

class TrackCfgPara(StructBasic):
    #_pack_ = 1
    _fields_ = [ ("mAlgSel", ctypes.c_int),
                ("mTrkUnDetAge", ctypes.c_float),
               ("mTrkPeriods", ctypes.c_int),
               ("mTrkMaxFaceNum", ctypes.c_int),
               ("mTrkSyncAddTh", ctypes.c_int),
               ("mTrkSyncAdjTh", ctypes.c_int),
               ("mTrkSyncBorderAddTh", ctypes.c_float),
               ("mTrkSyncBorderAdjTh", ctypes.c_float),
               ("mTrkSyncOverlapAdjTh", ctypes.c_float),
               ("mTrkSyncOverlapDelTh", ctypes.c_float),
               ("mTrkOnetAdj", ctypes.c_int),
               ]

    def __init__(self):
        ctypes.Structure.__init__(self)
        self.mAlgSel = 2
        self.mTrkUnDetAge = 2.0
        self.mTrkPeriods = 0
        self.mTrkMaxFaceNum = 40
        self.mTrkSyncAddTh = 48
        self.mTrkSyncAdjTh = 32
        self.mTrkSyncBorderAddTh = 0.6
        self.mTrkSyncBorderAdjTh = 0.6
        self.mTrkSyncOverlapAdjTh = 0.3
        self.mTrkSyncOverlapDelTh = 0.5
        self.mTrkOnetAdj = 0
        pass

    def __repr__(self):
        _str = 'mAlgSel = %d\n' % (self.mAlgSel)
        _str += 'mTrkUnDetAge = %f\n' % (self.mTrkUnDetAge)
        _str += 'mTrkPeriods = %d\n' % (self.mTrkPeriods)
        _str += 'mTrkMaxFaceNum = %d\n' % (self.mTrkMaxFaceNum)
        _str += 'mTrkSyncAddTh = %d\n' % (self.mTrkSyncAddTh)
        _str += 'mTrkSyncAdjTh = %d\n' % (self.mTrkSyncAdjTh)
        _str += 'mTrkSyncBorderAddTh = %f\n' % (self.mTrkSyncBorderAddTh)
        _str += 'mTrkSyncBorderAdjTh = %f\n' % (self.mTrkSyncBorderAdjTh)
        _str += 'mTrkSyncOverlapAdjTh = %f\n' % (self.mTrkSyncOverlapAdjTh)
        _str += 'mTrkSyncOverlapDelTh = %f\n' % (self.mTrkSyncOverlapDelTh)
        _str += 'mTrkOnetAdj = %d\n' % (self.mTrkOnetAdj)
        return _str

    def to_bytes(self):
        return struct.pack('if4i4fi', self.mAlgSel, self.mTrkUnDetAge, 
                           self.mTrkPeriods, self.mTrkMaxFaceNum,self.mTrkSyncAddTh,self.mTrkSyncAdjTh,
                           self.mTrkSyncBorderAddTh, self.mTrkSyncBorderAdjTh,self.mTrkSyncOverlapAdjTh,self.mTrkSyncOverlapDelTh,
                           self.mTrkOnetAdj)

    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 6 + ctypes.sizeof(ctypes.c_float) * 5


#struct Face_Snap_Filter_Config_Para {
# int mQualAlgSel {2};
#	int mFltMinMS {1000};
#	int mFltPeriodMS {0};
#	int mFltReportCnt {3};
#	int mFltReportMinSize {36}; - hk
#	int mFltAttriPeriodMS {20000};
#	float mFltMinQual {0.4}; - hk
#	float mFltFirstQual {0.8};
#	float mFltDeltaQual {0.4};
#	int mFltFrontalMinCnt {2}; 
#	int mFltPoseQualPeriods {160};
#	int mFltBestSelAtLeave {1}; - hk
#	int mFltReportDelayMs {0};
#	Face_Pose mDown {-90, -90, -90}; - hk
#	Face_Pose mUp {90, 90, 90}; - hk
#};

class FaceSnapFilterCfgPara(StructBasic):
    #_pack_ = 1
    _fields_ = [ ("mQualAlgSel", ctypes.c_int),
                ("mFltMinMS", ctypes.c_int),
               ("mFltPeriodMS", ctypes.c_int),
               ("mFltReportCnt", ctypes.c_int),
               ("mFltReportMinSize", ctypes.c_int),
               ("mFltAttriPeriodMS", ctypes.c_int),
               ("mFltMinQual", ctypes.c_float),
               ("mFltFirstQual", ctypes.c_float),
               ("mFltDeltaQual", ctypes.c_float),
               ("mFltFrontalMinCnt", ctypes.c_int),
               ("mFltPoseQualPeriods", ctypes.c_int),
               ("mFltBestSelAtLeave", ctypes.c_int),
               ("mFltReportDelayMs", ctypes.c_int),
               ("mDown", FacePose),
               ("mUp", FacePose),
               ]

    def __init__(self):
        ctypes.Structure.__init__(self)
        self.mQualAlgSel = 2
        self.mFltMinMS = 1000
        self.mFltPeriodMS = 0
        self.mFltReportCnt = 3
        self.mFltReportMinSize = 36
        self.mFltAttriPeriodMS = 20000
        self.mFltMinQual = 0.4
        self.mFltFirstQual = 0.8
        self.mFltDeltaQual = 0.4
        self.mFltFrontalMinCnt = 2
        self.mFltPoseQualPeriods = 160
        self.mFltBestSelAtLeave = 1
        self.mFltReportDelayMs = 0
        self.mDown.pitch = -90
        self.mDown.roll = -90
        self.mDown.yaw = -90
        self.mUp.pitch = 90
        self.mUp.roll = 90
        self.mUp.yaw = 90
        pass

    def __repr__(self):
        _str = 'mQualAlgSel = %d\n' % (self.mQualAlgSel)
        _str += 'mFltMinMS = %d\n' % (self.mFltMinMS)
        _str += 'mFltPeriodMS = %d\n' % (self.mFltPeriodMS)
        _str += 'mFltReportCnt = %d\n' % (self.mFltReportCnt)
        _str += 'mFltReportMinSize = %d\n' % (self.mFltReportMinSize)
        _str += 'mFltAttriPeriodMS = %d\n' % (self.mFltAttriPeriodMS)
        _str += 'mFltMinQual = %f\n' % (self.mFltMinQual)
        _str += 'mFltFirstQual = %f\n' % (self.mFltFirstQual)
        _str += 'mFltDeltaQual = %f\n' % (self.mFltDeltaQual)
        _str += 'mFltFrontalMinCnt = %d\n' % (self.mFltFrontalMinCnt)
        _str += 'mFltPoseQualPeriods = %d\n' % (self.mFltPoseQualPeriods)
        _str += 'mFltBestSelAtLeave = %d\n' % (self.mFltBestSelAtLeave)
        _str += 'mFltReportDelayMs = %d\n' % (self.mFltReportDelayMs)
        _str += 'mDown = %s\n' % (str(self.mDown))
        _str += 'mUp = %s\n' % (str(self.mUp))
        return _str

    def to_bytes(self):
        return struct.pack('6i', self.mQualAlgSel, self.mFltMinMS, self.mFltPeriodMS, 
                           self.mFltReportCnt, self.mFltReportMinSize, self.mFltAttriPeriodMS) + \
                    struct.pack('3f', self.mFltMinQual, self.mFltFirstQual, self.mFltDeltaQual) + \
                    struct.pack('4i', self.mFltFrontalMinCnt, self.mFltPoseQualPeriods, self.mFltBestSelAtLeave, self.mFltReportDelayMs) + \
                    self.mDown.to_bytes() + self.mUp.to_bytes()
    
    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 10 + ctypes.sizeof(ctypes.c_float) * 3 +  ctypes.sizeof(FacePose) * 2


#struct Face_Snap_Filter_Config_Dedup_Para {
#	//去重参数
#   int mGluonAlgSel {0}
#	int mSnapDedupMode {1}; -hk
#	int mDedupTimeMSTh {5000};  -hk, 新增加脸和库中faceid 脸的时间
#	int mDedupSpaceTh {500};
#	float mDedupCompareTh {0.36};  -hk, 
#	int mDedupTimeoutMSTh {15000}; -hk, 删数据库，防止溢出
#	int mDedupIgnorTimeMSTh {600000};
#  int mDedupPosPThres {90}; - hk，满足门限，才启动fe特征
#	int mDedupPosYThres {90};
#	int mDedupPosRThres {90};
#	int mDedupFltMinMs {200}; -hk, 出现多长时间内，必须选择一张做
#	int mDedupFltPeriodMs {0};
#	float mDedupFltMinQual {0.65}; -hk,
#	float mDedupFltFirstQual {0.7}; -hk， 第一次满足要求qual，做
#	float mDedupFltDeltaQual {0.02};  -hk,
#	int mDedupFltMinFace {70}; -hk,
#}; /* 抓拍选优配置参数 */

class FaceSnapFilterCfgDedupPara(StructBasic):
    #_pack_ = 1
    _fields_ = [("mGluonAlgSel", ctypes.c_int),
               ("mSnapDedupMode", ctypes.c_int),
               ("mDedupTimeMSTh", ctypes.c_int),
               ("mDedupSpaceTh", ctypes.c_int),
               ("mDedupCompareTh", ctypes.c_float),
               ("mDedupTimeoutMSTh", ctypes.c_int),
               ("mDedupIgnorTimeMSTh", ctypes.c_int),
               ("mDedupPosPThres", ctypes.c_int),
               ("mDedupPosYThres", ctypes.c_int),
               ("mDedupPosRThres", ctypes.c_int),
               ("mDedupFltMinMs", ctypes.c_int),
               ("mDedupFltPeriodMs", ctypes.c_int),
               ("mDedupFltMinQual", ctypes.c_float),
               ("mDedupFltFirstQual", ctypes.c_float),
               ("mDedupFltDeltaQual", ctypes.c_float),
                ("mDedupFltMinFace", ctypes.c_int),
               ]

    def __init__(self):
        ctypes.Structure.__init__(self)
        self.mGluonAlgSel = 0
        self.mSnapDedupMode = 1
        self.mDedupTimeMSTh = 5000
        self.mDedupSpaceTh = 500
        self.mDedupCompareTh = 0.34
        self.mDedupTimeoutMSTh = 10000
        self.mDedupIgnorTimeMSTh = 600000
        self.mDedupPosPThres = 90
        self.mDedupPosYThres = 90
        self.mDedupPosRThres = 90
        self.mDedupFltMinMs = 2000000
        self.mDedupFltPeriodMs = 0
        self.mDedupFltMinQual = 0.4
        self.mDedupFltFirstQual = 0.7
        self.mDedupFltDeltaQual = 0.02
        self.mDedupFltMinFace = 45
        pass

    def __repr__(self):
        _str = 'mGluonAlgSel = %d\n' % (self.mGluonAlgSel)
        _str += 'mSnapDedupMode = %d\n' % (self.mSnapDedupMode)
        _str += 'mDedupTimeMSTh = %d\n' % (self.mDedupTimeMSTh)
        _str += 'mDedupSpaceTh = %d\n' % (self.mDedupSpaceTh)
        _str += 'mDedupCompareTh = %f\n' % (self.mDedupCompareTh)
        _str += 'mDedupTimeoutMSTh = %d\n' % (self.mDedupTimeoutMSTh)
        _str += 'mDedupIgnorTimeMSTh = %d\n' % (self.mDedupIgnorTimeMSTh)
        _str += 'mDedupPosPThres = %d\n' % (self.mDedupPosPThres)
        _str += 'mDedupPosYThres = %d\n' % (self.mDedupPosYThres)
        _str += 'mDedupPosRThres = %d\n' % (self.mDedupPosRThres)
        _str += 'mDedupFltMinMs = %d\n' % (self.mDedupFltMinMs)
        _str += 'mDedupFltPeriodMs = %d\n' % (self.mDedupFltPeriodMs)
        _str += 'mDedupFltMinQual = %f\n' % (self.mDedupFltMinQual)
        _str += 'mDedupFltFirstQual = %f\n' % (self.mDedupFltFirstQual)
        _str += 'mDedupFltDeltaQual = %f\n' % (self.mDedupFltDeltaQual)
        _str += 'mDedupFltMinFace = %d\n' % (self.mDedupFltMinFace)
        return _str

    def to_bytes(self):
         return struct.pack('4if', self.mGluonAlgSel,self.mSnapDedupMode, self.mDedupTimeMSTh, self.mDedupSpaceTh, self.mDedupCompareTh) + \
                struct.pack('7i', self.mDedupTimeoutMSTh, self.mDedupIgnorTimeMSTh, self.mDedupPosPThres, 
                            self.mDedupPosYThres, self.mDedupPosRThres, self.mDedupFltMinMs, self.mDedupFltPeriodMs) + \
                struct.pack('3fi', self.mDedupFltMinQual, self.mDedupFltFirstQual, self.mDedupFltDeltaQual, self.mDedupFltMinFace)


    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 12 + ctypes.sizeof(ctypes.c_float) * 4 


#struct Face_Capture_Config_Para {
#	Video_Config_Para mVideoConfig;
#	Detect_Config_Para mDetectConfig;
#	Track_Config_Para mTrackConfig;
#	Face_Snap_Filter_Config_Para mSnapFilterConfig;
#	Face_Snap_Filter_Config_Dedup_Para mSnapFilterDedupConfig;
#};
class FaceCaptureConfigPara(StructBasic):
    #_pack_ = 1
    _fields_ = [("mVideoConfig", VideoCfgPara),
               ("mDetectConfig", DetectCfgPara),
               ("mTrackConfig", TrackCfgPara),
               ("mSnapFilterConfig", FaceSnapFilterCfgPara),
               ("mSnapFilterDedupConfig", FaceSnapFilterCfgDedupPara)
               ]

    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        mVideoConfig = VideoCfgPara.from_bytes(_bytes, _offs)
        _offs += mVideoConfig.bytes_sizeof()

        mDetectConfig = DetectCfgPara.from_bytes(_bytes, _offs)
        _offs += mDetectConfig.bytes_sizeof()

        mTrackConfig = TrackCfgPara.from_bytes(_bytes, _offs)
        _offs += mTrackConfig.bytes_sizeof()

        mSnapFilterConfig = FaceSnapFilterCfgPara.from_bytes(_bytes, _offs)
        _offs += mSnapFilterConfig.bytes_sizeof()

        mSnapFilterDedupConfig = FaceSnapFilterCfgDedupPara.from_bytes(_bytes,_offs)
        _offs += mSnapFilterDedupConfig.bytes_sizeof()

        # 构造对象并拷贝
        obj = _cls( mVideoConfig, mDetectConfig, mTrackConfig, mSnapFilterConfig, mSnapFilterDedupConfig)
        return obj

    def __init__(self, mVideoConfig, mDetectConfig, mTrackConfig, mSnapFilterConfig, mSnapFilterDedupConfig):
        self.mVideoConfig = mVideoConfig
        self.mDetectConfig = mDetectConfig
        self.mTrackConfig = mTrackConfig
        self.mSnapFilterConfig = mSnapFilterConfig
        self.mSnapFilterDedupConfig = mSnapFilterDedupConfig

    def bytes_sizeof(self):
        sz = self.mVideoConfig.bytes_sizeof()  
        sz += self.mDetectConfig.bytes_sizeof()   
        sz += self.mTrackConfig.bytes_sizeof() 
        sz +=  self.mSnapFilterConfig.bytes_sizeof()
        sz += self. mSnapFilterDedupConfig.bytes_sizeof()
        return sz

    def __repr__(self):
        return 'mVideoConfig=%s,\nmDetectConfig =%s,\nmTrackConfig=%s,\nmSnapFilterConfig=%s,\nmSnapFilterDedupConfig=%s'  % (self.mVideoConfig, self.mDetectConfig, self.mTrackConfig, self.mSnapFilterConfig, self.mSnapFilterDedupConfig)
        

    def to_bytes(self):
        return self.mVideoConfig.to_bytes() + \
            self.mDetectConfig.to_bytes() + \
            self.mTrackConfig.to_bytes() + \
            self.mSnapFilterConfig.to_bytes() + \
            self.mSnapFilterDedupConfig.to_bytes()

