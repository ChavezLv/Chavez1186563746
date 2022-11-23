import unittest
import os
from arctern import ArcternManager


def test_safety():
    datadir = os.path.abspath(os.path.join(os.path.dirname(__file__)), '../../../data/safety')
    safety_det_model_path = os.path.join(datadir, 'frozen_graph.pb')
    image_path = os.path.join(datadir, '001.jpg')
    arctern_manager = ArcternManager()
    arctern_manager.set_safety_model(face_det_model_path)   #加载模型
    print(arctern_manager.detect_safety(image_path))    # 安全性检测
