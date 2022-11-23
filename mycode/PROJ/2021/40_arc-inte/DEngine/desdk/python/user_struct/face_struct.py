import ctypes
import struct
from .basic_struct import StructBasic
from .cv_struct import *
from .pod_struct import *

#typedef struct {
#	s16 pitch; // 上下
#	s16 roll;	// 偏转
#	s16 yaw; // 左右
#}Face_Pose;
class FacePose(StructBasic):
    #_pack_ = 1
    _fields_ = [("pitch", ctypes.c_short),
               ("roll", ctypes.c_short),
               ("yaw", ctypes.c_short),
               ("reserved", ctypes.c_short),]

    def __repr__(self):
        #return 'pitch=%d,roll=%d,yaw=%d' % (self.pitch, self.roll,self.yaw)
        return '[%d,%d,%d]' % (self.pitch, self.roll,self.yaw)

    def to_bytes(self):
        return struct.pack('hhhh', self.pitch, self.roll, self.yaw, self.reserved)

    def bytes_sizeof(self):
        return ctypes.sizeof(FacePose)

#std::vector<FacePose>
class VecFacePose(StructBasic):
      #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(FacePose))]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(FacePose, _bytes, _offs)       

    def __init__(self, num = 0):
        self.create_vec_struct(FacePose, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(FacePose) * self.num
        return sz

#typedef struct {
#	int id;
#	Cv_Rect_Float detect;
#	Face_Pose pose;
#	Int_Float age;
#	Int_Float gender;
#	std::vector<char> feature;
#}Face_Struct;
class FaceStruct(StructBasic):
    #_pack_ = 4
    _fields_ = [("id", ctypes.c_int),
               ("detect", PairRectFloat),
               ("pose", FacePose),
               ("age", PairIntFloat),
               ("gender", PairIntFloat),
               ("feature", VecChar)]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        id = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        detect = PairRectFloat.from_bytes(_bytes, _offs)
        _offs += detect.bytes_sizeof()

        pose = FacePose.from_bytes(_bytes, _offs)
        _offs += pose.bytes_sizeof()

        age = PairIntFloat.from_bytes(_bytes, _offs)
        _offs += age.bytes_sizeof()

        gender = PairIntFloat.from_bytes(_bytes, _offs)
        _offs += gender.bytes_sizeof()

        feature = VecChar.from_bytes(_bytes,_offs)
        _offs += feature.bytes_sizeof()

        # 构造对象并拷贝
        obj = _cls( id, detect, pose, age, gender, feature)
        return obj

    def __init__(self, id, detect, pose, age, gender, feature):
        self.id = id
        self.detect = detect
        self.pose = pose
        self.age = age
        self.gender = gender
        self.feature = feature


    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int) # id
        sz += self.detect.bytes_sizeof()  
        sz += self.pose.bytes_sizeof()   
        sz += self.age.bytes_sizeof() 
        sz +=  self.gender.bytes_sizeof()
        sz += self. feature.bytes_sizeof()
        return sz

    def __repr__(self):
        return 'id=%d,\ndet =%s,\npose=%s,\nage=%s,\ngender=%s,\nfeature=%s' % (self.id, str(self.detect),str(self.pose),str(self.age), str(self.gender),str(self.feature))

    def to_bytes(self):
        # todo 
        assert False
        return False


# std::vector<Face_Struct> 
class VecFaceStruct(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(FaceStruct))]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        num, _offs = _cls.get_vec_num(_bytes, _offs) 
        obj = _cls(num)
        for i in range(0, obj.num):
            assert len(_bytes) >= _offs + ctypes.sizeof(FaceStruct)            
            obj.ptr[i] = FaceStruct.from_bytes(_bytes, _offs)
            # 注意：此处为可变size
            _offs += obj.ptr[i].bytes_sizeof()
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
           sz += self.ptr[i].bytes_sizeof()
        return sz

    def __init__(self, num = 0):
        self.create_vec_struct(FaceStruct, num)

    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()
  


#typedef struct {	
#	std::string img_name;
#	std::vector<Face_Struct> faces;
#}Img_Face_Struct;
class ImgFaceStruct(StructBasic):
    #_pack_ = 1
    _fields_ = [("name", StdString), 
                ("faces", VecFaceStruct)]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        _name = StdString.from_bytes(_bytes, _offs)
        _offs += _name.bytes_sizeof()
        _faces = VecFaceStruct.from_bytes(_bytes, _offs)
        obj = _cls(_name, _faces)
        return obj
    
    def __init__(self, _str, _faces):
        self.name = _str
        self.faces = _faces

    def __repr__(self):
        return "name = %s, faces = %s" % (str(self.name), str(self.faces))

    def bytes_sizeof(self):
        sz = self.name.bytes_sizeof()
        sz += self.faces.bytes_sizeof()
        return sz

    def to_bytes(self):
        # todo
        assert False
        return False

       
#std::vector<Img_Face_Struct>
class VecImgFaceStruct(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ImgFaceStruct))]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        num, _offs = _cls.get_vec_num(_bytes, _offs) 
        obj = _cls(num)
        for i in range(0, obj.num):                    
            obj.ptr[i] = ImgFaceStruct.from_bytes(_bytes, _offs)
            # 注意：此处为可变size
            _offs += obj.ptr[i].bytes_sizeof()
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
           sz += self.ptr[i].bytes_sizeof()
        return sz

    def __init__(self, num = 0):
        self.create_vec_struct(ImgFaceStruct, num)

    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    

#struct Frm_Record {
#	int frm_idx;
#	int is_detected_frm;
#	std::vector<Cv_Rect_Float> face_detect_result;
#	std::vector<int> track_faces_missed;
#	std::vector<Int_Cv_Rect> track_result;
#	std::vector<int> track_faces_aged;
#	std::vector<Face_Pose> face_pose_result;
#	std::vector<float> face_quality;
#	std::vector<std::vector<int8_t>> face_fe_result;
#};
class FaceFrmRecord(StructBasic):
    #_pack_ = 1
    _fields_ = [("frm_idx", ctypes.c_int), 
                ("is_detected_frm", ctypes.c_int),
               ("face_detect_result", VecPairRectFloat),
               ("track_faces_missed", VecInt),
               ("track_result", VecPairIntRect),
                ("track_faces_aged", VecInt),
               ("face_pose_result", VecFacePose),
               ("face_quality",VecFloat),
               ("face_fe_result", VecVecInt8)
                ]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        frm_idx = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        is_detected_frm = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        face_detect_result = VecPairRectFloat.from_bytes(_bytes, _offs)
        _offs += face_detect_result.bytes_sizeof()

        track_faces_missed = VecInt.from_bytes(_bytes, _offs)
        _offs += track_faces_missed.bytes_sizeof()

        track_result = VecPairIntRect.from_bytes(_bytes, _offs)
        _offs += track_result.bytes_sizeof()       

        track_faces_aged = VecInt.from_bytes(_bytes, _offs)
        _offs += track_faces_aged.bytes_sizeof()

        face_pose_result = VecFacePose.from_bytes(_bytes, _offs)
        _offs += face_pose_result.bytes_sizeof()

        face_quality = VecFloat.from_bytes(_bytes, _offs)
        _offs += face_quality.bytes_sizeof()

        face_fe_result = VecVecInt8.from_bytes(_bytes, _offs)
        _offs += face_fe_result.bytes_sizeof()

        obj = _cls(frm_idx,
                is_detected_frm, 
                face_detect_result, 
                track_faces_missed,
                track_result,
                track_faces_aged, 
                face_pose_result,
                face_quality,
                face_fe_result)
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int) # frm_idx
        sz += ctypes.sizeof(ctypes.c_int) # is_detected_frm
        sz += self.face_detect_result.bytes_sizeof()  
        sz += self.track_faces_missed.bytes_sizeof()   
        sz += self.track_result.bytes_sizeof() 
        sz +=  self.track_faces_aged.bytes_sizeof()
        sz += self. face_pose_result.bytes_sizeof()
        sz += self. face_quality.bytes_sizeof()
        sz += self. face_fe_result.bytes_sizeof()
        return sz

    def __init__(self, frm_idx,
                is_detected_frm, 
                face_detect_result, 
                track_faces_missed,
                track_result,
                track_faces_aged, 
                face_pose_result,
                face_quality,
                face_fe_result):
        self.frm_idx = frm_idx
        self.is_detected_frm = is_detected_frm
        self.face_detect_result = face_detect_result
        self.track_faces_missed = track_faces_missed
        self.track_result = track_result
        self.track_faces_aged =  track_faces_aged
        self.face_pose_result = face_pose_result
        self.face_quality = face_quality
        self.face_fe_result = face_fe_result
        pass

    def __repr__(self):
        str = "------------------------------------------\r\n"
        str += "frm = %d, detect = %d \r\n"   %(self.frm_idx, self.is_detected_frm)
        str += "detect rslt = %s " % (self.face_detect_result)
        str += "track_faces_missed  = %s " % (self.track_faces_missed)
        str += "track_result  = %s " % (self.track_result)
        str += "track_faces_aged  = %s " % (self.track_faces_aged)
        str += "face_quality  = %s" % (self.face_quality)
        return str

    def to_bytes(self):
        assert False


# cv::vector<Face_Frm_Record>
class VecFaceFrmRecord(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(FaceFrmRecord))]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(FaceFrmRecord, _bytes, _offs)       

    def __init__(self, num = 0):
        self.create_vec_struct(FaceFrmRecord, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz

