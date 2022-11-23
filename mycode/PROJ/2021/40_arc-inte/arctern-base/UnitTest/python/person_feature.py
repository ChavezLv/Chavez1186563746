import unittest
import yamlUtil
import arcternbase
import random


def getGroundTruth(yaml):
    #print('------------', yaml)
    groundTruths = []
    for i in yaml:
        gt = {}
        gt["file"] = i["ELEMENT"][0]["VALUE"]
        feature = i["ELEMENT"][1]["VALUE"][0]
        # convert list<float> to list<int>
        gt["feature"] = [int(i) for i in feature]
        groundTruths.append(gt)
    return groundTruths


def getInput(yaml):
    rectMap = {}
    for i in yaml:
        file = i["ELEMENT"][0]["VALUE"]
        rect = i["ELEMENT"][2]["VALUE"]
        rectMap[file] = rect
    return rectMap


class TestPyArctern_PERSONFEATURE(unittest.TestCase):
    def compare(self, item, attr):
        self.assertEqual(2064, len(attr))
        feat2=item["feature"]
        self.assertEqual(2064, len(feat2))
        score = arcternbase.compute_similarity(attr, feat2)
        # print(score)
        self.assertAlmostEqual(score, 1, delta=1e-2)

    def impl(self, modelPath, ymlInput, ymlGroundTruth) :
        imgDir = yamlUtil.getDIR() + "data/get_pedestrian_feat/"
        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)
        input_info = yamlUtil.readyaml(ymlInput)
        rectMap = getInput(input_info)
        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_person_feat_model(modelPath)

        # single detect
        for item in grounds:
            #print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            attr = arctMgr.get_pedestrian_feat(test_person_image, rectMap[file] )
            self.compare(item, attr)


        #batch detect
        idx = 0
        sum = len(grounds)
        while idx < sum:
            cnt = random.randint(1, sum - idx)
            pics = []
            personss=[]
            for picIdx in range(cnt):
                file = grounds[picIdx + idx]['file']
                pic = imgDir + file
                test_person_image = arcternbase.read_image(pic)
                pics.append(test_person_image)
                persons=[]
                persons.append(rectMap[file])
                personss.append(persons)

            attrs = arctMgr.get_pedestrian_feat_batch(pics, personss)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_person_feat090(self):
        ymlPath090 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-0.9.0-rc.yml"
        modelPath090 = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-0.9.0-var7.bin"
        ymlInputPath090 = yamlUtil.getDIR() + "input_params/person-feat-f32-d-0.9.0-i.yml"
        self.impl(modelPath090, ymlInputPath090, ymlPath090)

    def test_person_feat040(self):
        ymlPath040 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-0.4.0-rc.yml"
        modelPath040 = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-0.4.0-var7.bin"
        ymlInputPath040 = yamlUtil.getDIR() + "input_params/person-feat-0.4.0-i.yml"
        self.impl(modelPath040, ymlInputPath040, ymlPath040)

    def test_person_feat050(self):
        ymlPath050 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-tiny-f32-d-0.5.0-rc.yml"
        modelPath050 = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-feat-tiny-tvm-f32-d-0.5.0-var7.bin"
        ymlInputPath050 = yamlUtil.getDIR() + "input_params/person-feat-tiny-f32-d-0.5.0-i.yml"
        self.impl(modelPath050, ymlInputPath050, ymlPath050)

    def test_person_feat100(self):
        ymlPath050 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-1.0.0-rc.yml"
        modelPath050 = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-1.0.0-var7.bin"
        ymlInputPath050 = yamlUtil.getDIR() + "input_params/person-feat-f32-d-1.0.0-i.yml"
        self.impl(modelPath050, ymlInputPath050, ymlPath050)


if __name__ == '__main__':
    unittest.main()
