#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import sys
import time
import signal
import math 
from sklearn.preprocessing import label_binarize
from sklearn.metrics import precision_recall_curve
from scipy.sparse import csr_matrix

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

reload(sys)
sys.setdefaultencoding('utf8')

fname = "data/state_libsvm.txt"
if len(sys.argv) > 1 and len(sys.argv[1])>0:
  fname = sys.argv[1]

from sklearn.datasets import load_svmlight_file
from sklearn.linear_model import LogisticRegression
all_X,all_y=load_svmlight_file(fname)

regressionFunc =LogisticRegression(C=10, penalty='l2', tol=0.00001, verbose=0)

n_samples = len(all_y)
idx = range(n_samples)

step = 5

fig = plt.figure()
ax = fig.add_axes([0.1, 0.1, 0.65, 0.85])

for id1 in range(step):
  print "++ sample %d - %d" % (step, id1)
  trainIdx = []
  testIdx = []
  for id2 in range(n_samples):
    if id2 % step == id1:
      testIdx.append(id2)
    else:
      trainIdx.append(id2)

  train_X = all_X[trainIdx]
  train_y = all_y[trainIdx]
  test_X = all_X[testIdx]
  test_y = all_y[testIdx]

  train_sco = regressionFunc.fit(train_X,train_y).score(train_X,train_y)

  test_probs = regressionFunc.predict_proba(test_X)
  print " - score:%f" % (regressionFunc.score(test_X,test_y))
#print regressionFunc.__dict__ 

#print test_y
  test_btargets = label_binarize(test_y, classes=[-1, 1])
  n_classes = test_btargets.shape[1]

#print test_btargets
#  print n_classes
  precision = dict()
  recall = dict()
  for i in range(n_classes):
    precision[i], recall[i], _ = precision_recall_curve(test_btargets[:, i], test_probs[:, i])
    ax.plot(recall[i], precision[i], label='PR-'+str(id1) + ":" + str(i))

ax.set_xlabel('Recall')
ax.set_ylabel('Precision')
ax.set_ylim([0.0, 1.05])
ax.set_xlim([0.0, 1.0])
#plt.title('Precision-Recall')
plt.legend(bbox_to_anchor=(1.05, 0.5), loc="center left")
plt.savefig('myfig')

