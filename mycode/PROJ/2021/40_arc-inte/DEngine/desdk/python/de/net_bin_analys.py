

import struct
import os
import numpy as np

block_type_dict = {
	'NnpType': 0,  	    
	'CrghNnpType':1,	
	'DspType':2,  		
	'CrghType':3		
}

dsp_layer_type_dict = {
    'E_NN_DSP_LAYER_DATACVT': 0,

    'E_NN_DSP_LAYER_ALGO_CONV' : 1,
    'E_NN_DSP_LAYER_ALGO_DWCONV' : 2,
    'E_NN_DSP_LAYER_ALGO_POOL' : 3,
    'E_NN_DSP_LAYER_ALGO_FC' : 4,
    'E_NN_DSP_LAYER_ALGO_RELU' : 5,
    'E_NN_DSP_LAYER_ALGO_SIGMOID' : 6,
    'E_NN_DSP_LAYER_ALGO_SOFTRELU' : 7,
    'E_NN_DSP_LAYER_ALGO_TANH' : 8,
    'E_NN_DSP_LAYER_ALGO_LEAKY' : 9,
    'E_NN_DSP_LAYER_ALGO_ADD' : 10,
    'E_NN_DSP_LAYER_ALGO_PRELU' : 11,
    'E_NN_DSP_LAYER_ALGO_ELU' : 12,
    'E_NN_DSP_LAYER_ALGO_BN' : 13,
    'E_NN_DSP_LAYER_ALGO_L2NORMALIZE' : 14,
    'E_NN_DSP_LAYER_ALGO_SOFTMAX' : 15,

    'E_NN_DSP_LAYER_ALGO_RESIZE' : 16,
    'E_NN_DSP_LAYER_ALGO_PNETNMS' : 17,
    'E_NN_DSP_LAYER_ALGO_RNETNMS' : 18,
    'E_NN_DSP_LAYER_ALGO_ONETNMS' : 19,
    'E_NN_DSP_LAYER_ALGO_STN' : 20,

    'E_NN_DSP_LAYER_ALGO_TRANSPOSE' : 21,
    'E_NN_DSP_LAYER_ALGO_SLICE' : 22,
    'E_NN_DSP_LAYER_ALGO_CONCAT' : 23,
    'E_NN_DSP_LAYER_ALGO_PADDING' : 24,

    'E_NN_DSP_LAYER_ALGO_FE_COMPARE' : 25,
    'E_NN_DSP_LAYER_ALGO_OUTPUT' : 26,
    'E_NN_DSP_LAYER_ALGO_YOLONMS' : 27,

    'E_NN_DSP_LAYER_ALGO_SLICE_SIGMOID_CONCAT' : 28,
    'E_NN_DSP_LAYER_ALGO_SOFTMIN' : 29,
    'E_NN_DSP_LAYER_ALGO_SOFTSIGN' : 30,
    'E_NN_DSP_LAYER_ALGO_SELU' : 31,
    'E_NN_DSP_LAYER_ALGO_ROIPOOL' : 32,
    'E_NN_DSP_LAYER_ALGO_LRNORMALIZE' : 33,
    'E_NN_DSP_LAYER_ALGO_SLICE_LIKE' : 34,
    'E_NN_DSP_LAYER_ALGO_MULTIPLY' : 35,

    'E_NN_DSP_LAYER_ALGO_NMS' : 36,
    'E_NN_DSP_LAYER_ALGO_SSDNMS' : 37,
    'E_NN_DSP_LAYER_ALGO_MULTIBOX_PRIOR' : 38,
    'E_NN_DSP_LAYER_ALGO_PROPOSAL' : 39,
    'E_NN_DSP_LAYER_CVT_IMAGE_FORMAT' : 40,
    'E_NN_DSP_LAYER_TRACK_KCF' : 41,
    'E_NN_DSP_LAYER_ALGO_PSROIPOOL' : 42,

    'E_NN_DSP_LAYER_ALGO_EXP' : 43,
    'E_NN_DSP_LAYER_ALGO_LOG_SOFTMAX' : 44,
    'E_NN_DSP_LAYER_ALGO_ADAPTIVE_POOL' : 45,
    'E_NN_DSP_LAYER_ALGO_UPSAMPLE_POOL' : 46,
    'E_NN_DSP_LAYER_ALGO_ARG_INDEX' : 47,
    'E_NN_DSP_LAYER_ALGO_DECONV' : 48,
    'E_NN_DSP_LAYER_ALGO_EQUAL' : 49,
    'E_NN_DSP_LAYER_ALGO_CLIP' : 50,
    'E_NN_DSP_LAYER_ALGO_MOD' : 51,
    'E_NN_DSP_LAYER_ALGO_TOPK' : 52,
    'E_NN_DSP_LAYER_ALGO_RIGHT_SHIFT' : 53,
    'E_NN_DSP_LAYER_ALGO_MEAN' : 54,
    'E_NN_DSP_LAYER_ALGO_MAXIMUM' : 55,
    'E_NN_DSP_LAYER_ALGO_MINIMUM' : 56,
    'E_NN_DSP_LAYER_ALGO_GREATER' : 57,
    'E_NN_DSP_LAYER_ALGO_GREATER_EQUAL' : 58,
    'E_NN_DSP_LAYER_ALGO_TILE' : 59,
    'E_NN_DSP_LAYER_ALGO_TAKE' : 60,
    'E_NN_DSP_LAYER_ALGO_ABS' : 61,
    'E_NN_DSP_LAYER_ALGO_NEGATE' : 62,
    'E_NN_DSP_LAYER_ALGO_LOG' : 63,
    'E_NN_DSP_LAYER_ALGO_SQRT' : 64,
    'E_NN_DSP_LAYER_ALGO_RSQRT' : 65,
    'E_NN_DSP_LAYER_ALGO_FLOOR' : 66,
    'E_NN_DSP_LAYER_ALGO_CEIL' : 67,
    'E_NN_DSP_LAYER_ALGO_ROUND' : 68,
    'E_NN_DSP_LAYER_ALGO_TRUNC' : 69,
    'E_NN_DSP_LAYER_ALGO_SIGN' : 70,
    'E_NN_DSP_LAYER_ALGO_ROIALIGN' : 71,
    'E_NN_DSP_LAYER_ALGO_FLIP' : 72,
    'E_NN_DSP_LAYER_ALGO_RESIZE1' : 73,
    'E_NN_DSP_LAYER_ALGO_TRANSPOSE1' : 74,
    'E_NN_DSP_LAYER_ALGO_LEFT_SHIFT' : 75,
    'E_NN_DSP_LAYER_ALGO_LESS' : 76,
    'E_NN_DSP_LAYER_ALGO_LESS_EQUAL' : 77,
    'E_NN_DSP_LAYER_ALGO_NOT_EQUAL' : 78,
    'E_NN_DSP_LAYER_ALGO_POWER' : 79,
    'E_NN_DSP_LAYER_ALGO_FULL' : 80,
    'E_NN_DSP_LAYER_ALGO_SQUEEZE' : 81,
    'E_NN_DSP_LAYER_ALGO_EXPAND_DIMS' : 82,
    'E_NN_DSP_LAYER_ALGO_COPY' : 83,
    'E_NN_DSP_LAYER_ALGO_STACK' : 84,
    'E_NN_DSP_LAYER_ALGO_WHERE' : 85,
    'E_NN_DSP_LAYER_ALGO_RESHAPE' : 86,
    'E_NN_DSP_LAYER_ALGO_BATCH_FLATTEN' : 87,
    'E_NN_DSP_LAYER_ALGO_MAX' : 88,
    'E_NN_DSP_LAYER_ALGO_MIN' : 89,
    'E_NN_DSP_LAYER_ALGO_REPEAT' : 90,
    'E_NN_DSP_LAYER_ALGO_LOGICAL_AND' : 91,
    'E_NN_DSP_LAYER_ALGO_LOGICAL_OR' : 92,
    'E_NN_DSP_LAYER_ALGO_LOGICAL_NOT' : 93,
    'E_NN_DSP_LAYER_ALGO_VARIANCE' : 94,
    'E_NN_DSP_LAYER_ALGO_SUM' : 95,
    'E_NN_DSP_LAYER_ALGO_ALL' : 96,

    'E_NN_DSP_LAYER_ALGO_BOX_DECODE' : 97,
    'E_NN_DSP_LAYER_ALGO_DEPTH_TO_SPACE' : 98,
    'E_NN_DSP_LAYER_ALGO_SPACE_TO_DEPTH' : 99,
    'E_NN_DSP_LAYER_ALGO_LOGICAL_XOR' : 100,
    'E_NN_DSP_LAYER_ALGO_ANY' : 101,

    'E_NN_DSP_LAYER_MAX_TYPE' : 102
}
def get_block_num(netbin):
    block_num0 = netbin[0]
    block_num1 = netbin[1]
    block_num2 = netbin[2]
    block_num3 = netbin[3]
    a = block_num3<<24
    a |= block_num2<<16
    a |= block_num1<<8
    a |= block_num0
    #print("block_num: ",block_num0, block_num1, block_num2, block_num3, a)
    return a

def read_netbin(netbin_path):
    net_bin = np.fromfile(netbin_path, dtype = np.uint8)
    return net_bin

def get_layer_num_and_type(netbin, block_idx):
    offset = 32  
    for i in range(block_idx):
        block_size0 = netbin[offset+8]
        block_size1 = netbin[offset+9]
        block_size2 = netbin[offset+10]
        block_size3 = netbin[offset+11]
        a = block_size3<<24
        a |= block_size2<<16
        a |= block_size1<<8
        a |= block_size0
        offset += a
    return netbin[offset+2], netbin[offset+4]

def get_layer_type(netbin, block_idx, layer_idx):
    offset = 32
    for i in range(block_idx):
        block_size0 = netbin[offset+8]
        block_size1 = netbin[offset+9]
        block_size2 = netbin[offset+10]
        block_size3 = netbin[offset+11]
        a = block_size3<<24
        a |= block_size2<<16
        a |= block_size1<<8
        a |= block_size0
        offset += a
    offset += 20
    for i in range(layer_idx):
        layer_size0 = netbin[offset+4]
        layer_size1 = netbin[offset+5]
        layer_size2 = netbin[offset+6]
        layer_size3 = netbin[offset+7]
        a = layer_size3<<24
        a |= layer_size2<<16
        a |= layer_size1<<8
        a |= layer_size0
        offset += a
        #print("layer size & type:",layer_size0,layer_size1,layer_size2,layer_size3,a,netbin[offset+8])
    return netbin[offset+8]

def get_layer_info(net_bin_file):
    net_bin = read_netbin(net_bin_file)
    block_num = get_block_num(net_bin)
    layer_type = []
    print("block num is ",block_num)
    for i in range(block_num):
        layer_num, block_type = get_layer_num_and_type(net_bin, i)
        block_type_keys = block_type_dict.keys()
        block_type_nums = len(block_type_dict)
        if block_type == block_type_dict['DspType']:
            for j in range(layer_num):
                layer_type_idx = get_layer_type(net_bin, i, j)
                keys = dsp_layer_type_dict.keys()
                dsp_type = None
                dsp_type_num = len(dsp_layer_type_dict)
                #print("type idx is", i, j, layer_type_idx)
                for k in range(dsp_type_num):
                    if layer_type_idx == k:
                        dsp_type = list(keys)[k] 
                layer_type.append(dsp_type)
        else:
            dsp_type = None
            for k in range(block_type_nums):
                if block_type == k:
                    dsp_type = list(block_type_keys)[k] 
            layer_type.append(dsp_type)
    return layer_type

