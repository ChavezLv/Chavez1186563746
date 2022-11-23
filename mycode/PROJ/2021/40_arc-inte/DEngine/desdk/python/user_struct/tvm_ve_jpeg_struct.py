import ctypes
import struct
from .basic_struct import StructBasic
from .cv_struct import *
from .pod_struct import *

#typedef struct {
#	uint16_t width;
#	uint16_t height;
#    /*	
#	DEC_FMT_YUV420_PLANAR = 0,           // YYYY... UUUU... VVVV... 
#    DEC_FMT_YUV420_SEMIPLANAR,               // YYYY... UVUVUV...       
#    DEC_FMTC_YUV420_SEMIPLANAR_VU,           // YYYY... VUVUVU...        
#    DEC_FMT_YUV422_INTERLEAVED_YUYV,         // YUYVYUYV...              
#    DEC_FMT_YUV422_INTERLEAVED_UYVY,         // UYVYUYVY...  
#	*/	
#	uint8_t fmt;
#} tvm_ve_img_info_t;
class TvmVeImgInfo(StructBasic):
    #_pack_ = 1
    _fields_ = [("width", 		ctypes.c_uint16),
               ("height", 		ctypes.c_uint16),
               ("fmt", 			ctypes.c_uint8),
               ("reserved0", 	ctypes.c_uint8),]

    def __repr__(self):
        return '[%d,%d,%d]' % (self.width, self.height,self.fmt)

    def to_bytes(self):
        #这里其实reserved0不需要pack，python会自动在fmt字段后面填充1个字节
        return struct.pack('HHBB', self.width, self.height, self.fmt, self.reserved0)

    def bytes_sizeof(self):
        return ctypes.sizeof(TvmVeImgInfo)
        
#typedef struct{
#	uint32_t  v_addr;    //virtual address
#	uint32_t  bus_addr;  //physical address
#	uint32_t  size;
#} tvm_ve_jpegdec_buf;
class TvmVeJpegdecBuf(StructBasic):
    #_pack_ = 1
    _fields_ = [("v_addr", 	ctypes.c_uint32),
               ("bus_addr", ctypes.c_uint32),
               ("size", 	ctypes.c_uint32),]

    def __repr__(self):
        return '[%d,%d,%d]' % (self.v_addr, self.bus_addr,self.size)

    def to_bytes(self):
        return struct.pack('3I', self.v_addr, self.bus_addr,self.size)

    def bytes_sizeof(self):
        return ctypes.sizeof(TvmVeJpegdecBuf)

#typedef struct{
#    uint8_t fmt; 
#    uint8_t level;  
#    uint16_t width;
#    uint16_t height;
#    uint16_t xoffset;
#	 uint16_t yoffset;
#	 uint16_t codingWidth;
#	 uint16_t codingHeight;   
#    uint32_t input;       //input buffer physical address
#    uint32_t output;      //output buffer physical address
#	 uint32_t output_v;    //output buffer virtual address
#    uint32_t osize;   
#}tvm_ve_jpegenc_info;
class TvmVeJpegencInfo(StructBasic):
    #_pack_ = 1
    _fields_ = [("fmt",         ctypes.c_uint8),
               ("level",        ctypes.c_uint8),
               ("width",        ctypes.c_uint16),
               
               ("height",       ctypes.c_uint16),
               ("xoffset",      ctypes.c_uint16),
               
               ("yoffset",      ctypes.c_uint16),
               ("codingWidth",  ctypes.c_uint16),
               
               ("codingHeight", ctypes.c_uint16),
               ("reserved0",    ctypes.c_uint16),
               
               ("input",        ctypes.c_uint32),
               ("output",       ctypes.c_uint32),
               ("output_v",     ctypes.c_uint32),
               ("osize",        ctypes.c_uint32),]
    
    def __repr__(self):
        return '[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d]' % (self.fmt, self.level, self.width, self.height, \
            self.xoffset, self.yoffset, self.codingWidth, self.codingHeight, self.input, self.output, self.output_v, self.osize)
    
    def to_bytes(self):
        #这里其实reserved0字段不需要pack，python会自动在codingHeight后面填充2字节
        return struct.pack('2B7H4I', self.fmt, self.level, self.width, self.height,
            self.xoffset, self.yoffset, self.codingWidth, self.codingHeight, self.reserved0, self.input, self.output, self.output_v, self.osize)
            
    def bytes_sizeof(self):
        return ctypes.sizeof(TvmVeJpegencInfo)
            
            
