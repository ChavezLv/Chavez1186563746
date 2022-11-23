#!/usr/bin/env python3
import unittest
import sys
sys.path.append("../../cmake-build-debug")

from face_det_tiny import TestPyArctern_FACEDETTINY
from age_gender import TestPyArctern_AGEGENDER
from face_glass import TestPyArctern_FACEGLASS
from face_mask import TestPyArctern_FACEMASK
from face_quality import TestPyArctern_FACEQUALITY
from face_pose import TestPyArctern_FACEPOSE
from face_hat import TestPyArctern_FACEHAT
from face_expression import TestPyArctern_FACEEXPRESSION
from face_noise_motionblur import TestPyArctern_FACENOISEMOTIONBLUR
from face_abnomal_light import  TestPyArctern_FACEABNOMALLIGHT
from face_occlusion_det import TestPyArctern_FACEOCCLUSIONDET
from facebasic_multiattributes import TestPyArctern_FACEBASICMULTIATTRIBUTES
from face_feature import TestPyArctern_FACEFEATURE
from person_feature import TestPyArctern_PERSONFEATURE
from face_quality_pose import TestPyArctern_FACEQUALITYPOSE
from person_quality import TestPyArctern_PERSONQUALITY
from face_det_rtn import  TestPyArctern_FACEDETRTN
from facial_landmark import  TestPyArctern_FACIALLANDMARK
from person_completeness import TestPyArctern_PERSONCOMPLETENESS

if __name__ == '__main__':
    suite=unittest.TestSuite()
    loader=unittest.TestLoader()
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEDETTINY))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_AGEGENDER))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEGLASS))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEMASK))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEQUALITY))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEPOSE))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEHAT))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEEXPRESSION))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACENOISEMOTIONBLUR))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEABNOMALLIGHT))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEOCCLUSIONDET))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEBASICMULTIATTRIBUTES))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEFEATURE))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_PERSONFEATURE))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEQUALITYPOSE))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_PERSONQUALITY))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACEDETRTN))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_FACIALLANDMARK))
    suite.addTest(loader.loadTestsFromTestCase(TestPyArctern_PERSONCOMPLETENESS))
    runner=unittest.TextTestRunner(verbosity=3)
    result=runner.run(suite)

