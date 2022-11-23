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


class TestPyArctern_FACEHAT(unittest.TestCase):

    def compare(self, item, attr):
        self.assertEqual(attr[0], item["type"][0] == 1)
        self.assertAlmostEqual(attr[1], item["score"][0], delta=1e-2)

    def test_face_hat(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-hat-1.0.3-rc.yml"
        imgDir = yamlUtil.getDIR() + "data/predict_face_hat/"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-hat-tvm-f32-d-1.0.3-var7.bin"

        yaml_info = yamlUtil.readyaml(ymlPath)
        grounds = getGroundTruth(yaml_info)


        ymlInputPath = yamlUtil.getDIR() + "input_params/face-hat-1.0.3-i.yml"
        input_info = yamlUtil.readyaml(ymlInputPath)
        inputs = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_hat_model(modelPath)

        # single detect
        for item in grounds:
            print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_face_hat(test_person_image, inputs[file] )
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

            attrs = arctMgr.predict_face_hat_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt



if __name__ == '__main__':
    unittest.main()
