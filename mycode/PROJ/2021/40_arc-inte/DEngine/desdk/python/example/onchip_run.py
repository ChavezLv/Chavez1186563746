import sys, cv2
import de
import os
import logging
import time
import multiprocessing
import json

import numpy as np
from de import rpc
from de.contrib import util
from user_struct import  *
# from mm import jpeg_codec as jpeg
from nn import nn_deploy as nn
from enum import Enum
import datetime
import json

from collections import namedtuple
from net_bin_analys import *

class InputType(Enum):
    IMAGE = 0
    DATA = 1

# from common import *
def deal_image_use_cv2_method0(paths, net_size, rgb_en=1, interp=None, gray_en=False):
    """use cv2.INTER_LINER resize"""
    assert cv2 is not None, '`load_img` requires `cv2`.'
    if interp is None:
        interp = cv2.INTER_LINEAR
    if not isinstance(paths, list):
        paths = [paths]

    def _load_img(path):
        # imread:BGR, HWC
        if gray_en :
            img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
        else:
            img = cv2.imread(path)
        
        wh_tuple = (net_size[1], net_size[0])
        # resize the img to target W/H, the params of resize must be (w, h)
        if img.shape[1::-1] != wh_tuple:
            img = cv2.resize(img, wh_tuple, interpolation=interp)

        #if not rgb_en
        #    img = cv2.cvtColor(img, cv2.COLOR_BGR2YUV)

        # convert BGR to RGB if enable
        if rgb_en and not gray_en :
            img = img[:, :, ::-1]
        if len(img.shape) == 2:
            img = np.expand_dims(img, -1)
        # cur img shape is HWC, RGB or BGR format, the min(H, W) is target_size
        return img

    if len(paths) > 1:
        imgs = np.zeros((len(paths),) + (3,), dtype=np.float32)
        for (i, path) in enumerate(paths):
            imgs[i] = _load_img(path)
    else:
        imgs = np.array([_load_img(paths[0])], dtype=np.float32)

    if imgs is None:
        return None

    # change img shape to BCHW
    imgs = np.swapaxes(imgs, 1, 3)
    imgs = np.swapaxes(imgs, 2, 3)

    return imgs

def similarity(d2, d3):

    num = float(d2.dot(d3.T))
    denom = np.linalg.norm(d2) * np.linalg.norm(d3)
    cos = num / denom  # 余弦值
    sim = 0.5 + 0.5 * cos  # 根据皮尔逊相关系数归一化
    return sim

def ref_compare(out, ref):
    result_list = []
    if len(out) != len(ref):
        print('-- Test failed: len_ref = {}, len_test = {}'.format(len(ref), len(out)))
        return False, 0.0
    
    if len(out) > 1:
        sim = similarity(np.array(out), np.array(ref))
    else:
        sim = 1 - abs((out[0] - ref[0]) / ref[0])
    success = True
    if not sim >= 0.98:
        success = False
        for val, refval in zip(out, ref):
            if abs(refval) > 0.000039:
                if abs((val - refval) / refval) >= 0.03 and len(result_list) < 10:
                    result_list.append("ref val = {}, test_val = {}".format(refval, val))
    if success:
        print("-- Test success: data check succ!")
        return True, sim
    else:
        print("-- Test failed: data check failed! detail info is: {}".format(result_list))
        return False, sim

def init():
    if os.environ.get('TARGET_CH') == "net":
        print("Try to connect using net.")
        f = open("/DEngine/net.cfg", 'r')
        net_cfg = f.read()
        f.close()
        a = net_cfg.split(':')
        ip = a[0]
        port = int(a[1])
        print("connect to {}:{}".format(ip, port))
        remote = nn.device_init(0, ip, port)
    else:
        print("Try to connect using usb.")
        remote = nn.device_init(0)
    nn_deploy = nn.NNDeploy(remote)
    return nn_deploy

def print_analysis_info(batch, time_info, model_info_json_path=None):

    # "实际运行的NNP核数"
    if batch >= 4:
        real_core_count = 4
    else:
        real_core_count = batch

    time_total = float(time_info["model_time"])
    cyc_nnp = float(time_info["nnp_time"])
    cyc_dsp = float(time_info["dsp_time"])
    cyc_crgh = float(time_info["crgh_time"])
    cyc_sched = float(time_info["sched_time"])
    cyc_kcf = float(time_info["kcf_time"])
    cyc_total = cyc_nnp + cyc_dsp + cyc_crgh + cyc_sched + cyc_kcf

    time_nnp = time_total * cyc_nnp / cyc_total
    time_dsp = time_total * cyc_dsp / cyc_total
    time_crgh = time_total * cyc_crgh / cyc_total
    time_sched = time_total * cyc_sched / cyc_total
    time_kcf = time_total * cyc_kcf / cyc_total

    nnp_flops = 0

    if model_info_json_path is not None and os.path.exists(model_info_json_path) :
        with open(model_info_json_path, 'r') as fp:
            js = json.load(fp)
            fp.close()
        if 'NNP_GOPs' in js : 
            nnp_flops = js['NNP_GOPs']
    
    batch_nnp_flops = nnp_flops * batch
    nnp_use_ratio = batch_nnp_flops / time_nnp / real_core_count * 4 / 1.344 * 100
    fps = batch / time_total * 1000

    print("\n\033[42m----------------------------Info------------------------------------------\n")

    print('{:20}{:22}{}'.format("Name", "Time(millisecond)", "Percent"))
    print('{:20}{:20}'.format("time_total", str(time_total)))
    percent = "{:8.2f}".format(100 * time_nnp / time_total)
    print('{:20}{:20}{}%'.format("time_nnp", str(time_nnp), percent))
    percent = "{:8.2f}".format(100 * time_dsp / time_total)
    print('{:20}{:20}{}%'.format("time_dsp", str(time_dsp), percent))
    percent = "{:8.2f}".format(100 * time_crgh / time_total)
    print('{:20}{:20}{}%'.format("time_crgh", str(time_crgh), percent))
    percent = "{:8.2f}".format(100 * time_sched / time_total)
    print('{:20}{:20}{}%'.format("time_sched", str(time_sched), percent))
    percent = "{:8.2f}".format(100 * time_kcf / time_total)
    print('{:20}{:20}{}%'.format("time_kcf", str(time_kcf), percent))

    print()
    if model_info_json_path != None :
        print('{:20}{}'.format("batch", str(batch)))
        print('{:20}{}'.format("nnp_GOPs", str(nnp_flops)))
        percent = "{:3.2f}".format(nnp_use_ratio)
        print('{:20}{}%'.format("nnp_use_ratio", percent))
        print('{:20}{}'.format("fps", str(fps)))

    print("--------------------------------------------------------------------------\033[0m\n")
    sys.stdout.flush()

def test_run_model(nn_deploy, model_path, shape_list, img_path=None, batch=[1], compare=True, dec_onchip=True, model_info_json_path=None, gray_en=False, input_type=InputType.IMAGE):
    # test_models_dict = {}
    # model_dict = {}
    # model_dict["model_path"] = model_path
    # model_dict["img_format"] = ImgFormat
    # model_dict["shape_list"] = shape_list
    # index = model_path.rfind("/")
    # test_models_dict[model_path[index:]] = model_dict
    # json.dump(test_models_dict, open("test_models_cfg", "a"))
    # return
    result = True
    #加载模型, 注意，如果两次加载不同的模型，文件名不能相同
    module = nn_deploy.load_host(model_path + "/net.bin", model_path +"/model.bin", batch_max = max(batch))
    print("model load complete.\n")
    sys.stdout.flush()

    if img_path is None:
        #定点输入
        nd0 = nn_deploy.to_ndarray(model_path + "/data.bin", de.PixelFormat.DE_PIX_FMT_NONE, shape_list)
        #浮点输入
        #nd0 = nn_deploy.data_to_ndarray(model_path + "/data.bin", de.TypeCode.Float, 32, de.ShapeCode.NDHW, shape_list)
        print(f"[runonchip] use data.bin, shape={nd0.shape}")
    else:
        #打开一副jpeg图片，并转为标准输入格式
        if dec_onchip:
            jpeg_stream = bytearray(open(img_path, "rb").read())
            nd0 = nn_deploy.to_ndarray(jpeg_stream, de.PixelFormat.DE_PIX_FMT_NONE, [len(jpeg_stream)])
        else:
            in_data = deal_image_use_cv2_method0(img_path, shape_list, rgb_en=True, gray_en=gray_en)
            print("[runonchip] use image {}".format(img_path))

            if input_type == InputType.DATA:
                nd0 = nn_deploy.to_ndarray(in_data.astype('uint8'), de.PixelFormat.DE_PIX_FMT_NONE, [in_data.size])
            elif gray_en:
                nd0 = nn_deploy.to_ndarray(in_data.astype('uint8'), de.PixelFormat.DE_PIX_FMT_GRAY8, shape_list)
            else:
                nd0 = nn_deploy.to_ndarray(in_data.astype('uint8'), de.PixelFormat.DE_PIX_FMT_RGB888_PLANE, shape_list)
                
    sys.stdout.flush()

    run_info_file = os.path.join(model_path, "run_info.txt")
    fp = open(run_info_file, "w+")
    fp.write("model path: {}  compare: {}\n".format(model_path, compare))
    
    for batch in batch:
        if batch == 1:
            #设置模型的输入
            module.set_input(nd0)
            #设置batch
            module.batch_num = 1
        elif batch == 4:
            module.set_input(nd0, nd0, nd0, nd0)
            module.batch_num = 4
        elif batch == 8:
            module.set_input(nd0, nd0, nd0, nd0, nd0, nd0, nd0, nd0)
            module.batch_num = 8
        else:
            print("batch num err!!!")
            sys.exit(-1)
        
        #配置1表示batch4 分到两个dsp执行，0为单个dsp执行
        module.dsp_split = 1
        #执行模型
        module.run()
        print("model running...")
        sys.stdout.flush()
        
        nd_out = module.get_outputs()
        out_num = len(nd_out)
        print(f"[runonchip] output_num={out_num}")
        for i in range(out_num):
            print(f"output {i}, shape={nd_out[i].shape}, dtype={nd_out[i].dtype}")

        #获取模型输出结果，结果为numpy.ndarray的list
        if batch == 1 and compare is True:
            
            total_out = []
            for idx in range(out_num):
                total_out = np.concatenate((total_out, nd_out[idx].flatten()), axis=0)

            np.array(total_out).astype("float32").tofile(os.path.join(model_path, "chip_out.bin"))
            total_out.tofile(os.path.join(model_path, "chip_out.txt"), "\n")
            
            #比对数据 

            tvm_fix_out_path = os.path.join(model_path, "tvm_fix_out.bin")
            if not os.path.exists(tvm_fix_out_path):
                tvm_fix_out_path = os.path.join(model_path, "out_ref.bin")

            if os.path.exists(tvm_fix_out_path):
                out_ref = np.fromfile(tvm_fix_out_path, dtype=np.float32)
                result, similarity = ref_compare(total_out.astype(np.float32), out_ref)
            else:
                result = False
                similarity = 0
            
            print("compare result {} img_path {} similarity={}".format(result, img_path, similarity))
            fp.write("compare result {} img_path {} similarity={}\n".format(result, img_path, similarity))
            
        sys.stdout.flush()

        #time_info = module.profile(model_path + "/net.bin")
        time_info, layer_time = module.profile()

        layer_info = get_layer_info(model_path + "/net.bin")

        LayerTimeInfo = namedtuple('LayerTimeInfo', ['layerNum', 'blockCycle', 'layer_name'])
        layer_time_info = []

        #f_nn = open(model_path + "/profile.txt", "a")
        for i in range(len(layer_info)):
            layer_time_info.append(LayerTimeInfo(i, layer_time[i].blockCycle, layer_info[i]))
            #x = [i, layer_time[i].blockCycle, layer_info[i]]
            #np.savetxt(f_nn, x, fmt='%.4d', newline='  ', delimiter=',')
            #f_nn.write("\n")
        #f_nn.close()
         
        #print(layer_time_info)

        print_analysis_info(batch, time_info, model_info_json_path)

    fp.close()
    
    #卸载模型
    nn_deploy.un_load(module)
    return result, nd_out

def test_loop_run_model(loop_times, nn_deploy, model_path, shape_list, img_path=None, batch=[1], compare=True, dec_onchip=True, model_info_json_path=None, gray_en=False):
    if loop_times <= 1:
        loop_times = 1

    for i in range(0, loop_times):
        print("Testing loop " + str(i) + "...")
        test_run_model(nn_deploy, model_path, shape_list, img_path, batch, compare, dec_onchip, model_info_json_path, gray_en=gray_en)
        print("Testing loop " + str(i) + " over.")