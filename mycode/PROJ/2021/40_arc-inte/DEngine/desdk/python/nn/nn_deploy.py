import de
import sys
import os
import logging
import time
import multiprocessing
from enum import Enum

import numpy
from de import rpc
from de.contrib import util
from user_struct import  *
#from mm import jpeg_codec as jpeg
import datetime
from user_struct import nn_time_struct
from nn import nn_profile

def device_init(dev_id, host = "192.168.33.101", port = 9200):
    if not de.module.enabled("rpc"):
        return
    remote = rpc.connect(dev_id, host, port)
    return remote

class ShapeCode(Enum):
    NDHW = 0
    DHW = 1
    HW = 2
    W = 3

class TypeCode(Enum):
    Int = 0
    UInt = 1
    Float = 2


class NNModule:
    def __init__(self, remote, handle, batch_num = 1, dsp_split = 0):
        self._remote = remote
        self._handle = handle
        self._input_tensor = []
        self._resize_type = 0
        self._byte_out = None
        self._dsp_split = dsp_split


    @property
    def handle(self):
        return self._handle

    @property
    def batch_num(self):
        return self._batch_num

    @batch_num.setter
    def batch_num(self, batch_num):
        self._batch_num = batch_num

    @property
    def dsp_split(self):
        return self._dsp_split

    @dsp_split.setter
    def dsp_split(self, dsp_split):
        self._dsp_split = dsp_split


    def set_input(self, *args):
        for nd in args:
            self._input_tensor.append(nd)

    def run(self):
        start = datetime.datetime.now()
        self._byte_out = self._remote.get_function("de.AiEngine.predict.python")(self._handle,
                                len(self._input_tensor), *(self._input_tensor), self.batch_num)
        end = datetime.datetime.now()
        self._input_tensor.clear()

    def get_nn_layer_num(self, net_bin_file):
        block_types = []
        NN_HEADER_SIZE = 32
        with open(net_bin_file, "rb") as f:
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

        return len(block_types)


    def get_outputs(self):
        nd_num = self._remote.get_function("ndarray.vector.DePackage")(self._byte_out)
        nd_out = []
        for i in range(nd_num):
            nd = self._remote.get_function("ndarray.vector.GetOne")(i)
            nd_out.append(self._remote.get_function("ndarray.vector.GetOne")(i).asnumpy())
        return nd_out

    def profile(self):
        bytes =  self._remote.get_function("de.AiEngine.get.model.time.info")(self._handle)
        model_time = nn_time_struct.TModelTime.from_bytes(bytearray(bytes))
        profile = nn_profile.NN_Profile(model_time)
        return profile.profile()

    def display_profile_info(self, model_time):
        print("model_time: {} ms".format(model_time['model_time']))
        print("nnp_time: {} cycle".format(model_time['nnp_time']))
        print("dsp_time: {} cycle".format(model_time['dsp_time']))
        print("crgh_time: {} cycle".format(model_time['crgh_time']))
        print("sched_time: {} cycle".format(model_time['sched_time']))
        print("kcf_time: {} cycle".format(model_time['kcf_time']))


class NNDeploy:
    def __init__(self, remote, log_level = 3):
        self._log_level = log_level
        self._remote = remote

    def get_remote(self):
        return self._remote

    def load(self, handle, net_bin_file, model_bin_file, batch_max, encrypt = False, resize_type = 0, tensor_num_per_batch = 1):
        start = datetime.datetime.now()
        self._remote.get_function("de.AiEngine.load.model")(handle, net_bin_file, model_bin_file, batch_max, encrypt, resize_type, tensor_num_per_batch)
        end = datetime.datetime.now()
        print(f"load finished time {(end-start).seconds*1000 + (end-start).microseconds/1000} ms")

        return NNModule(self._remote, handle)

    def load_host(self, net_bin_host, model_bin_host, batch_max = 8, resize_type = 0, tensor_num_per_batch = 1, profile = 1):
        handle = self._remote.get_function("de.AiEngine.create")(profile,0,1)
        start = datetime.datetime.now()
        net_bin_device = os.path.join("/root", os.path.basename(net_bin_host))
        model_bin_device = os.path.join("/root", os.path.basename(model_bin_host))
        self._remote.upload(net_bin_host, net_bin_device)
        self._remote.upload(model_bin_host, model_bin_device)
        end = datetime.datetime.now()
        print(f"upload model finished, time {(end-start).seconds*1000 + (end-start).microseconds/1000} ms")
        return self.load(handle, net_bin_device, model_bin_device, batch_max, False, resize_type, tensor_num_per_batch)


    def un_load(self, module):
        self._remote.get_function("de.AiEngine.destroy")(module.handle)
        #self._remote.get_function("nn_unload_handle")(module.handle)

    def check_img_data(self, data, img_format, shapes):
        calc_len = 1
        for s in shapes:
            calc_len = calc_len * s

        if img_format == de.PixelFormat.DE_PIX_FMT_RGB888_PLANE \
                or img_format == de.PixelFormat.DE_PIX_FMT_BGR888_PLANE\
                or img_format == de.PixelFormat.DE_PIX_FMT_RGB888\
                or img_format == de.PixelFormat.DE_PIX_FMT_BGR888:
            calc_len = calc_len * 3
        elif img_format == de.PixelFormat.DE_PIX_FMT_NV12 \
                or img_format == de.PixelFormat.DE_PIX_FMT_NV21 \
                or img_format == de.PixelFormat.DE_PIX_FMT_YUV420P:
            calc_len = calc_len * 3 / 2
        elif img_format == de.PixelFormat.DE_PIX_FMT_YUV422_UYVY \
                or img_format == de.PixelFormat.DE_PIX_FMT_YUV422_YUYV:
            calc_len = calc_len * 2

        if isinstance(data, bytearray):
            img_len = len(data)
        elif isinstance(data, numpy.ndarray):
            img_len = len(bytearray(data))
        elif isinstance(data, str):
            img_len = len(bytearray(open(data, "rb").read()))

        if img_len == calc_len:
            return True

        print("Error: img file length: {} not equal to shapes len: {}".format(img_len, calc_len))
        return False

    def check_data(self, data, bits, shapes):
        calc_len = 1
        for s in shapes:
            calc_len = calc_len * s
        if bits == 16:
            calc_len = calc_len * 2
        elif bits == 32:
            calc_len = calc_len * 4

        if isinstance(data, str):
            data_len = len(open(data, "rb").read())
        elif isinstance(data, numpy.ndarray):
            data_len = len(bytearray(data))
        else:
            data_len = len(data)

        if data_len == calc_len:
            return True

        print("Error: data file length: {} not equal to shapes len: {}".format(data_len, calc_len))
        return False

    def data_to_ndarray(self, data, type_code, bits, shape_code, shapes):
        if self.check_data(data, bits, shapes) == False:
            return None

        blob = data
        if isinstance(data, str):
            blob = bytearray(open(data, "rb").read())
        elif isinstance(data, numpy.ndarray):
            blob = bytearray(data)

        nd = self._remote.get_function("NDArray.Create.From.Data")(blob, type_code.value, bits, shape_code.value, len(shapes), *shapes)

        return nd

    def to_ndarray(self, data, img_format, shapes):

        if self.check_img_data(data, img_format, shapes) == False:
            return None

        if isinstance(data, bytearray):
            blob = self._remote.get_function("NDArray.Create.From.Img")(data, img_format, len(shapes), *shapes)
            nd = self._remote.get_function("de.mm.jpeg.decoder.decode")(blob)

            if (numpy.size(nd.asnumpy()) == 0):
                print("JpegDecoder not support!!")
            #nd = jpeg.jpeg_codec(self._remote).tvm_jpeg_decode(blob)
            return nd
        elif isinstance(data, numpy.ndarray):
            blob = bytearray(data)
            nd = self._remote.get_function("NDArray.Create.From.Img")(blob, img_format, len(shapes), *shapes)
            return nd
        elif isinstance(data, str):
            blob = bytearray(open(data, "rb").read())

            nd = self._remote.get_function("NDArray.Create.From.Img")(blob, img_format, len(shapes), *shapes)
            return nd


