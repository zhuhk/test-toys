import sklearn.datasets
import sklearn.linear_model
import numpy.random

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

# Train
logisticRegression = sklearn.linear_model.LogisticRegression()
logisticRegression.fit(train_features, train_targets)
# Predict
predict_targets = logisticRegression.predict(test_features) 
predict_probs = logisticRegression.predict_proba(test_features) 

