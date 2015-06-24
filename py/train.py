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

regressionFunc =LogisticRegression(C=1.0, penalty='l2', tol=0.00001, verbose=0)

n_samples = len(all_y)
idx = range(n_samples)

step = 5

fig = plt.figure()
plt.subplot(211)
ax = fig.add_axes([0.1, 0.1, 0.65, 0.85])

plt.subplot(212)
ax1 = fig.add_axes([0.1, 0.1, 0.65, 0.85])

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
  #print test_y

  test_probs = regressionFunc.predict_proba(test_X)
  print " - score:%f" % (regressionFunc.score(test_X,test_y))
  print test_probs[:10]
  print test_y[:10]

#print test_y
  test_btargets = label_binarize(test_y, classes=[1, -1])
  n_classes = test_btargets.shape[1]
  print test_btargets[:10]

#print test_btargets
#  print n_classes
  precision = dict()
  recall = dict()
  threshold = dict()
#  for i in range(n_classes):
  i = 0
  precision[i], recall[i], threshold[i] = precision_recall_curve(test_btargets[:, i], test_probs[:, i])
  #precision[i], recall[i], threshold[i] = precision_recall_curve(test_btargets[:, i], test_probs[:, 1])
  num = len(precision[i][:-1])
plt.subplot(211)
plt.plot(recall[i], precision[i], label='PR-'+str(id1) + ":" + str(i))
plt.subplot(212)
plt.plot(threshold[i], recall[i][:-1], label='recall-'+str(id1) + ":" + str(i))
plt.plot(threshold[i], precision[i][:-1], label='precision-'+str(id1) + ":" + str(i))
#print regressionFunc.__dict__ 
for j in range(num):
  print "probs", precision[i][j], recall[i][j], threshold[i][j]

plt.subplot(211)
plt.xlabel('Recall')
plt.ylabel('Precision')
plt.ylim([0.0, 1.05])
plt.xlim([0.0, 1.0])
#plt.title('Precision-Recall')
plt.legend(loc="lower left", ncol=2)

plt.subplot(212)
plt.xlabel('threshold')
plt.ylabel('rate')
plt.ylim([0.0, 1.05])
plt.xlim([0.0, 1.0])
plt.legend(loc="lower left", ncol=2)

plt.savefig('myfig')

