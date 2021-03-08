#!/bin/env python3
#coding=utf-8

import tensorflow as tf
import numpy as np
import uuid
import random

def fake_input():
  with open("input.csv", "w") as fp:
    col_num = 5
    fcnt = 0
    for line in range(100):
      fp.write("%d" % (1 if random.random()<0.1 else 0))
      for col in range(col_num):
        fcnt += 1
        values = []
        for j in range(fcnt%(col_num+1)+1):
          values.append("%.2f" % random.random())
        cval = ":".join(values)
        fp.write(",%s" % cval)
      fp.write("\n");


#fake_input()

def parse_csv(value):
  str_columns = tf.decode_csv(value, [['']] * 6)
  print("after decode_csv:%s" % str_columns)
  labels = tf.reshape(tf.cast(tf.strings.to_number(str_columns[0]), tf.int64), shape=[-1, 1])
  print("labels:%s" % labels)
  features = {}
  for i in range(5):
    sparse_col = tf.strings.split(str_columns[i+1], sep=":")
    dense_shape = sparse_col.get_shape()
    print("sparse_col:%s" % (sparse_col))
#    ragged = tf.RaggedTensor.from_sparse(sparse_col)
#    print("ragged:%s" % ragged)
#    dense_col = ragged.to_tensor(default_value="0.0")
    dense_col = tf.sparse.to_dense(sparse_col, default_value="0.0")
    print("dense_col:%s" % dense_col)
#    dense_col = tf.strings.to_number(dense_col, tf.float32)
#    print("to_numer(dense_col):%s" % dense_col)
    features[str(i)] = dense_col
    print("")
  return features, labels 


def input_fn():
  dataset = tf.data.TextLineDataset("./input.csv")
  dataset = dataset.batch(3).map(parse_csv)

  iterator = dataset.make_one_shot_iterator()
  features, labels = iterator.get_next()
  return features, labels

features, labels = input_fn()
import pprint
with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  
  for i in range(5):
    data_features, data_labels = sess.run([features, labels])
    for k,v in zip(data_labels,data_features.values()):
      print("%s\n%s\n" % (k,v))

