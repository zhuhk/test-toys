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
classList = [2, 1, 0]
test_btargets = label_binarize(test_targets, classes=classList)
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
#plt.clf()
fig = plt.figure()
ax = fig.add_axes([0.1, 0.2, 0.85, 0.75])
for i in range(n_classes):
  precision[i], recall[i], _ = precision_recall_curve(test_btargets[:, i], test_probs[:, i])
  #print precision[i]
  #print recall[i]
  #print  ""
  ax.plot(recall[i], precision[i], label='class-'+str(classList[i]))
 #plt.show()
#plt.subplot(121)
ax.set_xlabel('Recall')
ax.set_ylabel('Precision')
ax.set_ylim([0.0, 1.05])
ax.set_xlim([0.0, 1])
ax.set_title('Precision-Recall curve')

#x = ax.get_position()
#ax.set_position([box.x0, box.y0 + box.height * 0.1,
#                   box.width, box.height * 0.9])
ax.legend(bbox_to_anchor=(0.5, -0.1),ncol=3,loc="upper center", borderaxespad=0.)
plt.savefig('scikit_lr')
