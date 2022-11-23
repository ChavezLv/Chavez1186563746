import sys
import cv2
import os
import multiprocessing
import datetime
import numpy as np
import de
from infer_engine import InferEngine, load_image
from common import resize
from acctest.dataset.coco2014 import coco, convert_pred_transform_yolov3_caffe, convert_pred_yolov3_caffe, nms

if __name__ == "__main__":
    print("InferEngine example start...", flush=True)

    net_file = "/DEngine/model/dp1000/public/1nnp/caffe_yolov3_416/net.bin"
    model_file = "/DEngine/model/dp1000/public/1nnp/caffe_yolov3_416/model.bin"
    img_dir = "/DEngine/data/datasets/coco_val2014/val2014"
   
    size = 416
    # 配置resize_shape到模型输入大小
    resize_shape = (size, size)

    engine = InferEngine(net_file, model_file, max_batch=8)

    # 以JPEG图像输入
    # format = de.PixelFormat.DE_PIX_FMT_JPEG
    # 以RGB图像输入
    format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE

    test_num = 20
    test_id = 0
    for img_name in os.listdir(img_dir)[0:test_num]:
        test_id += 1
        img_path = os.path.join(img_dir, img_name)

        print("")
        print("===========================================")
        print(f"test id {test_id}, image {img_name}")

        if img_path[-3:] != "jpg":
            continue
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

        # 打印结果输出
        print(f"predict result: data_out len={len(data_out)}")
        for i in range(len(data_out)):
            print(f"data_out[{i}], shape={data_out[i].shape}, dtype={data_out[i].dtype}")

        # 后处理
        pred_lbbox = data_out[0].reshape((1,) + data_out[0].shape)
        pred_sbbox = data_out[1].reshape((1,) + data_out[1].shape)
        pred_mbbox = data_out[2].reshape((1,) + data_out[2].shape)

        # 保存数据
        os.system("mkdir -p yolov3_result")
        out_file = "yolov3_result/" + os.path.basename(img_name).split(".")[0]
        # np.save(out_file + "_pred_sbbox", pred_sbbox)
        # np.save(out_file + "_pred_mbbox", pred_mbbox)
        # np.save(out_file + "_pred_lbbox", pred_lbbox)

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
        score_threshold = 0.5
        nms_thresh = 0.45
        im = cv2.imread(img_path)
        bboxes = convert_pred_yolov3_caffe(pred_bbox, size, (im.shape[0], im.shape[1]), (0, np.inf), score_threshold)
        bboxes = nms(bboxes, score_threshold, nms_thresh, method='nms')
        # print(f"box_shape={bboxes[0].shape} boxes={bboxes}")
        for i in range(len(bboxes)):
            b = bboxes[i].tolist()
            print(f"box[{i}]={b}")
            if b[5] == 0.0:
                color = (0, 255, 0) # 人体用绿色表示
            else:
                color = (0, 0, 255) # 其他用红色表示
            cv2.rectangle(im, (int(b[0]), int(b[1])), (int(b[2]), int(b[3])), color, 2)
        cv2.imwrite(out_file + ".jpg", im)

        if len(bboxes) == 0:
            print("this image no detect box")
            continue
        dets_boxes = bboxes[0][np.newaxis,:]
        for kk in range(1, len(bboxes)):
            dets_boxes = np.concatenate([dets_boxes, bboxes[kk][np.newaxis,:]], axis=0)

        class_num = 80
        all_boxes = [[] for _ in range(class_num)]
        for j in range(1, class_num):
            inds = np.where(dets_boxes[:, 5] == j-1)[0]
            cls_box = dets_boxes[inds,:5]
            all_boxes[j] = cls_box

        # Limit to max_per_image detections *over all classes*
        max_per_image = 400
        if max_per_image > 0:
            image_scores = np.hstack([all_boxes[j][:, -1]
                                      for j in range(1, class_num)])
            if len(image_scores) > max_per_image:
                image_thresh = np.sort(image_scores)[-max_per_image]
                for j in range(1, class_num):
                    keep = np.where(all_boxes[j][:, -1] >= image_thresh)[0]
                    all_boxes[j] = all_boxes[j][keep, :]

        #print(f"boxes={all_boxes}")

    engine.profile()
