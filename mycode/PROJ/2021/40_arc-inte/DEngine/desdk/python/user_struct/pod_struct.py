import ctypes
import struct
from .basic_struct import StructBasic

# std::pair<int, float>
class PairIntFloat(StructBasic):
    #_pack_ = 1
    _fields_ = [("int", ctypes.c_int),
               ("float", ctypes.c_float) ]

    def __repr__(self):
        return 'int = %d,  float = %.2f' % (self.int, self.float)

    def to_bytes(self):
        return struct.pack('if', self.int, self.float)

    def bytes_sizeof(self):        
        return ctypes.sizeof(PairIntFloat)


# std::vector<std::pair<int, float>>
class VecPairIntFloat(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(PairIntFloat))]   

    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(PairIntFloat, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(PairIntFloat, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):      
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(PairIntFloat) * self.num
        return sz



# std::vector<int>
class VecInt(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_int))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_int, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_int, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_int) * self.num
        return sz

# std::vector<int64_t>
class VecInt64(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_int64))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_int64, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_int64, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_int64) * self.num
        return sz

# std::vector<float>
class VecFloat(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_float))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_float, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_float, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_float) * self.num
        return sz


# std::vector<char>
class VecChar(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_int8))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_int8, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_int8, num)
            
    def __repr__(self):
        # string 
        return  ''.join(chr(self.ptr[i]) for i in range(0, self.num))
        #return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes(ctypes.c_int8)

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_int8) * self.num
        return sz



# std::pair<std::string, int>
class PairStringInt(StructBasic):
    #_pack_ = 1
    _fields_ = [("key", VecChar),
               ("val", ctypes.c_int) ]

    def __repr__(self):
        return "key = %s, value = %d" % (self.key, self.val)

    def to_bytes(self):
        _bytes = self.key.to_bytes()
        _bytes += struct.pack('i', self.val)
        return _bytes

    @classmethod
    def from_str_int(_cls, _str, _int):
        key_bytes = bytearray(ctypes.c_int64(len(_str))) + _str.encode('utf-8')
        return _cls( VecChar.from_bytes(key_bytes), _int)        

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        _key = VecChar.from_bytes(_bytes, _offs)
        _offs += _key.bytes_sizeof()

        _val = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)
        return _cls(_key, _val)
        
    def __init__(self, _key, _val):
        self.key = _key
        self.val = _val

    def bytes_sizeof(self):        
        return self.key.bytes_sizeof() + ctypes.sizeof(ctypes.c_int)


# std::map<std::string, int>
class MapStringInt(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(PairStringInt))]
    
    # dict.key = str, dict.val = int
    @classmethod
    def from_dict(_cls, _dict):
        _bytes = bytearray(ctypes.c_int64(len(_dict)))
        for _str, _int in _dict.items():
            _bytes += bytearray(ctypes.c_int64(len(_str))) + _str.encode('utf-8')
            _bytes +=  struct.pack('i', _int)
        return _cls.from_bytes(_bytes)

    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(PairStringInt, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(PairStringInt, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz

# std::vector<int8_t>
class VecInt8(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_int8))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_int8, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_int8, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_int8) * self.num
        return sz
        


# std::vector<uint8_t>
class VecUInt8(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(ctypes.c_uint8))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(ctypes.c_uint8, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(ctypes.c_uint8, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def to_chars(self):
        ss = ""
        for i in range(0, self.num):
            ss +=  "%s" % (chr(self.ptr[i]))
        #ss += "\n"
        return ss

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        sz += ctypes.sizeof(ctypes.c_uint8) * self.num
        return sz

# std::vector<std::vector<int8_t>>
class VecVecInt8(StructBasic):
     #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(VecInt8))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(VecInt8, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(VecInt8, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz


# std::vector<std::vector<uint8_t>>
class VecVecUInt8(StructBasic):
     #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64), 
                ("ptr", ctypes.POINTER(VecUInt8))]
    
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(VecUInt8, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(VecUInt8, num)
            
    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz
       

#std::string
class StdString(VecChar):
        
    #@classmethod 
    #def from_bytes(_cls, _bytes, _offs = 0):
    #    int_sz  = struct.calcsize("Q")
    #    str_len = struct.unpack("Q", _bytes[_offs: _offs + int_sz])
    #    str = _bytes[_offs + int_sz : _offs + int_sz + str_len[0]]       
    #    return _cls(str)

    #def __init__(self, _str):        
    #    assert _str != None
    #    print(_str)
    #    self.str = _str
    #    self.ptr = ctypes.c_char_p(bytes(self.str))
    
    @classmethod
    def from_str(_cls, str_source):
        _bytes = bytes(str_source, 'utf-8')
        _bytearr = bytearray(struct.pack('Q', len(_bytes)) + _bytes)
        return _cls.from_bytes(_bytearr)
    
    def __repr__(self):        
        str = ''
        for i in range(0,self.num):
            str += chr(self.ptr[i])
        return str

    def to_bytes(self):
        _bytes = struct.pack('Q', self.num)
        for i in range(0, self.num):
            _bytes += struct.pack('b', self.ptr[i])
        return _bytes

# std::map<stdstring, int>, ?python???vector<pair>??c++??map
class VecPairStringInt(StructBasic):
    #_pack_ = 1
    _fields_ = [("num", ctypes.c_int64),
               ("ptr", ctypes.POINTER(PairStringInt))]

    def __init__(self, num = 0):
        self.create_vec_struct(PairStringInt, num)

    def __repr__(self):
        return self.vec_repr()

    def to_bytes(self):
        return self.vec_to_bytes()

    def bytes_sizeof(self):        
        sz = ctypes.sizeof(ctypes.c_int64) # num
        for i in range(0, self.num):
            sz += self.ptr[i].bytes_sizeof()
        return sz
