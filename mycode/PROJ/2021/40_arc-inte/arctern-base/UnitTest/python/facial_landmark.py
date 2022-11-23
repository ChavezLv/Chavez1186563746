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
        landmark = i["ELEMENT"][1]["VALUE"][0]
        # convert list<float> to list<int>
        gt["landmark"] = landmark
        groundTruths.append(gt)
    return groundTruths


def getInput(yaml):
    rectMap = {}
    for i in yaml:
        file = i["ELEMENT"][0]["VALUE"]
        rect = i["ELEMENT"][1]["VALUE"]
        rectMap[file] = rect
    return rectMap


class TestPyArctern_FACIALLANDMARK(unittest.TestCase):
    def compare(self, item, attr):
        points1=item["landmark"]
        for i in zip(attr, points1) :
           # print(i[0],i[1])
            self.assertAlmostEqual(i[0], i[1], delta=2)

        # print(score)


    def impl(self, modelPath, ymlInput, ymlGroundTruth) :
        imgDir = yamlUtil.getDIR() + "data/predict_facial_landmark/"
        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)
        input_info = yamlUtil.readyaml(ymlInput)
        rectMap = getInput(input_info)

        print('11111')
        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_facial_landmark_model(modelPath)
        print('11112')
        # single detect
        for item in grounds:
            #print(item)
            file = item['file']
            pic = imgDir + file
            test_face_image = arcternbase.read_image(pic)
            attr = arctMgr.predict_facial_landmark(test_face_image, rectMap[file] )
            self.compare(item, attr)

        print('###')
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
                test_face_image = arcternbase.read_image(pic)
                pics.append(test_face_image)
                faces=[]
                faces.append(rectMap[file])
                facess.append(faces)
            attrs = arctMgr.predict_facial_landmark_batch(pics, facess)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_facial_landmark130(self):
        ymlPath130 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-d-1.3.0-rc.yml"
        modelPath130 = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-landmark-tvm-f32-d-1.3.0-var7.bin"
        ymlInputPath130 = yamlUtil.getDIR() + "input_params/face-landmark-d-1.3.0-i.yml"
        self.impl(modelPath130, ymlInputPath130, ymlPath130)




if __name__ == '__main__':
    unittest.main()
