import sys
import pickle
import cv2
import os
import datetime
from dataset.coco2014 import coco, convert_pred_transform_yolov3_caffe, convert_pred_yolov3_caffe, nms
import numpy as np
import de
from infer_engine import InferEngine, load_image
from common import resize
import logging
logger = logging.getLogger('acctest')

def coco_yolov3_det(model_path, net_path, datasets_path, test_img_nums=50000):

    """ coco deal """
    score_threshold = 0.01
    nms_thresh = 0.45
    data_path = os.path.join(datasets_path, "val2014")
    ann_file = os.path.join(datasets_path, "annotation/instances_minival2014.json")
    assert os.path.isdir(data_path), "datasets must exist folder {}".format(data_path)
    assert os.path.isfile(ann_file), "annotation.json must exist".format(ann_file)

    # coco deal
    print("prepare datasets...", flush=True)
    coco_class = coco(data_path, ann_file, '2014')
    num_images = len(coco_class.image_index)
    class_num = coco_class.num_classes
    t0 = datetime.datetime.now()
    all_boxes = [[[] for _ in range(num_images)] for _ in range(class_num)]
    t1 = datetime.datetime.now()
    print(f"prepare datasets cost {t1-t0}", flush=True)
    data_len = min(num_images, test_img_nums)
    if data_len < num_images:
        print(f"[warning] test num {data_len} is lower than dataset num {num_images}, may affect the results!")
    
    t2 = datetime.datetime.now()
    print(f"load host cost {t2-t1}", flush=True)
    print("start to run model.", flush=True)
    img_idx = 0
    total = 0
    
    output_dir = "result_yolov3"
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)
    
    # 设置模型输入大小
    size = 416
    # 配置resize_shape到模型输入大小
    resize_shape = (size, size)
  
    engine = InferEngine(net_path, model_path, max_batch=8)
  
    # 以JPEG图像输入，在芯片侧解码，对格式有要求，参考集成开发指南
    # format = de.PixelFormat.DE_PIX_FMT_JPEG
    # 以RGB图像输入
    format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE

    for img_idx in range(data_len):
        img_path = coco_class.image_path_at(img_idx)
        t0 = datetime.datetime.now()
    
        if format == de.PixelFormat.DE_PIX_FMT_JPEG:
            img = bytearray(open(img_path, "rb").read())
            shape = (1, 1, 1, len(img))
            img = np.array(img)
        else:
            img = resize.get_image_use_cv2_pad(imgpath=img_path, model_size=resize_shape, norm_en=0, rgb_en=1, nhwc=0, pad_value=128)
            img = np.array(img.astype('uint8'))
            shape = (1, 3, size, size)

        # 执行推理，data为每个样本的输入列表，batch时可配置多data
        data = [(format, shape, img)]
        data_out = engine.predict(data)

        # 模型后处理
        # print(f"output shape {nd_out[0].shape},{nd_out[1].shape},{nd_out[2].shape}")
        pred_lbbox = data_out[0].reshape((1,) + data_out[0].shape)
        pred_sbbox = data_out[1].reshape((1,) + data_out[1].shape)
        pred_mbbox = data_out[2].reshape((1,) + data_out[2].shape)
        # t2 = datetime.datetime.now()
        # print(f"get output cost {t2-t1}")

        if logger.level == logging.DEBUG:
            out_file = os.path.join(output_dir, os.path.basename(img_path).split(".")[0])
            out_file += "_" + module.device_type + "_" + str(data.dtype)
            np.save(out_file + "_pred_sbbox", pred_sbbox)
            np.save(out_file + "_pred_mbbox", pred_mbbox)
            np.save(out_file + "_pred_lbbox", pred_lbbox)

        '''change dim to 13x13x255 26x26x255 39x39x255'''
        pred_sbbox = np.swapaxes(pred_sbbox, 1, 3)
        pred_sbbox = np.swapaxes(pred_sbbox, 1, 2)
        pred_mbbox = np.swapaxes(pred_mbbox, 1, 3)
        pred_mbbox = np.swapaxes(pred_mbbox, 1, 2)
        pred_lbbox = np.swapaxes(pred_lbbox, 1, 3)
        pred_lbbox = np.swapaxes(pred_lbbox, 1, 2)

        biases = [10, 13, 16, 30, 33, 23, 30, 61, 62, 45, 59, 119, 116, 90, 156, 198, 373, 326];
        deal_sbbox = convert_pred_transform_yolov3_caffe(pred_sbbox, size, biases,6)
        deal_mbbox = convert_pred_transform_yolov3_caffe(pred_mbbox, size, biases,3)
        deal_lbbox = convert_pred_transform_yolov3_caffe(pred_lbbox, size, biases,0)

        pred_bbox = np.concatenate([deal_sbbox,deal_mbbox,deal_lbbox], axis=0)
        '''here do  score thread'''
        im = cv2.imread(img_path)
        bboxes = convert_pred_yolov3_caffe(pred_bbox, size, (im.shape[0], im.shape[1]), (0, np.inf), score_threshold)

        bboxes = nms(bboxes, score_threshold, nms_thresh, method='nms')

        # t3 = datetime.datetime.now()
        # print(f"postprocess cost {t3-t2}")

        if len(bboxes) == 0:
            print("this image no detect box")
            continue
        dets_boxes = bboxes[0][np.newaxis,:]
        for kk in range(1, len(bboxes)):
            dets_boxes = np.concatenate([dets_boxes, bboxes[kk][np.newaxis,:]], axis=0)

        for j in range(1, class_num):
            inds = np.where(dets_boxes[:, 5] == j-1)[0]
            cls_box = dets_boxes[inds,:5]
            all_boxes[j][img_idx] = cls_box

        # Limit to max_per_image detections *over all classes*
        max_per_image = 400
        if max_per_image > 0:
            image_scores = np.hstack([all_boxes[j][img_idx][:, -1]
                                      for j in range(1, class_num)])
            if len(image_scores) > max_per_image:
                image_thresh = np.sort(image_scores)[-max_per_image]
                for j in range(1, class_num):
                    keep = np.where(all_boxes[j][img_idx][:, -1] >= image_thresh)[0]
                    all_boxes[j][img_idx] = all_boxes[j][img_idx][keep, :]

        # t4 = datetime.datetime.now()
        # print(f"score cost {t4-t3}")
        total = total + 1
        print(f"{total}/{data_len} {img_path}", flush=True)


    if logger.level == logging.DEBUG:
        out_res_file = os.path.join(output_dir, "result.pkl")
        with open(out_res_file, "wb") as fb:
            pickle.dump(all_boxes, fb)
            
    result = "dataset: " + data_path + " test num: " + str(data_len) + "\n"
    # 计算mAP
    result += coco_class.evaluate_detections(all_boxes, output_dir)
    return result

if __name__ == "__main__":

    start = datetime.datetime.now()
    print(f"\ncaffe_yolov3 acctest start at {start}", flush=True)

    datasets_path = "/DEngine/data/datasets/coco_val2014"
    model_path = "/DEngine/model/dp1000/public/4nnp/caffe_yolov3_416/model.bin"
    net_path = "/DEngine/model/dp1000/public/4nnp/caffe_yolov3_416/net.bin"
    test_img_nums=5000
    coco_yolov3_det(model_path=model_path, net_path=net_path, test_img_nums=test_img_nums, datasets_path=datasets_path)
    end = datetime.datetime.now()
    print(f"\ncaffe_yolov3 acctest end at {end}, cost {end-start}\n", flush=True)

