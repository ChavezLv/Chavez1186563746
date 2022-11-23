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


class TestPyArctern_AGEGENDER(unittest.TestCase):

    def compare(self, item, attr):
        print(item)
        print(attr.age)
        print(attr.gender)
        self.assertEqual(attr.age[0], item["type"][0][1])
        self.assertAlmostEqual(attr.age[1], item["score"][0][1], delta=1e-2)
        male = item["type"][0][0] == 1
        self.assertEqual(attr.gender[0], male)
        self.assertAlmostEqual(attr.gender[1], item["score"][0][0], delta=1e-2)
        #
        # self.assertAlmostEqual(faces[i][0][0], item["rects"][0 + 4 * i], delta=2)
        # self.assertAlmostEqual(faces[i][0][1], item["rects"][1 + 4 * i], delta=2)


    def impl(self, modelPath, ymlInput, ymlGroundTruth):
       # ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-f32-d-0.2.0-rc.yml"
        imgDir = yamlUtil.getDIR() + "data/predict_face_gender_age/"
        #modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.2.0-base.bin"

        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)

        input_info = yamlUtil.readyaml(ymlInput)
        inputs = getInput(input_info)
        #print(inputs)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_gender_age_model(modelPath)

        # single detect
        for item in grounds:
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_face_gender_age(test_person_image, inputs[file] )
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

            attrs = arctMgr.predict_face_gender_age_batch(pics, facess)
            print(attrs)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt
            print(cnt, sum)

    def test_face_age_gender020(self):
        ymlInput = yamlUtil.getDIR() + "input_params/face-gender-age-f32-d-0.2.0-i.yml"
        ymlGroundTruth = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-f32-d-0.2.0-rc.yml"
        modelPath =  yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.2.0-var7.bin"
        self.impl(modelPath,ymlInput,ymlGroundTruth)

    def test_face_age_gender010(self):
        ymlInput = yamlUtil.getDIR() + "input_params/face-gender-age-d-0.1.0-i.yml"
        ymlGroundTruth = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-d-0.1.0-rc.yml"
        modelPath =  yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.1.0-var7.bin"
        self.impl(modelPath,ymlInput,ymlGroundTruth)


if __name__ == '__main__':
    unittest.main()
