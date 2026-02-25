## 环境设定
import numpy as np
import matplotlib.pyplot as plt
from deap import base, tools, creator, algorithms
import random

params = {
    'font.family': 'serif',
    'figure.dpi': 300,
    'savefig.dpi': 300,
    'font.size': 12,
    'legend.fontsize': 'small'
}
plt.rcParams.update(params)

import copy
# --------------------
## 问题定义
creator.create('FitnessMax', base.Fitness, weights=(1.0,))  # 最大化问题
creator.create('Individual', list, fitness=creator.FitnessMax)

## 个体编码
toolbox = base.Toolbox()
geneLength = 11
toolbox.register('perm', np.random.permutation, geneLength)
toolbox.register('individual', tools.initIterate,
                 creator.Individual, toolbox.perm)

## 评价函数
# 类似链表，用字典存储每个节点的可行路径，用于解码
nodeDict = {'1': [2, 3, 4], '2': [3, 5, 6], '3': [4, 6, 7], '4': [7], '5': [8], '6': [5, 8, 9, 10],
            '7': [6, 10], '8': [9, 11], '9': [10, 11], '10': [11]}

def genPath(ind, nodeDict=nodeDict):
    '''输入一个优先度序列之后，返回一条从节点1到节点25的可行路径 '''
    path = [1]
    endNode = len(ind)
    while not path[-1] == endNode:
        curNode = path[-1]  # 当前所在节点
        if nodeDict[str(curNode)]:  # 当前节点指向的下一个节点不为空时，到达下一个节点
            toBeSelected = nodeDict[str(curNode)]  # 获取可以到达的下一个节点列表
        else:
            return path
        priority = np.asarray(ind)[np.asarray(
            toBeSelected)-1]  # 获取优先级,注意列表的index是0-9
        nextNode = toBeSelected[np.argmax(priority)]
        path.append(nextNode)
    return path


# 存储每条边的剩余容量，用于计算路径流量和更新节点链表
capacityDict = {'1,2': 60, '1,3': 60, '1,4': 60, '2,3': 30, '2,5': 40, '2,6': 30,
               '3,4': 30, '3,6': 50, '3,7': 30, '4,7': 40, '5,8': 60, '6,5': 20,
               '6,8': 30, '6,9': 40, '6,10': 30, '7,6': 20, '7,10': 40, '8,9': 30,
               '8,11': 60, '9,10': 30, '9,11': 50, '10,11': 50}

def traceCapacity(path, capacityDict):
    ''' 获取给定path的最大流量，更新各边容量 '''
    pathEdge = list(zip(path[::1], path[1::1]))
    keys = []
    edgeCapacity = []
    for edge in pathEdge:
        key = str(edge[0]) + ',' + str(edge[1])
        keys.append(key)  # 保存edge对应的key
        edgeCapacity.append(capacityDict[key])  # 该边对应的剩余容量
    pathFlow = min(edgeCapacity)  # 路径上的最大流量
    # 更新各边的剩余容量
    for key in keys:
        capacityDict[key] -= pathFlow  # 注意这里是原位修改
    return pathFlow


def updateNodeDict(capacityDict, nodeDict):
    ''' 对剩余流量为0的节点，删除节点指向；对于链表指向为空的节点，由于没有下一步可以移动的方向，
    从其他所有节点的指向中删除该节点
    '''
    for edge, capacity in capacityDict.items():
        if capacity == 0:
            key, toBeDel = str(edge).split(',')  # 用来索引节点字典的key，和需要删除的节点toBeDel
            if int(toBeDel) in nodeDict[key]:
                nodeDict[key].remove(int(toBeDel))
    delList = []
    for node, nextNode in nodeDict.items():
        if not nextNode:  # 如果链表指向为空的节点，从其他所有节点的指向中删除该节点
            delList.append(node)
    for delNode in delList:
        for node, nextNode in nodeDict.items():
            if delNode in nextNode:
                nodeDict[node].remove(delNode)


def evaluate(ind, outputPaths=False):
    '''评价函数'''
    # 初始化所需变量
    nodeDictCopy = copy.deepcopy(nodeDict)  # 浅复制
    capacityDictCopy = copy.deepcopy(capacityDict)
    paths = []
    pathFlows = []
    fOverall = 0
    # 开始循环
    while nodeDictCopy['1']:
        path = genPath(ind, nodeDictCopy)  # 生成路径
        # 当路径无法抵达终点，说明经过这个节点已经无法往下走，从所有其他节点的指向中删除该节点
        if path[-1] != 11:
             for node, nextNode in nodeDictCopy.items():
                 if path[-1] in nextNode:
                     nodeDictCopy[node].remove(path[-1])
             continue
        paths.append(path) # 保存路径
        pathFlow = traceCapacity(path, capacityDictCopy) # 计算路径最大流量
        pathFlows.append(pathFlow) # 保存路径的流量
        fOverall += pathFlow # 更新最大流量
        updateNodeDict(capacityDictCopy, nodeDictCopy) # 更新节点链表
    if outputPaths:
        return fOverall, paths, pathFlows
    return fOverall,
toolbox.register('evaluate', evaluate)

# 迭代数据
stats = tools.Statistics(key=lambda ind:ind.fitness.values)
stats.register('max', np.max)
stats.register('avg', np.mean)
stats.register('std', np.std)

# 生成初始族群
toolbox.popSize = 100
toolbox.register('population', tools.initRepeat, list, toolbox.individual)
pop = toolbox.population(toolbox.popSize)

# 注册工具
toolbox.register('select', tools.selTournament, tournsize=2)
toolbox.register('mate', tools.cxOrdered)
toolbox.register('mutate', tools.mutShuffleIndexes, indpb=0.5)

# --------------------
# 遗传算法参数
toolbox.ngen = 200
toolbox.cxpb = 0.8
toolbox.mutpb = 0.05

# 遗传算法主程序部分
pop, logbook= algorithms.eaSimple(pop, toolbox, cxpb=toolbox.cxpb, mutpb=toolbox.mutpb,
                   ngen = toolbox.ngen, stats=stats, verbose=True)