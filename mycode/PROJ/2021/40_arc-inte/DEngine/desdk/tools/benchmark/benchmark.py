# coding=utf-8 
import sys, cv2
import de
import sys
import os
import logging
import time
import multiprocessing
import threading

import numpy as np
from de import rpc
from de.contrib import util
from user_struct import  *
from nn import nn_deploy as nn
import datetime
import json
import collections


threadLock = threading.Lock()

def dict2list(dic):
    a=[]
    for k,v in dic.items():
        d = {}
        d[k]=v
        # print(d)
        a.append(d)
    # print(a)
    return a

def mergelists(la,lb):
    size = min ( len( la ), len( lb ))
    c = []
    for i in range(size):
        if la:
            c.append( la.pop(0) )
        if lb:
            c.append( lb.pop(0) )
    return c

def test_init():
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

def get_labels(file_path):
    """
    get contents from file
    """
    with open(file_path, 'r') as f:
        labels = [l.rstrip() for l in f]
        return labels

def graph_eval_result_proc(tensor, imgname, val_lab_dict, syn_lab, topn=5):
    """
    graph evaluate result proc
    """
    assert topn >= 0
    a = np.argsort(tensor.flatten())[::-1]
    # print(a)
    flag1 = False
    flag5 = False
    predict_sym_info = ""
    check_id = val_lab_dict[imgname]
    for idx in a[0:topn]:
        if check_id == idx:
            predict_sym_info = syn_lab[idx]
            flag5 = True
            flag1 = idx == a[0]
    return flag1, flag5, predict_sym_info


def deal_image_use_cv2_method0(paths, net_size, rgb_en=1, interp=None, gray_en=False):
    """use cv2.INTER_LINER resize"""
    assert cv2 is not None, '`load_img` requires `cv2`.'
    if interp is None:
        interp = cv2.INTER_LINEAR
    if not isinstance(paths, list):
        paths = [paths]

    def _load_img(path):
        # imread:BGR, HWC
        img = cv2.imread(path)
        
        wh_tuple = (net_size[1], net_size[0])
        # resize the img to target W/H, the params of resize must be (w, h)
        if img.shape[1::-1] != wh_tuple:
            img = cv2.resize(img, wh_tuple, interpolation=interp)

        if gray_en :
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

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

'''
def deal_image_use_cv2_method0(paths, net_size, rgb_en=1, gray_en=1, interp=None):
    """use cv2.INTER_LINER resize"""
    assert cv2 is not None, '`load_img` requires `cv2`.'
    if interp is None:
        interp = cv2.INTER_LINEAR
    if not isinstance(paths, list):
        paths = [paths]

    def _load_img(path):
        # imread:BGR, HWC
        img = cv2.imread(path)
        wh_tuple = (net_size[1], net_size[0])
        # resize the img to target W/H, the params of resize must be (w, h)
        if img.shape[1::-1] != wh_tuple:
            img = cv2.resize(img, wh_tuple, interpolation=interp)
        # convert BGR to RGB if enable
        if rgb_en:
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
'''
def classification_accuracy_onecase(nn_deploy, netpath, modelpath, datasets_path, rgb_en, inshape, val_label_dict, synsets_label, topn, num_criteria):
    total = 0.0
    positive1 = 0.0
    positive5 = 0.0
    nn_deploy = test_init()
    module = nn_deploy.load_host(netpath, modelpath, 1)
    while 1:
        for imgname in os.listdir(datasets_path):
            print(imgname)
            imgpath = os.path.join(datasets_path, imgname)
            print(imgpath)
            total = total + 1

            #norm resize
            data = deal_image_use_cv2_method0(imgpath, (inshape[0], inshape[1]), rgb_en=rgb_en)
            # print(data)
            if rgb_en:
                nd0 = nn_deploy.to_ndarray(data.astype('uint8'),
                                        de.PixelFormat.DE_PIX_FMT_RGB888_PLANE, [inshape[0], inshape[1]])
            else:
                nd0 = nn_deploy.to_ndarray(data.astype('uint8'),
                                        de.PixelFormat.DE_PIX_FMT_BGR888_PLANE, [inshape[0], inshape[1]])
            print("set model input ...... ")
            module.batch_num = 1
            module.dsp_split = 1
            module.set_input(nd0)
            print("model run ...... ")
            module.run()
            print("get output of model ...... ")
            nd_out = module.get_outputs()
            print("output processing ...... ")
            tvm_output = nd_out[0]
            #print(tvm_output.reshape(1,1,1000))
            print(tvm_output.shape)
            # print(tvm_output)
            np.array(tvm_output).tofile("chip_out_f16.bin")
            flag1, flag5, model_info = graph_eval_result_proc(tvm_output, imgname, val_label_dict, synsets_label, topn)
            #print("max score = ", max(tvm_output.asnumpy()[0, :]))
            if flag1:
                positive1 = positive1 + 1
            if flag5:
                positive5 = positive5 + 1
            print("current total = ", total)
            if total%100.0 == 0.0:
                print("current total = ", total)
                print("\n current top1 = ", positive1 / total)
                print("\n current top5 = ", positive5 / total)
            if total >= num_criteria:
                break
        if total >= num_criteria:
            break
    print("\nfinish total = ", total)
    print("\n top1 = ", positive1 / total)
    print("\n top5 = ", positive5 / total)
    nn_deploy.un_load(module)
    return total, positive1 / total, positive5 / total

def test_model_pred_fps(nn_deploy, netpath, modelpath, datasets_path, rgb_en, inshape,  batch_list, time_criteria, num_criteria):
    fps_list = []
    module = nn_deploy.load_host(netpath, modelpath, 8)
    for bt in batch_list:
        img_num = 0
        run_time = 0.0
        stop_flag = False
        while 1:
            for imgname in os.listdir(datasets_path):
                imgpath = os.path.join(datasets_path, imgname)
                print(imgpath)
                
                #norm resize
                data = deal_image_use_cv2_method0(imgpath, (inshape[0], inshape[1]), rgb_en=rgb_en)
                # print(data)

                if rgb_en:
                    nd0 = nn_deploy.to_ndarray(data.astype('uint8'),
                                              de.PixelFormat.DE_PIX_FMT_RGB888_PLANE, [inshape[0], inshape[1]])
                else:
                    nd0 = nn_deploy.to_ndarray(data.astype('uint8'),
                                              de.PixelFormat.DE_PIX_FMT_BGR888_PLANE, [inshape[0], inshape[1]])
                module.dsp_split = 1
                   
                if bt == 1:
                    module.set_input(nd0)
                    module.batch_num = 1
                elif bt == 4:
                    module.set_input(nd0, nd0, nd0, nd0)
                    module.batch_num = 4
                elif bt == 8:
                    module.set_input(nd0, nd0, nd0, nd0, nd0, nd0, nd0, nd0)
                    module.batch_num = 8
                else:
                    print("batch num err!!!")
                    sys.exit(-1)
                start = time.time()
                module.run()  
                img_num += bt
                end = time.time()
                print("module run cost {} seconds. run_time {} img_num {}".format(end - start, run_time, img_num))
                run_time += end-start
                if run_time >= time_criteria or img_num > num_criteria:
                    print("===stop===")
                    stop_flag = True
                    break
            if stop_flag:
                break
        print("batchsize {}  fps = {}.".format(bt, img_num/run_time))
        fps_list.append(img_num/run_time)
    return fps_list

def classification_accuracy_onecase_graph(dev_id, netpath, modelpath, datasets_path, rgb_en, inshape, val_label_dict, synsets_label, topn, num_criteria, input_quesize):
    total = 0.0
    positive1 = 0.0
    positive5 = 0.0
    de.get_global_func("de.benchmark.load_model")(dev_id, netpath, modelpath)
    graph = de.get_global_func("de.benchmark.predict.init")(netpath, modelpath, num_criteria, 1, 1, 1, 1, 0, 0, 4, 1, 0, input_quesize)
    img_id = 0
    img_list = []
    while 1:
        for imgname in os.listdir(datasets_path):
            print(imgname)
            imgpath = os.path.join(datasets_path, imgname)
            data = deal_image_use_cv2_method0(imgpath, (inshape[0], inshape[1]), rgb_en=rgb_en)
            img_id += 1
            total = total + 1
            if rgb_en:
                de.get_global_func("de.benchmark.predict.setinput")(graph, bytearray(data.astype('uint8')), img_id, inshape[0], inshape[1], de.PixelFormat.DE_PIX_FMT_RGB888_PLANE,1)
            else:
                de.get_global_func("de.benchmark.predict.setinput")(graph, bytearray(data.astype('uint8')), img_id, inshape[0], inshape[1], de.PixelFormat.DE_PIX_FMT_BGR888_PLANE,1)
            img_list.append(imgname)
            if img_id >= num_criteria:
                break
        if img_id >= num_criteria:
            break
    for i in range(num_criteria):
        tvm_output = de.get_global_func("de.benchmark.predict.output")(graph, i, 0).asnumpy()
        np.array(tvm_output).tofile("chip_out_f16_graph.bin")
        flag1, flag5, model_info = graph_eval_result_proc(tvm_output, img_list[i], val_label_dict, synsets_label, topn)
        if flag1:
            positive1 = positive1 + 1
        if flag5:
            positive5 = positive5 + 1

    print("\nfinish total = ", total)
    print("\n top1 = ", positive1 / total)
    print("\n top5 = ", positive5 / total)
    de.get_global_func("de.benchmark.predict.output.del")(graph)
    print("output.del")
    de.get_global_func("de.benchmark.unload_model")(dev_id, netpath)
    print("unload")
    return total, positive1 / total, positive5 / total

def test_model_pred_fps_graph(dev_id, netpath, modelpath, input_way_num, datasets_formats, datasets_path, rgb_en, gray_en, raw_data, inshape,  batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list,input_quesize, interval_type, interval_time):
    print("====== fps enter =========")
    fps_list = []
    delay_list = []
    sr_list = []
    chip_decode = False
    use_raw_file = [False]*input_way_num
    #0 int 1 uint 2 float
    raw_file_type =[0]*input_way_num
    dicts = [{}]*input_way_num
    data_list = []

    for way in range(input_way_num):
        if datasets_formats[way] != "image":
            use_raw_file[way] = True
            if datasets_formats[way] == "float":
                raw_file_type[way] = 2
    
    image_format = de.PixelFormat.DE_PIX_FMT_BGR888_PLANE
    if (rgb_en):
        image_format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE

    for way in range(input_way_num):
        for imgname in os.listdir(datasets_path[way]):
            imgpath = os.path.join(datasets_path[way], imgname)
            if use_raw_file[way]:
                data = bytearray(open(imgpath, "rb").read())
            else:
                data = bytearray(deal_image_use_cv2_method0(imgpath, (inshape[0], inshape[1]), rgb_en=rgb_en, gray_en=gray_en))
            dicts[way][imgname] = data
        print("====== {} load ok, format {} =========".format(datasets_path[way],datasets_formats[way]))

    if way > 1:
        data_list=mergelists(dicts[0],dicts[1])
    else:
        data_list=dict2list(dicts[0])

    #no interval
    if interval_type == 0:
        interval_list = [0]*num_criteria
    #fixed interval
    if interval_type == 1:
        interval_list = [interval_time]*num_criteria
	#possion distribution interval
    if interval_type == 2:
        interval_list = np.random.poisson(interval_time,num_criteria).tolist()
        print(interval_list)

    #batchlist idx
    idx = 0
    for bt in batch_list:
        interval_idx = 0	
        delay = 0
        sr = [0.0, 0.0, 0.0, 0.0]
        graph = de.get_global_func("de.benchmark.predict.init")(netpath, modelpath, num_criteria, bt, bt, 1, 0, chip_decode, 0, cur_resource_num_list[idx], 2**cur_nnp_num_list[idx] - 1, dev_id, input_quesize, interval_type, interval_time)
        print("====== [{}] init ok batchsize max {} batchnow {} input {} resource{} nnp {}=========".format(dev_id, 1, 1, 1, cur_resource_num_list[idx], 2**cur_nnp_num_list[idx] - 1))
        img_id = 0
        start = time.time()
        while 1:
            for j in range(0, len(data_list), input_way_num):
                #get model inputs once, for 2 inputs model get 2 inputs to tmplist
                tmplist = data_list[j: j + input_way_num]
                img_id += 1
                if img_id == num_criteria:
                    print("====== all input cost {} =========".format(time.time() - start))
                #set inputs due to batchsize    
                for i in range(bt):
                    last_one_pkt = False
                    if i == bt-1:
                        last_one_pkt = True

                    b = time.time()
                    for tmpdict in tmplist:
                        for k,v in tmpdict.items():
                            print(k)
                            data=v
                            if datasets_formats[j%input_way_num] == "image":
                                if (raw_data):
                                    de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, len(data), 1, de.PixelFormat.DE_PIX_FMT_NONE, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                                else:
                                    de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, inshape[0], inshape[1], image_format, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                            else:
                                de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, len(data), 1, de.PixelFormat.DE_PIX_FMT_NONE, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                    print("===== setinput cost {} ======".format(time.time() - b))
                
                if img_id >= num_criteria:
                    print("====== setinput ok =========")
                    run_time = time.time() - start
                    totaldelay = 0
                    for i in range(num_criteria):
                        totaldelay += de.get_global_func("de.benchmark.predict.delay")(graph, i)
                    delay = totaldelay/num_criteria
                    sr[0] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 1)
                    sr[1] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 2)
                    sr[2] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 4)
                    sr[3] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 8)
                    de.get_global_func("de.benchmark.predict.output.del")(graph)
                    break
                time.sleep(interval_list[interval_idx]/1000)
                interval_idx = interval_idx + 1
            if img_id >= num_criteria:
                break
        fps = bt*img_id/run_time
        print("===== batchsize {}  fps = {}. =====".format(bt, fps))
        fps_list.append(fps)
        delay_list.append(delay)
        sr_list.append(sr)
        idx+=1
    return fps_list, delay_list, sr_list


def test_model_pred_fps_graph_onchip(dev_id, netpath, modelpath, input_way_num, datasets_formats, datasets_path, rgb_en, gray_en, raw_data, inshape,  batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list,input_quesize, interval_type, interval_time):
    print("====== onchip fps enter =========")
    
    fps_list = []
    delay_list = []
    sr_list = []
    chip_decode = False
    use_raw_file = [False]*input_way_num
    #0 int 1 uint 2 float
    raw_file_type =[0]*input_way_num
    dicts = [{}]*input_way_num
    data_list = []

    for way in range(input_way_num):
        if datasets_formats[way] != "image":
            use_raw_file[way] = True
            if datasets_formats[way] == "float":
                raw_file_type[way] = 2
    
    image_format = de.PixelFormat.DE_PIX_FMT_BGR888_PLANE
    if (rgb_en):
        image_format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE

    for way in range(input_way_num):
        for imgname in os.listdir(datasets_path[way]):
            imgpath = os.path.join(datasets_path[way], imgname)
            if use_raw_file[way]:
                data = bytearray(open(imgpath, "rb").read())
            else:
                data = bytearray(deal_image_use_cv2_method0(imgpath, (inshape[0], inshape[1]), rgb_en=rgb_en, gray_en=gray_en))
            dicts[way][imgname] = data
        print("====== {} load ok, format {} =========".format(datasets_path[way],datasets_formats[way]))

    if way > 1:
        data_list=mergelists(dicts[0],dicts[1])
    else:
        data_list=dict2list(dicts[0])

    #no interval
    if interval_type == 0:
        interval_list = [0]*num_criteria
    #fixed interval
    if interval_type == 1:
        interval_list = [interval_time]*num_criteria
	#possion distribution interval
    if interval_type == 2:
        interval_list = np.random.poisson(interval_time,num_criteria).tolist()
        print(interval_list)
        
    #batchlist idx
    idx = 0
    for bt in batch_list:
        interval_idx = 0	
        delay = 0
        sr = [0.0, 0.0, 0.0, 0.0]
        graph = de.get_global_func("de.benchmark.predict.init")(netpath, modelpath, num_criteria, bt, bt, 1, 0, chip_decode, 1, cur_resource_num_list[idx], 2**cur_nnp_num_list[idx] - 1, dev_id, input_quesize, interval_type, interval_time)
        print("====== [{}] init ok batchsize max {} batchnow {} input_way_num {} resource{} nnp {}=========".format(dev_id, bt, bt, input_way_num, cur_resource_num_list[idx], 2**cur_nnp_num_list[idx] - 1))
        
        time.sleep(1)
        img_id = 0
        start = time.time()
        
        for j in range(0, len(data_list), input_way_num):
            #get model inputs once, for 2 inputs model get 2 inputs to tmplist
            tmplist = data_list[j: j + input_way_num]

            img_id += 1
            #set inputs due to input_way_num    
            for i in range(input_way_num):
                last_one_pkt = 0
                if i == input_way_num-1:
                    last_one_pkt = 1

                b = time.time()
                for tmpdict in tmplist:
                    for k,v in tmpdict.items():
                        print(k)
                        # print(use_raw_file[j%input_way_num])
                        # print(raw_file_type[j%input_way_num])
                        data=v
                        if datasets_formats[j%input_way_num] == "image":
                            if (raw_data):
                                de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, len(data), 1, de.PixelFormat.DE_PIX_FMT_NONE, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                            else:
                                de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, inshape[0], inshape[1], image_format, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                        else:
                            de.get_global_func("de.benchmark.predict.setinput")(graph, data, img_id, len(data), 1, de.PixelFormat.DE_PIX_FMT_NONE, last_one_pkt, use_raw_file[j%input_way_num], raw_file_type[j%input_way_num])
                print("===== setinput cost {} ======".format(time.time() - b))
                
            fps = de.get_global_func("de.benchmark.get.chip.result")(graph,0)
            delay = de.get_global_func("de.benchmark.get.chip.result")(graph,1)
            fps_list.append(fps)
            delay_list.append(delay)
            run_time = time.time() - start
 
            sr[0] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 1)
            sr[1] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 2)
            sr[2] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 4)
            sr[3] = de.get_global_func("de.benchmark.dspprofile.get")(graph, 0, 8)
            sr_list.append(sr)
            de.get_global_func("de.benchmark.predict.output.del")(graph)
            time.sleep(3)
            break
        idx+=1
    return fps_list, delay_list, sr_list


class FpsThread (threading.Thread):
    def __init__(self, threadID, name, fpsfunc, dev_id, netpath, modelpath, input_way_num, datasets_formats, datasets_paths, rgb_en, gray_en, raw_data, inshape,  batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list,input_quesize,interval_type, interval_time):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.fps_list, self.delay_list, self.sr_list = [],[],[]
        self.fpsfunc = fpsfunc
        self.dev_id, self.netpath, self.modelpath, self.input_way_num, self.datasets_formats, self.datasets_paths, self.rgb_en, self.gray_en, self.raw_data, self.inshape,  self.batch_list, self.time_criteria, self.num_criteria, self.cur_nnp_num_list, self.cur_resource_num_list, self.input_quesize, self.interval_type, self.interval_time = dev_id, netpath, modelpath, input_way_num, datasets_formats, datasets_paths, rgb_en, gray_en, raw_data, inshape,  batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list, input_quesize, interval_type, interval_time
    def run(self):
        print (str(self.dev_id) + " start thread: " + self.name)
        self.fps_list, self.delay_list, self.sr_list= self.fpsfunc(self.dev_id, self.netpath, self.modelpath, self.input_way_num, self.datasets_formats, self.datasets_paths, self.rgb_en, self.gray_en, self.raw_data, self.inshape,  self.batch_list, self.time_criteria, self.num_criteria, self.cur_nnp_num_list, self.cur_resource_num_list,self.input_quesize,self.interval_type, self.interval_time)
        print (str(self.dev_id) + " exit thread: " + self.name)
 

class DeviceThread (threading.Thread):
    def __init__(self, thread_id, name, dev_id, benchmark_dict, test_result_dict):
        threading.Thread.__init__(self)
        self.thread_id = thread_id
        self.name = name
        self.dev_id = dev_id
        self.benchmark_dict = benchmark_dict
        self.test_result_dict = test_result_dict
    def run(self):
        print ("Device" + str(self.dev_id) + " start thread: " + self.name)
        fps_benchmark(self.dev_id, self.benchmark_dict, self.test_result_dict)
        print ("Device" + str(self.dev_id) + " exit thread: " + self.name)


def accuracy_benchmark(dev_id,test_suite_dict, test_result_dict):
    if dev_id != 0:
        return
    datasets_path = test_suite_dict["accuracy_datasets_path"]
    synset_path = test_suite_dict["synset_path"]
    val_path = test_suite_dict["val_path"]    
    models_root = test_suite_dict["models_root"]
    topn = test_suite_dict["topn"]
    time_criteria = test_suite_dict["time_criteria"]
    num_criteria = test_suite_dict["num_criteria"]
    input_quesize = test_suite_dict["input_quesize"]
    test_models = collections.OrderedDict(test_suite_dict["test_models"])

    accuracy_test = test_suite_dict["accuracy_test"]
    
    if accuracy_test:
        assert os.path.isdir(datasets_path), "datasets named ILSVRC2012_img_val must be in folder {}".format(datasets_path)
        assert os.path.isfile(synset_path), "synset_1000.txt must be in folder {}".format(synset_path)
        assert os.path.isfile(val_path), "val.txt must be in folder {}".format(val_path)
        # get labels from txt
        synsets_label = get_labels(synset_path)
        val_label = get_labels(val_path)
        val_label_dict = {cur_label.split(" ")[0]: int(cur_label.split(" ")[1]) for cur_label in val_label}	
        for md in test_models:
            if test_models[md]["do_test"]:
                if md not in test_result_dict:
                    test_result_dict[md] = {}
                if 1 not in test_result_dict[md]:
                    test_result_dict[md][1] = {}
                shape = test_models[md]["shape"]
                rgb_en = test_models[md]["rgb"]
                inshape = shape
                netpath =  os.path.join(models_root, md ,"net.bin")
                modelpath = os.path.join(models_root, md,"model.bin")
                
                print("{} accuracy test start".format(md))
                total, top1, top5 = classification_accuracy_onecase_graph(dev_id, netpath,
                                 modelpath, datasets_path, rgb_en, inshape, val_label_dict, synsets_label, topn, num_criteria, input_quesize)
                test_result_dict[md][1]["top1"] = top1
                test_result_dict[md][1]["top5"] = top5
                print("{} accuracy test end".format(md))

def fps_benchmark(dev_id, test_suite_dict, test_result_dict):
    fps_datasets_formats = test_suite_dict["fps_datasets_formats"]
    fps_datasets_paths = test_suite_dict["fps_datasets_paths"]
    models_root = test_suite_dict["models_root"]
    batch_list = test_suite_dict["batch_list"]
    thread_list = test_suite_dict["thread_list"]
    nnp_num_list = test_suite_dict["nnp_num_list"]
    resource_num_list = test_suite_dict["resource_num_list"]
    time_criteria = test_suite_dict["time_criteria"]
    num_criteria = test_suite_dict["num_criteria"]
    test_models = collections.OrderedDict(test_suite_dict["test_models"])
    input_quesize = test_suite_dict["input_quesize"]
    fps_test = test_suite_dict["fps_test"]
    interval_type = test_suite_dict["interval_type"]
    interval_time = test_suite_dict["interval_time"]
    end2end_test = test_suite_dict["end2end_test"]
    chip_test = test_suite_dict["chip_test"]
    
    input_way_num = 1
    if fps_test:
        for md in test_models:
            if test_models[md]["do_test"]:
                if md not in test_result_dict:
                    test_result_dict[md] = {}
                shape = test_models[md]["shape"]
                rgb_en = test_models[md]["rgb"]
                if "raw_data" in test_models[md]:
                    raw_data = test_models[md]["raw_data"]
                else:
                    raw_data = False
                if "gray" in test_models[md]:
                    gray_en = test_models[md]["gray"]
                else:
                    gray_en = False
                inshape = shape
                netpath =  os.path.join(models_root, md ,"net.bin")
                modelpath = os.path.join(models_root, md,"model.bin")
                modelinfopath = os.path.join(models_root, md,"model_info.json")

                de.get_global_func("de.benchmark.load_model")(dev_id, netpath, modelpath)
                print("model load ok {}", netpath)
                
                if "thread_list" in test_models[md]:
                    cur_thread_list = test_models[md]["thread_list"]
                else:
                    cur_thread_list = thread_list
                
                if "batch_list" in test_models[md]:
                    cur_batch_list = test_models[md]["batch_list"]
                else:
                    cur_batch_list = batch_list

                if "nnp_num_list" in test_models[md]:
                    cur_nnp_num_list = test_models[md]["nnp_num_list"]
                else:
                    cur_nnp_num_list = nnp_num_list

                if "resource_num_list" in test_models[md]:
                    cur_resource_num_list = test_models[md]["resource_num_list"]
                else:
                    cur_resource_num_list = resource_num_list
                
                if "input_way_num" in test_models[md]:
                    input_way_num = test_models[md]["input_way_num"]

                for thd_num in cur_thread_list:
                    if thd_num not in test_result_dict[md]:
                        test_result_dict[md][thd_num] = {}
                    if end2end_test:
                        threads = []
                        for n in range(thd_num):
                            thread1 = FpsThread(n, "Thread-"+str(n), test_model_pred_fps_graph, dev_id, netpath,
                                            modelpath, input_way_num, fps_datasets_formats, fps_datasets_paths, rgb_en, gray_en, raw_data, inshape, cur_batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list, input_quesize, interval_type, interval_time)
                            thread1.start()
                            threads.append(thread1)
                        fps_lists=[]
                        delay_lists=[]
                        sr_lists=[]
                        for t in threads:
                            t.join()
                            fps_lists.append(t.fps_list)
                            delay_lists.append(t.delay_list)
                            sr_lists.append(t.sr_list)
                        
                        test_result_dict[md][thd_num]["nnp"] = dict(zip(cur_batch_list,cur_nnp_num_list))
                        test_result_dict[md][thd_num]["resource"] = dict(zip(cur_batch_list,cur_resource_num_list))
                        test_result_dict[md][thd_num]["fps"] = dict(zip(cur_batch_list, np.sum(fps_lists, axis = 0).tolist()))
                        test_result_dict[md][thd_num]["delay"]= dict(zip(cur_batch_list, (np.sum(delay_lists, axis = 0)/thd_num).tolist()))
                        test_result_dict[md][thd_num]["chip_srdict"] = dict(zip(cur_batch_list, (np.sum(sr_lists, axis = 0)/thd_num).tolist()))
                    
                    print("****************************************")
                    if chip_test:
                        threads = []
                        for n in range(thd_num):
                            thread1 = FpsThread(n, "Thread-"+str(n), test_model_pred_fps_graph_onchip, dev_id, netpath,
                                            modelpath, input_way_num, fps_datasets_formats, fps_datasets_paths, rgb_en, gray_en, raw_data, inshape, cur_batch_list, time_criteria, num_criteria, cur_nnp_num_list, cur_resource_num_list, input_quesize, interval_type, interval_time)
                            thread1.start()
                            threads.append(thread1)
                        fps_lists=[]
                        delay_lists=[]
                        sr_lists=[]
                        for t in threads:
                            t.join()
                            fps_lists.append(t.fps_list), 
                            delay_lists.append(t.delay_list)
                            sr_lists.append(t.sr_list)
                        
                        test_result_dict[md][thd_num]["nnp"] = dict(zip(cur_batch_list,cur_nnp_num_list))
                        test_result_dict[md][thd_num]["resource"] = dict(zip(cur_batch_list,cur_resource_num_list))
                        test_result_dict[md][thd_num]["chip_fps"] = dict(zip(cur_batch_list, np.sum(fps_lists, axis = 0).tolist()))
                        test_result_dict[md][thd_num]["chip_delay"]= dict(zip(cur_batch_list, (np.sum(delay_lists, axis = 0)/thd_num).tolist()))
                        test_result_dict[md][thd_num]["chip_srdict"] = dict(zip(cur_batch_list, (np.sum(sr_lists, axis = 0)/thd_num).tolist()))

                    test_result_dict[md][thd_num]["end2end_test"] = end2end_test
                    test_result_dict[md][thd_num]["chip_test"] = chip_test

                    if os.path.exists(modelinfopath):
                        modelinfo_dict = json.load(open(modelinfopath))
                        if "FLOPs" in modelinfo_dict:
                            test_result_dict[md][thd_num]["flops"] = modelinfo_dict["FLOPs"]
                        else:
                            test_result_dict[md][thd_num]["flops"] = 0
                    else:
                        test_result_dict[md][thd_num]["flops"] = 0

                #net.bin second int32 * batchsize as memory need
                binfile = open(netpath, 'rb') #打开二进制文件
                size = os.path.getsize(netpath) #获得文件大小
                if size > 8:
                    data = binfile.read(4)
                    data = binfile.read(4) #the second 4 bytes
                    memsize = int.from_bytes(data, "little")
                    for thd_num in cur_thread_list:
                        test_result_dict[md][thd_num]["memsize"] = memsize/1024.0/1024.0
                binfile.close()

                de.get_global_func("de.benchmark.unload_model")(dev_id, netpath)
                print("{} test result: {} ".format(md, test_result_dict[md]))

def detection_accuracy_benchmark(detect_dict, detect_result_dict):
    pass

def test_benchmark():
    nn = test_init()
    nn.get_remote().get_function("set.dsp.log.level")(0, 5)
    nn.get_remote().get_function("set.dsp.log.level")(1, 5)
    nn.get_remote().get_function("de.set.log.level")(5)
    test_result_dict_total = collections.OrderedDict()
    
    if len(sys.argv) > 1:
        print(f"benchmark run with param {sys.argv[1]}")
        benchmark_json = json.load(open(sys.argv[1]))
    else:
        print(f"benchmark run with benchmark.json")
        benchmark_json = json.load(open("benchmark.json"))
    device_num = benchmark_json["device_num"]
    handles = []
    end2end_test=False
    chip_test=False
    for dev_id in range(device_num):
        handles.append(de.get_global_func("de.benchmark.load.library")(dev_id, "host:/DEngine/desdk/platform/dev_linux-dp1000/lib/libbenchmark.so"))
    threads = []
    for dev_id in range(device_num):
        test_result_dict = {}
        for test_suite_dict in benchmark_json["test_suites"]:
            print(test_suite_dict)
            end2end_test = test_suite_dict["end2end_test"]
            chip_test = test_suite_dict["chip_test"]
            if dev_id == 0:
                accuracy_benchmark(dev_id, test_suite_dict, test_result_dict)
            thread1 = DeviceThread(dev_id, "Thd"+str(dev_id), dev_id, test_suite_dict, test_result_dict)
            thread1.start()
            threads.append(thread1)
    #wait and merge result
    for t in threads:
        t.join()
        for md in t.test_result_dict:
            if md in test_result_dict_total:
                for thd in test_result_dict_total[md]:
                    if end2end_test:
                        for bt in test_result_dict_total[md][thd]["fps"]:
                            test_result_dict_total[md][thd]["fps"][bt] += t.test_result_dict[md][thd]["fps"][bt]
                    if chip_test:
                        for bt in test_result_dict_total[md][thd]["chip_fps"]:
                            test_result_dict_total[md][thd]["chip_fps"][bt] += t.test_result_dict[md][thd]["chip_fps"][bt]
            else:
                test_result_dict_total[md] = t.test_result_dict[md]

    for dev_id in range(device_num):
        de.get_global_func("de.benchmark.unload.library")(dev_id, handles[dev_id])
    json.dump(test_result_dict_total, open("benchmark_result.txt", "w"))
    summary_print(test_result_dict_total)
    write_to_excel(test_result_dict_total, end2end_test, chip_test)

def summary_print(test_result_dict):
    print(test_result_dict)

def worksheet_merge(worksheet, row_start_idx, col_start_idx, row_end_idx, col_end_idx, content, style):
    if row_start_idx == row_end_idx and col_start_idx == col_end_idx:
        worksheet.write(row_start_idx, col_start_idx, content, style)
    else:
        worksheet.merge_range(row_start_idx, col_start_idx, row_end_idx, col_end_idx, content, style)

def write_to_excel(classi_result_dict, end2end_test, chip_test):
    import xlsxwriter, time
    time_str = time.strftime("%Y%m%d_%H%M%S", time.localtime())
    if len(sys.argv) > 1:
        excel_file_name = sys.argv[1] + ".xlsx"
    else:
        excel_file_name = "benchmark.xlsx"
    if os.path.exists(excel_file_name):
        os.remove(excel_file_name)
    workbook = xlsxwriter.Workbook(excel_file_name)
    worksheet = workbook.add_worksheet(time_str)

    style_header = workbook.add_format({'font_size': 9,
                                        'border': 1,
                                        'bold': 1,
                                        'bg_color': '#B4C6E7',
                                        'align': 'center',
                                        'valign': 'vcenter'})
    style_text = workbook.add_format({'font_size': 9,
                                        'border': 1,
                                        'align': 'center'})
    style_number = workbook.add_format({'font_size': 9,
                                        'border': 1,
                                        'align': 'center'})
    #表头
    worksheet.merge_range('A1:A2','模型类型',style_header)
    worksheet.merge_range('B1:B2','模型名称',style_header)
    worksheet.merge_range('C1:C2','测试集',style_header)
    worksheet.merge_range('D1:F1','精度',style_header)
    worksheet.write('D2','top1',style_header)
    worksheet.write('E2','top5',style_header)
    worksheet.write('F2','mAP',style_header)
    worksheet.merge_range('G1:G2','模型算力',style_header)
    worksheet.merge_range('H1:H2','线程数',style_header)
    worksheet.merge_range('I1:I2','Batch',style_header)
    worksheet.merge_range('J1:J2','NNP',style_header)
    worksheet.merge_range('K1:K2','RES',style_header)
    worksheet.merge_range('L1:L2','芯片时延(ms)',style_header)
    worksheet.merge_range('M1:M2','芯片吞吐(fps)',style_header)
    worksheet.merge_range('N1:N2','MAC利用率',style_header)
    worksheet.merge_range('O1:O2','主控时延(ms)',style_header)
    worksheet.merge_range('P1:P2','主控吞吐(fps)',style_header)
    worksheet.merge_range('Q1:T1','NNP utilization',style_header)
    worksheet.write('Q2','NNP1',style_header)
    worksheet.write('R2','NNP2',style_header)
    worksheet.write('S2','NNP3',style_header)
    worksheet.write('T2','NNP4',style_header)
    worksheet.merge_range('U1:U2','DDR(M)',style_header)

    worksheet.set_column("A:A", 15)
    worksheet.set_column("B:B", 20)
    worksheet.set_column("C:C", 15)
    worksheet.set_column("D:F", 8)
    worksheet.set_column("G:G", 10)
    worksheet.set_column("H:H", 8)
    worksheet.set_column("I:I", 8)
    worksheet.set_column("J:J", 8)
    worksheet.set_column("K:K", 8)
    worksheet.set_column("L:L", 10)
    worksheet.set_column("M:M", 10)
    worksheet.set_column("N:N", 10)
    worksheet.set_column("O:O", 10)
    worksheet.set_column("P:P", 10)
    worksheet.set_column("Q:T", 10)
    worksheet.set_column("U:U", 10)

    #分类模型结果
    row_start_idx = 2
    col_start_idx = 1
    model_num = len(classi_result_dict)
    if model_num > 0:
        row_idx = row_start_idx
        for md in classi_result_dict:
            threadlist_sorted = sorted(classi_result_dict[md].keys())
            for thdnum in threadlist_sorted:
                col_idx = col_start_idx
                batch_len = 1
                if "fps" in classi_result_dict[md][thdnum] :
                    fps_dict = classi_result_dict[md][thdnum]["fps"]
                    batch_len = len(fps_dict)
                if "chip_fps" in classi_result_dict[md][thdnum] :
                    chip_fps_dict = classi_result_dict[md][thdnum]["chip_fps"]
                    batch_len = len(chip_fps_dict)
                worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, md, style_text)
                col_idx += 1
                worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, "ILSVRC2012", style_text)
                col_idx += 1
                if "top1" in classi_result_dict[md][thdnum]:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, classi_result_dict[md][thdnum]["top1"], style_text)
                else:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, "", style_text)
                col_idx += 1
                if "top5" in classi_result_dict[md][thdnum]:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, classi_result_dict[md][thdnum]["top5"], style_text)
                else:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, "", style_text)
                col_idx += 1
                if "mAP" in classi_result_dict[md][thdnum]:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, classi_result_dict[md][thdnum]["mAP"], style_text)
                else:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, "", style_text)
                col_idx += 1
                if "flops" in classi_result_dict[md][thdnum]:
                    worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, classi_result_dict[md][thdnum]["flops"], style_text)
                col_idx += 1
                worksheet_merge(worksheet, row_idx, col_idx, row_idx + batch_len - 1, col_idx, thdnum, style_text)

                if "fps" in classi_result_dict[md][thdnum] or "chip_fps" in classi_result_dict[md][thdnum]:
                    batchlist_sorted=[]
                    chip_fps_dict={}
                    fps_dict={}
                    chip_sr_dict={}
                    if end2end_test:
                        fps_dict = classi_result_dict[md][thdnum]["fps"]
                        delay_dict = classi_result_dict[md][thdnum]["delay"]
                        batchlist_sorted = sorted(fps_dict.keys())
                        chip_sr_dict = classi_result_dict[md][thdnum]["chip_srdict"]
                    if chip_test:
                        chip_fps_dict = classi_result_dict[md][thdnum]["chip_fps"]
                        chip_delay_dict = classi_result_dict[md][thdnum]["chip_delay"]
                        batchlist_sorted = sorted(chip_fps_dict.keys())
                        chip_sr_dict = classi_result_dict[md][thdnum]["chip_srdict"]
                    nnp_dict = classi_result_dict[md][thdnum]["nnp"]
                    resource_dict = classi_result_dict[md][thdnum]["resource"]
                    
                    for btsize in batchlist_sorted:
                        worksheet.write(row_idx, 8, btsize, style_number)
                        worksheet.write(row_idx, 9, nnp_dict[btsize], style_number)
                        worksheet.write(row_idx, 10, resource_dict[btsize], style_number)
                        if end2end_test:
                            worksheet.write(row_idx, 14, delay_dict[btsize], style_number)
                            worksheet.write(row_idx, 15, fps_dict[btsize], style_number)
                            # NNP 750MHz下算力为1.4Tops，1GHz下算力为1.792Tops
                            worksheet.write(row_idx, 13, classi_result_dict[md][thdnum]["flops"]*fps_dict[btsize]/(1400/4*nnp_dict[btsize]), style_number)
                        if chip_test: # 如果两个测试都执行，最终MAC利用率为芯片测试的
                            worksheet.write(row_idx, 11, chip_delay_dict[btsize], style_number)
                            worksheet.write(row_idx, 12, chip_fps_dict[btsize], style_number)
                            # NNP 750MHz下算力为1.4Tops，1GHz下算力为1.792Tops
                            worksheet.write(row_idx, 13, classi_result_dict[md][thdnum]["flops"]*chip_fps_dict[btsize]/(1400/4*nnp_dict[btsize]), style_number)
                        worksheet.write(row_idx, 16, chip_sr_dict[btsize][0], style_number)
                        worksheet.write(row_idx, 17, chip_sr_dict[btsize][1], style_number)
                        worksheet.write(row_idx, 18, chip_sr_dict[btsize][2], style_number)
                        worksheet.write(row_idx, 19, chip_sr_dict[btsize][3], style_number)
                        worksheet.write(row_idx, 20, classi_result_dict[md][thdnum]["memsize"]*(int(btsize)), style_number)
                        row_idx += 1
                else:
                    worksheet.write(row_idx, 8, "", style_text)
                    worksheet.write(row_idx, 9, "", style_text)
                    worksheet.write(row_idx, 10, "", style_text)
                    worksheet.write(row_idx, 11, "", style_text)
                    worksheet.write(row_idx, 12, "", style_text)
                    worksheet.write(row_idx, 13, "", style_text)
                    worksheet.write(row_idx, 14, "", style_text)
                    worksheet.write(row_idx, 15, "", style_text)
                    worksheet.write(row_idx, 16, "", style_text)
                    worksheet.write(row_idx, 17, "", style_text)
                    worksheet.write(row_idx, 18, "", style_text)
                    worksheet.write(row_idx, 19, "", style_text)
                    worksheet.write(row_idx, 20, "", style_text)
                    row_idx += 1
        worksheet_merge(worksheet, row_start_idx, 0, row_idx - 1, 0, "分类模型", style_text)
    #检测模型
    workbook.close()


if __name__ == "__main__":
    # test_result_dict = json.load(open("benchmark_result.txt"))
    # write_to_excel(test_result_dict,0,1)
    test_benchmark()


    
