import tensorflow as tf
import time
import pprint

x = tf.random_uniform([5, 10])

print(x)

z = tf.zeros([10])

print(z)
for x_i in tf.unstack(x):
      z += x_i

sess = tf.Session()
start = time.time()
z_val = sess.run(z)
#print("Took %f seconds." % (time.time() - start))
#print(",".join(map(str,z_val)))

pprint.pprint(tf.contrib.graph_editor.get_tensors(tf.get_default_graph()))
