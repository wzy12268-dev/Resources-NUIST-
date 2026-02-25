import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import ConfusionMatrixDisplay

# 混淆矩阵
cm = np.array([
    [976,   0,   1,   1,   0,   0,   1,   0,   1,   0],
    [  0,1128,   2,   3,   0,   1,   1,   0,   0,   0],
    [  2,   2,1021,   0,   1,   0,   0,   5,   1,   0],
    [  0,   0,   2,1002,   0,   3,   0,   2,   1,   0],
    [  0,   0,   1,   0, 975,   0,   1,   0,   2,   3],
    [  1,   0,   0,   3,   0, 886,   2,   0,   0,   0],
    [  3,   2,   0,   0,   1,   3, 948,   0,   1,   0],
    [  0,   2,   7,   1,   0,   0,   0,1015,   1,   2],
    [  3,   0,   3,   1,   0,   1,   0,   1, 962,   3],
    [  2,   0,   0,   0,   4,   4,   0,   3,   2, 994]
])

# 类别标签（MNIST: 0~9）
labels = list(range(10))

fig, ax = plt.subplots(figsize=(6, 6))
disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=labels)
disp.plot(ax=ax, cmap=plt.cm.Blues, colorbar=True)

ax.set_title("Confusion Matrix on MNIST Test Set")
plt.xlabel("Predicted label")
plt.ylabel("True label")
plt.tight_layout()
plt.show()
# 如果想保存图像：
plt.savefig("confusion_matrix_mnist.png", dpi=300)
