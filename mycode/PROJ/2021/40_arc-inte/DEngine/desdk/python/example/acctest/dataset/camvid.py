import numpy as np
import cv2
import os

def calc_camvid_acc(test_out, labels):

    assert len(test_out) == len(labels), "length of test_out not equal to labels."

    iter = len(test_out)

    numClasses = 11
    unknown_class = 11

    totalpoints = 0
    cf = np.zeros((iter, numClasses, numClasses))
    globalacc = 0.0


    for ind in range(iter):
        assert os.path.exists(test_out[ind]), "out file %s not exitsts."%(test_out[ind])
        assert os.path.exists(labels[ind]), "label file %s not exitsts."%(labels[ind])
        pred = cv2.imread(test_out[ind], cv2.IMREAD_GRAYSCALE)

        # pred = pred + 1
        annot = cv2.imread(labels[ind], cv2.IMREAD_GRAYSCALE)
        # annot = annot + 1

        # pixels_ignore = annot == unknown_class
        # pred[pixels_ignore] = -1
        # annot[pixels_ignore] = -1

        totalpoints += np.sum(annot < unknown_class)

        for j in range(numClasses):
            for k in range(numClasses):
                c1 = annot == j
                c1p = pred == k
                index = c1 * c1p
                a = np.sum(index)
                cf[ind, j, k] += np.sum(index)

            c1 = annot == j
            c1p = pred == j
            index = c1 * c1p
            globalacc += np.sum(index)
        a = 0
    cf = np.sum(cf, axis=0)
    cf = np.squeeze(cf)

    #Compute confusion matrix
    conf = np.zeros((numClasses, numClasses))
    for i in range(numClasses):
        if i != unknown_class and np.sum(cf[i, :]) > 0:
            conf[i,:] = cf[i,:] / np.sum(cf[i,:])

    globalacc = np.sum(globalacc) / np.sum(totalpoints)

    #Compute intersection over union for each class and its mean
    intoverunion = np.zeros((numClasses,1))
    for i in range(numClasses):
        if i != unknown_class and np.sum(conf[i,:]) > 0:
            intoverunion[i] = (cf[i,i])/(np.sum(cf[i,:])+np.sum(cf[:,i])-cf[i,i])

    result = "[ Global acc = %.4f, Class average acc = %.4f, Mean Int over Union = %.4f ]"%(globalacc, np.sum(np.diag(conf))/(numClasses), np.sum(intoverunion)/(numClasses))
    return result

