import de
import struct
from enum import Enum
from user_struct import nn_time_struct
from collections import namedtuple
import numpy as np

NN_HEADER_SIZE = 32
BLOCK_HEADER_SIZE = 20
HALF_MAX_TIME = 0x7fffffff
WHOLE_MAX_TIME = 0xffffffff

class BlockType(Enum):
    NNP_TYPE = 0
    CRGH_NNP_TYPE = 1
    DSP_TYPE = 2
    CRGH_TYPE = 3
    KCF_TYPE = 4

class NN_Profile:
    def __init__(self, model_time):
        #self._net_bin_file = net_bin_file
        self._model_time = model_time
    '''
    def get_nn_block_type(self):
        block_types = []
        with open(self._net_bin_file, "rb") as f:
            block_num = struct.unpack('i', f.read(4))[0]
            f.read(NN_HEADER_SIZE - 4)
            for _ in range(block_num):
                f.read(2)
                layer_num = struct.unpack('h', f.read(2))[0]
                block_type = struct.unpack('i', f.read(4))[0]
                block_size = struct.unpack('i', f.read(4))[0]
                f.read(block_size - 12)
                for i in range(layer_num):
                    block_types.append(block_type)

        return block_types
    '''
    def get_nn_block_type(self):
        block_types = []
        for i in range(self._model_time.layer_type.num):
            block_types.append(self._model_time.layer_type.ptr[i].block_type)

        return block_types

    def layer_time_batch_merge(self, batch_num, layer_type, layer_num, layer_time):
        LayerTime = namedtuple('LayerTime', ['start_time', 'end_time'])
        merge_layer_time = []

        for i in range(layer_num):
            if (layer_type[i] == BlockType.DSP_TYPE.value):
                merge_layer_time.append(LayerTime(0, 0))
            else:
                min_start_time = layer_time.ptr[i].layer_start_cyctime
                max_end_time = layer_time.ptr[i].layer_end_cyctime
                for j in range(1, batch_num):
                    start_time = layer_time.ptr[i + j * layer_num].layer_start_cyctime
                    end_time = layer_time.ptr[i + j * layer_num].layer_end_cyctime
                    if (((start_time < min_start_time) and ((min_start_time - start_time) < HALF_MAX_TIME))
                        or ((start_time > min_start_time) and ((start_time - min_start_time) > HALF_MAX_TIME))):
                        min_start_time = start_time
                    if (((end_time > max_end_time) and ((end_time - max_end_time) < HALF_MAX_TIME))
                        or ((end_time < max_end_time) and ((max_end_time - end_time) > HALF_MAX_TIME))):
                        max_end_time = end_time
                merge_layer_time.append(LayerTime(min_start_time, max_end_time))

        return merge_layer_time

    def get_look_info(self, layer_type, merge_layer_time, layer_num, start_layer_index):
        j = start_layer_index - 1
        look_type0 = 0
        cur_type = layer_type[start_layer_index]
        look_time = [0, 0]
        while (j >= 0):
            if (cur_type == BlockType.DSP_TYPE.value and (layer_type[j] != cur_type)):
                look_type0 |= 1
                look_time[0] = merge_layer_time[j].end_time
                break

            if ((cur_type == BlockType.CRGH_TYPE.value) and
                    ((layer_type[j] == BlockType.NNP_TYPE.value)
                     or (layer_type[j] == BlockType.CRGH_NNP_TYPE.value))):
                look_type0 |= 1
                look_time[0] = merge_layer_time[j].end_time
                break

            if ((cur_type == BlockType.KCF_TYPE.value) and
                    ((layer_type[j] == BlockType.NNP_TYPE.value)
                     or (layer_type[j] == BlockType.CRGH_NNP_TYPE.value)
                     or (layer_type[j] == BlockType.CRGH_TYPE.value))):
                look_type0 |= 1
                look_time[0] = merge_layer_time[j].end_time
                break

            j -= 1

        j = start_layer_index + 1
        while (j < layer_num):
            if ((cur_type == BlockType.CRGH_NNP_TYPE.value) and (layer_type[j] == BlockType.NNP_TYPE.value)):
                look_type0 |= 2
                look_time[1] = merge_layer_time[j].start_time
                break
            elif ((cur_type == BlockType.NNP_TYPE.value) and (layer_type[j] == BlockType.CRGH_NNP_TYPE.value)):
                look_type0 |= 2
                look_time[1] = merge_layer_time[j].start_time
                break
            elif ((cur_type == BlockType.DSP_TYPE.value) and (layer_type[j] != cur_type)):
                look_type0 |= 2
                look_time[1] = merge_layer_time[j].start_time
                break
            elif ((cur_type == BlockType.CRGH_TYPE.value)
                  and ((layer_type[j] == BlockType.NNP_TYPE.value) or (layer_type[j] == BlockType.CRGH_NNP_TYPE.value))):
                look_type0 |= 2
                look_time[1] = merge_layer_time[j].start_time
                break
            elif ((cur_type == BlockType.KCF_TYPE)
                  and ((layer_type[j] == BlockType.NNP_TYPE.value) or (layer_type[j] == BlockType.CRGH_NNP_TYPE.value)
                  or (layer_type[j] == BlockType.CRGH_TYPE.value))):
                look_type0 |= 2
                look_time[1] = merge_layer_time[j].start_time
                break

            j += 1

        return look_type0, look_time


    def get_profile_detail(self):

        layer_type = self.get_nn_block_type()

        merge_layer_time = self.layer_time_batch_merge(self._model_time.batchNum, layer_type,
                                                       self._model_time.blockNum,
                                                       self._model_time.block_cyctime)

        dsp_layer_num = 0
        block_type_time = [0,0,0,0,0]
        LayerTime = namedtuple('LayerTime', ['blockNum', 'blockCycle'])
        layer_time = []
        for i in range(self._model_time.blockNum):
            look_type, look_time = self.get_look_info(layer_type, merge_layer_time, self._model_time.blockNum, i)
            if (layer_type[i] == BlockType.DSP_TYPE.value):
                add_layer = False
                for j in range(self._model_time.batchNum):
                    block_time0 = 0
                    layer_time_info = self._model_time.block_cyctime.ptr[i + j * self._model_time.blockNum]
                    start_time = layer_time_info.layer_start_cyctime
                    end_time = layer_time_info.layer_end_cyctime
                    if ((1 == (look_type & 0x1)) and (start_time != end_time)):
                        if (((start_time > look_time[0]) and ((start_time - look_time[0]) > HALF_MAX_TIME))
                         or ((start_time < look_time[0]) and ((look_time[0] - start_time) < HALF_MAX_TIME))):
                            start_time = look_time[0]

                        if (((end_time > look_time[0]) and ((end_time - look_time[0]) > HALF_MAX_TIME))
                         or ((end_time < look_time[0]) and ((look_time[0] - end_time) < HALF_MAX_TIME))):
                            end_time = look_time[0]

                    if (((look_type & 2) == 2) and (start_time != end_time)):
                        if (((start_time > look_time[1]) and ((start_time - look_time[1]) < HALF_MAX_TIME))
                         or ((start_time < look_time[1]) and ((look_time[1] - start_time) > HALF_MAX_TIME))):
                            start_time = look_time[1]

                        if (((end_time > look_time[1]) and ((end_time - look_time[1]) < HALF_MAX_TIME))
                         or ((end_time < look_time[1]) and ((look_time[1] - end_time) > HALF_MAX_TIME))):
                            end_time = look_time[1]

                    if ((end_time != start_time) and (add_layer == False)):
                        dsp_layer_num += 1
                        add_layer = True

                    if (end_time < start_time):
                        block_time0 = WHOLE_MAX_TIME - start_time + end_time + 1
                    else:
                        block_time0 = end_time - start_time
                    
                    layer_time.append(LayerTime(i, block_time0))
                    block_type_time[layer_type[i]] += block_time0

            else:
                if (dsp_layer_num < 2):
                    dsp_layer_num = 0

                layer_time0 = 0
                start_time = merge_layer_time[i].start_time
                end_time = merge_layer_time[i].end_time

                if (((look_type & 1) == 1) and (start_time != end_time)):
                    if (((start_time > look_time[0]) and ((start_time - look_time[0]) > HALF_MAX_TIME))
                     or ((start_time < look_time[0]) and ((look_time[0] - start_time) < HALF_MAX_TIME))):
                        start_time = look_time[0]

                    if (((end_time > look_time[0]) and ((end_time - look_time[0]) > HALF_MAX_TIME))
                     or ((end_time < look_time[0]) and ((look_time[0] - end_time) < HALF_MAX_TIME))):
                        end_time = look_time[0]

                if (((look_type & 2) == 2) and (start_time != end_time)):
                    if (((start_time > look_time[1]) and ((start_time - look_time[1]) < HALF_MAX_TIME))
                     or ((start_time < look_time[1]) and ((look_time[1] - start_time) > HALF_MAX_TIME))):
                        start_time = look_time[1]

                    if (((end_time > look_time[1]) and ((end_time - look_time[1]) < HALF_MAX_TIME))
                     or ((end_time < look_time[1]) and ((look_time[1] - end_time) > HALF_MAX_TIME))):
                        end_time = look_time[1]

                if (end_time < start_time):
                    block_time0 = WHOLE_MAX_TIME - start_time + end_time + 1
                else:
                    block_time0 = end_time - start_time

                layer_time.append(LayerTime(i, block_time0))
                
                block_type_time[layer_type[i]] += block_time0
        

        if ((self._model_time.batchNum > 1) and (dsp_layer_num > 2)):
            block_type_time[BlockType.DSP_TYPE.value] >>= 1

        return block_type_time, layer_time

    def profile(self):
        block_type_time, layer_time = self.get_profile_detail()
        model_time = dict()
        model_time['sched_time'] = self._model_time.model_cyctime
        for time in block_type_time:
            model_time['sched_time'] -= time

        model_time['nnp_time'] = block_type_time[0] + block_type_time[1]
        model_time['dsp_time'] = block_type_time[2]
        model_time['crgh_time'] = block_type_time[3]
        model_time['kcf_time'] = block_type_time[4]
        model_time['model_time'] = self._model_time.model_time

        return model_time, layer_time

class DspProfileType(Enum):
    kNNP0 = 0
    kNNP1 = 1
    kNNP2 = 2
    kNNP3 = 3
    kCrgh = 4
    kCrghNnp0 = 5
    kCrghNnp1 = 6
    kCrghFft = 7
    kBoxdma = 8


class DspProfile:
    def __init__(self, remote):
        self._remote = remote

    def open(self, bit_map):
        self._handle = self._remote.get_function("de.dspprofile.open")(bit_map)

    def close(self, bit_map):
        self._remote.get_function("de.dspprofile.close")(self._handle, bit_map)

    def get_sched_profile(self, bit_map, dsp_id):
        bytes = self._remote.get_function("de.dspprofile.get")(self._handle, bit_map, dsp_id)
        profile_info = nn_time_struct.VecDspProfileInfo.from_bytes(bytearray(bytes))
        return profile_info






