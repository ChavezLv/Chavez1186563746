import sys
import cv2
import os
import multiprocessing
import datetime
import numpy as np
import de
from infer_engine import InferEngine, load_image
from ...dataset.voc2007 import pascal_voc, vis_detections, cpu_nms

if __name__ == "__main__":
    print("InferEngine example start...", flush=True)

    net_file = "/DEngine/model/dp1000/public/4nnp/caffe_mobilenet-ssd/net.bin"
    model_file = "/DEngine/model/dp1000/public/4nnp/caffe_mobilenet-ssd/model.bin"
    img_dir = "/DEngine/data/datasets/VOC2007/JPEGImages/"
    # 如果模型不配置resize，需要resize到模型输入大小
    resize = None
    # resize = 416, 416

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
            img = load_image(img_path, resize=resize)
            shape = img.shape

        # 执行推理，data为每个样本的输入列表，batch时可配置多data
        data = [(format, shape, img)]
        data_out = engine.predict(data)

        # 打印结果输出
        print(f"predict result: data_out len={len(data_out)}")
        for i in range(len(data_out)):
            print(f"data_out[{i}], shape={data_out[i].shape}, dtype={data_out[i].dtype}")

        # 后处理
        score_threshold = 0.01
        h = img.shape[2]
        w = img.shape[3]
        print(f"img h{h}, w{w}")
        bboxes = []
        class_num = 21
        data = data_out[0].reshape(-1, 6)
        all_boxes = [[] for _ in range(class_num)]
        print(f"predict result: data len={len(data)}")
        if data.shape[0] > 0:
            det_label = data[:, 0]
            det_conf = data[:, 1]
            det_xmin = data[:, 2]
            det_ymin = data[:, 3]
            det_xmax = data[:, 4]
            det_ymax = data[:, 5]
            # all_boxes_ = []
            for j in range(1, class_num):
                inds = np.where(det_label == j-1)[0]
                det_conf_this = det_conf[inds]
                det_xmin_this = det_xmin[inds]
                det_ymin_this = det_ymin[inds]
                det_xmax_this = det_xmax[inds]
                det_ymax_this = det_ymax[inds]

                score_ind = np.where(det_conf_this > score_threshold)[0]
                det_xmin_this = det_xmin_this[score_ind]
                det_ymin_this = det_ymin_this[score_ind]
                det_xmax_this = det_xmax_this[score_ind]
                det_ymax_this = det_ymax_this[score_ind]
                det_conf_this = det_conf_this[score_ind]

                if len(score_ind) > 0:
                    top_xmin = det_xmin_this[0] * w
                    top_ymin = det_ymin_this[0] * h
                    top_xmax = det_xmax_this[0] * w
                    top_ymax = det_ymax_this[0] * h
                    score = det_conf_this[0]
                    #print(det_xmin_this, ' ', det_ymin_this, ' ', det_xmax_this, ' ', det_ymax_this)
                    result = np.array([[top_xmin, top_ymin, top_xmax, top_ymax, score]])
                else:
                    continue
                for ii in range(1, len(score_ind)):
                    top_xmin = det_xmin_this[ii] * w
                    top_ymin = det_ymin_this[ii] * h
                    top_xmax = det_xmax_this[ii] * w
                    top_ymax = det_ymax_this[ii] * h
                    score = det_conf_this[ii]
                    thisr = np.array([[top_xmin, top_ymin, top_xmax, top_ymax, score]])
                    result = np.concatenate([result, thisr], axis=0)

                keep = cpu_nms(result, 0.45)
                bboxes = result[keep,:]
                all_boxes[j] = bboxes

        im = cv2.imread(img_path)
        class_idx = 0
        CLASSES = ('background',
           'aeroplane', 'bicycle', 'bird', 'boat',
           'bottle', 'bus', 'car', 'cat', 'chair',
           'cow', 'diningtable', 'dog', 'horse',
           'motorbike', 'person', 'pottedplant',
           'sheep', 'sofa', 'train', 'tvmonitor')
        for boxes in all_boxes:
            for box in boxes:
                print(f"box={box}")
                color = (0, 0, 255) # 其他用红色表示
                cv2.rectangle(im, (int(box[0]), int(box[1])), (int(box[2]), int(box[3])), color, 2)
                text = "%s:%.2f" % (CLASSES[class_idx], box[4])
                color_text = (0, 255, 0)
                cv2.putText(im, text, (int(box[0]), int(box[1])), cv2.FONT_HERSHEY_SIMPLEX, 0.75, color_text, 2)
            class_idx += 1

        # 保存数据
        os.system("mkdir -p ssd_result")
        out_file = "ssd_result/" + os.path.basename(img_name).split(".")[0]
        cv2.imwrite(out_file + ".jpg", im)

        if len(all_boxes) == 0:
            print("this image no detect box")
    engine.profile()
