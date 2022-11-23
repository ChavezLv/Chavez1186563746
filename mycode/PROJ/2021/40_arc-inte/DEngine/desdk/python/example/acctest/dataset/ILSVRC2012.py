import os
import pickle
import numpy as np


def get_labels(file_path):
    """
    get contents from file
    :param file_path: str, file path
    :return: list of line contents
    """
    with open(file_path, 'r') as f:
        labels = [l.rstrip() for l in f]
        return labels

def graph_eval_result_proc(tensor, imgname, val_lab_dict, syn_lab, topn=5, label_offset=0):
    """
    graph evaluate result proc
    :param tensor: ndarray, output of evaluate
    :param imgname: str, the name of evaluated image
    :param val_lab_dict: dict
    :param syn_lab: list
    :param topn: int
    :return:
    """
    # prob = np.squeeze(tensor.asnumpy())
    # a = np.argsort(prob)[::-1]
    a = np.argsort(tensor[0].flatten())[::-1] - label_offset

    return get_topk_result(a, imgname, val_lab_dict, syn_lab, topn)

def get_topk_result(arg_tensor, imgname, val_lab_dict, syn_lab, topn=5,):

    assert topn >= 0
    flag1 = False
    flag5 = False
    predict_sym_info = ""
    check_id = val_lab_dict[imgname]
    for idx in arg_tensor[0:topn]:
        if check_id == idx:
            predict_sym_info = syn_lab[int(idx)]
            flag5 = True
            flag1 = idx == arg_tensor[0]
    return flag1, flag5, predict_sym_info


def cal_ILSVRC2012_accuracy(dataset_info, datapath = None, result_file = "result.pkl", topn = 5):
    """
    :param dataset_info:
    :param data_path:
    :param result_file:
    :return:
    """

    assert "name" in dataset_info, "name field not in dataset_info dict."
    assert "datapath" in dataset_info, "datapath field not in dataset_info dict."
    assert "synset_path" in dataset_info, "synset_path field not in dataset_info dict."
    assert "val_path" in dataset_info, "synset_path field not in dataset_info dict."


    image_set = dataset_info["name"]
    if datapath == None:
        env = os.environ
        if "TEST_DATA_ROOT" not in os.environ:
            datasets_root_path = "./datasets"
            assert os.path.exists(
                datasets_root_path), "datasets not exist in current folder."
        else:
            datasets_root_path = os.environ["TEST_DATA_ROOT"]
            assert os.path.exists(
                datasets_root_path), "TEST_DATA_ROOT not exist in system."
        datapath = os.path.join(datasets_root_path, image_set)

    assert os.path.isdir(datapath), "datasets must exist folder {}".format(datapath)

    datafilepath = os.path.join(datapath, dataset_info["datapath"])
    synset_path = os.path.join(datapath, dataset_info["synset_path"])
    val_path = os.path.join(datapath, dataset_info["val_path"])

    assert os.path.isdir(datafilepath), "datasets named {} must be in folder {}".format(
        dataset_info["datapath"], datapath)
    assert os.path.isfile(synset_path), "synset file {} must be in folder {}".format(
        dataset_info["synset_path"], datapath)
    assert os.path.isfile(val_path), "val_path file {val_path} must be in folder {}".format(
        dataset_info["val_path"], datapath)

    # get labels from txt
    synsets_label = get_labels(synset_path)
    val_label = get_labels(val_path)
    val_label_dict = {cur_label.split(" ")[0]: int(cur_label.split(" ")[1]) for cur_label in
                      val_label}

    with open(result_file, "rb") as fb:
        all_cls = pickle.load( fb)

    top1 = 0.0
    topk = 0.0
    for img_name, arg_tensor in all_cls.items():
        check_id = val_label_dict[img_name]
        top1 += (check_id == arg_tensor[0])
        topk += (check_id in arg_tensor[0:topn])

    total = len(all_cls)
    top1_acc = top1 / total
    topk_acc = topk / total

    result = "val accuracy:\ntotal: %d, top1/top5 Accuracy: %.6f/%.6f\n"%(total, top1_acc, topk_acc)
    return result