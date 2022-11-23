# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#pylint: disable=unused-argument
"""Internal module for registering attribute for annotation."""

import cv2
import numpy as np

def get_image_use_cv2_pad(imgpath='', rgb_en=1, model_size=(320, 320), \
                          norm_en=0, img_mean=(0.0, 0.0, 0.0), img_scale=1.0, \
                          nhwc=1, correct_box=False, bboxes=None, pad_value=127.5):
    """
    ratio resize then pad128
    """
    image = cv2.imread(imgpath)
    target_shape = (model_size[0], model_size[1])

    h_target, w_target = target_shape
    h_org, w_org, _ = image.shape

    if rgb_en == 1:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB).astype(np.float32)

    resize_ratio = min(1.0 * w_target / w_org, 1.0 * h_target / h_org)
    resize_w = int(resize_ratio * w_org)
    resize_h = int(resize_ratio * h_org)
    image_resized = cv2.resize(image, (resize_w, resize_h))

    image_paded = np.full((h_target, w_target, 3), pad_value)
    dw_p = int((w_target - resize_w) / 2)
    dh_p = int((h_target - resize_h) / 2)
    image_paded[dh_p:resize_h+dh_p, dw_p:resize_w+dw_p, :] = image_resized
    image = image_paded / 1.0 #255.0

    img = np.array([image], dtype=np.float32)
    if nhwc == 0:
        img = np.swapaxes(img, 1, 3)
        img = np.swapaxes(img, 2, 3)

    if correct_box:
        bboxes[:, [0, 2]] = bboxes[:, [0, 2]] * resize_ratio + dw_p
        bboxes[:, [1, 3]] = bboxes[:, [1, 3]] * resize_ratio + dh_p
        return image, bboxes
    return img

def get_image_use_cv2_square(imgpath='', rgb_en=0, model_size=(224, 224), \
                            norm_en=0, img_mean=(0.0, 0.0, 0.0), img_scale=1.0, interp=None):
    """
    crop resize
    """
    # img shape is BHWC, RGB format if rgb_en is True, otherwise is BGR format
    assert cv2 is not None, '`load_img` requires `cv2`.'
    # interp = cv2.INTER_LINEAR
    if interp is None:
        interp = cv2.INTER_LINEAR

    img = cv2.imread(imgpath)
    if img is None:
        return None

    h_d, w_d, _ = img.shape
    if h_d < w_d:
        off = int((w_d-h_d)/2)
        img = img[:, off:off+h_d]
    else:
        off = int((h_d-w_d)/2)
        img = img[off:off+w_d, :]
    img = cv2.resize(img, model_size, interpolation=interp)

    if rgb_en:
        img = img[:, :, ::-1]

    if len(img.shape) == 2:
        img = np.expand_dims(img, -1)

    img = np.array([img], dtype=np.float32)
    # change img shape to BCHW
    img = np.swapaxes(img, 1, 3)
    img = np.swapaxes(img, 2, 3)

    if norm_en == 0:
        return img

    means = np.array(img_mean)
    if isinstance(img_scale, float):
        scale = np.array([img_scale, img_scale, img_scale])
    else:
        assert len(img_scale) == 3, "scale len is not 1, must be 3"
        scale = np.array([img_scale[0], img_scale[1], img_scale[2]])
    means = means[np.newaxis, :, np.newaxis, np.newaxis]  # per-pixel mean subtracted
    img = img - means
    scale = scale[np.newaxis, :, np.newaxis, np.newaxis]
    img /= scale
    img = img.astype('float32')
    return img

def crop_start_idx(total_size, crop_size, crop_loc, crop_grid):
    """
    get row and col start idx according to given shape for crop image
    :param total_size: int, tuple or list, (h,w) format when tuple or list
    :param crop_size: int, tuple or list, (h,w) format when tuple or list
    :param crop_loc: ???
    :param crop_grid: ???
    :return:row_start, col_start
    """
    # corp_size: h,w
    if isinstance(total_size, int):
        total_size = (total_size, total_size)
    if isinstance(crop_size, int):
        crop_size = (crop_size, crop_size)
    if crop_loc > -1:
        row_loc = crop_loc // crop_grid[0]
        col_loc = crop_loc % crop_grid[1]
        row_start = row_loc * (total_size[0] - crop_size[0]) // 2
        col_start = col_loc * (total_size[1] - crop_size[1]) // 2
    else:
        row_start = np.random.randint(0, total_size[0] - crop_size[0], 1)[0]
        col_start = np.random.randint(0, total_size[1] - crop_size[1], 1)[0]
    return row_start, col_start

def crop_image(img, crop_size, crop_loc=4, crop_grid=(3, 3)):
    """
    crop image to crop_size
    :param img: ndarray, BHWC shape
    :param crop_size: int or tuple, (h,w) format when tuple
    :param crop_loc:????
    :param crop_grid:????
    :return:ndarray, bhwc format
    """
    # crop_size: h, w
    if isinstance(crop_size, int):
        crop_size = (crop_size, crop_size)
    if isinstance(crop_loc, list):
        imgs = np.zeros((img.shape[0], len(crop_loc), crop_size[1], crop_size[0], 3), np.float32)
        for (i, loc) in enumerate(crop_loc):
            r_value, c_value = crop_start_idx(img.shape[1:3], crop_size, loc, crop_grid)
            imgs[:, i] = img[:, r_value:r_value+crop_size[0], c_value:c_value+crop_size[1], :]
        return imgs
    elif crop_loc == np.prod(crop_grid) + 1:
        imgs = np.zeros((img.shape[0], crop_loc, crop_size[1], crop_size[0], 3), np.float32)
        r_value, c_value = crop_start_idx(img.shape[1:3], crop_size, 4, crop_grid)
        imgs[:, 0] = img[:, r_value:r_value+crop_size[0], c_value:c_value+crop_size[1], :]
        imgs[:, 1] = img[:, 0:crop_size[0], 0:crop_size[1], :]
        imgs[:, 2] = img[:, 0:crop_size[0], -crop_size[1]:, :]
        imgs[:, 3] = img[:, -crop_size[0]:, 0:crop_size[1], :]
        imgs[:, 4] = img[:, -crop_size[0]:, -crop_size[1]:, :]
        imgs[:, 5:] = np.flip(imgs[:, :5], axis=3)
        return imgs

    r_value, c_value = crop_start_idx(img.shape[1:3], crop_size, crop_loc, crop_grid)
    return img[:, r_value:r_value+crop_size[0], c_value:c_value+crop_size[1], :]

def load_img_according_to(paths, rgb_en, target_size, crop_size, interp=None):
    """
    support crop resize
    """
    assert cv2 is not None, '`load_img` requires `cv2`.'
    if interp is None:
        interp = cv2.INTER_LINEAR #2020.11.24
    if not isinstance(paths, list):
        paths = [paths]
    if len(paths) > 1 and (target_size is None or
                           isinstance(target_size, int)):
        raise ValueError('A tuple `target_size` should be provided '
                         'when loading multiple images.')

    def _load_img(path):
        # imread:BGR, HWC
        img = cv2.imread(path)

        if target_size:
            # equal scaling the min(H, W) to target_size
            # get target W/H, so wh_tuple = (W, H)
            if isinstance(target_size, int):
                wh_tuple = tuple([x * target_size // min(img.shape[:2])
                                  for x in img.shape[1::-1]])
            else:
                wh_tuple = (target_size[1], target_size[0])

            # resize the img to target W/H, the params of resize must be (w, h)
            if img.shape[1::-1] != wh_tuple:
                img = cv2.resize(img, wh_tuple, interpolation=interp)
        # convert BGR to RGB if enable
        if rgb_en:
            img = img[:, :, ::-1]

        if len(img.shape) == 2:
            img = np.expand_dims(img, -1)

        # cur img shape is HWC, RGB or BGR format, the min(H, W) is target_size
        return img

    if len(paths) > 1:
        imgs = np.zeros((len(paths),) + target_size + (3,), dtype=np.float32)
        for (i, path) in enumerate(paths):
            imgs[i] = _load_img(path)
    else:
        imgs = np.array([_load_img(paths[0])], dtype=np.float32)

    #imgs shape is (1,h,w,c) before crop
    if crop_size is not None:
        imgs = crop_image(imgs, crop_size)

    return imgs

def get_image_use_cv2_crop(imgpath='', rgb_en=0, target_size=256, corp_shape=(224, 224), \
                            norm_en=0, img_mean=(0.0, 0.0, 0.0), img_scale=1.0, \
                            interp=cv2.INTER_LINEAR):
    """
    crop resize
    """
    # img shape is BHWC, RGB format if rgb_en is True, otherwise is BGR format
    img = load_img_according_to(imgpath, rgb_en, target_size, corp_shape, interp=interp)

    if img is None:
        return None

    # change img shape to BCHW
    img = np.swapaxes(img, 1, 3)
    img = np.swapaxes(img, 2, 3)

    if norm_en == 0:
        return img

    means = np.array(img_mean)
    if isinstance(img_scale, float):
        scale = np.array([img_scale, img_scale, img_scale])
    else:
        assert len(img_scale) == 3, "scale len is not 1, must be 3"
        scale = np.array([img_scale[0], img_scale[1], img_scale[2]])
    means = means[np.newaxis, :, np.newaxis, np.newaxis]  # per-pixel mean subtracted
    img = img - means
    scale = scale[np.newaxis, :, np.newaxis, np.newaxis]
    img /= scale
    img = img.astype('float32')
    return img

def deal_image_use_cv2_method0(paths, net_size, rgb_en=1, interp=None,
                               norm_en=0, img_mean=(0.0, 0.0, 0.0), img_scale=(1.0), gray_img=0):
    """get resize data"""
    # gray_img = 1
    if len(net_size) == 3:
        if net_size[2] == 1:
            gray_img = 1
    assert cv2 is not None, '`load_img` requires `cv2`.'
    if interp is None:
        interp = cv2.INTER_LINEAR
        # interp = cv2.INTER_AREA
    if not isinstance(paths, list):
        paths = [paths]

    def _load_img(path):
        # imread:BGR, HWC
        if gray_img == 0:
            img = cv2.imread(path)
        else:
            img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
            # rgb_en = 0
        wh_tuple = (net_size[1], net_size[0])
        # resize the img to target W/H, the params of resize must be (w, h)
        if img.shape[1::-1] != wh_tuple:
            img = cv2.resize(img, wh_tuple, interpolation=interp)
        # convert BGR to RGB if enable
        if rgb_en and gray_img == 0:
            img = img[:, :, ::-1]
        if len(img.shape) == 2:
            img = np.expand_dims(img, -1)
        # cur img shape is HWC, RGB or BGR format, the min(H, W) is target_size
        return img

    if len(paths) > 1:
        imgs = np.zeros((len(paths),) + (3,), dtype=np.float32)
        for (i, path) in enumerate(paths):
            imgs[i] = _load_img(path)
    else:
        imgs = np.array([_load_img(paths[0])], dtype=np.float32)

    if imgs is None:
        return None

    # change img shape to BCHW
    img = np.swapaxes(imgs, 1, 3)
    img = np.swapaxes(img, 2, 3)

    if norm_en == 0:
        return img
    means = np.array(img_mean)
    if isinstance(img_scale, float):
        scale = np.array([img_scale, img_scale, img_scale])
    else:
        assert len(img_scale) == 3, "scale len is not 1, must be 3"
        scale = np.array([img_scale[0], img_scale[1], img_scale[2]])

    means = means[np.newaxis, :, np.newaxis, np.newaxis]  # per-pixel mean subtracted
    img = img - means
    scale = scale[np.newaxis, :, np.newaxis, np.newaxis]
    img /= scale
    img = img.astype('float32')
    return img

def im_list_to_blob(ims_):
    """Convert a list of images into a network input.

    Assumes images are already prepared (means subtracted, BGR order, ...).
    """
    max_shape = np.array([im_.shape for im_ in ims_]).max(axis=0)
    num_images = len(ims_)
    blob = np.zeros((num_images, max_shape[0], max_shape[1], 3),
                    dtype=np.float32)
    for i in range(num_images):
        im_i = ims_[i]
        blob[i, 0:im_i.shape[0], 0:im_i.shape[1], :] = im_i
    # Move channels (axis 3) to axis 1
    # Axis order will become: (batch elem, channel, height, width)
    channel_swap = (0, 3, 1, 2)
    blob = blob.transpose(channel_swap)
    return blob

FALG_WARNING = [0]
def _get_image_blob_resize(img_in, net_size=(600,), iminfo_shape=(1, 3)):
    """Converts an image into a network input"""

    im_orig = img_in.astype(np.float32, copy=True)
    #im_orig -= pix_means

    im_shape = im_orig.shape

    processed_ims = []
    im_scale_factors = []

    im_h_size = im_shape[0]
    im_w_size = im_shape[1]
    im_h_ratio = float(net_size[0]) / float(im_h_size)
    im_w_ratio = float(net_size[1]) / float(im_w_size)

    # rfcn/faster_rcnn... now do resize according to net_shape
    im_resize = cv2.resize(im_orig, (net_size[1], net_size[0]), \
                           interpolation=cv2.INTER_LINEAR)

    if iminfo_shape[1] == 3:
        im_scale_factors.append(im_w_ratio)
        if im_h_ratio != im_w_ratio and FALG_WARNING[0] == 0:
            FALG_WARNING[0] = 1
            print("~warning! iminfo shape is [1 3], but find resize_ratio is {:.3f} and {:.3f}".
                  format(im_h_ratio, im_w_ratio))
    else:
        assert iminfo_shape[1] == 4, "when iminfo shape not [1 3], this function only support [1,4]"
        im_scale_factors.append(im_w_ratio)
        im_scale_factors.append(im_h_ratio)

    processed_ims.append(im_resize)
    # Create a blob to hold the input images
    blob = im_list_to_blob(processed_ims)
    if iminfo_shape[1] == 3:
        im_info = np.array([[blob.shape[2], blob.shape[3], im_scale_factors[0]]], dtype=np.float32)
    else:
        im_info = np.array([[blob.shape[2], blob.shape[3], im_scale_factors[0],
                             im_scale_factors[1]]], dtype=np.float32)
    return [blob, im_info]

def _get_image_blob(img_in, net_size=(600,)):
    """Converts an image into a network input.

    Arguments:
        im (ndarray): a color image in BGR order

    Returns:
        blob (ndarray): a data blob holding an image pyramid
        im_scale_factors (list): list of image scales (relative to im) used
            in the image pyramid
    """
    #pix_means = np.array([[[102.9801, 115.9465, 122.7717]]])
    max_size = 1000

    im_orig = img_in.astype(np.float32, copy=True)
    #im_orig -= pix_means

    im_shape = im_orig.shape
    im_size_min = np.min(im_shape[0:2])
    im_size_max = np.max(im_shape[0:2])

    processed_ims = []
    im_scale_factors = []

    for target_size in net_size:
        im_scale = float(target_size) / float(im_size_min)
        # Prevent the biggest axis from being more than MAX_SIZE
        if np.round(im_scale * im_size_max) > max_size:
            im_scale = float(max_size) / float(im_size_max)
        # rfcn_normal_deal
        im_resize = cv2.resize(im_orig, None, None, fx=im_scale, fy=im_scale, \
                               interpolation=cv2.INTER_LINEAR)
        # rfcn_special_deal
        im_resize = cv2.resize(im_orig, (800, target_size), \
                            interpolation=cv2.INTER_LINEAR) #todo not 600 800
        im_scale_factors.append(im_scale)
        processed_ims.append(im_resize)

    # Create a blob to hold the input images
    blob = im_list_to_blob(processed_ims)
    im_info = np.array([[blob.shape[2], blob.shape[3], im_scale_factors[0]]], dtype=np.float32)
    return [blob, im_info]

def deal_image_use_cv2_rfcn(paths, net_size=(600,), iminfo_shape=(1, 3), rgb_en=0, interp=None):
    """get resize data"""
    img_ = cv2.imread(paths)

    data_iminfo = _get_image_blob_resize(img_, net_size, iminfo_shape)

    return data_iminfo

def img_resize(imgpath, model_size, rgb_en=1, \
                norm_en=0, img_mean=(0.0, 0.0, 0.0), \
                img_scale=1.0, method=0, nhwc=0, input2_shape=(1, 3), interp=cv2.INTER_LINEAR):
    """resize img and get the result"""
    # intellif deepeye predeal default use:
    #  1, input_num = 1: methond = 0
    #  2, input_num = 2: methond = 1
    # when custorm your own predeal:
    #  1, deepeye_quantize config 'custom_pre_deal' = 1
    #  2, write your own code in method=5
    #    2.1 when 1 output , make sure numpy
    #    2.2 when 2 outputs, the output is [out1, out2], both numpy


    if method == 0: #cossponding to crgh, fixedno need to normize
        image_data = deal_image_use_cv2_method0(imgpath, model_size, rgb_en=rgb_en,
                                                norm_en=norm_en,
                                                img_mean=img_mean,
                                                img_scale=img_scale, interp=interp)
    elif method == 1:
        image_data = deal_image_use_cv2_rfcn(imgpath, (model_size[0], model_size[1]),
                                             iminfo_shape=input2_shape)
    elif method == 2:
        target_size = 256
        image_data = get_image_use_cv2_crop(imgpath=imgpath, rgb_en=rgb_en, \
                        target_size=target_size, corp_shape=model_size, \
                        norm_en=norm_en, \
                        img_mean=img_mean, \
                        img_scale=img_scale, \
                        interp=interp)
    elif method == 3:
        image_data = get_image_use_cv2_square(imgpath=imgpath, rgb_en=rgb_en, \
                        model_size=model_size, \
                        norm_en=norm_en, \
                        img_mean=img_mean, \
                        img_scale=img_scale, \
                        interp=interp)
    elif method == 4:
        image_data = get_image_use_cv2_pad(imgpath=imgpath, rgb_en=rgb_en, \
                        model_size=model_size, \
                        norm_en=norm_en, \
                        img_mean=img_mean, \
                        img_scale=img_scale, \
                        nhwc=nhwc)
    elif method == 5:
        # custom your owm pre_deal
        # for example:
        image_data = deal_image_use_cv2_method0(imgpath, model_size, rgb_en=rgb_en,
                                                norm_en=norm_en,
                                                img_mean=img_mean,
                                                img_scale=img_scale)
        input1 = np.ones((1, 313, 56, 56)).astype('float32')
        image_data = [image_data, input1]
    return image_data
