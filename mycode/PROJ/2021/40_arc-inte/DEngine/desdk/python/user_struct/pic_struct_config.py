import ctypes
import struct
from .basic_struct import StructBasic
from .cv_struct import *
from .pod_struct import *
from .face_struct import FacePose

#struct PicProcPara {
#	int mObjType {0};
#	int mFaceDetAlgSel {1};
#	int mFaceQualAlgSel {2};
#	int mFaceQualPoseAlgSel {1};
#	int mFaceAgeAlgSel {1};
#	int mFaceGenderAlgSel {1};
#	int mFaceGluonAlgSel {0};
#	int mLivnessAlgSel {0};
#	int mPersonDetAlgSel {0};
#	int mPersonQualAlgSel {0};
#	int mPersonGluonAlgSel {0};
#	//std::vector<cv::Rect> faces;
#};
class PicProcPara(StructBasic):
    #_pack_ = 1
    _fields_ = [("mObjType", ctypes.c_int),
               ("mFaceDetAlgSel", ctypes.c_int),
               ("mFaceQualAlgSel", ctypes.c_int),
               ("mFaceQualPoseAlgSel", ctypes.c_int),
               ("mFaceAgeAlgSel", ctypes.c_int),
               ("mFaceGenderAlgSel", ctypes.c_int),
               ("mFaceGluonAlgSel", ctypes.c_int),
               ("mLivnessAlgSel", ctypes.c_int),
               ("mPersonDetAlgSel", ctypes.c_int),
               ("mPersonQualAlgSel", ctypes.c_int),
               ("mPersonGluonAlgSel", ctypes.c_int),               
               ]

    def __init__(self):
        ctypes.Structure.__init__(self)
        self.mObjType = 0
        self.mFaceDetAlgSel = 1
        self.mFaceQualAlgSel = 2
        self.mFaceQualPoseAlgSel = 1
        self.mFaceAgeAlgSel = 1
        self.mFaceGenderAlgSel = 1
        self.mFaceGluonAlgSel = 0
        self.mLivnessAlgSel = 0
        self.mPersonDetAlgSel = 0
        self.mPersonQualAlgSel = 0
        self.mPersonGluonAlgSel = 0
        pass

    def __repr__(self):
        _str = 'PicProcPara:\n'
        _str += 'mObjType = %d\n' % (self.mObjType)
        _str += 'mFaceDetAlgSel = %d\n' % (self.mFaceDetAlgSel)
        _str += 'mFaceQualAlgSel = %d\n' % (self.mFaceQualAlgSel)
        _str += 'mFaceQualPoseAlgSel = %d\n' % (self.mFaceQualPoseAlgSel)
        _str += 'mFaceAgeAlgSel = %d\n' % (self.mFaceAgeAlgSel)
        _str += 'mFaceGenderAlgSel = %d\n' % (self.mFaceGenderAlgSel)
        _str += 'mFaceGluonAlgSel = %d\n' % (self.mFaceGluonAlgSel)
        _str += 'mLivnessAlgSel = %d\n' % (self.mLivnessAlgSel)
        _str += 'mPersonDetAlgSel = %d\n' % (self.mPersonDetAlgSel)
        _str += 'mPersonQualAlgSel = %d\n' % (self.mPersonQualAlgSel)
        _str += 'mPersonGluonAlgSel = %d\n' % (self.mPersonGluonAlgSel)
        return _str

    def to_bytes(self):
         return struct.pack('4i', self.mObjType,self.mFaceDetAlgSel, self.mFaceQualAlgSel, self.mFaceQualPoseAlgSel)  + \
                struct.pack('7i', self.mFaceAgeAlgSel, self.mFaceGenderAlgSel, self.mFaceGluonAlgSel, 
                            self.mLivnessAlgSel, self.mPersonDetAlgSel, self.mPersonQualAlgSel, self.mPersonGluonAlgSel)


    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 11