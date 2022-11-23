import sys, os, re
import configparser

from onchip_run import *

import logging
logging.basicConfig(level=logging.DEBUG)

config = configparser.ConfigParser()
inifile = sys.argv[1]
inipath = os.path.split(os.path.abspath(inifile))[0]
run_section = 'run'
common_section = 'common'

keyMustBeSet = ['dec_onchip']
keyCommonMustBeSet = ['input_shape_h', 'input_shape_w', 'input_shape_c']
keyBoolTypeSet = ['dec_onchip']
sys.stdout.flush()

def runFailAndExit() :
    print('\n\033[5;41mMODEL_COMPILE_FAIL\033[0m')
    sys.stdout.flush()
    os._exit(0)

def checkIniFile(filename) :
    if not os.path.exists(filename):
        print("\033[41m \"{FileName}\" is no exist, please check!\033[0m".format(FileName=filename))
        runFailAndExit()

def checkKeyExist(Section, KeySet, Message=None) :
    for Key in KeySet :
        if not config.has_option(Section, Key):
            if Message == None :
                print("\033[41mINI file have not key named \"{KeyName}\", please check!\033[0m".format(KeyName=Key))
            else:
                print("\033[41mINI file have not key named \"{KeyName}\" {Message}, please check!\033[0m".format(KeyName=Key, Message=Message))
            runFailAndExit()

def checkBoolValue(key) :
    boolstring = config.get(run_section, key)
    if (boolstring.lower() != 'true') and (boolstring.lower() != 'false') :
        print("\033[41m\"{KeyName}\" should be True or False, please check!\033[0m".format(KeyName=key))
        runFailAndExit()

def checkFileExist(key, filename) :
    if not os.path.exists(filename):
        print("\033[41m \"{key}\": \"{FileName}\" is no exist, please check!\033[0m".format(key=key, FileName=filename))
        runFailAndExit()

def decodePath(base, src):
    param = re.match('\$\{.*\}', src)
    if param is None:
        return os.path.join(base, src)
    else:
        split1 = param.string.split('${')
        path_prefix = split1[0]
        split2 = split1[1].split('}')
        path_suffix = split2[1]
        config_key_name = split2[0]
        config_key_content = config.get(common_section, config_key_name)
        if len(path_prefix) == 0:
            rel_path = config_key_content + '/' + path_suffix
        else:
            rel_path = path_prefix + '/' + config_key_content + '/' + path_suffix
        rel_path = rel_path.replace('//', '/')
        path = os.path.abspath(os.path.join(base, rel_path))
        if not os.path.exists(path):
            print("\033[41m \"{path}\": is no exist, please check!\033[0m".format(path=path))
            compileFailAndExit()
        return path

checkIniFile(inifile)
config.read(inifile, encoding='utf-8')

checkKeyExist(run_section, keyMustBeSet)
checkKeyExist(common_section, keyCommonMustBeSet)

for key in keyBoolTypeSet :
    checkBoolValue(key)

if config.has_option(run_section, 'netbin_folder_path'):
    netbin_folder_path = os.path.join(inipath, config.get(run_section, 'netbin_folder_path'))
else:
    netbin_folder_path = os.path.join(inipath, os.path.splitext(os.path.basename(inifile))[0])

if config.has_option(run_section, 'img_path'):
    img_path = decodePath(inipath, config.get(run_section, 'img_path'))
    checkFileExist('img_path', img_path)
else :
    img_path = None

model_info_json_path = os.path.join(netbin_folder_path, "model_info.json")
if not os.path.exists(model_info_json_path):
    model_info_json_path = None

img_shape_c = config.getint(common_section, 'input_shape_c')
img_shape = [config.getint(common_section, 'input_shape_h'), config.getint(common_section, 'input_shape_w')]
dec_onchip = config.getboolean(run_section, 'dec_onchip')

gray_en = False
if img_shape_c == 1:
    gray_en = True
elif img_shape_c == 3:
    gray_en = False

loop_times = 1
if config.has_option(run_section, 'loop_times') :
    loop_times = config.getint(run_section, 'loop_times')

if loop_times <= 1:
    loop_times = 1

print("\n\033[42m--------------------------------------------Run Config-----------------------------------------------------\n")

print("\tnetbin_folder_path\t" + os.path.abspath(str(netbin_folder_path)))
print("\timg_path\t\t" + os.path.abspath(str(img_path)))
print("\tmodel_info\t\t" + str(model_info_json_path))
print("\timg_shape\t\t" + str(img_shape))
print("\tdec_onchip\t\t" + str(dec_onchip))
print("\tloop_times\t\t" + str(loop_times))
print("\n---------------------------------------------------------------------------------------------------------------\033[0m\n")
sys.stdout.flush()

try :
    nn_deploy = init()
    print('\ninit success.\n')
    sys.stdout.flush()
    test_loop_run_model(loop_times, nn_deploy, netbin_folder_path, img_shape, img_path=img_path, batch=[1], dec_onchip=dec_onchip, model_info_json_path=model_info_json_path, gray_en=gray_en)
except :
    print('\n\033[5;41mMODEL_RUN_FAIL\033[0m\n\n')
    sys.stdout.flush()
    raise

print('\n\033[5;42mMODEL_RUN_SUCCESS\033[0m\n\n')
sys.stdout.flush()
