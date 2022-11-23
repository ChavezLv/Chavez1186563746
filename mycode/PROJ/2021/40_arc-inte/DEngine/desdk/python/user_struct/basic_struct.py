import ctypes
import struct

class StructBasic(ctypes.Structure):
    @classmethod  
    def from_bytes(_cls, _bytes, _offs = 0):
        _cls.check_bytes(_bytes, ctypes.sizeof(_cls), _offs)
        return _cls.from_buffer(_bytes, _offs)

    @classmethod  
    def get_vec_num(_cls, _bytes, _offs = 0):
        field_len =  ctypes.sizeof(ctypes.c_int64)
        _cls.check_bytes(_bytes, field_len)
        num = ctypes.c_int64.from_buffer(_bytes, _offs)
        _offs += field_len
        return num, _offs
        
    @classmethod  
    def get_vec_from_bytes(_cls, _ele_cls, _bytes, _offs = 0):
        num, _offs = _cls.get_vec_num(_bytes, _offs) 
        obj = _cls(num)        
        for i in range(0, obj.num):
            try:
                obj.ptr[i] = _ele_cls.from_bytes(_bytes, _offs)
            except:
                obj.ptr[i] = _ele_cls.from_buffer(_bytes, _offs)

            try:
                _offs += obj.ptr[i].bytes_sizeof()
            except:
                _offs += ctypes.sizeof(_ele_cls)

            assert len(_bytes) >= _offs
        return obj
        
    @staticmethod  
    def to_int(_num): 
        try:
            num = int(_num)
        except:
            num = _num.value
        return num

    @staticmethod  
    def check_bytes(_bytes, _reserve_num, _offs = 0):
        assert len(_bytes) >= _offs + _reserve_num

    def vec_repr(self):
        ss = 'num = %d\n' % self.num
        if self.num < 10:
            for i in range(0, self.num):
                ss +=  '[%d]: %s\n' % ( i, str(self.ptr[i]))
        else:
            ss += '[%s, %s, %s, %s,...]' % (str(self.ptr[0]),  str(self.ptr[1]), str(self.ptr[2]), str(self.ptr[3]))
            pass
        return ss
    
    def vec_to_bytes(self, ele_type = None):
        _bytes = struct.pack('q', self.num)
        for i in range(0, self.num):
            if ele_type == ctypes.c_int8:
                _bytes += struct.pack('b', self.ptr[i])
                pass
            else:
                _bytes += self.ptr[i].to_bytes()
        return _bytes


    def create_vec_struct(self, _ele_cls, _num):
        elems =  (_ele_cls * StructBasic.to_int(_num))()
        self.ptr = ctypes.cast(elems,ctypes.POINTER(_ele_cls))
        self.num = _num
        pass





