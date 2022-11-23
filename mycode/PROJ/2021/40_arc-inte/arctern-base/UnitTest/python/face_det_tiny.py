import unittest
import yamlUtil
import arcternbase
import random


def getGroundTruth(yaml):
    groundTruths = []
    for i in yaml:
        gt = {}
        num = len(i["ELEMENT"])
        gt["file"] = i["ELEMENT"][0]["VALUE"]
        if num > 1:
            gt["rects"] = i["ELEMENT"][1]["VALUE"]
            gt["confidences"] = i["ELEMENT"][2]["VALUE"]
        # print(gt)
        groundTruths.append(gt)
    return groundTruths


class TestPyArctern_FACEDETTINY(unittest.TestCase):

    def compare(self, item, faces):
        print(item)
        print(faces)
        detFaceNum = len(faces)
        realFaceNum = len(item["confidences"]) if "confidences" in item else 0
        self.assertEqual(detFaceNum, realFaceNum)
        for i in range(detFaceNum):
            self.assertAlmostEqual(faces[i][0][0], item["rects"][0 + 4 * i], delta=2)
            self.assertAlmostEqual(faces[i][0][1], item["rects"][1 + 4 * i], delta=2)
            self.assertAlmostEqual(faces[i][0][2], item["rects"][2 + 4 * i], delta=2)
            self.assertAlmostEqual(faces[i][0][3], item["rects"][3 + 4 * i], delta=2)
            self.assertAlmostEqual(faces[i][1], item["confidences"][i], delta=1e-2)

    def test_face_det_tiny(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-d-0.0.3-rc.yml"
        print(ymlPath)
        imgDir = yamlUtil.getDIR() + "data/detect_tiny_face/"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-det-video-tvm-f32-d-0.0.3-var7.bin"
        yaml_info = yamlUtil.readyaml(ymlPath)
        grounds = getGroundTruth(yaml_info)

        arctMgr = arcternbase.ArcternManager()
        param = arcternbase.FaceDetectorYolov3Parameter()
        param.conf_thresh = 0.5;
        arctMgr.set_face_tiny_det_model(modelPath, param)

        print(grounds)

        # single detect
        for item in grounds:
            pic = imgDir + item["file"]
            test_person_image = arcternbase.read_image(pic)
            faces = arctMgr.detect_face_tiny(test_person_image)
            self.compare(item, faces)

        # batch detect
        idx = 0
        sum = len(grounds)
        while idx < sum:
            cnt = random.randint(1, sum - idx)
            pics = []
            for picIdx in range(cnt):
                pic = imgDir + grounds[idx + picIdx]["file"]
                test_person_image = arcternbase.read_image(pic)
                pics.append(test_person_image)
            facess = arctMgr.detect_face_tiny_batch(pics)
            for picIdx in range(cnt):

                self.compare(item=grounds[picIdx + idx], faces=facess[picIdx])

            idx += cnt
            print(cnt, sum)


if __name__ == '__main__':
    unittest.main()