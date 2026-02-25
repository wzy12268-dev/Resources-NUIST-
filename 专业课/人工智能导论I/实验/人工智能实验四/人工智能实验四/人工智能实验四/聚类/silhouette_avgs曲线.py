import matplotlib.pyplot as plt

k_values = [2, 3, 4, 5, 6, 7, 8, 9, 10]
silhouette_avgs = [0.705, 0.5882, 0.6505, 0.5615, 0.4858, 0.4973, 0.3807, 0.384, 0.3913]

plt.figure(figsize=(8, 4))
plt.plot(k_values, silhouette_avgs, marker='o', linestyle='-')
plt.xticks(k_values)  # x 轴刻度显示所有 k
plt.xlabel("Number of clusters k")
plt.ylabel("Average silhouette score")
plt.title("Silhouette score vs number of clusters k")
plt.grid(alpha=0.3)
plt.tight_layout()
plt.show()
