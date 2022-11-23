import numpy as np

def similarity(d2, d3):

    num = float(d2.dot(d3.T))
    denom = np.linalg.norm(d2) * np.linalg.norm(d3)
    cos = num / denom  # 余弦值
    sim = 0.5 + 0.5 * cos  # 根据皮尔逊相关系数归一化
    return sim

def ref_compare(ref, out):
    result_list = []
    if len(out) != len(ref):
        print('-- Test failed: data1 = {}, data2 = {}'.format(len(ref), len(out)))
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
                    result_list.append("data1 = {}, data2 = {}".format(refval, val))
    if success:
        print("-- Test success: data check succ!")
        return True, sim
    else:
        print("-- Test failed: data check failed! detail info is: {}".format(result_list))
        return False, sim
        
data1 = np.fromfile("chip_out.bin", dtype=np.float32)
data2 = np.fromfile("tvm_fix_out.bin", dtype=np.float32)
result, similarity = ref_compare(data1, data2)
print(f"similarity={similarity}")