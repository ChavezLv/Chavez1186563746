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
    #print(yaml)
    rectMap = {}
    landmarkMap = {}

    for i in yaml:
        l = len(i["ELEMENT"])
        #print('----------------------------',l)
        file = i["ELEMENT"][0]["VALUE"]
        rect = i["ELEMENT"][2]["VALUE"]

        if l < 4 :
            landmarks=[]
        else :
            landmarks=i["ELEMENT"][3]["landmarks"][0][0]
        #print(landmarks)
        rectMap[file] = rect
        landmarkMap[file]=landmarks
    return rectMap, landmarkMap


class TestPyArctern_FACEFEATURE(unittest.TestCase):
    def compare(self, item, attr):
        self.assertEqual(2064, len(attr))
        feat2=item["feature"]
        self.assertEqual(2064, len(feat2))
        score = arcternbase.compute_similarity(attr, feat2)
        # print(score)
        self.assertAlmostEqual(score, 1, delta=1e-2)

    def impl(self, modelPath, ymlInput, ymlGroundTruth) :
        imgDir = yamlUtil.getDIR() + "data/get_face_feat/"
        yaml_info = yamlUtil.readyaml(ymlGroundTruth)
        grounds = getGroundTruth(yaml_info)
        #print('------------------------',grounds)

        input_info = yamlUtil.readyaml(ymlInput)
        rectMap, landmarkMap = getInput(input_info)

        arctMgr = arcternbase.ArcternManager()
        arctMgr.set_face_feat_model(modelPath)

        # single detect
        for item in grounds:
            #print(item)
            file = item['file']
            pic = imgDir + file
            test_person_image = arcternbase.read_image(pic)
            #print(pic, rectMap[file], landmarkMap[file])
            attr = arctMgr.get_face_feat(test_person_image, rectMap[file], landmarkMap[file] )
            self.compare(item, attr)


        #batch detect
        idx = 0
        sum = len(grounds)
        while idx < sum:
            cnt = random.randint(1, sum - idx)
            pics = []
            facess=[]
            landmarkss=[]
            for picIdx in range(cnt):
                file = grounds[picIdx + idx]['file']
                pic = imgDir + file
                test_person_image = arcternbase.read_image(pic)
                pics.append(test_person_image)
                faces=[]
                faces.append(rectMap[file])
                facess.append(faces)
                landmarks=[]
                landmarks.append(landmarkMap[file])
                landmarkss.append(landmarks)

            attrs = arctMgr.get_face_feat_batch(pics, facess, landmarkss)
            for picIdx in range(cnt):
                self.compare(item=grounds[picIdx + idx], attr=attrs[picIdx][0])

            idx += cnt

    def test_face_gluon380(self):
        ymlPath380 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.8.0-rc.yml"
        modelPath380 = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.8.0-var7.bin"
        ymlInputPath380 = yamlUtil.getDIR() + "input_params/face-gluon-f32-d-3.8.0-i.yml"
        self.impl(modelPath380, ymlInputPath380, ymlPath380)

    def test_face_gluon390(self):
        ymlPath390 = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.0-rc.yml"
        modelPath390 = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.9.0-var7.bin"
        ymlInputPath390 = yamlUtil.getDIR() + "input_params/face-gluon-f32-d-3.9.0-i.yml"
        self.impl(modelPath390, ymlInputPath390, ymlPath390)

    def test_face_gluon370(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.7.0-rc.yml"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.7.0-var7.bin"
        ymlInputPath = yamlUtil.getDIR() + "input_params/face-gluon-d-3.7.0-i.yml"
        self.impl(modelPath, ymlInputPath, ymlPath)

    def test_face_gluon360(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.6.0-rc.yml"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.6.0-var7.bin"
        ymlInputPath = yamlUtil.getDIR() + "input_params/face-gluon-d-3.6.0-i.yml"
        self.impl(modelPath, ymlInputPath, ymlPath)

    def test_face_gluon250(self):
        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-2.5.0-rc.yml"
        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-2.5.0-var7.bin"
        ymlInputPath = yamlUtil.getDIR() + "input_params/face-gluon-2.5.0-i.yml"
        self.impl(modelPath, ymlInputPath, ymlPath)

#    def test_face_gluon350(self):
#        ymlPath = yamlUtil.getDIR() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.5.0-rc.yml"
#        modelPath = yamlUtil.getModelDir() + "develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.5.0-var7.bin"
#        ymlInputPath = yamlUtil.getDIR() + "input_params/face-gluon-f32-d-3.5.0-i.yml"
#        self.impl(modelPath, ymlInputPath, ymlPath)


if __name__ == '__main__':
    unittest.main()
