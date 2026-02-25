import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np

from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
from sklearn.metrics import (
    silhouette_samples,
    silhouette_score,
    rand_score,              # Rand index
    adjusted_rand_score,     # Adjusted Rand index
)

# Generating the sample data from make_blobs
# This particular setting has one distinct cluster and 3 clusters placed close
# together.
X, y = make_blobs(
    n_samples=500,  # 生成样本数
    n_features=2,   # 样本维度
    centers=4,      # 类别数
    cluster_std=1,  # 类别方差为1
    center_box=(-10.0, 10.0),  # 簇中心坐标范围
    shuffle=True,   # 打乱样本顺序
    random_state=1,
)  # For reproducibility

plt.figure(figsize=(8, 6))
scatter = plt.scatter(
    X[:, 0],
    X[:, 1],
    c=y,                 # 用真实标签 y 上色
    cmap="nipy_spectral",
    s=30,
    edgecolor="k",
    alpha=0.7,
)
plt.title("Original data distribution with true labels")
plt.xlabel("Feature 1")
plt.ylabel("Feature 2")
plt.grid(alpha=0.3)
cbar = plt.colorbar(scatter)
cbar.set_label("True label")
plt.tight_layout()
plt.show()

# 2~10
range_n_clusters = range(2, 11)  # 遍历簇数 k = 2,3,...,10

# 用来存结果
silhouette_avgs = []          # 存每个 k 的平均 silhouette
rand_indices = []             # 存每个 k 的 Rand index
adjusted_rand_indices = []    # 存每个 k 的 Adjusted Rand index

for n_clusters in range_n_clusters:
    # Create a subplot with 1 row and 2 columns
    fig, (ax1, ax2) = plt.subplots(1, 2)  # 创建两个子图
    fig.set_size_inches(18, 7)  # 图大小

    # The 1st subplot is the silhouette plot
    # The silhouette coefficient can range from -1, 1 but in this example all
    # lie within [-0.1, 1]
    ax1.set_xlim([-0.1, 1])
    # The (n_clusters+1)*10 is for inserting blank space between silhouette
    # plots of individual clusters, to demarcate them clearly.
    ax1.set_ylim([0, len(X) + (n_clusters + 1) * 10])

    # Initialize the clusterer with n_clusters value and a random generator
    # seed of 10 for reproducibility.
    clusterer = KMeans(
        n_clusters=n_clusters,
        n_init="auto",
        random_state=10
    )  # 进行kmeans聚类

    # 在数据 X 上拟合 KMeans 聚类（寻找簇中心），并直接给出每个样本的簇标签。
    # 输出 cluster_labels 是一个 shape (500,) 的数组，每个元素是 0~(n_clusters-1)。
    cluster_labels = clusterer.fit_predict(X)

    # 计算平均 silhouette 系数
    silhouette_avg = silhouette_score(X, cluster_labels)
    # 保存
    silhouette_avgs.append(silhouette_avg)

    # 计算 Rand index 和 Adjusted Rand index（需要真实标签 y）
    ri = rand_score(y, cluster_labels)
    ari = adjusted_rand_score(y, cluster_labels)
    rand_indices.append(ri)
    adjusted_rand_indices.append(ari)

    print(
        f"For n_clusters = {n_clusters}, "
        f"silhouette_avg = {silhouette_avg:.4f}, "
        f"Rand index = {ri:.4f}, "
        f"Adjusted Rand index = {ari:.4f}"
    )

    # 计算每个样本的 silhouette 值
    sample_silhouette_values = silhouette_samples(X, cluster_labels)

    y_lower = 10
    # 按簇画 silhouette 条块
    for i in range(n_clusters):
        # Aggregate the silhouette scores for samples belonging to
        # cluster i, and sort them
        ith_cluster_silhouette_values = sample_silhouette_values[cluster_labels == i]

        ith_cluster_silhouette_values.sort()

        size_cluster_i = ith_cluster_silhouette_values.shape[0]
        y_upper = y_lower + size_cluster_i

        color = cm.nipy_spectral(float(i) / n_clusters)
        ax1.fill_betweenx(
            np.arange(y_lower, y_upper),
            0,
            ith_cluster_silhouette_values,
            facecolor=color,
            edgecolor=color,
            alpha=0.7,
        )

        # Label the silhouette plots with their cluster numbers at the middle
        ax1.text(-0.05, y_lower + 0.5 * size_cluster_i, str(i))

        # Compute the new y_lower for next plot
        y_lower = y_upper + 10  # 10 for the 0 samples

    ax1.set_title("The silhouette plot for the various clusters.")
    ax1.set_xlabel("The silhouette coefficient values")
    ax1.set_ylabel("Cluster label")

    # The vertical line for average silhouette score of all the values
    ax1.axvline(x=silhouette_avg, color="red", linestyle="--")

    ax1.set_yticks([])  # Clear the yaxis labels / ticks
    ax1.set_xticks([-0.1, 0, 0.2, 0.4, 0.6, 0.8, 1])

    # 2nd Plot showing the actual clusters formed 画散点图
    colors = cm.nipy_spectral(cluster_labels.astype(float) / n_clusters)
    ax2.scatter(
        X[:, 0], X[:, 1], marker=".", s=30, lw=0, alpha=0.7, c=colors, edgecolor="k"
    )

    # Labeling the clusters
    centers = clusterer.cluster_centers_
    # Draw white circles at cluster centers
    ax2.scatter(
        centers[:, 0],
        centers[:, 1],
        marker="o",
        c="white",
        alpha=1,
        s=200,
        edgecolor="k",
    )

    for i, c in enumerate(centers):
        ax2.scatter(c[0], c[1], marker="$%d$" % i, alpha=1, s=50, edgecolor="k")

    ax2.set_title("The visualization of the clustered data.")
    ax2.set_xlabel("Feature space for the 1st feature")
    ax2.set_ylabel("Feature space for the 2nd feature")

    plt.suptitle(
        "Silhouette analysis for KMeans clustering on sample data with n_clusters = %d"
        % n_clusters,
        fontsize=14,
        fontweight="bold",
    )

# 原有的 silhouette 图和散点图照常展示
plt.show()

# 最后汇总打印一遍，方便复制到实验报告里做表格
print("\n=== Summary for k = 2..10 ===")
print("k                    :", list(range_n_clusters))
print("silhouette_avgs      :", [round(v, 4) for v in silhouette_avgs])
print("Rand indices         :", [round(v, 4) for v in rand_indices])
print("Adjusted Rand indices:", [round(v, 4) for v in adjusted_rand_indices])
