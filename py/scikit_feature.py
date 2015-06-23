from sklearn.feature_extraction import DictVectorizer, FeatureHasher
import sklearn.linear_model

hasher = FeatureHasher()

features = [{"f01":0.5, "f02":0.7,"f03":0.8},{"f01":0.6,"f04":0.7},{"f02":0.9,"f05":0.7}]
X = hasher.transform(features)
y = [0,1,1]

logisticRegression = sklearn.linear_model.LogisticRegression()
logisticRegression.fit(X, y)
