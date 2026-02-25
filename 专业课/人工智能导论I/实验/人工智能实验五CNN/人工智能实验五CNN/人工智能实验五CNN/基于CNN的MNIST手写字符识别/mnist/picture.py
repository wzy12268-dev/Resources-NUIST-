import matplotlib.pyplot as plt

# ------------------------------
# ① 把你打印出来的训练集 loss 填到这里（每个 epoch 一个值）
# ------------------------------
train_losses = [0.1693,	0.0670,	0.0521,	0.0456,	0.0405,	0.0401,	0.0383,
                0.0374,	0.0352,	0.0359,	0.0358, 0.0342,	0.0351, 0.0335
]

# ------------------------------
# ② 把测试集 loss 填到这里（每个 epoch 一个值）
# ------------------------------
test_losses = [0.0461,	0.0388,	0.0351,	0.0313,	0.0299,	0.0309,	0.0293,
0.0277,	0.0285,	0.0283,	0.0280, 0.0279,	0.0284, 0.0283

]

# epoch 编号
epochs = range(1, len(train_losses) + 1)

# ------------------------------
# ③ 绘图
# ------------------------------
plt.figure(figsize=(8, 5))
plt.plot(epochs, train_losses, marker='o', label="Train Loss")
plt.plot(epochs, test_losses, marker='s', label="Test Loss")

plt.xlabel("Epoch")
plt.ylabel("Loss")
plt.title("Train/Test Loss Curve on MNIST")
plt.grid(True)
plt.legend()

plt.savefig("loss_curve.png", dpi=300)
plt.show()
