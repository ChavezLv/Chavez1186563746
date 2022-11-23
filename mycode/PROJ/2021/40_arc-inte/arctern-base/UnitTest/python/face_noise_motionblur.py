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


class TestPyArctern_FACENOISEMOTIONBLUR(unittest.TestCase):

    def compare(self, item, attr):
        print(attr)
        self.assertEqual(attr[0], arcternbase.NoiseType(item["type"][0]))
        self.assertAlmostEqual(attr[1], item["score"][0], delta=1e-2)

    def impl(self, modelPath, ymlInput, ymlGroundTruth):
        # ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.0.9-rc.yml"
        imgDir = yamlUtil.getDIR() + "data/predict_face_noise_motionblur/"
        # modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.0.9-base.bin"

        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)

        # ymlInputPath = yamlUtil.getDIR() + "input_params/face-noise-motionblur-classifier-f32-d-0.0.9-i.yml"
        input_info = yamlUtil.readyaml(ymlInput)
        inputs = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_noise_motionblur_classifier_model(modelPath)

        # single detect
        for item in grounds:
            print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_face_noise_motionblur(test_person_image, inputs[file])
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

            attrs = arctMgr.predict_face_noise_motionblur_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_face_noisemotionblur009(self):
        ymlInput = yamlUtil.getDIR() + "input_params/face-noise-motionblur-classifier-f32-d-0.0.9-i.yml"
        modelpath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.0.9-var7.bin"
        ymlGroundtruth = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.0.9-rc.yml"
        self.impl(modelpath, ymlInput, ymlGroundtruth)

    def test_face_noisemotionblur010(self):
        ymlInput = yamlUtil.getDIR() + "input_params/face-noise-motionblur-classifier-f32-d-0.1.0-i.yml"
        modelpath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.1.0-var7.bin"
        ymlGroundtruth = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.1.0-rc.yml"
        self.impl(modelpath, ymlInput, ymlGroundtruth)


if __name__ == '__main__':
    unittest.main()
