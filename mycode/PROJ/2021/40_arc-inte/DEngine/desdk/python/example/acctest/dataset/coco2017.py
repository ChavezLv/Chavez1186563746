import os
import pickle
import tempfile
import numpy as np
from pycocotools.coco import COCO
from pycocotools.cocoeval import COCOeval
import six
import tensorflow as tf
import atexit


# https://github.com/cocodataset/cocoapi/issues/49
if six.PY3:
  import pycocotools.coco
  pycocotools.coco.unicode = str

def coco2017_eval(predictions, val_json_file):
  """Use model predictions to compute mAP.

  Args:
    predictions: a list of tuples returned by decoded_predictions function,
      each containing the following elements:
      image source_id, box coordinates in XYWH order, probability score, label
    val_json_file: path to COCO annotation file
  Returns:
    A dictionary that maps all COCO metrics (keys) to their values
  """

  if val_json_file.startswith("gs://"):
    _, local_val_json = tempfile.mkstemp(suffix=".json")
    tf.gfile.Remove(local_val_json)

    tf.gfile.Copy(val_json_file, local_val_json)
    atexit.register(tf.gfile.Remove, local_val_json)
  else:
    local_val_json = val_json_file

  cocoGt = COCO(local_val_json)
  cocoDt = cocoGt.loadRes(np.array(predictions))
  E = COCOeval(cocoGt, cocoDt, iouType='bbox')
  E.evaluate()
  E.accumulate()
  E.summarize()
  print("Current AP: {:.5f}".format(E.stats[0]))
  metric_names = ['AP', 'AP50', 'AP75', 'APs', 'APm', 'APl', 'ARmax1',
                  'ARmax10', 'ARmax100', 'ARs', 'ARm', 'ARl']

  # Prefix with "COCO" to group in TensorBoard.
  return {"COCO/" + key: value for key, value in zip(metric_names, E.stats)}



def cal_coco2017_accuracy(dataset_info, datapath = None, result_file = "result.pkl"):
    """
    :param dataset_info:
    :param data_path:
    :param result_file:
    :return:
    """

    assert "name" in dataset_info, "name field not in model_info dict."
    assert "datapath" in dataset_info, "datapath field not in model_info dict."
    assert "ann_path" in dataset_info, "year field not in model_info dict."

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

    dataset_path = os.path.join(datapath, dataset_info["datapath"])
    ann_file = os.path.join(datapath, dataset_info["ann_path"])
    assert os.path.isdir(dataset_path), "datasets must exist folder {}".format(dataset_path)
    assert os.path.isfile(ann_file), "annotation.json must exist".format(ann_file)

    assert os.path.isfile(result_file), "result_file %s not exist."%(result_file)

    with open(result_file, "rb") as fb:
        predictions = pickle.load(fb)

    result = coco2017_eval(np.array(predictions, "float32"), ann_file)

    return result