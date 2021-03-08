#!/bin/env python3

import tensorflow as tf
import numpy as np

def get_shape(tensor):
  static_shape = tensor.shape.as_list()
  dynamic_shape = tf.unstack(tf.shape(tensor))
  dims = [s[1] if s[0] is None else s[0]
          for s in zip(static_shape, dynamic_shape)]
  return dims

b = tf.placeholder(tf.float32, [None, 10, 32])
shape = get_shape(b)
print(shape)

b = tf.reshape(b, [shape[0], shape[1]*shape[2]])

print(b)

def reshape(tensor, dims_list):
  shape = get_shape(tensor)
  print(shape[0], shape[1], shape[2])
  dims_prod = []
  for dims in dims_list:
    if isinstance(dims, int):
      dims_prod.append(shape[dims])
    elif all([isinstance(shape[d], int) for d in dims]):
      dims_prod.append(np.prod([shape[d] for d in dims]))
    else:
      dims_prod.append(tf.prod([shape[d] for d in dims]))
  print(dims_prod)
  tensor = tf.reshape(tensor, dims_prod)
  return tensor

b = tf.placeholder(tf.float32, [None, 10, 32])
b = reshape(b, [0, [1,2]])
print(b)
