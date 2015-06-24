from sklearn.feature_extraction import DictVectorizer, FeatureHasher
import sklearn.linear_model

hasher = FeatureHasher(non_negative=True)

features = [{"f01":0.5, "f02":0.7, "f03":0.8}, {"f01":0.6, "f02":0.7, "f04":0.7}, {"f02":0.9, "f05":0.7, "f06":1}]
X = hasher.transform(features)
y = [0,1,1]

#print X
for x in X:
  print x.shape
logisticRegression = sklearn.linear_model.LogisticRegression()
logisticRegression.fit(X, y)
