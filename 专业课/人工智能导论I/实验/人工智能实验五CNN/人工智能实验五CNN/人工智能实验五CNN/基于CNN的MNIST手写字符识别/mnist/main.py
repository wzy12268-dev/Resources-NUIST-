from __future__ import print_function
import argparse
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms
from torch.optim.lr_scheduler import StepLR

import matplotlib.pyplot as plt
from sklearn.metrics import f1_score, classification_report, confusion_matrix
import numpy as np


# ----------------- 网络结构定义 -----------------
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        # 1 个输入通道，32 个输出通道，3x3 卷积核，stride=1
        self.conv1 = nn.Conv2d(1, 32, 3, 1)
        # 32 -> 64 通道
        self.conv2 = nn.Conv2d(32, 64, 3, 1)

        self.dropout1 = nn.Dropout(0.25)
        self.dropout2 = nn.Dropout(0.5)

        # 经过两次 conv + 一次 2x2 pool 后，特征维度为 9216
        self.fc1 = nn.Linear(9216, 128)
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        x = self.conv1(x)
        x = F.relu(x)

        x = self.conv2(x)
        x = F.relu(x)

        x = F.max_pool2d(x, 2)
        x = self.dropout1(x)

        x = torch.flatten(x, 1)  # 展平

        x = self.fc1(x)
        x = F.relu(x)
        x = self.dropout2(x)

        x = self.fc2(x)
        output = F.log_softmax(x, dim=1)
        return output


# ----------------- 训练 -----------------
def train(args, model, device, train_loader, optimizer, epoch):
    model.train()
    train_loss = 0.0  # 累计本 epoch 的总损失（sum）

    for batch_idx, (data, target) in enumerate(train_loader):
        data, target = data.to(device), target.to(device)

        optimizer.zero_grad()
        output = model(data)

        # reduction='sum' 方便后面算平均
        loss = F.nll_loss(output, target, reduction='sum')
        loss.backward()
        optimizer.step()

        train_loss += loss.item()

        if batch_idx % args.log_interval == 0:
            avg_loss_batch = loss.item() / len(data)
            print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx * len(data), len(train_loader.dataset),
                100. * batch_idx / len(train_loader), avg_loss_batch))

            if args.dry_run:
                break

    # 本 epoch 平均训练 loss
    train_loss /= len(train_loader.dataset)
    print('\nTrain set: Average loss: {:.4f}\n'.format(train_loss))
    return train_loss


# ----------------- 测试 / 评估（含每类指标） -----------------
def evaluate(model, device, test_loader):
    model.eval()
    test_loss = 0.0
    correct = 0

    all_targets = []
    all_preds = []

    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)

            test_loss += F.nll_loss(output, target, reduction='sum').item()

            pred = output.argmax(dim=1)  # [batch]
            correct += pred.eq(target).sum().item()

            all_targets.extend(target.cpu().tolist())
            all_preds.extend(pred.cpu().tolist())

    test_loss /= len(test_loader.dataset)
    accuracy = correct / len(test_loader.dataset)
    macro_f1 = f1_score(all_targets, all_preds, average='macro')

    # ====== 每类指标 ======
    all_targets_np = np.array(all_targets)
    all_preds_np = np.array(all_preds)

    num_classes = 10  # MNIST 0~9 共 10 类

    # 每类 F1
    per_class_f1 = f1_score(
        all_targets,
        all_preds,
        average=None,
        labels=list(range(num_classes))
    )

    # 每类 Accuracy（在所有真实标签为 c 的样本中，预测对的比例）
    per_class_acc = []
    for c in range(num_classes):
        idx = (all_targets_np == c)
        if idx.sum() == 0:
            acc_c = 0.0
        else:
            acc_c = (all_preds_np[idx] == c).mean()
        per_class_acc.append(acc_c)

    # sklearn 自带的分类报告
    cls_report = classification_report(
        all_targets,
        all_preds,
        labels=list(range(num_classes)),
        digits=4
    )

    print('Test set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%), Macro-F1: {:.4f}\n'
          .format(test_loss, correct, len(test_loader.dataset),
                  100. * accuracy, macro_f1))

    print('Per-class metrics:')
    for c in range(num_classes):
        print(f'  Class {c}: acc = {per_class_acc[c]:.4f}, '
              f'F1 = {per_class_f1[c]:.4f}')
    print('\nClassification report:\n', cls_report)

    # 混淆矩阵（行：真实类别，列：预测类别）
    cm = confusion_matrix(all_targets, all_preds, labels=list(range(num_classes)))
    print('Confusion matrix:\n', cm, '\n')

    # 返回整体指标 + 每类指标
    return test_loss, accuracy, macro_f1, per_class_acc, per_class_f1


# ----------------- 主函数 -----------------
def main():
    # 参数设置
    parser = argparse.ArgumentParser(description='PyTorch MNIST Example')
    parser.add_argument('--batch-size', type=int, default=64, metavar='N',
                        help='input batch size for training (default: 64)')
    parser.add_argument('--test-batch-size', type=int, default=1000, metavar='N',
                        help='input batch size for testing (default: 1000)')
    parser.add_argument('--epochs', type=int, default=14, metavar='N',
                        help='number of epochs to train (default: 14)')
    parser.add_argument('--lr', type=float, default=1.0, metavar='LR',
                        help='learning rate (default: 1.0)')
    parser.add_argument('--gamma', type=float, default=0.7, metavar='M',
                        help='Learning rate step gamma (default: 0.7)')
    parser.add_argument('--no-cuda', action='store_true', default=False,
                        help='disables CUDA training')
    parser.add_argument('--no-mps', action='store_true', default=False,
                        help='disables macOS GPU training')
    parser.add_argument('--dry-run', action='store_true', default=False,
                        help='quickly check a single pass')
    parser.add_argument('--seed', type=int, default=1, metavar='S',
                        help='random seed (default: 1)')
    parser.add_argument('--log-interval', type=int, default=10, metavar='N',
                        help='how many batches to wait before logging training status')
    parser.add_argument('--save-model', action='store_true', default=False,
                        help='For Saving the current Model')

    args = parser.parse_args()

    # 设备选择
    use_cuda = not args.no_cuda and torch.cuda.is_available()
    use_mps = not args.no_mps and torch.backends.mps.is_available()

    torch.manual_seed(args.seed)

    if use_cuda:
        device = torch.device("cuda")
    elif use_mps:
        device = torch.device("mps")
    else:
        device = torch.device("cpu")

    # DataLoader 参数
    train_kwargs = {'batch_size': args.batch_size}
    test_kwargs = {'batch_size': args.test_batch_size}
    if use_cuda:
        cuda_kwargs = {'num_workers': 1,
                       'pin_memory': True,
                       'shuffle': True}
        train_kwargs.update(cuda_kwargs)
        test_kwargs.update(cuda_kwargs)

    # 数据预处理与加载 MNIST
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    dataset1 = datasets.MNIST('../data', train=True, download=True,
                              transform=transform)
    dataset2 = datasets.MNIST('../data', train=False,
                              transform=transform)

    train_loader = torch.utils.data.DataLoader(dataset1, **train_kwargs)
    test_loader = torch.utils.data.DataLoader(dataset2, **test_kwargs)

    # 模型 & 优化器 & 学习率调度器
    model = Net().to(device)
    optimizer = optim.Adadelta(model.parameters(), lr=args.lr)
    scheduler = StepLR(optimizer, step_size=1, gamma=args.gamma)

    # 记录每个 epoch 的整体指标
    train_losses = []
    test_losses = []
    test_accuracies = []
    test_macro_f1s = []

    # 如果你想记录每个 epoch 的“每类指标”，也可以开两个大列表：
    # all_epoch_per_class_acc = []
    # all_epoch_per_class_f1 = []

    # 训练 + 测试循环
    for epoch in range(1, args.epochs + 1):
        print('================ Epoch {} ================'.format(epoch))
        train_loss = train(args, model, device, train_loader, optimizer, epoch)
        test_loss, test_acc, test_macro_f1, per_class_acc, per_class_f1 = evaluate(model, device, test_loader)
        scheduler.step()

        train_losses.append(train_loss)
        test_losses.append(test_loss)
        test_accuracies.append(test_acc)
        test_macro_f1s.append(test_macro_f1)

        # 如果要按 epoch 保存每类指标，可以解除注释：
        # all_epoch_per_class_acc.append(per_class_acc)
        # all_epoch_per_class_f1.append(per_class_f1)

    # （可选）保存模型
    if args.save_model:
        torch.save(model.state_dict(), "mnist_cnn.pt")

    # ---------- 画图 ----------
    epochs = range(1, args.epochs + 1)

    # 1) 训练 / 测试 loss 曲线
    plt.figure()
    plt.plot(epochs, train_losses, label='Train Loss')
    plt.plot(epochs, test_losses, label='Test Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.title('Train & Test Loss on MNIST')
    plt.legend()
    plt.grid(True)
    plt.savefig('loss_curve.png', dpi=300)

    # 2) 测试 Accuracy & Macro-F1 曲线
    plt.figure()
    plt.plot(epochs, test_accuracies, label='Test Accuracy')
    plt.plot(epochs, test_macro_f1s, label='Test Macro-F1')
    plt.xlabel('Epoch')
    plt.ylabel('Metric')
    plt.title('Test Accuracy & Macro-F1 on MNIST')
    plt.legend()
    plt.grid(True)
    plt.savefig('metric_curve.png', dpi=300)

    # 如果你想直接弹出图像再看，可以取消下面注释
    # plt.show()


if __name__ == '__main__':
    main()
