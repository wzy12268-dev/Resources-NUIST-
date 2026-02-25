"""
===================================================
Faces recognition example using eigenfaces and SVMs
===================================================

The dataset used in this example is a preprocessed excerpt of the
"Labeled Faces in the Wild", aka LFW_:

  http://vis-www.cs.umass.edu/lfw/lfw-funneled.tgz (233MB)

.. _LFW: http://vis-www.cs.umass.edu/lfw/

"""
# %%
from time import time
import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split
from sklearn.model_selection import RandomizedSearchCV
from sklearn.datasets import fetch_lfw_people
from sklearn.metrics import classification_report
from sklearn.metrics import ConfusionMatrixDisplay
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.svm import SVC
# from sklearn.utils.fixes import loguniform
from scipy.stats import loguniform


# %%
# Download the data, if not already on disk and load it as numpy arrays

lfw_people = fetch_lfw_people(min_faces_per_person=70, resize=0.4,data_home=r"D:\Pycharm\人工智能实验\kmeans聚类")
# 只保留至少出现 min_faces_per_person 张照片的那几个人，频次太少的人被过滤掉
# resize:把原始人脸图片缩放到 40% 大小，减少分辨率，减小特征维度，加快后续计算
# lfw_people 是一个类似 Bunch 的对象，里面包含：
# images：原始图像数组
# data：把图像展平（flatten）后的二维特征矩阵
# target：标签（每张人脸对应的人名索引）
# target_names：每个索引对应的人名字符串数组
# 等其他一些属性

# introspect the images arrays to find the shapes (for plotting)
n_samples, h, w = lfw_people.images.shape
# n_sumples:样本总数
# h:图像高度
# w:图像宽度

# for machine learning we use the 2 data directly (as relative pixel
# positions info is ignored by this model)
X = lfw_people.data  # (n_samples, n_features)
n_features = X.shape[1]

# the label to predict is the id of the person
y = lfw_people.target  # (n_samples)
target_names = lfw_people.target_names
n_classes = target_names.shape[0]

print("Total dataset size:")
print("n_samples: %d" % n_samples)
print("n_features: %d" % n_features)
print("n_classes: %d" % n_classes)


# %%
# Split into a training set and a test and keep 25% of the data for testing.
# 保留原本25%作为测试集，随机种子设置42
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.25, random_state=42
)
# 创建一个标准化器对象，让每一列特征均值为 0、方差为 1。
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)  # 训练集标准化之后结果，计算每一列特征的均值和标准差
X_test = scaler.transform(X_test)  # 测试集标准化之后结果

# %%
# 在整个脸部数据集上做 PCA 主成分分析；把 PCA 得到的主成分叫作 eigenfaces（特征脸）；
# 这是一个无监督的特征提取 / 降维步骤（PCA 不用标签，只看数据本身）。

# 保留主成分个数为150个，即从1850压缩到150维度
n_components = 150

print(
    "Extracting the top %d eigenfaces from %d faces" % (n_components, X_train.shape[0])
)
t0 = time()
# 创建一个PCA对象，使用随机化 SVD 算法来做 PCA，在降维之后再做一次标准化，得到的特征更适合后面用线性模型 / SVM
pca = PCA(n_components=n_components, svd_solver="randomized", whiten=True).fit(X_train)
print("done in %0.3fs" % (time() - t0))

eigenfaces = pca.components_.reshape((n_components, h, w))

print("Projecting the input data on the eigenfaces orthonormal basis")
t0 = time()
X_train_pca = pca.transform(X_train)
X_test_pca = pca.transform(X_test)
print("done in %0.3fs" % (time() - t0))


# %%
# Train a SVM classification model

print("Fitting the classifier to the training set")
t0 = time()
param_grid = {
    "C": loguniform(1e3, 1e5),
    "gamma": loguniform(1e-4, 1e-1),
}
clf = RandomizedSearchCV(
    SVC(kernel="rbf", class_weight="balanced"), param_grid, n_iter=10
)
clf = clf.fit(X_train_pca, y_train)
print("done in %0.3fs" % (time() - t0))
print("Best estimator found by grid search:")
print(clf.best_estimator_)


# %%
# Quantitative evaluation of the model quality on the test set

print("Predicting people's names on the test set")
t0 = time()
y_pred = clf.predict(X_test_pca)
print("done in %0.3fs" % (time() - t0))

print(classification_report(y_test, y_pred, target_names=target_names))
ConfusionMatrixDisplay.from_estimator(
    clf, X_test_pca, y_test, display_labels=target_names, xticks_rotation="vertical"
)
plt.tight_layout()
plt.savefig("confusion_matrix.png", dpi=300)
plt.show()


# %%
# Qualitative evaluation of the predictions using matplotlib


def plot_gallery(images, titles, h, w, n_row=5, n_col=6):
    """Helper function to plot a gallery of portraits"""
    plt.figure(figsize=(1.8 * n_col, 2.4 * n_row))
    plt.subplots_adjust(bottom=0.01, left=0.01, right=0.99, top=0.90, hspace=0.35)
    for i in range(n_row * n_col):
        plt.subplot(n_row, n_col, i + 1)
        plt.imshow(images[i].reshape((h, w)), cmap=plt.cm.gray)
        plt.title(titles[i], size=12)
        plt.xticks(())
        plt.yticks(())


# %%
# plot the result of the prediction on a portion of the test set


def title(y_pred, y_test, target_names, i):
    pred_name = target_names[y_pred[i]].rsplit(" ", 1)[-1]
    true_name = target_names[y_test[i]].rsplit(" ", 1)[-1]
    return "predicted: %s\ntrue:      %s" % (pred_name, true_name)


prediction_titles = [
    title(y_pred, y_test, target_names, i) for i in range(y_pred.shape[0])
]

plot_gallery(X_test, prediction_titles, h, w)
# %%
# plot the gallery of the most significative eigenfaces

eigenface_titles = ["eigenface %d" % i for i in range(eigenfaces.shape[0])]
plot_gallery(eigenfaces, eigenface_titles, h, w)
plt.savefig("eigenfaces_gallery.png", dpi=300)
plt.show()

# %%
# Face recognition problem would be much more effectively solved by training
# convolutional neural networks but this family of models is outside of the scope of
# the scikit-learn library. Interested readers should instead try to use pytorch or
# tensorflow to implement such models.
