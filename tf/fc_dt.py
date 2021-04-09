#!/bin/env python3
#coding=utf-8

import tensorflow as tf
import numpy as np
import uuid
import random

vab = ['a','b','c','d','e']
col_num = 5

def fake_input():
  with open("fc_dt.csv", "w") as fp:

    fcnt = 0
    for line in range(100):
      cols = [str(random.randint(0,1))]
      for col in range(col_num):
        fcnt += 1
        values = []
        for j in range(5):
          values.append(vab[random.randint(0,len(vab)-1)])
        cols.append(":".join(values))
      fp.write(",".join(cols))
      fp.write("\n")

#fake_input()

def parse_csv(value):
  str_columns = tf.decode_csv(value, [['']] * (col_num+1))
  #print("after decode_csv:%s" % str_columns)
  labels = tf.reshape(tf.cast(tf.strings.to_number(str_columns[0]), tf.int64), shape=[-1, 1])
  #print("labels:%s" % labels)
  features = {}
  for i in range(col_num):
    sparse_col = tf.strings.split(str_columns[i+1], sep=":")
    dense_shape = sparse_col.get_shape()
    #print("sparse_col:%s" % (sparse_col))
#    ragged = tf.RaggedTensor.from_sparse(sparse_col)
#    print("ragged:%s" % ragged)
#    dense_col = ragged.to_tensor(default_value="0.0")
    dense_col = tf.sparse.to_dense(sparse_col, default_value="NN")
    #print("dense_col:%s" % dense_col)
#    dense_col = tf.strings.to_number(dense_col, tf.float32)
#    print("to_numer(dense_col):%s" % dense_col)
    features["cols-" + str(i)] = dense_col
  return features, labels 


def input_fn():
  dataset = tf.data.TextLineDataset("./fc_dt.csv")
  dataset = dataset.batch(3).map(parse_csv).repeat()

  iterator = dataset.make_one_shot_iterator()
  features, labels = iterator.get_next()
  return features, labels

features, labels = input_fn()

k1 = tf.feature_column.categorical_column_with_vocabulary_list(
     'cols-1', vab, dtype=tf.string)

e1 = tf.feature_column.embedding_column(k1, 32)
k2 = tf.feature_column.categorical_column_with_vocabulary_list(
     'cols-2', vab, dtype=tf.string)
e2 = tf.feature_column.embedding_column(k2, 32)

print("k1:%s" % str(k1))

columns = [e1, e2]
inputs = tf.feature_column.input_layer(features, columns)

with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  sess.run(tf.tables_initializer())
  
  v = sess.run(inputs)
  print(v)
  exit()
  for i in range(1):
    data_features, data_labels = sess.run([features, labels])
    for i in range(len(data_labels)):
      print("label:%s" % data_labels[i])
      print("%s" % data_features)


