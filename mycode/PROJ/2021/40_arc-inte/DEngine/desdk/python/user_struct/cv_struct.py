import ctypes
import struct
from .basic_struct import StructBasic

# cv::Rect
class Rect(StructBasic):
    #_pack_ = 1
    _fields_ = [("x", ctypes.c_int), 
                ("y", ctypes.c_int), 
                ("width", ctypes.c_int),
               ("height", ctypes.c_int) ]

    def __repr__(self):
        return '[%d, %d, %d, %d]' % (self.x, self.y, self.width, self.height)

    def to_bytes(self):
        return struct.pack('iiii', self.x, self.y, self.width, self.height)

    def bytes_sizeof(self):      
        return ctypes.sizeof(Rect)


# cv::vector<cv::Rect>
class VecRect(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(Rect))]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(Rect, _bytes, _offs)       

    def __init__(self, num = 0):
        self.create_vec_struct(Rect, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(Rect) * self.num
        return sz

# std::pair<int, cv::Rect>
class PairIntRect(StructBasic):
    #_pack_ = 1
    _fields_ = [("int", ctypes.c_int),
              ("rect", Rect) ]

    def __repr__(self):
        return 'int = %d, rect = %s' % (self.int, str(self.rect))

    def to_bytes(self):        
        return  struct.pack('i', self.int) + self.rect.to_bytes()

    def bytes_sizeof(self):
        return ctypes.sizeof(PairIntRect) 


# std::vector<std::pair<int, cv::Rect>> 
class VecPairIntRect(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(PairIntRect)) ]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(PairIntRect, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(PairIntRect, num)

    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(PairIntRect) * self.num
        return sz


# std::pair<cv::Rect, float>
class PairRectFloat(StructBasic):
    #_pack_ = 1
    _fields_ = [("rect", Rect),
               ("float", ctypes.c_float) ]
    
    def __repr__(self):
        return 'rect = %s,  float = %.2f' % (str(self.rect), self.float)

    def to_bytes(self):        
        return self.rect.to_bytes() +  struct.pack('f', self.float)

    def bytes_sizeof(self):
        return ctypes.sizeof(PairRectFloat) 


# std::vector<std::pair<cv::Rect, float>> 
class VecPairRectFloat(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(PairRectFloat)) ]

    @classmethod  
    def from_bytes(cls, _bytes, _offs = 0):
        return cls.get_vec_from_bytes(PairRectFloat, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(PairRectFloat, num)

    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(PairRectFloat) * self.num
        return sz
        



