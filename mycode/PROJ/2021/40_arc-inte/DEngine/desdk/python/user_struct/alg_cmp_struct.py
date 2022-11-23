import ctypes
import struct
from .basic_struct import StructBasic
from .cv_struct import *
from .pod_struct import *
from .face_struct import *

#struct Alg_Cmp_Frm_Record {
#	int frm_idx;
#	std::vector<Cv_Rect_Float> face_detect_result_arctern_mtcnn_0_9;
#	std::vector<Cv_Rect_Float> face_detect_result_mtcnn_fixed_1_0;
#	std::vector<Cv_Rect_Float> face_detect_result_arctern_yolo_tiny;
#	std::vector<cv::Rect> fake_face_attr_rects;
#	std::vector<cv::Rect> face_attr_rects;
#	std::vector<float> face_quality_fixed_0_0_1;
#	std::vector<float> face_quality_0_0_1;
#	std::vector<float> face_quality_0_0_3;
#	std::string img_path;
#};

class AlgCmpFrmRecord(StructBasic):
    #_pack_ = 1
    _fields_ = [("frm_idx", ctypes.c_int), 
               ("face_detect_result_arctern_mtcnn_0_9", VecPairRectFloat),
                ("face_detect_result_mtcnn_fixed_1_0", VecPairRectFloat),
                 ("face_detect_result_arctern_yolo_tiny", VecPairRectFloat),
               ("fake_face_attr_rects", VecRect),
               ("face_attr_rects", VecRect),
               ("face_quality_fixed_0_0_1", VecFloat),
               ("face_quality_0_0_1", VecFloat),
               ("face_quality_0_0_3",VecFloat),
               ("img_path", StdString)
                ]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        frm_idx = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        face_detect_result_arctern_mtcnn_0_9 = VecPairRectFloat.from_bytes(_bytes, _offs)
        _offs += face_detect_result_arctern_mtcnn_0_9.bytes_sizeof()

        face_detect_result_mtcnn_fixed_1_0 = VecPairRectFloat.from_bytes(_bytes, _offs)
        _offs += face_detect_result_mtcnn_fixed_1_0.bytes_sizeof()

        face_detect_result_arctern_yolo_tiny = VecPairRectFloat.from_bytes(_bytes, _offs)
        _offs += face_detect_result_arctern_yolo_tiny.bytes_sizeof()

        fake_face_attr_rects = VecRect.from_bytes(_bytes, _offs)
        _offs += fake_face_attr_rects.bytes_sizeof()

        face_attr_rects = VecRect.from_bytes(_bytes, _offs)
        _offs += face_attr_rects.bytes_sizeof()

        face_quality_fixed_0_0_1 = VecFloat.from_bytes(_bytes, _offs)
        _offs += face_quality_fixed_0_0_1.bytes_sizeof()

        face_quality_0_0_1 = VecFloat.from_bytes(_bytes, _offs)
        _offs += face_quality_0_0_1.bytes_sizeof()

        face_quality_0_0_3 = VecFloat.from_bytes(_bytes, _offs)
        _offs += face_quality_0_0_3.bytes_sizeof()

        img_path = StdString.from_bytes(_bytes, _offs)
        _offs += img_path.bytes_sizeof()

        obj = _cls(frm_idx,
                face_detect_result_arctern_mtcnn_0_9, 
                face_detect_result_mtcnn_fixed_1_0, 
                face_detect_result_arctern_yolo_tiny,
                fake_face_attr_rects,
                face_attr_rects, 
                face_quality_fixed_0_0_1,
                face_quality_0_0_1,
                face_quality_0_0_3,
                img_path)
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int) # frm_idx        
        sz += self.face_detect_result_arctern_mtcnn_0_9.bytes_sizeof()  
        sz += self.face_detect_result_mtcnn_fixed_1_0.bytes_sizeof()   
        sz += self.face_detect_result_arctern_yolo_tiny.bytes_sizeof() 
        sz +=  self.fake_face_attr_rects.bytes_sizeof()
        sz += self. face_attr_rects.bytes_sizeof()
        sz += self. face_quality_fixed_0_0_1.bytes_sizeof()
        sz += self. face_quality_0_0_1.bytes_sizeof()
        sz += self. face_quality_0_0_3.bytes_sizeof()
        sz += self. img_path.bytes_sizeof()
        return sz

    def __init__(self, frm_idx,
                face_detect_result_arctern_mtcnn_0_9, 
                face_detect_result_mtcnn_fixed_1_0, 
                face_detect_result_arctern_yolo_tiny,
                fake_face_attr_rects,
                face_attr_rects, 
                face_quality_fixed_0_0_1,
                face_quality_0_0_1,
                face_quality_0_0_3,
                img_path):
        self.frm_idx = frm_idx
        self.face_detect_result_arctern_mtcnn_0_9 = face_detect_result_arctern_mtcnn_0_9
        self.face_detect_result_mtcnn_fixed_1_0 = face_detect_result_mtcnn_fixed_1_0
        self.face_detect_result_arctern_yolo_tiny = face_detect_result_arctern_yolo_tiny
        self.fake_face_attr_rects = fake_face_attr_rects
        self.face_attr_rects =  face_attr_rects
        self.face_quality_fixed_0_0_1 = face_quality_fixed_0_0_1
        self.face_quality_0_0_1 = face_quality_0_0_1
        self.face_quality_0_0_3 = face_quality_0_0_3
        self.img_path = img_path
        pass

    def __repr__(self):
        str = "------------------------------------------\r\n"
        str += "frm = %d \r\n"   %(self.frm_idx)
        str += "face_detect_result_arctern_mtcnn_0_9 rslt = %s " % (self.face_detect_result_arctern_mtcnn_0_9)
        str += "face_detect_result_mtcnn_fixed_1_0 rslt = %s " % (self.face_detect_result_mtcnn_fixed_1_0)
        str += "face_detect_result_arctern_yolo_tiny rslt = %s " % (self.face_detect_result_arctern_yolo_tiny)
        str += "face_attr_rects  = %s " % (self.face_attr_rects)
        str += "face_quality_fixed_0_0_1  = %s " % (self.face_quality_fixed_0_0_1)
        str += "face_quality_0_0_1  = %s" % (self.face_quality_0_0_1)
        str += "face_quality_0_0_3  = %s" % (self.face_quality_0_0_3)
        str += "img_path  = %s" % (self.img_path)
        return str

    def to_bytes(self):
        assert False

# cv::vector<Alg_Cmp_Frm_Record>
class VecAlgCmpFrmRecord(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(AlgCmpFrmRecord))]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(AlgCmpFrmRecord, _bytes, _offs)       

    def __init__(self, num = 0):
        self.create_vec_struct(AlgCmpFrmRecord, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz

