import yaml
import os
def readyaml(file):
    if os.path.isfile(file):
        with open(file, 'r') as fin:
            # some operator on raw content of c may be needed
            c = fin.read()
            c = "%YAML 1.1" + os.linesep + "---" + c[len("%YAML:1.0"):] if c.startswith("%YAML:1.0") else c
            #  \' not support. so replace it
            c=c.replace("\\'", '')
           # c=c.replace("---\n", '')
       #     print(c)
            yaml_info = yaml.load(c)
            return yaml_info["ROOT"]
        return None
    return None

def getDIR():
    return "../../../arctern-metadata/ModelsVerifyData/";

def getModelDir():
    return "../../../arctern-models/"