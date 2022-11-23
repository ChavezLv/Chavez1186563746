import numpy as np
from PIL import Image
import os
import scipy.io

def make_palette(num_classes):
    """
    Maps classes to colors in the style of PASCAL VOC.
    Close values are mapped to far colors for segmentation visualization.
    See http://host.robots.ox.ac.uk/pascal/VOC/voc2012/index.html#devkit

    Takes:
        num_classes: the number of classes
    Gives:
        palette: the colormap as a k x 3 array of RGB colors
    """
    palette = np.zeros((num_classes, 3), dtype=np.uint8)
    for k in range(0, num_classes):
        label = k
        i = 0
        while label:
            palette[k, 0] |= (((label >> 0) & 1) << (7 - i))
            palette[k, 1] |= (((label >> 1) & 1) << (7 - i))
            palette[k, 2] |= (((label >> 2) & 1) << (7 - i))
            label >>= 3
            i += 1
    return palette

def color_seg(seg, palette):
    """
    Replace classes with their colors.

    Takes:
        seg: H x W segmentation image of class IDs
    Gives:
        H x W x 3 image of class colors
    """
    return palette[seg.flat].reshape(seg.shape + (3,))

def vis_seg(img, seg, palette, alpha=0.5):
    """
    Visualize segmentation as an overlay on the image.

    Takes:
        img: H x W x 3 image in [0, 255]
        seg: H x W segmentation image of class IDs
        palette: K x 3 colormap for all classes
        alpha: opacity of the segmentation in [0, 1]
    Gives:
        H x W x 3 image with overlaid segmentation
    """
    vis = np.array(img, dtype=np.float32)
    mask = seg > 0
    vis[mask] *= 1. - alpha
    vis[mask] += alpha * palette[seg[mask].flat]
    vis = vis.astype(np.uint8)
    return vis


def fast_hist(a, b, n):
    k = (a >= 0) & (a < n)
    return np.bincount(n * a[k].astype(int) + b[k], minlength=n**2).reshape(n, n)

def calc_voc_seg_acc(res_gt_lst, class_nums = 21):
    """
    Visualize segmentation as an overlay on the image.

    Takes:
        res_gt_lst: list that contains out file and ground_truth file
    Gives:
        H x W x 3 image with overlaid segmentation
    """
    hist = np.zeros((class_nums, class_nums))

    for item in res_gt_lst:
        # res = np.load(item[0])
        out = Image.open(item[0])
        res = np.array(out, dtype=np.uint8)

        im = Image.open(item[1])
        label = np.array(im, dtype=np.uint8)
        label0 = np.array(im, dtype=np.float32)
        # label = label[np.newaxis, ...]

        hist += fast_hist(label.flatten(), res.flatten(),
                          class_nums)
    # overall accuracy
    global_acc = np.diag(hist).sum() / hist.sum()
    # per-class accuracy
    cls_acc = np.diag(hist) / hist.sum(1)
    # per-class IU
    iu = np.diag(hist) / (hist.sum(1) + hist.sum(0) - np.diag(hist))
    freq = hist.sum(1) / hist.sum()
    freq_out = freq[freq > 0] * iu[freq > 0].sum()
    res = "overall accuracy: %6.3f, mean accuracy: %6.3f, mean IU: %6.3f"%\
          (global_acc, np.nanmean(cls_acc), np.nanmean(iu))
    # print(res)
    # print("fwavacc: ", freq_out)
    return res


def load_pascal59_label(file, labels_400_file, labels_59_file):
    """
    Load label image as 1 x height x width integer array of label indices.
    The leading singleton dimension is required by the loss.
    The full 400 labels are translated to the 59 class task labels.
    """
    mat = scipy.io.loadmat(file)

    label_400 = mat['LabelMap']
    label = np.zeros_like(label_400, dtype=np.uint8)
    # for idx, name in  np.genfromtxt(labels_400_file , delimiter=':', dtype=None):
    #     a = name.replace(" ", "")
    #     b = 0
    labels_400 = [label for idx, label in
                       np.genfromtxt(labels_400_file , delimiter=':', dtype=None)]
    labels_59 = [label for idx, label in
                      np.genfromtxt(labels_59_file, delimiter=':', dtype=None)]
    for idx, l in enumerate(labels_59):
        idx_400 = labels_400.index(l) + 1
        label[label_400 == idx_400] = idx + 1
    # label = label[np.newaxis, ...]
    return label

