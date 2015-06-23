import sklearn.datasets
import sklearn.linear_model
import numpy.random
import numpy as np
from sklearn.preprocessing import label_binarize
from sklearn.metrics import precision_recall_curve

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

iris = sklearn.datasets.load_iris()

# Split the dataset with sampleRatio
sampleRatio = 0.5
n_samples = len(iris.target)
sampleBoundary = int(n_samples * sampleRatio)

# Shuffle the whole data
shuffleIdx = range(n_samples)
numpy.random.shuffle(shuffleIdx)


# Make the training data
train_features = iris.data[shuffleIdx[:sampleBoundary]]
train_targets = iris.target[shuffleIdx [:sampleBoundary]]

# Make the testing data
test_features = iris.data[shuffleIdx[sampleBoundary:]]
test_targets = iris.target[shuffleIdx[sampleBoundary:]]
test_btargets = label_binarize(test_targets, classes=[0,1,2])
n_classes = test_btargets.shape[1]

# Train
logisticRegression = sklearn.linear_model.LogisticRegression()
logisticRegression.fit(train_features, train_targets)
# Predict
test_predict_targets = logisticRegression.predict(test_features) 
test_probs = logisticRegression.predict_proba(test_features) 
print logisticRegression.score(test_features,test_targets)

precision = dict()
recall = dict()
for i in range(n_classes):
  precision[i], recall[i], _ = precision_recall_curve(test_btargets[:, i], test_probs[:, i])
  #print precision[i]
  #print recall[i]
  #print  ""
  plt.clf()
  plt.plot(recall[i], precision[i], label='Precision-Recall curve')
  plt.xlabel('Recall')
  plt.ylabel('Precision')
  plt.ylim([0.0, 1.05])
  plt.xlim([0.0, 1.0])
  plt.title('Precision-Recall example')
  plt.legend(loc="lower left")
  plt.savefig('myfig' + str(i))
 #plt.show()
