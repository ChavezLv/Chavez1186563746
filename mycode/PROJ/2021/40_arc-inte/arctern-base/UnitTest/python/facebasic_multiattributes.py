import unittest
import yamlUtil
import arcternbase
import random


def getGroundTruth(yaml):
    groundTruths = []
    for i in yaml:
        gt = {}
        gt["file"] = i["ELEMENT"][0]["VALUE"]
        gt["type"] = i["ELEMENT"][1]["VALUE"]
        gt["score"] = i["ELEMENT"][2]["VALUE"]
        # print(gt)
        groundTruths.append(gt)
    return groundTruths


def getInput(yaml):
    inputs = {}
    for i in yaml:
        file = i["ELEMENT"][0]["VALUE"]
        rect = i["ELEMENT"][1]["VALUE"]
        inputs[file] = rect
    return inputs


class TestPyArctern_FACEBASICMULTIATTRIBUTES(unittest.TestCase):

    def compare(self, item, attr):
        self.assertEqual(attr.gender[0], item["type"][0][1])
        self.assertAlmostEqual(attr.gender[1], item["score"][0][0], delta=1e-2)
        self.assertEqual(attr.age[0], item["type"][0][3])
        self.assertAlmostEqual(attr.age[1], item["score"][0][1], delta=1e-2)
        self.assertEqual(attr.glasses[0], item["type"][0][5])
        self.assertAlmostEqual(attr.glasses[1], item["score"][0][2], delta=1e-2)
        self.assertEqual(attr.mask[0], item["type"][0][7])
        self.assertAlmostEqual(attr.mask[1], item["score"][0][3], delta=1e-2)
        self.assertEqual(attr.headwear[0], item["type"][0][9])
        self.assertAlmostEqual(attr.headwear[1], item["score"][0][4], delta=1e-2)

    def test_facebasic_multiattributes(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.1.1-rc.yml"
        imgDir = yamlUtil.getDIR() + "data/predict_facebasic_multiattributes/"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-multi-attributes-tvm-f32-d-0.1.1-var7.bin"

        yaml_info = yamlUtil.readyaml(ymlPath)
        grounds = getGroundTruth(yaml_info)
        print("----",grounds)

        ymlInputPath = yamlUtil.getDIR() + "input_params/face-multi-attributes-f32-d-0.1.1-i.yml"
        input_info = yamlUtil.readyaml(ymlInputPath)
        inputs = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_basicmultiattribute_model(modelPath)

        # single detect
        for item in grounds:
            print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_facebasic_multiattributes(test_person_image, inputs[file] )
            self.compare(item, attr)

        #batch detect
        idx = 0
        sum = len(grounds)
        while idx < sum:
            cnt = random.randint(1, sum - idx)
            pics = []
            facess=[]
            for picIdx in range(cnt):
                file = grounds[picIdx + idx]['file']
                pic = imgDir + file
                test_person_image = arcternbase.read_image(pic)
                pics.append(test_person_image)
                faces=[]
                faces.append(inputs[file])
                facess.append(faces)

            attrs = arctMgr.predict_facebasic_multiattributes_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt



if __name__ == '__main__':
    unittest.main()
