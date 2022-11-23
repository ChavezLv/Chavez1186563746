import os
import os.path as osp
import pickle
import numpy as np
import itertools
import datetime
import time
from collections import defaultdict
import copy
import json
import scipy.sparse
from .voc2007 import nms, iou_calc1



class Params:
    '''
    Params for coco evaluation api
    '''
    def __init__(self):
        self.imgIds = []
        self.catIds = []
        # np.arange causes trouble.  the data point on arange is slightly larger than the true value
        self.iouThrs = np.linspace(.5, 0.95, int(np.round((0.95-.5)/.05))+1, endpoint=True)
        self.recThrs = np.linspace(.0, 1.00, int(np.round((1.00-.0)/.01))+1, endpoint=True)
        self.maxDets = [1, 10, 100]
        self.areaRng = [[0**2, 1e5**2], [0**2, 32**2], [32**2, 96**2], [96**2, 1e5**2]]
        self.useSegm = 0
        self.useCats = 1

class mask:
    def iou(dt, gt, iscrowd):
        #todo
        o = np.zeros([len(dt), len(gt)])
        m = len(dt)
        n = len(gt)

        if len(dt) == 0 or len(gt) == 0:
            return []
        o = np.zeros([len(dt), len(gt)])
        m = len(dt)
        n = len(gt)
        for d in range(m):
            x1 = dt[d][0]
            y1 = dt[d][1]
            x2 = dt[d][0]+dt[d][2]
            y2 = dt[d][1]+dt[d][3]
            area_d = (x2-x1) * (y2-y1)
            for g in range(n):
                x1_ = gt[g][0]
                y1_ = gt[g][1]
                x2_ = gt[g][0]+gt[g][2]
                y2_ = gt[g][1]+gt[g][3]
                area_g = (x2_-x1_) * (y2_-y1_)

                xx1 = max(x1, x1_)
                yy1 = max(y1, y1_)
                xx2 = min(x2, x2_)
                yy2 = min(y2, y2_)

                w = xx2 - xx1
                if w <= 0:
                    continue
                h = yy2 - yy1
                if h <= 0:
                    continue
                inter = w * h
                u = area_d + area_g - inter + 0.1 if iscrowd[g] == 0 else area_d
                o[d, g] = inter / u
        return o

class COCOeval:
    # Interface for evaluating detection on the Microsoft COCO dataset.
    #
    # The usage for CocoEval is as follows:
    #  cocoGt=..., cocoDt=...       # load dataset and results
    #  E = CocoEval(cocoGt,cocoDt); # initialize CocoEval object
    #  E.params.recThrs = ...;      # set parameters as desired
    #  E.evaluate();                # run per image evaluation
    #  E.accumulate();              # accumulate per image results
    #  E.summarize();               # display summary metrics of results
    # For example usage see evalDemo.m and http://mscoco.org/.
    #
    # The evaluation parameters are as follows (defaults in brackets):
    #  imgIds     - [all] N img ids to use for evaluation
    #  catIds     - [all] K cat ids to use for evaluation
    #  iouThrs    - [.5:.05:.95] T=10 IoU thresholds for evaluation
    #  recThrs    - [0:.01:1] R=101 recall thresholds for evaluation
    #  areaRng    - [...] A=4 object area ranges for evaluation
    #  maxDets    - [1 10 100] M=3 thresholds on max detections per image
    #  useSegm    - [1] if true evaluate against ground-truth segments
    #  useCats    - [1] if true use category labels for evaluation    #
    #  Note: if useSegm=0 the evaluation is run on bounding boxes.
    # Note: if useCats=0 category labels are ignored as in proposal scoring.
    # Note: multiple areaRngs [Ax2] and maxDets [Mx1] can be specified.
    #
    # evaluate(): evaluates detections on every image and every category and
    # concats the results into the "evalImgs" with fields:
    #  dtIds      - [1xD] id for each of the D detections (dt)
    #  gtIds      - [1xG] id for each of the G ground truths (gt)
    #  dtMatches  - [TxD] matching gt id at each IoU or 0
    #  gtMatches  - [TxG] matching dt id at each IoU or 0
    #  dtScores   - [1xD] confidence of each dt
    #  gtIgnore   - [1xG] ignore flag for each gt
    #  dtIgnore   - [TxD] ignore flag for each dt at each IoU
    #
    # accumulate(): accumulates the per-image, per-category evaluation
    # results in "evalImgs" into the dictionary "eval" with fields:
    #  params     - parameters used for evaluation
    #  date       - date evaluation was performed
    #  counts     - [T,R,K,A,M] parameter dimensions (see above)
    #  precision  - [TxRxKxAxM] precision for every evaluation setting
    #  recall     - [TxKxAxM] max recall for every evaluation setting
    # Note: precision and recall==-1 for settings with no gt objects.
    #
    # See also coco, mask, pycocoDemo, pycocoEvalDemo
    #
    # Microsoft COCO Toolbox.      version 2.0
    # Data, paper, and tutorials available at:  http://mscoco.org/
    # Code written by Piotr Dollar and Tsung-Yi Lin, 2015.
    # Licensed under the Simplified BSD License [see coco/license.txt]
    def __init__(self, cocoGt=None, cocoDt=None):
        '''
        Initialize CocoEval using coco APIs for gt and dt
        :param cocoGt: coco object with ground truth annotations
        :param cocoDt: coco object with detection results
        :return: None
        '''
        self.cocoGt = cocoGt              # ground truth COCO API
        self.cocoDt = cocoDt              # detections COCO API
        self.params   = {}                  # evaluation parameters
        self.evalImgs = defaultdict(list)   # per-image per-category evaluation results [KxAxI] elements
        self.eval     = {}                  # accumulated evaluation results
        self._gts = defaultdict(list)       # gt for evaluation
        self._dts = defaultdict(list)       # dt for evaluation
        self.params = Params()              # parameters
        self._paramsEval = {}               # parameters for evaluation
        self.stats = []                     # result summarization
        self.ious = {}                      # ious between all gts and dts
        if not cocoGt is None:
            self.params.imgIds = sorted(cocoGt.getImgIds()) #TODO  cocoGt
            self.params.catIds = sorted(cocoGt.getCatIds())


    def _prepare(self):
        '''
        Prepare ._gts and ._dts for evaluation based on params
        :return: None
        '''
        #
        def _toMask(objs, coco):
            # modify segmentation by reference
            for obj in objs:
                t = coco.imgs[obj['image_id']]
                if type(obj['segmentation']) == list:
                    pass
                elif type(obj['segmentation']) == dict and type(obj['segmentation']['counts']) == list:
                    pass
                else:
                    raise Exception('segmentation format not supported.')
        p = self.params
        if p.useCats:
            gts=self.cocoGt.loadAnns(self.cocoGt.getAnnIds(imgIds=p.imgIds, catIds=p.catIds))
            dts=self.cocoDt.loadAnns(self.cocoDt.getAnnIds(imgIds=p.imgIds, catIds=p.catIds))
        else:
            gts=self.cocoGt.loadAnns(self.cocoGt.getAnnIds(imgIds=p.imgIds))
            dts=self.cocoDt.loadAnns(self.cocoDt.getAnnIds(imgIds=p.imgIds))

        if p.useSegm:
            _toMask(gts, self.cocoGt)
            _toMask(dts, self.cocoDt)
        self._gts = defaultdict(list)       # gt for evaluation
        self._dts = defaultdict(list)       # dt for evaluation
        for gt in gts:
            self._gts[gt['image_id'], gt['category_id']].append(gt)
        for dt in dts:
            self._dts[dt['image_id'], dt['category_id']].append(dt)
        self.evalImgs = defaultdict(list)   # per-image per-category evaluation results
        self.eval     = {}                  # accumulated evaluation results

    def evaluate(self):
        '''
        Run per image evaluation on given images and store results (a list of dict) in self.evalImgs
        :return: None
        '''
        tic = time.time()
        print('Running per image evaluation...      ')
        p = self.params
        p.imgIds = list(np.unique(p.imgIds))
        if p.useCats:
            p.catIds = list(np.unique(p.catIds))
        p.maxDets = sorted(p.maxDets)
        self.params=p

        self._prepare()
        # loop through images, area range, max detection number
        catIds = p.catIds if p.useCats else [-1]

        computeIoU = self.computeIoU
        self.ious = {(imgId, catId): computeIoU(imgId, catId) \
                        for imgId in p.imgIds
                        for catId in catIds}

        evaluateImg = self.evaluateImg
        maxDet = p.maxDets[-1]
        self.evalImgs = [evaluateImg(imgId, catId, areaRng, maxDet)
                 for catId in catIds
                 for areaRng in p.areaRng
                 for imgId in p.imgIds
             ]
        self._paramsEval = copy.deepcopy(self.params)
        toc = time.time()
        print('DONE (t=%0.2fs).'%(toc-tic))

    def computeIoU(self, imgId, catId):
        p = self.params
        if p.useCats:
            gt = self._gts[imgId,catId]
            dt = self._dts[imgId,catId]
        else:
            gt = [_ for cId in p.catIds for _ in self._gts[imgId,cId]]
            dt = [_ for cId in p.catIds for _ in self._dts[imgId,cId]]
        if len(gt) == 0 or len(dt) ==0:
            return []
        dt = sorted(dt, key=lambda x: -x['score'])
        if len(dt) > p.maxDets[-1]:
            dt=dt[0:p.maxDets[-1]]

        if p.useSegm:
            g = [g['segmentation'] for g in gt]
            d = [d['segmentation'] for d in dt]
        else:
            g = [g['bbox'] for g in gt]
            d = [d['bbox'] for d in dt]

        # compute iou between each dt and gt region
        iscrowd = [int(o['iscrowd']) for o in gt]
        ious = mask.iou(d,g,iscrowd)
        #print('\nious = ', ious)
        return ious

    def evaluateImg(self, imgId, catId, aRng, maxDet):
        '''
        perform evaluation for single category and image
        :return: dict (single image results)
        '''
        #
        p = self.params
        if p.useCats:
            gt = self._gts[imgId,catId]
            dt = self._dts[imgId,catId]
        else:
            gt = [_ for cId in p.catIds for _ in self._gts[imgId,cId]]
            dt = [_ for cId in p.catIds for _ in self._dts[imgId,cId]]
        if len(gt) == 0 and len(dt) ==0:
            return None

        for g in gt:
            if 'ignore' not in g:
                g['ignore'] = 0
            if g['iscrowd'] == 1 or g['ignore'] or (g['area']<aRng[0] or g['area']>aRng[1]):
                g['_ignore'] = 1
            else:
                g['_ignore'] = 0

        # sort dt highest score first, sort gt ignore last
        # gt = sorted(gt, key=lambda x: x['_ignore'])
        #gtind = [ind for (ind, g) in sorted(enumerate(gt), key=lambda (ind, g): g['_ignore']) ] #TODO

        #gt = [gt[ind] for ind in gtind]
        #dt = sorted(dt, key=lambda x: -x['score'])[0:maxDet]

        gtind = np.argsort([g['_ignore'] for g in gt], kind='mergesort')
        gt = [gt[i] for i in gtind]
        dtind = np.argsort([-d['score'] for d in dt], kind='mergesort')
        dt = [dt[i] for i in dtind[0:maxDet]]

        iscrowd = [int(o['iscrowd']) for o in gt]
        # load computed ious
        N_iou = len(self.ious[(imgId, catId)])
        ious = self.ious[(imgId, catId)][0:maxDet, np.array(gtind)] if N_iou >0 else self.ious[(imgId, catId)]

        T = len(p.iouThrs)
        G = len(gt)
        D = len(dt)
        gtm  = np.zeros((T,G))
        dtm  = np.zeros((T,D))
        gtIg = np.array([g['_ignore'] for g in gt])
        dtIg = np.zeros((T,D))

        if not len(ious)==0:
            for tind, t in enumerate(p.iouThrs):
                for dind, d in enumerate(dt):
                    # information about best match so far (m=-1 -> unmatched)
                    iou = min([t,1-1e-10])
                    m   = -1
                    for gind, g in enumerate(gt):
                        # if this gt already matched, and not a crowd, continue
                        if gtm[tind,gind]>0 and not iscrowd[gind]:
                            continue
                        # if dt matched to reg gt, and on ignore gt, stop
                        if m>-1 and gtIg[m]==0 and gtIg[gind]==1:
                            break
                        # continue to next gt unless better match made
                        if ious[dind,gind] < iou:
                            continue
                        # match successful and best so far, store appropriately
                        iou=ious[dind,gind]
                        m=gind
                    # if match made store id of match for both dt and gt
                    if m ==-1:
                        continue
                    dtIg[tind,dind] = gtIg[m]
                    dtm[tind,dind]  = gt[m]['id']
                    gtm[tind,m]     = d['id']
        # set unmatched detections outside of area range to ignore
        a = np.array([d['area']<aRng[0] or d['area']>aRng[1] for d in dt]).reshape((1, len(dt)))
        dtIg = np.logical_or(dtIg, np.logical_and(dtm==0, np.repeat(a,T,0)))
        # store results for given image and category
        return {
                'image_id':     imgId,
                'category_id':  catId,
                'aRng':         aRng,
                'maxDet':       maxDet,
                'dtIds':        [d['id'] for d in dt],
                'gtIds':        [g['id'] for g in gt],
                'dtMatches':    dtm,
                'gtMatches':    gtm,
                'dtScores':     [d['score'] for d in dt],
                'gtIgnore':     gtIg,
                'dtIgnore':     dtIg,
            }

    def accumulate(self, p = None):
        '''
        Accumulate per image evaluation results and store the result in self.eval
        :param p: input params for evaluation
        :return: None
        '''
        print('Accumulating evaluation results...   ')
        tic = time.time()
        if not self.evalImgs:
            print('Please run evaluate() first')
        # allows input customized parameters
        if p is None:
            p = self.params
        p.catIds = p.catIds if p.useCats == 1 else [-1]
        T           = len(p.iouThrs)
        R           = len(p.recThrs)
        K           = len(p.catIds) if p.useCats else 1
        A           = len(p.areaRng)
        M           = len(p.maxDets)
        precision   = -np.ones((T,R,K,A,M)) # -1 for the precision of absent categories
        recall      = -np.ones((T,K,A,M))

        # create dictionary for future indexing
        _pe = self._paramsEval
        catIds = _pe.catIds if _pe.useCats else [-1]
        setK = set(catIds)
        setA = set(map(tuple, _pe.areaRng))
        setM = set(_pe.maxDets)
        setI = set(_pe.imgIds)
        # get inds to evaluate
        k_list = [n for n, k in enumerate(p.catIds)  if k in setK]
        m_list = [m for n, m in enumerate(p.maxDets) if m in setM]
        a_list = [n for n, a in enumerate(map(lambda x: tuple(x), p.areaRng)) if a in setA]
        i_list = [n for n, i in enumerate(p.imgIds)  if i in setI]
        # K0 = len(_pe.catIds)
        I0 = len(_pe.imgIds)
        A0 = len(_pe.areaRng)
        # retrieve E at each category, area range, and max number of detections
        for k, k0 in enumerate(k_list):
            Nk = k0*A0*I0
            for a, a0 in enumerate(a_list):
                Na = a0*I0
                for m, maxDet in enumerate(m_list):
                    E = [self.evalImgs[Nk+Na+i] for i in i_list]
                    #E = filter(None, E)
                    E = [e for e in E if not e is None]
                    if len(E) == 0:
                        continue
                    dtScores = np.concatenate([e['dtScores'][0:maxDet] for e in E])

                    # different sorting method generates slightly different results.
                    # mergesort is used to be consistent as Matlab implementation.
                    inds = np.argsort(-dtScores, kind='mergesort')

                    dtm  = np.concatenate([e['dtMatches'][:,0:maxDet] for e in E], axis=1)[:,inds]
                    dtIg = np.concatenate([e['dtIgnore'][:,0:maxDet]  for e in E], axis=1)[:,inds]
                    gtIg = np.concatenate([e['gtIgnore']  for e in E])
                    npig = len([ig for ig in gtIg if ig == 0])
                    if npig == 0:
                        continue
                    tps = np.logical_and(               dtm,  np.logical_not(dtIg) )
                    fps = np.logical_and(np.logical_not(dtm), np.logical_not(dtIg) )

                    tp_sum = np.cumsum(tps, axis=1).astype(dtype=np.float)
                    fp_sum = np.cumsum(fps, axis=1).astype(dtype=np.float)
                    for t, (tp, fp) in enumerate(zip(tp_sum, fp_sum)):
                        tp = np.array(tp)
                        fp = np.array(fp)
                        nd = len(tp)
                        rc = tp / npig
                        pr = tp / (fp+tp+np.spacing(1))
                        q  = np.zeros((R,))

                        if nd:
                            recall[t,k,a,m] = rc[-1]
                        else:
                            recall[t,k,a,m] = 0

                        # numpy is slow without cython optimization for accessing elements
                        # use python array gets significant speed improvement
                        pr = pr.tolist(); q = q.tolist()

                        for i in range(nd-1, 0, -1):
                            if pr[i] > pr[i-1]:
                                pr[i-1] = pr[i]

                        inds = np.searchsorted(rc, p.recThrs)
                        try:
                            for ri, pi in enumerate(inds):
                                q[ri] = pr[pi]
                        except:
                            pass
                        precision[t,:,k,a,m] = np.array(q)
        self.eval = {
            'params': p,
            'counts': [T, R, K, A, M],
            'date': datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            'precision': precision,
            'recall':   recall,
        }
        toc = time.time()
        print('DONE (t=%0.2fs).'%( toc-tic ))

    def summarize(self):
        '''
        Compute and display summary metrics for evaluation results.
        Note this functin can *only* be applied on the default parameter setting
        '''
        def _summarize( ap=1, iouThr=None, areaRng='all', maxDets=100 ):
            p = self.params
            iStr        = ' {:<18} {} @[ IoU={:<9} | area={:>6} | maxDets={:>3} ] = {}'
            titleStr    = 'Average Precision' if ap == 1 else 'Average Recall'
            typeStr     = '(AP)' if ap==1 else '(AR)'
            iouStr      = '%0.2f:%0.2f'%(p.iouThrs[0], p.iouThrs[-1]) if iouThr is None else '%0.2f'%(iouThr)
            areaStr     = areaRng
            maxDetsStr  = '%d'%(maxDets)

            aind = [i for i, aRng in enumerate(['all', 'small', 'medium', 'large']) if aRng == areaRng]
            mind = [i for i, mDet in enumerate([1, 10, 100]) if mDet == maxDets]
            if ap == 1:
                # dimension of precision: [TxRxKxAxM]
                s = self.eval['precision']
                # IoU
                if iouThr is not None:
                    t = np.where(iouThr == p.iouThrs)[0]
                    s = s[t]
                # areaRng
                s = s[:,:,:,aind,mind]
            else:
                # dimension of recall: [TxKxAxM]
                s = self.eval['recall']
                s = s[:,:,aind,mind]
            if len(s[s>-1])==0:
                mean_s = -1
            else:
                mean_s = np.mean(s[s>-1])

            return mean_s

        if not self.eval:
            raise Exception('Please run accumulate() first')
        self.stats = np.zeros((12,))
        self.stats[0] = _summarize(1)
        self.stats[1] = _summarize(1,iouThr=.5)
        self.stats[2] = _summarize(1,iouThr=.75)
        self.stats[3] = _summarize(1,areaRng='small')
        self.stats[4] = _summarize(1,areaRng='medium')
        self.stats[5] = _summarize(1,areaRng='large')
        self.stats[6] = _summarize(0,maxDets=1)
        self.stats[7] = _summarize(0,maxDets=10)
        self.stats[8] = _summarize(0,maxDets=100)
        self.stats[9]  = _summarize(0,areaRng='small')
        self.stats[10] = _summarize(0,areaRng='medium')
        self.stats[11] = _summarize(0,areaRng='large')

    def __str__(self):
        self.summarize()

class COCO:
    def __init__(self, annotation_file=None):
        """
        Constructor of Microsoft COCO helper class for reading and visualizing annotations.
        :param annotation_file (str): location of annotation file
        :param image_folder (str): location to the folder that hosts images.
        :return:
        """
        # load dataset
        self.dataset = {}
        self.anns = []
        self.imgToAnns = {}
        self.catToImgs = {}
        self.imgs = {}
        self.cats = {}
        if not annotation_file == None:
            print('loading annotations into memory...', flush=True)
            tic = time.time()
            dataset = json.load(open(annotation_file, 'r'))
            print('Done (t=%0.2fs)'%(time.time()- tic), flush=True)
            self.dataset = dataset
            self.createIndex()

    def createIndex(self):
        # create index
        print('creating index...', flush=True)
        anns = {}
        imgToAnns = {}
        catToImgs = {}
        cats = {}
        imgs = {}
        if 'annotations' in self.dataset:
            imgToAnns = {ann['image_id']: [] for ann in self.dataset['annotations']}
            anns =      {ann['id']:       [] for ann in self.dataset['annotations']}
            for ann in self.dataset['annotations']:
                imgToAnns[ann['image_id']] += [ann]
                anns[ann['id']] = ann

        if 'images' in self.dataset:
            imgs      = {im['id']: {} for im in self.dataset['images']}
            for img in self.dataset['images']:
                imgs[img['id']] = img

        if 'categories' in self.dataset:
            cats = {cat['id']: [] for cat in self.dataset['categories']}
            for cat in self.dataset['categories']:
                cats[cat['id']] = cat
            catToImgs = {cat['id']: [] for cat in self.dataset['categories']}
            if 'annotations' in self.dataset:
                for ann in self.dataset['annotations']:
                    catToImgs[ann['category_id']] += [ann['image_id']]

        print('index created!', flush=True)

        # create class members
        self.anns = anns
        self.imgToAnns = imgToAnns
        self.catToImgs = catToImgs
        self.imgs = imgs
        self.cats = cats

    def info(self):
        """
        Print information about the annotation file.
        :return:
        """
        for key, value in self.dataset['info'].items():
            print('%s: %s'%(key, value))

    def getAnnIds(self, imgIds=[], catIds=[], areaRng=[], iscrowd=None):
        """
        Get ann ids that satisfy given filter conditions. default skips that filter
        :param imgIds  (int array)     : get anns for given imgs
               catIds  (int array)     : get anns for given cats
               areaRng (float array)   : get anns for given area range (e.g. [0 inf])
               iscrowd (boolean)       : get anns for given crowd label (False or True)
        :return: ids (int array)       : integer array of ann ids
        """
        imgIds = imgIds if type(imgIds) == list else [imgIds]
        catIds = catIds if type(catIds) == list else [catIds]

        if len(imgIds) == len(catIds) == len(areaRng) == 0:
            anns = self.dataset['annotations']
        else:
            if not len(imgIds) == 0:
                # this can be changed by defaultdict
                lists = [self.imgToAnns[imgId] for imgId in imgIds if imgId in self.imgToAnns]
                anns = list(itertools.chain.from_iterable(lists))
            else:
                anns = self.dataset['annotations']
            anns = anns if len(catIds)  == 0 else [ann for ann in anns if ann['category_id'] in catIds]
            anns = anns if len(areaRng) == 0 else [ann for ann in anns if ann['area'] > areaRng[0] and ann['area'] < areaRng[1]]
        if not iscrowd == None:
            ids = [ann['id'] for ann in anns if ann['iscrowd'] == iscrowd]
        else:
            ids = [ann['id'] for ann in anns]
        return ids

    def getCatIds(self, catNms=[], supNms=[], catIds=[]):
        """
        filtering parameters. default skips that filter.
        :param catNms (str array)  : get cats for given cat names
        :param supNms (str array)  : get cats for given supercategory names
        :param catIds (int array)  : get cats for given cat ids
        :return: ids (int array)   : integer array of cat ids
        """
        catNms = catNms if type(catNms) == list else [catNms]
        supNms = supNms if type(supNms) == list else [supNms]
        catIds = catIds if type(catIds) == list else [catIds]

        if len(catNms) == len(supNms) == len(catIds) == 0:
            cats = self.dataset['categories']
        else:
            cats = self.dataset['categories']
            cats = cats if len(catNms) == 0 else [cat for cat in cats if cat['name']          in catNms]
            cats = cats if len(supNms) == 0 else [cat for cat in cats if cat['supercategory'] in supNms]
            cats = cats if len(catIds) == 0 else [cat for cat in cats if cat['id']            in catIds]
        ids = [cat['id'] for cat in cats]
        return ids

    def getImgIds(self, imgIds=[], catIds=[]):
        '''
        Get img ids that satisfy given filter conditions.
        :param imgIds (int array) : get imgs for given ids
        :param catIds (int array) : get imgs with all given cats
        :return: ids (int array)  : integer array of img ids
        '''
        imgIds = imgIds if type(imgIds) == list else [imgIds]
        catIds = catIds if type(catIds) == list else [catIds]

        if len(imgIds) == len(catIds) == 0:
            ids = self.imgs.keys()
        else:
            ids = set(imgIds)
            for i, catId in enumerate(catIds):
                if i == 0 and len(ids) == 0:
                    ids = set(self.catToImgs[catId])
                else:
                    ids &= set(self.catToImgs[catId])
        return list(ids)

    def loadAnns(self, ids=[]):
        """
        Load anns with the specified ids.
        :param ids (int array)       : integer ids specifying anns
        :return: anns (object array) : loaded ann objects
        """
        if type(ids) == list:
            return [self.anns[id] for id in ids]
        elif type(ids) == int:
            return [self.anns[ids]]

    def loadCats(self, ids=[]):
        """
        Load cats with the specified ids.
        :param ids (int array)       : integer ids specifying cats
        :return: cats (object array) : loaded cat objects
        """
        if type(ids) == list:
            return [self.cats[id] for id in ids]
        elif type(ids) == int:
            return [self.cats[ids]]

    def loadRes(self, anns):
        """
        Load result file and return a result api object.
        :param   resFile (str)     : file name of result file
        :return: res (obj)         : result api object
        """
        res = COCO()
        res.dataset['images'] = [img for img in self.dataset['images']]
        # res.dataset['info'] = copy.deepcopy(self.dataset['info'])
        # res.dataset['licenses'] = copy.deepcopy(self.dataset['licenses'])

        print('Loading and preparing results...     ')
        tic = time.time()
        #anns    = json.load(open(resFile))
        assert type(anns) == list, 'results in not an array of objects'
        annsImgIds = [ann['image_id'] for ann in anns]
        assert set(annsImgIds) == (set(annsImgIds) & set(self.getImgIds())), \
               'Results do not correspond to current coco set'
        if 'caption' in anns[0]:
            imgIds = set([img['id'] for img in res.dataset['images']]) & set([ann['image_id'] for ann in anns])
            res.dataset['images'] = [img for img in res.dataset['images'] if img['id'] in imgIds]
            for id, ann in enumerate(anns):
                ann['id'] = id+1
        elif 'bbox' in anns[0] and not anns[0]['bbox'] == []:
            res.dataset['categories'] = copy.deepcopy(self.dataset['categories'])
            for id, ann in enumerate(anns):
                bb = ann['bbox']
                x1, x2, y1, y2 = [bb[0], bb[0]+bb[2], bb[1], bb[1]+bb[3]]
                if not 'segmentation' in ann:
                    ann['segmentation'] = [[x1, y1, x1, y2, x2, y2, x2, y1]]
                ann['area'] = bb[2]*bb[3]
                ann['id'] = id+1
                ann['iscrowd'] = 0
        elif 'segmentation' in anns[0]:
            res.dataset['categories'] = copy.deepcopy(self.dataset['categories'])
            for id, ann in enumerate(anns):
                # now only support compressed RLE format as segmentation results
                ann['area'] = mask.area([ann['segmentation']])[0]
                if not 'bbox' in ann:
                    ann['bbox'] = mask.toBbox([ann['segmentation']])[0]
                ann['id'] = id+1
                ann['iscrowd'] = 0
        print('DONE (t=%0.2fs)'%(time.time()- tic))

        res.dataset['annotations'] = anns
        res.createIndex()
        return res


    def loadImgs(self, ids=[]):
        """
        Load anns with the specified ids.
        :param ids (int array)       : integer ids specifying img
        :return: imgs (object array) : loaded img objects
        """
        if type(ids) == list:
            return [self.imgs[id] for id in ids]
        elif type(ids) == int:
            return [self.imgs[ids]]

class ds_utils():
    def unique_boxes(boxes, scale=1.0):
        """Return indices of unique boxes."""
        v = np.array([1, 1e3, 1e6, 1e9])
        hashes = np.round(boxes * scale).dot(v)
        _, index = np.unique(hashes, return_index=True)
        return np.sort(index)

    def xywh_to_xyxy(boxes):
        """Convert [x y w h] box format to [x1 y1 x2 y2] format."""
        return np.hstack((boxes[:, 0:2], boxes[:, 0:2] + boxes[:, 2:4] - 1))

    def xyxy_to_xywh(boxes):
        """Convert [x1 y1 x2 y2] box format to [x y w h] format."""
        return np.hstack((boxes[:, 0:2], boxes[:, 2:4] - boxes[:, 0:2] + 1))

    def validate_boxes(boxes, width=0, height=0):
        """Check that a set of boxes are valid."""
        x1 = boxes[:, 0]
        y1 = boxes[:, 1]
        x2 = boxes[:, 2]
        y2 = boxes[:, 3]
        assert (x1 >= 0).all()
        assert (y1 >= 0).all()
        assert (x2 >= x1).all()
        assert (y2 >= y1).all()
        assert (x2 < width).all()
        assert (y2 < height).all()

    def filter_small_boxes(boxes, min_size):
        w = boxes[:, 2] - boxes[:, 0]
        h = boxes[:, 3] - boxes[:, 1]
        keep = np.where((w >= min_size) & (h > min_size))[0]
        return keep

class coco():
    def __init__(self, image_set, anno_path,year):
        # COCO specific config options
        self.config = {'top_k' : 2000,
                       'use_salt' : True,
                       'cleanup' : True,
                       'crowd_thresh' : 0.7,
                       'rpn_file': None,
                       'min_size' : 2}
        # name, paths
        self._year = year
        self._image_set = image_set
        self._data_path = osp.join("/home/yhh/Desktop/", 'coco')
        # load COCO API, classes, class <-> id mappings
        self._COCO = COCO(anno_path)
        cats = self._COCO.loadCats(self._COCO.getCatIds())
        self._classes = tuple(['__background__'] + [c['name'] for c in cats])
        self._class_to_ind = dict(zip(self.classes, range(self.num_classes)))
        self._class_to_coco_cat_id = dict(zip([c['name'] for c in cats],
                                              self._COCO.getCatIds()))
        self._image_index = self._load_image_set_index()
        # Default to roidb handler
        #self.set_proposal_method('selective_search')
        self.competition_mode(False)

        # Some image sets are "views" (i.e. subsets) into others.
        # For example, minival2014 is a random 5000 image subset of val2014.
        # This mapping tells us where the view's images and proposals come from.
        self._view_map = {
            'minival2014' : 'val2014',          # 5k val2014 subset
            'valminusminival2014' : 'val2014',  # val2014 \setminus minival2014
        }
        coco_name = image_set # + year  # e.g., "val2014"
        self._data_name = 'val2014'
        #self._data_name = (self._view_map[coco_name]
        #                   if self._view_map.has_key(coco_name)
        #                   else coco_name)
        # Dataset splits that have ground-truth annotations (test splits
        # do not have gt annotations)
        self._gt_splits = ('train', 'val', 'minival')

    @property
    def num_classes(self):
        return len(self._classes)

    @property
    def classes(self):
        return self._classes

    @property
    def image_index(self):
        return self._image_index

    def _get_ann_file(self):
        prefix = 'instances' if self._image_set.find('test') == -1 \
                             else 'image_info'
        return osp.join(self._data_path, 'annotations',
                        prefix + '_' + self._image_set + self._year + '.json')

    def _load_image_set_index(self):
        """
        Load image ids.
        """
        image_ids = self._COCO.getImgIds()
        return image_ids

    def _get_widths(self):
        anns = self._COCO.loadImgs(self._image_index)
        widths = [ann['width'] for ann in anns]
        return widths

    def image_path_at(self, i):
        """
        Return the absolute path to image i in the image sequence.
        """
        return self.image_path_from_index(self._image_index[i])

    def image_path_from_index(self, index):
        """
        Construct an image path from the image's "index" identifier.
        """
        # Example image path for index=119993:
        #   images/train2014/COCO_train2014_000000119993.jpg
        prefix = 'COCO_' + self._data_name + '_' if self._year == "2014" else ""
        file_name = ( prefix +
                     str(index).zfill(12) + '.jpg')
        image_path = osp.join(self._image_set, file_name)
        if osp.exists(image_path):
            return image_path
        assert osp.exists(image_path), 'Path does not exist: {}'.format(image_path)
        return

    def selective_search_roidb(self):
        return self._roidb_from_proposals('selective_search')

    def edge_boxes_roidb(self):
        return self._roidb_from_proposals('edge_boxes_AR')

    def mcg_roidb(self):
        return self._roidb_from_proposals('MCG')

    def rpn_roidb(self):
        pass

    def _load_rpn_roidb(self, gt_roidb):
        pass

    def _roidb_from_proposals(self, method):
        """
        Creates a roidb from pre-computed proposals of a particular methods.
        """
        pass

    def _load_proposals(self, method, gt_roidb):
        pass

    def gt_roidb(self):
        """
        Return the database of ground-truth regions of interest.
        This function loads/saves from/to a cache file to speed up future calls.
        """
        pass

    def _load_coco_annotation(self, index):
        """
        Loads COCO bounding-box instance annotations. Crowd instances are
        handled by marking their overlaps (with all categories) to -1. This
        overlap value means that crowd "instances" are excluded from training.
        """
        im_ann = self._COCO.loadImgs(index)[0]
        width = im_ann['width']
        height = im_ann['height']

        annIds = self._COCO.getAnnIds(imgIds=index, iscrowd=None)
        objs = self._COCO.loadAnns(annIds)
        # Sanitize bboxes -- some are invalid
        valid_objs = []
        for obj in objs:
            x1 = np.max((0, obj['bbox'][0]))
            y1 = np.max((0, obj['bbox'][1]))
            x2 = np.min((width - 1, x1 + np.max((0, obj['bbox'][2] - 1))))
            y2 = np.min((height - 1, y1 + np.max((0, obj['bbox'][3] - 1))))
            if obj['area'] > 0 and x2 >= x1 and y2 >= y1:
                obj['clean_bbox'] = [x1, y1, x2, y2]
                valid_objs.append(obj)
        objs = valid_objs
        num_objs = len(objs)

        boxes = np.zeros((num_objs, 4), dtype=np.uint16)
        gt_classes = np.zeros((num_objs), dtype=np.int32)
        overlaps = np.zeros((num_objs, self.num_classes), dtype=np.float32)
        seg_areas = np.zeros((num_objs), dtype=np.float32)

        # Lookup table to map from COCO category ids to our internal class
        # indices
        coco_cat_id_to_class_ind = dict([(self._class_to_coco_cat_id[cls],
                                          self._class_to_ind[cls])
                                         for cls in self._classes[1:]])

        for ix, obj in enumerate(objs):
            cls = coco_cat_id_to_class_ind[obj['category_id']]
            boxes[ix, :] = obj['clean_bbox']
            gt_classes[ix] = cls
            seg_areas[ix] = obj['area']
            if obj['iscrowd']:
                # Set overlap to -1 for all classes for crowd objects
                # so they will be excluded during training
                overlaps[ix, :] = -1.0
            else:
                overlaps[ix, cls] = 1.0

        ds_utils.validate_boxes(boxes, width=width, height=height)
        overlaps = scipy.sparse.csr_matrix(overlaps)
        return {'boxes' : boxes,
                'gt_classes': gt_classes,
                'gt_overlaps' : overlaps,
                'flipped' : False,
                'seg_areas' : seg_areas}

    def _get_box_file(self, index):
        # first 14 chars / first 22 chars / all chars + .mat
        # COCO_val2014_0/COCO_val2014_000000447/COCO_val2014_000000447991.mat
        file_name = ('COCO_' + self._data_name +
                     '_' + str(index).zfill(12) + '.mat')
        return osp.join(file_name[:14], file_name[:22], file_name)

    def _print_detection_eval_metrics(self, coco_eval):
        IoU_lo_thresh = 0.5
        IoU_hi_thresh = 0.5
        def _get_thr_ind(coco_eval, thr):
            ind = np.where((coco_eval.params.iouThrs > thr - 1e-5) &
                           (coco_eval.params.iouThrs < thr + 1e-5))[0][0]
            iou_thr = coco_eval.params.iouThrs[ind]
            assert np.isclose(iou_thr, thr)
            return ind

        ind_lo = _get_thr_ind(coco_eval, IoU_lo_thresh)
        ind_hi = _get_thr_ind(coco_eval, IoU_hi_thresh)
        # precision has dims (iou, recall, cls, area range, max dets)
        # area range index 0: all area ranges
        # max dets index 2: 100 per image
        precision = \
            coco_eval.eval['precision'][ind_lo:(ind_hi + 1), :, :, 0, 2]
        ap_default = np.mean(precision[precision > -1])
        print ('~~~~ Mean and per-category AP @ IoU=[{:.2f},{:.2f}] ~~~~'.format(IoU_lo_thresh, IoU_hi_thresh))
        print('{:.1f}'.format(100 * ap_default))
        result = '~~~~ Mean and per-category AP @ IoU=[{:.2f},{:.2f}] ~~~~\n'.format(IoU_lo_thresh, IoU_hi_thresh)
        result += '{:.1f} '.format(100 * ap_default)
        for cls_ind, cls in enumerate(self.classes):
            if cls == '__background__':
                continue
            # minus 1 because of __background__
            precision = coco_eval.eval['precision'][ind_lo:(ind_hi + 1), :, cls_ind - 1, 0, 2]
            ap = np.mean(precision[precision > -1])
            print('{:.1f}'.format(100 * ap))
            result += '{:.1f} '.format(100 * ap)

        print('~~~~ Summary metrics ~~~~')
        coco_eval.summarize()
        return result

    def _do_detection_eval(self, res_file_result, output_dir):
        ann_type = 'bbox'
        coco_dt = self._COCO.loadRes(res_file_result)
        coco_eval = COCOeval(self._COCO, coco_dt)
        coco_eval.params.useSegm = (ann_type == 'segm')
        coco_eval.evaluate()
        coco_eval.accumulate()
        result = ""
        result = self._print_detection_eval_metrics(coco_eval)
        #eval_file = osp.join(output_dir, 'detection_results.pkl')
        #with open(eval_file, 'wb') as fid:
        #    pickle.dump(coco_eval, fid, pickle.HIGHEST_PROTOCOL)
        #print('Wrote COCO eval results to: {}'.format(eval_file))
        return result
    def _coco_results_one_category(self, boxes, cat_id):
        results = []
        for im_ind, index in enumerate(self.image_index):
            dets = boxes[im_ind] #.astype(np.float)
            if dets == []:
                continue
            scores = dets[:, -1]
            xs = dets[:, 0]
            ys = dets[:, 1]
            ws = dets[:, 2] - xs + 1
            hs = dets[:, 3] - ys + 1
            results.extend(
              [{'image_id' : index,
                'category_id' : cat_id,
                'bbox' : [xs[k], ys[k], ws[k], hs[k]],
                'score' : scores[k]} for k in range(dets.shape[0])])
        return results

    def _write_coco_results_file(self, all_boxes, res_file):
        # [{"image_id": 42,
        #   "category_id": 18,
        #   "bbox": [258.15,41.29,348.26,243.78],
        #   "score": 0.236}, ...]
        results = []
        for cls_ind, cls in enumerate(self.classes):
            if cls == '__background__':
                continue
            print('Collecting {} results ({:d}/{:d})'.format(cls, cls_ind, self.num_classes - 1))
            coco_cat_id = self._class_to_coco_cat_id[cls]
            results.extend(self._coco_results_one_category(all_boxes[cls_ind],
                                                           coco_cat_id))
        print('Writing results json to {}'.format(res_file))
        with open(res_file, 'w') as fid:
            json.dump(results, fid)
        return results

    def evaluate_detections(self, all_boxes, output_dir):
        res_file = osp.join(output_dir, ('detections_results.json'))
        dt_results = self._write_coco_results_file(all_boxes, res_file)
        ap_result = ""
        # Only do evaluation on non-test sets
        print("self._image_set.find('test'): ", self._image_set.find('test') )
        # if self._image_set.find('test') == -1:
            #self._do_detection_eval(res_file, output_dir)
        ap_result = self._do_detection_eval(dt_results, output_dir)
        # Optionally cleanup results json file
        #if self.config['cleanup']:
        #    os.remove(res_file)
        return ap_result

    def competition_mode(self, on):
        if on:
            self.config['use_salt'] = False
            self.config['cleanup'] = False
        else:
            self.config['use_salt'] = True
            self.config['cleanup'] = True



def convert_pred_transform_yolov3_caffe(pred_bbox, test_input_size, bias, mask):
    """
    box transform
    """
    pred_bbox = np.array(pred_bbox)
    pred_xywh = pred_bbox[0]
    coor_shape = pred_xywh.shape

    for ii in range(coor_shape[0]):
        for jj in range(coor_shape[1]):
            pred_xywh[ii, jj, 0] = (pred_xywh[ii,jj,0] + jj) / coor_shape[0]
            pred_xywh[ii, jj, 1] = (pred_xywh[ii,jj,1] + ii) / coor_shape[0]
            pred_xywh[ii, jj, 2] = np.exp(pred_xywh[ii,jj,2]) * bias[2*mask] / test_input_size
            pred_xywh[ii, jj, 3] = np.exp(pred_xywh[ii,jj,3]) * bias[2*mask+1] / test_input_size

            pred_xywh[ii, jj, 0+85] = (pred_xywh[ii, jj, 0+85] + jj) / coor_shape[0]
            pred_xywh[ii, jj, 1+85] = (pred_xywh[ii, jj, 1+85] + ii) / coor_shape[0]
            pred_xywh[ii, jj, 2+85] = np.exp(pred_xywh[ii, jj, 2+85]) * bias[2 * (mask+1)] / test_input_size
            pred_xywh[ii, jj, 3+85] = np.exp(pred_xywh[ii, jj, 3+85]) * bias[2 * (mask+1) + 1] / test_input_size

            pred_xywh[ii, jj, 0+85*2] = (pred_xywh[ii, jj, 0+85*2] + jj) / coor_shape[0]
            pred_xywh[ii, jj, 1+85*2] = (pred_xywh[ii, jj, 1+85*2] + ii) / coor_shape[0]
            pred_xywh[ii, jj, 2+85*2] = np.exp(pred_xywh[ii, jj, 2+85*2]) * bias[2 * (mask+2)] / test_input_size
            pred_xywh[ii, jj, 3+85*2] = np.exp(pred_xywh[ii, jj, 3+85*2]) * bias[2 * (mask+2) + 1] / test_input_size

    pred_xywh = pred_xywh.reshape([-1,85])
    return pred_xywh

def convert_pred_yolov3_caffe(pred_bbox, test_input_size, org_img_shape, valid_scale, score_thread):
    """

    """
    pred_bbox = np.array(pred_bbox)

    pred_xywh = pred_bbox[:, 0:4]
    pred_conf = pred_bbox[:, 4]
    pred_prob = pred_bbox[:, 5:]
    h, w = org_img_shape
    netw = test_input_size
    neth = test_input_size

    if netw/w < neth/h:
        new_w = netw
        new_h = (h * netw) / w
    else:
        new_h = neth;
        new_w = (w * neth) / h;
    pred_xywh[:,0] = (pred_xywh[:,0] - (netw - new_w)/2./netw) / (new_w/netw)
    pred_xywh[:,1] = (pred_xywh[:,1] - (neth - new_h)/2./neth) / (new_h/neth)
    pred_xywh[:, 2] *= netw/new_w
    pred_xywh[:, 3] *= neth/new_h

    pred_xywh[:,0] *= w
    pred_xywh[:,1] *= h
    pred_xywh[:, 2] *= w
    pred_xywh[:, 3] *= h

    # (1) (x, y, w, h) --> (xmin, ymin, xmax, ymax)
    pred_coor = np.concatenate([(pred_xywh[:, :2] - pred_xywh[:, 2:] * 0.5 )* 1,
                                (pred_xywh[:, :2] + pred_xywh[:, 2:] * 0.5 )* 1], axis=-1)
    # (2)
    # 需要注意的是，无论我们在训练的时候使用什么数据增强方式，都不影响此处的转换方式
    # 假设我们对输入测试图片使用了转换方式A，那么此处对bbox的转换方式就是方式A的逆向过程
    org_h, org_w = org_img_shape

    # (3)将预测的bbox中超出原图的部分裁掉
    pred_coor = np.concatenate([np.maximum(pred_coor[:, :2], [0, 0]),
                                np.minimum(pred_coor[:, 2:], [org_w - 1, org_h - 1])], axis=-1)
    # (4)将无效bbox的coor置为0
    invalid_mask = np.logical_or((pred_coor[:, 0] > pred_coor[:, 2]), (pred_coor[:, 1] > pred_coor[:, 3]))
    pred_coor[invalid_mask] = 0

    # (4)去掉不在有效范围内的bbox
    bboxes_scale = np.sqrt(np.multiply.reduce(pred_coor[:, 2:4] - pred_coor[:, 0:2], axis=-1))
    scale_mask = np.logical_and((valid_scale[0] < bboxes_scale), (bboxes_scale < valid_scale[1]))

    # (4)将score低于score_threshold的bbox去掉
    classes = np.argmax(pred_prob, axis=-1)
    scores = pred_conf * pred_prob[np.arange(len(pred_coor)), classes]
    score_mask = scores > score_thread

    mask = np.logical_and(scale_mask, score_mask)

    coors = pred_coor[mask]
    scores = scores[mask]
    classes = classes[mask]

    bboxes = np.concatenate([coors, scores[:, np.newaxis], classes[:, np.newaxis]], axis=-1)

    return bboxes


def nms(bboxes, score_threshold, iou_threshold, sigma=0.3, method='nms'):
    """
    :param bboxes:
    假设有N个bbox的score大于score_threshold，那么bboxes的shape为(N, 6)，存储格式为(xmin, ymin, xmax, ymax, score, class)
    其中(xmin, ymin, xmax, ymax)的大小都是相对于输入原图的，score = conf * prob，class是bbox所属类别的索引号
    :return: best_bboxes
    假设NMS后剩下N个bbox，那么best_bboxes的shape为(N, 6)，存储格式为(xmin, ymin, xmax, ymax, score, class)
    其中(xmin, ymin, xmax, ymax)的大小都是相对于输入原图的，score = conf * prob，class是bbox所属类别的索引号
    """
    classes_in_img = list(set(bboxes[:, 5]))
    best_bboxes = []

    for cls in classes_in_img:
        cls_mask = (bboxes[:, 5] == cls)
        cls_bboxes = bboxes[cls_mask]
        while len(cls_bboxes) > 0:
            max_ind = np.argmax(cls_bboxes[:, 4])
            best_bbox = cls_bboxes[max_ind]
            best_bboxes.append(best_bbox)
            cls_bboxes = np.concatenate([cls_bboxes[: max_ind], cls_bboxes[max_ind + 1:]])
            iou = iou_calc1(best_bbox[np.newaxis, :4], cls_bboxes[:, :4])
            assert method in ['nms', 'soft-nms']
            weight = np.ones((len(iou),), dtype=np.float32)
            if method == 'nms':
                iou_mask = iou > iou_threshold
                weight[iou_mask] = 0.0
            if method == 'soft-nms':
                weight = np.exp(-(1.0 * iou ** 2 / sigma))
            cls_bboxes[:, 4] = cls_bboxes[:, 4] * weight
            score_mask = cls_bboxes[:, 4] > score_threshold
            cls_bboxes = cls_bboxes[score_mask]
    return best_bboxes


def cal_coco2014_accuracy(dataset_info, datapath = None, result_file = "result.pkl"):
    """
    :param dataset_info:
    :param data_path:
    :param result_file:
    :return:
    """

    assert "name" in dataset_info, "name field not in model_info dict."
    assert "datapath" in dataset_info, "datapath field not in model_info dict."
    assert "ann_path" in dataset_info, "year field not in model_info dict."

    image_set = dataset_info["name"]
    if datapath == None:
        env = os.environ
        if "TEST_DATA_ROOT" not in os.environ:
            datasets_root_path = "./datasets"
            assert os.path.exists(
                datasets_root_path), "datasets not exist in current folder."
        else:
            datasets_root_path = os.environ["TEST_DATA_ROOT"]
            assert os.path.exists(
                datasets_root_path), "TEST_DATA_ROOT not exist in system."
        datapath = os.path.join(datasets_root_path, image_set)

    assert os.path.isdir(datapath), "datasets must exist folder {}".format(datapath)

    dataset_path = os.path.join(datapath, dataset_info["datapath"])
    ann_file = os.path.join(datapath, dataset_info["ann_path"])
    assert os.path.isdir(dataset_path), "datasets must exist folder {}".format(dataset_path)
    assert os.path.isfile(ann_file), "annotation.json must exist".format(ann_file)
    coco_class = coco(dataset_path, ann_file, '2014')

    assert os.path.isfile(result_file), "result_file %s not exist."%(result_file)

    with open(result_file, "rb") as fb:
        all_boxes = pickle.load( fb)

    output_dir = "result"
    result = coco_class.evaluate_detections(all_boxes, output_dir)
    return result