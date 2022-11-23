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
        visible_softmask = i["ELEMENT"][1]["VALUE"][0]
        truncations = i["ELEMENT"][2]["VALUE"][0]
        visible_softmask_score = i["ELEMENT"][3]["VALUE"][0]
        print(visible_softmask)
        print(truncations)
        print(visible_softmask_score)
        gt['truncation'] = truncations
        gt['visible_softmask'] = visible_softmask
        gt['visible_softmask_score'] = visible_softmask_score
        # convert list<float> to list<int>

        groundTruths.append(gt)
    return groundTruths


def getInput(yaml):
    rectMap = {}
    for i in yaml:
        file = i["ELEMENT"][0]["VALUE"]
        rect = i["ELEMENT"][1]["VALUE"]
        rectMap[file] = rect
    return rectMap

def to_bool(s):
    return 1 if s == True else 0

class TestPyArctern_PERSONCOMPLETENESS(unittest.TestCase):
    def compare(self, item, attr):
        for i in zip(attr.truncation, item["truncation"]) :
            self.assertAlmostEqual(i[0], i[1], delta=1e-2)

        softmask=[]
        softmaskScore=[]

        for i in attr.visible_parts:
            softmask.append(i[0])
            softmaskScore.append(i[1])

        for i in  attr.soft_mask:
            for k in i :
                softmask.append(k[0])
                softmaskScore.append(k[1])



        print('--------' + item['file'])
        print(softmask)
        print('+++++++++')
        print(item['visible_softmask'])
        print('**********')
        print(softmaskScore)
        print('/////////')
        print(item['visible_softmask_score'])


        # for  i in zip(softmask, item['visible_softmask']) :
        #     # print(id)
        #    # print(i)
        #     self.assertEqual(to_bool(i[0]), i[1])

        for i in zip(softmaskScore, item['visible_softmask_score']) :
            self.assertAlmostEqual(i[0], i[1], delta=5e-2)



        # print(attr.truncation)
        # print(attr.visible_parts)
        # print(attr.soft_mask)
        # self.assertEqual(2064, len(attr))
        # feat2=item["feature"]
        # self.assertEqual(2064, len(feat2))
        # score = arcternbase.compute_similarity(attr, feat2)
        # # print(score)
        # self.assertAlmostEqual(score, 1, delta=1e-2)

    def impl(self, modelPath, ymlInput, ymlGroundTruth) :
        imgDir = yamlUtil.getDIR() + "data/predict_person_completeness/"
        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)
        input_info = yamlUtil.readyaml(ymlInput)
        rectMap = getInput(input_info)
        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_person_completeness_model(modelPath)

        # single detect
        for item in grounds:
            #print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            print(rectMap[file])
            attr = arctMgr.predict_person_completeness(test_person_image, rectMap[file] )
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

            # attrs = arctMgr.predict_person_completeness_batch(pics, personss)
            # for picIdx in range(cnt):
            #     self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_person_completeness000(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/person-completeness-0.0.0-rc.yml"
        modelPath = yamlUtil.getModelDir() + "develop/body/tvm0.7/avx2/person-completeness-tvm-f32-d-0.0.0-var7.bin"
        ymlInputPath = yamlUtil.getDIR() + "input_params/person-completeness-0.0.0-i.yml"
        self.impl(modelPath, ymlInputPath, ymlPath)


if __name__ == '__main__':
    unittest.main()
