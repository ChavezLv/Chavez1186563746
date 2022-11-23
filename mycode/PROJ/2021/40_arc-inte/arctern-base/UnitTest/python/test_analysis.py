import unittest
import os
from arctern import ArcternManager


def test_analysis():
    datadir = os.path.abspath(os.path.join(os.path.dirname(__file__)), '../../../data')
    face_det_model_path = os.path.join(datadir, 'face-det-0.9.0.bin')
    arctern_manager = ArcternManager()
    arctern_manager.set_face_det_model(face_det_model_path)
