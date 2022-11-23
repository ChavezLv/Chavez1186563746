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


class TestPyArctern_FACEABNOMALLIGHT(unittest.TestCase):

    def compare(self, item, attr):
        self.assertEqual(attr[0], arcternbase.AbnomalLightType(item["type"][0]))

        self.assertAlmostEqual(attr[1], item["score"][0], delta=1e-2)

    def impl(self, modelPath, ymlInputPath, ymlPath):
        imgDir = yamlUtil.getDIR() + "data/predict_face_abnomal_light/"
        yaml_info = yamlUtil.readyaml(ymlPath)
        grounds = getGroundTruth(yaml_info)

        input_info = yamlUtil.readyaml(ymlInputPath)
        inputs = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_abnomal_light_classifier_model(modelPath)

        # single detect
        for item in grounds:
            print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_face_abnomal_light(test_person_image, inputs[file])
            self.compare(item, attr)

        # batch detect
        idx = 0
        sum = len(grounds)
        while idx < sum:
            cnt = random.randint(1, sum - idx)
            pics = []
            facess = []
            for picIdx in range(cnt):
                file = grounds[picIdx + idx]['file']
                pic = imgDir + file
                test_person_image = arcternbase.read_image(pic)
                pics.append(test_person_image)
                faces = []
                faces.append(inputs[file])
                facess.append(faces)

            attrs = arctMgr.predict_face_abnomal_light_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_face_abnomal_light004(self):
        ymlInput=yamlUtil.getDIR() + "input_params/face-abnormal-light-classifier-f32-d-0.0.4-i.yml"
        modelpath=yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-abnormal-light-classifier-tvm-f32-d-0.0.4-var7.bin"
        ymlGroundTruth=yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.4-rc.yml"
        self.impl(modelpath, ymlInput, ymlGroundTruth)

    def test_face_abnomal_light003(self):
        ymlInput=yamlUtil.getDIR() + "input_params/face-abnormal-light-classifier-f32-d-0.0.3-i.yml"
        modelpath=yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-abnormal-light-classifier-tvm-f32-d-0.0.3-var7.bin"
        ymlGroundTruth=yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.3-rc.yml"
        self.impl(modelpath, ymlInput, ymlGroundTruth)

    def test_face_abnomal_light002(self):
        ymlInput=yamlUtil.getDIR() + "input_params/face-abnomal-light-classifier-d-0.0.2-i.yml"
        modelpath=yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-abnomal-light-classifier-tvm-f32-d-0.0.2-var7.bin"
        ymlGroundTruth=yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnomal-light-classifier-d-0.0.2-rc.yml"
        self.impl(modelpath, ymlInput, ymlGroundTruth)

if __name__ == '__main__':
    unittest.main()
