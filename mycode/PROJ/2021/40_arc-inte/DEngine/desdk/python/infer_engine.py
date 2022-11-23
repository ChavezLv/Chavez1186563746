# coding=utf-8 
import sys
import os
import logging
import time
import multiprocessing
import threading
import numpy as np
import datetime
import json
import collections
from PIL import Image

import de

def load_image(file, resize=None, rgb_en=True, gray_en=False):
  def load_image_(file, resize=None, rgb_en=True, gray_en=False):
    img = Image.open(file)

    if resize is not None:
      w, h = resize
      print('image resize to (' + str(w), ',' + str(h) + ')')
      img = img.resize((w, h))

    if gray_en:
      img = img.convert('L')

    #img.show()
    return img

  img = load_image_(file, resize, rgb_en, gray_en)
  mode = img.mode
  img = np.array(img)
  if len(img.shape) == 2:
      img = np.expand_dims(img, -1)

  if not gray_en and rgb_en and mode == 'BGR':
    # convert to RGB
    img = img[:, :, ::-1]

  # add batch size at index 0
  img = img[np.newaxis, :, :, :]

  # change img shape to BCHW
  img = np.swapaxes(img, 1, 3)
  img = np.swapaxes(img, 2, 3)

  return img

class InferEngine:
    def __init__(self, net_bin_path, model_bin_path, dev_id = 0, input_num = 1, max_batch = 1):
        self.dev_id_ = dev_id
        self.input_num_ = input_num
        self.t_ = [0 for i in range(7)]
        self.t_[0] = datetime.datetime.now()
        # self.handle_ = de.get_global_func("de.load_library")(dev_id, "host:/DEngine/desdk/platform/dev_linux-dp1000/lib/libinfer_engine.so")
        self.t_[1] = datetime.datetime.now()

        self.engine_ = de.get_global_func("de.infer_engine.create")(net_bin_path, model_bin_path, dev_id, max_batch)
        self.t_[2] = datetime.datetime.now()

    def __del__(self):
        de.get_global_func("de.infer_engine.destroy")(self.engine_)
        # de.get_global_func("de.unload_library")(self.dev_id_, self.handle_)

    # 输入数据的格式为args[batch][tensor]，每个tensor格式为(format,shape,data)
    def predict(self, *args):
        self.t_[3] = datetime.datetime.now()
        nd_in = []
        self.batch_num_ = 0
        for nps in args: # batch
            # print(f"nps={nps}")
            input_num = 0
            for np in nps: # tensor
                # print(f"np={np}")
                nd = self.to_ndarray(np[0], np[1], np[2])
                nd_in.append(nd)
                input_num += 1
            if input_num != self.input_num_:
                print(f"[ERROR] input num must be {self.input_num_}, current is {input_num}!")
            self.batch_num_ += 1
        self.t_[4] = datetime.datetime.now()
        
        tensor_num = de.get_global_func("de.infer_engine.predict")(self.engine_, self.input_num_, self.batch_num_, *nd_in)
        self.t_[5] = datetime.datetime.now()
        
        data_out = []
        for i in range(tensor_num):
            data_out.append(de.get_global_func("de.infer_engine.get_output")(self.engine_, i).asnumpy())
        self.t_[6] = datetime.datetime.now()
        
        return data_out

    def to_ndarray(self, format, shape, data):
        return de.get_global_func("de.to_ndarray")(format, shape[2], shape[3], bytearray(data.astype('uint8')))

    def profile(self):
        print("")
        print("profile:")
        print(f"[{self.t_[1]}] load library cost {(self.t_[1]-self.t_[0]).seconds*1000 + (self.t_[1]-self.t_[0]).microseconds/1000} ms")
        print(f"[{self.t_[2]}] engine create cost {(self.t_[2]-self.t_[1]).seconds*1000 + (self.t_[2]-self.t_[1]).microseconds/1000} ms")
        print(f"[{self.t_[4]}] data to ndarray cost {(self.t_[4]-self.t_[3]).seconds*1000 + (self.t_[4]-self.t_[3]).microseconds/1000} ms")
        print(f"[{self.t_[5]}] predict batch={self.batch_num_} cost {(self.t_[5]-self.t_[4]).seconds*1000 + (self.t_[5]-self.t_[4]).microseconds/1000} ms")
        print(f"[{self.t_[6]}] get output cost {(self.t_[6]-self.t_[5]).seconds*1000 + (self.t_[6]-self.t_[5]).microseconds/1000} ms")
