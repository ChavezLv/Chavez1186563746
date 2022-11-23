import sys
import cv2
import os
import datetime
from common import resize
from dataset.ILSVRC2012 import get_labels, graph_eval_result_proc

import numpy as np
import de
from infer_engine import InferEngine, load_image
import logging
logger = logging.getLogger('acctest')

def ILSVRC2012_classification(model_path, net_path, datasets_path, test_img_nums=50000, topn=5):
    """
    get accuracy of mxnet classification models
    :param model_path: str,
    :param cfg_path: str, model quantization cfg.txt path
    :param datasets_path: str, the path of datasets for accuracy calc
    :param quantized: whether the model is quantized
    :param topn: int, topn result of models test
    :return: total_num, top1_acc, topn_acc
    """

    # change relative path to abs path
    datapath = os.path.join(datasets_path, "ILSVRC2012_img_val")
    synset_path = os.path.join(datasets_path, "synset_1000.txt")
    val_path = os.path.join(datasets_path, "val.txt")

    assert os.path.isdir(datapath), "datasets named ILSVRC2012_img_val must be in folder {}".format(datasets_path)
    assert os.path.isfile(synset_path), "synset_1000.txt must be in folder {}".format(datasets_path)
    assert os.path.isfile(val_path), "val.txt must be in folder {}".format(datasets_path)

    # get labels from txt
    synsets_label = get_labels(synset_path)
    val_label = get_labels(val_path)
    val_label_dict = {cur_label.split(" ")[0]: int(cur_label.split(" ")[1]) for cur_label in val_label}
    
    test_list = os.listdir(datapath)
    data_len = min(len(test_list), test_img_nums)
    if data_len < len(test_list):
        print(f"[warning] test num {data_len} is lower than dataset num {len(test_list)}, may affect the results!")
    total = 0
    positive1 = 0.0
    positive5 = 0.0

    # 设置模型输入尺寸
    size = 224
    # 配置resize_shape到模型输入大小
    resize_shape = (size, size)

    engine = InferEngine(net_path, model_path, max_batch=8)
  
    # 以JPEG图像输入
    # format = de.PixelFormat.DE_PIX_FMT_JPEG
    # 以RGB图像输入
    format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE
    
    for i in range(data_len):
        imgname = test_list[i]
        if imgname == "Thumbs.db":
            continue
        img_path = os.path.join(datapath, imgname)

        if format == de.PixelFormat.DE_PIX_FMT_JPEG:
            img = bytearray(open(img_path, "rb").read())
            shape = (1, 1, 1, len(img))
            img = np.array(img)
        else:
            img = resize.get_image_use_cv2_crop(imgpath=img_path, corp_shape=resize_shape, rgb_en=1, norm_en=0, interp=cv2.INTER_LINEAR)
            img = np.array(img.astype('uint8'))
            shape = (1, 3, size, size)

        # 执行推理，data为每个样本的输入列表，batch时可配置多data
        data = [(format, shape, img)]
        data_out = engine.predict(data)

        # 后处理计算top1/top5正确性
        flag1, flag5, model_info = graph_eval_result_proc(data_out, imgname, val_label_dict, synsets_label, topn)

        if flag1:
            positive1 = positive1 + 1
        if flag5:
            positive5 = positive5 + 1

        total = total + 1
        print(f"{total}/{test_img_nums} {imgname} top1:{flag1}, top5:{flag5}", flush=True)

    print(f"\n finish total = {total}")
    print(f"\n top1 = {positive1/total}")
    print(f"\n top5 = {positive5/total}")
    return total, positive1 / total, positive5 / total


if __name__ == "__main__":

    start = datetime.datetime.now()
    print(f"\ncaffe_resnet50 acctest start at {start}", flush=True)

    datasets_path = "/DEngine/data/datasets/ILSVRC2012"
    model_path = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/model.bin"
    net_path = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/net.bin"
    test_img_nums=5000

    ILSVRC2012_classification(model_path=model_path, net_path=net_path, test_img_nums=test_img_nums, datasets_path=datasets_path)
    end = datetime.datetime.now()
    print(f"\ncaffe_resnet50 acctest end at {end}, cost {end-start}\n", flush=True)
