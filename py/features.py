#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import sys
import time
import signal
import redis
import math 
import constant as c
from func import HammingDistance,extract_images
from sklearn.feature_extraction import FeatureHasher,DictVectorizer
from sklearn.datasets import dump_svmlight_file

reload(sys)
sys.setdefaultencoding('utf8')

sep1=(chr)(1)
sep2=(chr)(2)
sep3=(chr)(3)

global stop_flag 
stop_flag = 0

def sig_handler(signum,frame):
  global stop_flag
  stop_flag = 1

signal.signal(signal.SIGUSR1, sig_handler);

def Sleep(seconds):
  global stop_flag
  print "sleep " + str(seconds) + "s"
  i = 0
  while i < seconds and stop_flag != 1:
    time.sleep(1)
    i += 1

#r = redis.StrictRedis(host=c.REDIS_HOST, port=c.REDIS_PORT, password=c.REDIS_PASSWD)

now = int(time.time())

xList = []
yList = []

line_no = -1
for line in sys.stdin:
  if stop_flag != 0:
    break

  cols = line.strip().split("\t")
  if len(cols) < 2:
    continue
  docId = str(cols[0])
  try:
    items = json.loads(cols[-1])
  except:
    items = {}
  
  line_no = line_no + 1

#  if items["state"] != 0:
#    print line

#  print "%s\t%s\t%s" % (docId, items["authorName"], ":".join(items["categories"]).encode("utf-8"))
#  continue
#  sims = []
#  for item in items:
#    if "similarity" not in item:
#      continue
#    sims.append(item['similarity'])
#  print "%s\t%s" % (docId, " ".join(items['pageServer'].keys()))
  if items["state"] > 0:
    state = 1
  elif items["state"] < 0:
    state = -1
  else:
    state = 0

  features = {}

  f_contLen = math.log(len(items["pageServer"]["content_seg"])+1.0)
  features[str(1001+int(f_contLen)).encode("utf-8")] = 1.0 
  images = extract_images(items["url"], items["htmlContent"])
  f_imgLen = math.log(len(images)+1.0)
  features[str(1100+int(f_imgLen)).encode("utf-8")] = 1.0 
  for f in items['pageServer']['topic_vector'].split(","):
    arr = f.split(":")
    if len(arr) != 2:
      continue
    k,v = arr
    features[k.encode("utf-8")] = float(v)
  for f in items['pageServer']['category'].split(","):
    arr = f.split(":")
    if len(arr) != 2:
      continue
    k,v = arr
    features[k[:5].encode("utf-8")] = 1.0
    break
  if len(items['authorName'])>0:
    features[items['authorName']] = 1.0
  xList.append(features)
  yList.append(state)
 # features = sorted(features.iteritems())
 # print "%s" % (state),
 # for k,v in features:
 #   print " %d:%.2f" % (k,v),
 # print ""

#hasher = FeatureHasher(non_negative=True)
hasher = DictVectorizer(sparse = True)
#print xList
X = hasher.fit_transform(xList)
dump_svmlight_file(X,yList,"dump.svm")

#  print "%s\t%d\t%d\t%s\t%s\t%s" % (docId, len(items["content"]), len(images),items["state"], items["title"], items["url"])
#  print "%s\t%d\t%d\t%s\t%s" % (docId, len(items["content"]), len(images),items["state"], items["pageServer"].topic_vector)


