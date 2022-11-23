from .basic_struct import StructBasic
from ctypes import *
import ctypes

BATCH_MAX_NUM = 32

#T_Block_Time_Info
class BlockTimeInfo(StructBasic):
    _fields_ = [("layer_sched_cyctime", ctypes.c_uint),
                ("layer_start_cyctime", ctypes.c_uint),
                ("layer_end_cyctime", ctypes.c_uint)]

    def bytes_sizeof(self):
        return ctypes.sizeof(BlockTimeInfo)

class LayerTypeInfo(StructBasic):
    _fields_ = [("block_type", ctypes.c_uint),
                ("dsp_layer_type", ctypes.c_uint)]

    def bytes_sizeof(self):
        return ctypes.sizeof(LayerTypeInfo)

class VecLayerTypeInfo(StructBasic):
    _fields_ = [("num", ctypes.c_int64),
                ("ptr", ctypes.POINTER(LayerTypeInfo))]

    @classmethod
    def from_bytes(_cls, _bytes, _offs=0):
        return _cls.get_vec_from_bytes(LayerTypeInfo, _bytes, _offs)

    def __init__(self, num=0):
        self.create_vec_struct(LayerTypeInfo, num)

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64)
        sz += ctypes.sizeof(LayerTypeInfo) * self.num
        return sz

#std::vector<T_Block_Time_Info>
class VecTBlockTimeInfo(StructBasic):
    _fields_ = [("num", ctypes.c_int64),
                ("ptr", ctypes.POINTER(BlockTimeInfo))]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(BlockTimeInfo, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(BlockTimeInfo, num)

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64)
        sz += ctypes.sizeof(BlockTimeInfo) * self.num
        return sz

#T_Model_Time
class TModelTime(StructBasic):
    _fields_ = [("model_cyctime", ctypes.c_uint),
                ("model_time", ctypes.c_float),
                ("batchNum", ctypes.c_ushort),
                ("blockNum", ctypes.c_ushort),
                ("model_perbatch_cyctime", (ctypes.c_uint * BATCH_MAX_NUM)),
                ("model_start_cyctime", ctypes.c_uint),
                ("model_end_cyctime", ctypes.c_uint),
                ("block_cyctime", VecTBlockTimeInfo),
                ("layer_type", VecLayerTypeInfo)]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        model_cyctime = ctypes.c_uint.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint)

        model_time = ctypes.c_float.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_float)

        batchNum = ctypes.c_ushort.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_ushort)

        blockNum = ctypes.c_ushort.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_ushort)

        model_perbatch_cyctime = (ctypes.c_uint * BATCH_MAX_NUM).from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint * BATCH_MAX_NUM)

        model_start_cyctime = ctypes.c_uint.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint)

        model_end_cyctime = ctypes.c_uint.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint)

        block_cyctime = VecTBlockTimeInfo.from_bytes(_bytes, _offs)
        _offs += block_cyctime.bytes_sizeof()

        layer_type = VecLayerTypeInfo.from_bytes(_bytes, _offs)
        _offs += layer_type.bytes_sizeof()

        obj = _cls(model_cyctime,
                   model_time,
                   batchNum,
                   blockNum,
                   model_perbatch_cyctime,
                   model_start_cyctime,
                   model_end_cyctime,
                   block_cyctime,
                   layer_type)
        return obj

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_uint) + ctypes.sizeof(ctypes.c_float) \
            + ctypes.sizeof(ctypes.c_ushort) * 2 + ctypes.sizeof(ctypes.c_uint * BATCH_MAX_NUM) \
            + ctypes.sizeof(ctypes.c_uint) * 2 + self.block_cyctime.bytes_sizeof() + self.layer_type.bytes_sizeof()
        return sz

#T_Model_Time
class DspProfileInfo(StructBasic):
    _fields_ = [("type", ctypes.c_int),
                ("reserved", ctypes.c_int),
                ("period_start_cycle", ctypes.c_uint64),
                ("period_end_cycle", ctypes.c_uint64),
                ("period_sched_rate", ctypes.c_double),
                ("tot_start_cycle", ctypes.c_uint64),
                ("tot_end_cycle", ctypes.c_uint64),
                ("tot_sched_rate", ctypes.c_double)]

    @classmethod
    def from_bytes(_cls, _bytes, _offs=0):
        type = ctypes.c_int.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_int)

        period_start_cycle = ctypes.c_uint64.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint64)

        period_end_cycle = ctypes.c_uint64.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint64)

        period_sched_rate = ctypes.c_double.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_double)

        tot_start_cycle = ctypes.c_uint64.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint64)

        tot_end_cycle = ctypes.c_uint64.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_uint64)

        tot_sched_rate = ctypes.c_double.from_buffer(_bytes, _offs)
        _offs += ctypes.sizeof(ctypes.c_double)

        obj = _cls(type,
                   period_start_cycle,
                   period_end_cycle,
                   period_sched_rate,
                   tot_start_cycle,
                   tot_end_cycle,
                   tot_sched_rate)
        return obj

#std::vector<DspProfileInfo>
class VecDspProfileInfo(StructBasic):
    _fields_ = [("num", ctypes.c_int64),
                ("ptr", ctypes.POINTER(DspProfileInfo))]

    @classmethod
    def from_bytes(_cls, _bytes, _offs = 0):
        return _cls.get_vec_from_bytes(DspProfileInfo, _bytes, _offs)

    def __init__(self, num = 0):
        self.create_vec_struct(DspProfileInfo, num)

    def bytes_sizeof(self):
        sz = ctypes.sizeof(ctypes.c_int64)
        sz += ctypes.sizeof(DspProfileInfo) * self.num
        return sz