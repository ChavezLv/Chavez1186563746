import unittest
import yamlUtil
import arcternbase
import random


def getGroundTruth(yaml):
    groundTruths = []
    for i in yaml:
        gt = {}
        gt["file"] = i["ELEMENT"][0]["VALUE"]
        gt["score"] = i["ELEMENT"][1]["VALUE"]
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


class TestPyArctern_PERSONQUALITY(unittest.TestCase):

    def compare(self, item, attr):
        self.assertAlmostEqual(attr, item["score"][0], delta=1e-2)

    def impl(self, modelPath, ymlInputPath, ymlPath):
        imgDir = yamlUtil.getDIR() + "data/predict_person_quality/"
        yaml_info = yamlUtil.readyaml(ymlPath)
        grounds = getGroundTruth(yaml_info)
        #print("----",grounds)

        input_info = yamlUtil.readyaml(ymlInputPath)
        inputs = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_person_quality_predictor(modelPath)

        # single detect
        for item in grounds:
            print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_person_quality(test_person_image, inputs[file] )
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

            attrs = arctMgr.predict_person_quality_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt


    def test_person_quality(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-quality-d-1.0.3-rc.yml"
        modelPath = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-quality-tvm-f32-d-1.0.3-var7.bin"
        ymlInputPath = yamlUtil.getDIR() + "input_params/person-quality-d-1.0.3-i.yml"
        self.impl(modelPath,ymlInputPath,ymlPath)

if __name__ == '__main__':
    unittest.main()
