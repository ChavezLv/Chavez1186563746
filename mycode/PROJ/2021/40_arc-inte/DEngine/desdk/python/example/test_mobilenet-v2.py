import sys
import cv2
import os
import multiprocessing
import datetime
from common import resize
import numpy as np
import de
from infer_engine import InferEngine, load_image

def get_labels(file_path):
    """
    get contents from file
    :param file_path: str, file path
    :return: list of line contents
    """
    with open(file_path, 'r') as f:
        labels = [l.rstrip() for l in f]
        return labels

if __name__ == "__main__":
    print("InferEngine example start...", flush=True)

    net_file = "/DEngine/model/dp1000/public/4nnp/caffe_mobilenet-v2/net.bin"
    model_file = "/DEngine/model/dp1000/public/4nnp/caffe_mobilenet-v2/model.bin"
    img_dir = "/DEngine/data/datasets/ILSVRC2012/ILSVRC2012_img_val"
    # 设置模型输入尺寸
    size = 224
    # 配置resize_shape到模型输入大小
    resize_shape = (size, size)

    engine = InferEngine(net_file, model_file, max_batch=8)

    # 以JPEG图像输入
    # format = de.PixelFormat.DE_PIX_FMT_JPEG
    # 以RGB图像输入
    format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE

    test_num = 10
    test_id = 0
    for img_name in os.listdir(img_dir)[0:test_num]:
        test_id += 1
        img_path = os.path.join(img_dir, img_name)

        print("")
        print("===========================================")
        print(f"test id {test_id}, image {img_name}")

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

        # 打印结果输出
        print(f"predict result: data_out len={len(data_out)}")
        for i in range(len(data_out)):
            print(f"data_out[{i}], shape={data_out[i].shape}, dtype={data_out[i].dtype}")

        # 后处理
        a = np.argsort(-data_out[0].flatten())
        # 获取label
        datasets_path = "/DEngine/data/datasets/ILSVRC2012"
        synset_path = os.path.join(datasets_path, "synset_1000.txt")
        synsets_label = get_labels(synset_path)
        print(f"predict id = {a[0]}, label = {synsets_label[a[0]]}, prob = {data_out[0][a[0]]}", flush=True)

    engine.profile()



    

