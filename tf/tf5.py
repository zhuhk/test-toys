#!/bin/env python3
#coding=utf-8

import tensorflow as tf
import numpy as np
import uuid

def t1():
  n = tf.constant(5)

  c = tf.TensorArray(tf.int32, n)
  c = c.write(0, 1)
  c = c.write(1, 1)

  def cond(i, a, b, c):
      return i < n

  def body(i, a, b, c):
      c = c.write(i, a+b)
      return i + 1, b, a + b, c

  i, a, b, c = tf.while_loop(cond, body, (2, 1, 1, c))

  print(c)
  c = c.stack()
  print(c)

  print(tf.Session().run(b))
  print(tf.Session().run(c))

def relu(inputs):
    # Define the op in python
    def _relu(x):
        return np.maximum(x, 0.)

    # Define the op's gradient in python
    def _relu_grad(x):
        return np.float32(x > 0)

    # An adapter that defines a gradient op compatible with TensorFlow
    def _relu_grad_op(op, grad):
        x = op.inputs[0]
        x_grad = grad * tf.py_func(_relu_grad, [x], tf.float32)
        return x_grad

    # Register the gradient with a unique id
    grad_name = "MyReluGrad_" + str(uuid.uuid4())
    tf.RegisterGradient(grad_name)(_relu_grad_op)

    # Override the gradient of the custom op
    g = tf.get_default_graph()
    with g.gradient_override_map({"PyFunc": grad_name}):
        output = tf.py_func(_relu, [inputs], tf.float32)
    return output

def t2():
  x = tf.random_normal([10])
  a = tf.reduce_mean(x)
  v = tf.sqrt(tf.reduce_mean(tf.square(x-a)))
  y = relu(x * x)

  with tf.Session() as sess:
      print("v:", sess.run([a,v]))
      diff = tf.test.compute_gradient_error(x, [10], y, [10])
      print("diff:", diff)

def t3():
  mnist = tf.keras.datasets.mnist

  (x_train, y_train),(x_test, y_test) = mnist.load_data()
  x_train, x_test = x_train / 255.0, x_test / 255.0

  print(x_train.shape, y_train.shape)
  print(x_test.shape)
  return 

  model = tf.keras.models.Sequential([
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(512, activation=tf.nn.relu),
    tf.keras.layers.Dropout(0.2),
    tf.keras.layers.Dense(10, activation=tf.nn.softmax)
  ])
  model.compile(optimizer='adam',
                loss='sparse_categorical_crossentropy',
                metrics=['accuracy'])

  model.fit(x_train, y_train, epochs=1)
  model.evaluate(x_test, y_test)

t3()
