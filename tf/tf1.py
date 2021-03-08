#!/bin/env python

import numpy as np
import tensorflow as tf

# Placeholders are used to feed values from python to TensorFlow ops. We define
# two placeholders, one for input feature x, and one for output y.
x = tf.placeholder(tf.float32)
y = tf.placeholder(tf.float32)

# Assuming we know that the desired function is a polynomial of 2nd degree, we
# allocate a vector of size 3 to hold the coefficients. The variable will be
# automatically initialized with random noise.
w = tf.get_variable("w", shape=[3, 1])

# We define yhat to be our estimate of y.
f = tf.stack([tf.square(x), x, tf.ones_like(x)], 1)
yhat = tf.squeeze(tf.matmul(f, w), 1)

# The loss is defined to be the l2 distance between our estimate of y and its
# true value. We also added a shrinkage term, to ensure the resulting weights
# would be small.
loss = tf.nn.l2_loss(yhat - y) + 0.1 * tf.nn.l2_loss(w)

# We use the Adam optimizer with learning rate set to 0.1 to minimize the loss.
train_op = tf.train.AdamOptimizer(0.1).minimize(loss)

def generate_data():
    x_val = np.random.uniform(-10.0, 10.0, size=100)
    y_val = 5 * np.square(x_val) + 3
    return x_val, y_val

sess = tf.Session()
# Since we are using variables we first need to initialize them.
sess.run(tf.global_variables_initializer())
for i in range(1000):
    x_val, y_val = generate_data()
    _, loss_val, w_val = sess.run([train_op, loss, w], {x: x_val, y: y_val})
    if i % 10 == 9:
      #print("input:", ",".join(str(x) for x in np.squeeze(y_val)), "\t", ",".join(str(x) for x in np.squeeze(x_val)))
      print("\t")
      print("metric:", i, loss_val, ",".join(str(x) for x in np.squeeze(w_val)))

print(loss_val, ",".join(str(x) for x in w_val))
#print(sess.run([w])) 
