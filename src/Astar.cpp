#include "Astar.h"  // 包含Astar类的头文件
#include "PathSet.h"  // 包含路径集合类的头文件
#include "ODMatrix.h"  // 包含OD矩阵类的头文件
#include "StarNetwork.h"  // 包含星形网络类的头文件
#include "Origin.h"  // 包含起点类的头文件
#include "ODSet.h"  // 包含OD集合类的头文件
#include "StarNode.h"  // 包含星形节点类的头文件

#include <set>  // 包含集合容器
#include <algorithm>  // 包含算法库
#include <sstream>  // 包含字符串流
#include <limits>  // 包含数值限制

// 初始化节点
void Astar::initNodes(){
    static const FPType FPType_Max = std::numeric_limits<FPType>::max();  // 定义FPType类型的最大值
    std::fill(LabelVector->begin(), LabelVector->end(), FPType_Max);  // 初始化标签向量为最大值
    std::fill(Predecessors->begin(), Predecessors->end(),  -1);  // 初始化前驱节点向量为-1
}

// 获取目标节点的成本
FPType Astar::getCost(int destIndex) const{
    assert((destIndex >= 0) && (destIndex < nbNodes_));  // 确保目标索引有效
    return (*LabelVector)[destIndex];  // 返回目标节点的成本
}

// 获取目标节点的入边
StarLink* Astar::getInComeLink(int destIndex) const{
    assert((destIndex >= 0) && (destIndex < nbNodes_));  // 确保目标索引有效
    int linkIndex = (*Predecessors)[destIndex];  // 获取前驱链接索引
    return  linkIndex == -1 ?  NULL :  netPointer_->getLink(linkIndex);  // 返回前驱链接或NULL
}

// 构造函数
Astar::Astar(StarNetwork* netPointer, ODMatrix* odMatrix) : ShortestPath(), 
            bestKnownPathCost_(std::numeric_limits<FPType>::infinity()),  // 初始化最佳路径成本为无穷大
            pathSet_(NULL) {  // 初始化路径集合为空

    netPointer_ = netPointer;  // 设置网络指针
    nbNodes_ = netPointer_->getNbNodes();  // 获取节点数量

    Queue = new PriorityQueue();  // 创建优先队列
    LabelVector = new std::vector<FPType>(nbNodes_);  // 创建标签向量
    Predecessors = new std::vector<int>(nbNodes_);  // 创建前驱节点向量

    nbDests_ = 0;  // 初始化目标节点数量为0

    netPointer_->calculateLinkCosts();  // 计算链接成本

    // 我们需要计算从每个节点到目的地的零流量旅行时间
    // 由于起点总数等于目的地总数，我们使用以下代码
}

// 初始化边界
void Astar::initializeBounds(ODMatrix* odMatrix) {
    // 所有目的地
    std::set<int> destinations;  // 创建目的地集合

    for (OriginIterator it = odMatrix->begin(); it != odMatrix->end(); ++it){  // 遍历所有起点
        Origin* origin = *it;  // 获取当前起点
        for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {  // 遍历所有OD对
            PairOD* od = *jt;  // 获取当前OD对
            destinations.insert(od->getIndex());  // 将目的地索引插入集合
        }
    }

    // 集合的最后一个元素是最大的，这是我们必须要分配的最大值
    nbDests_ = 1+*destinations.rbegin();  // 设置目标节点数量

    zeroFlowTimes = new std::vector<FPType>(nbNodes_*nbDests_);  // 创建零流量时间向量

    for(int i = 0; i < nbNodes_; i++){  // 遍历所有节点
        calculate(i);  // 计算从当前节点到所有目的地的路径
        for (std::set<int>::iterator it=destinations.begin(); it!=destinations.end(); ++it){  // 遍历所有目的地
            zeroFlowTimes->at(i*nbDests_ + *it) = LabelVector->at(*it);  // 存储零流量时间
        }
    }
}

// 析构函数
Astar::~Astar(){
    delete Queue;  // 释放优先队列
    delete zeroFlowTimes;  // 释放零流量时间向量
    delete LabelVector;  // 释放标签向量
    delete Predecessors;  // 释放前驱节点向量
}

// 计算从起点O到所有目的地的路径
void Astar::calculate(int O) {
    int u, v;  // 定义节点变量
    FPType Duv;  // 定义距离变量
    StarLink *nextLink;  // 定义链接指针
    StarNode *curNode;  // 定义节点指针

    PriorityQueue &Q = *Queue;  // 获取优先队列引用
    std::vector<FPType> &L = *LabelVector;  // 获取标签向量引用
    std::vector<int> &P = *Predecessors;  // 获取前驱节点向量引用
    StarNetwork &NP = *netPointer_;  // 获取网络引用

    initNodes();  // 初始化节点

    Q = PriorityQueue();  // 清空优先队列

    L[O] = 0;  // 设置起点标签为0
    Q.push(PQPair(0, O));  // 将起点加入队列

    while ( !Q.empty() ){  // 当队列不为空时
        FPType Du = Q.top().first;  // 获取当前节点的距离
        u = Q.top().second;  // 获取当前节点
        Q.pop();  // 弹出队列顶部元素

        curNode = NP.beginNode(u);  // 获取当前节点

        if (curNode == NULL)  // 如果节点不存在
            continue;  // 继续下一次循环

        if (shouldSkipZone(curNode, u, O))  // 如果应该跳过该区域
            continue;  // 继续下一次循环

        for (nextLink = NP.beginLink();  // 遍历所有出边
                nextLink != NULL;
                nextLink = NP.getNextLink()) {

            v = nextLink->getNodeToIndex();  // 获取目标节点

            Duv = Du + nextLink->getTime();  // 计算新的距离

            if ( Duv < L[v] ){  // 如果找到更短的路径
                L[v] = Duv;  // 更新标签
                P[v] = nextLink->getIndex();  // 更新前驱节点
                Q.push(PQPair(Duv, v));  // 将新节点加入队列
            }
        } 
    } 
}

// 计算从起点O到目标节点D的最短路径
void Astar::calculate(int O, int D, int odPairIndex) {
    updateBestKnownPathCost(odPairIndex);  // 更新最佳路径成本

    int u, v;  // 定义节点变量
    FPType Duv;  // 定义距离变量
    StarLink *nextLink;  // 定义链接指针
    StarNode *curNode;  // 定义节点指针

    PriorityQueue &Q = *Queue;  // 获取优先队列引用
    std::vector<FPType> &L = *LabelVector;  // 获取标签向量引用
    std::vector<FPType> &H = *zeroFlowTimes;  // 获取零流量时间向量引用
    std::vector<int> &P = *Predecessors;  // 获取前驱节点向量引用
    StarNetwork &NP = *netPointer_;  // 获取网络引用

    initNodes();  // 初始化节点
    Q = PriorityQueue();  // 清空优先队列

    L[O] = 0;  // 设置起点标签为0
    Q.push(PQPair(0, O));  // 将起点加入队列
    while ( !Q.empty() ){  // 当队列不为空时
        u = Q.top().second;  // 获取当前节点
        FPType Du = L[u];  // 获取当前节点的距离
        Q.pop();  // 弹出队列顶部元素

        if ( u == D ){  // 如果到达目标节点
            break;  // 退出循环
        }

        curNode = NP.beginNode(u);  // 获取当前节点

        if (curNode == NULL)  // 如果节点不存在
            continue;  // 继续下一次循环

        if (shouldSkipZone(curNode, u, O))  // 如果应该跳过该区域
            continue;  // 继续下一次循环

        for (nextLink = NP.beginLink();  // 遍历所有出边
                nextLink != NULL;
                nextLink = NP.getNextLink()) {

            v = nextLink->getNodeToIndex();  // 获取目标节点

            Duv = Du + nextLink->getTime();  // 计算新的距离

            if ( Duv < L[v] ){
                L[v] = Duv;
                P[v] = nextLink->getIndex();
                FPType estimatedPathCost = Duv + H[v*nbDests_ + D];
                if (estimatedPathCost < bestKnownPathCost_) {
                    Q.push(PQPair(estimatedPathCost, v));
                    if (v == D) {
                        bestKnownPathCost_ = estimatedPathCost;
                    }
                }
            }

        }
    }
}

// 判断是否应该跳过区域节点
bool Astar::shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const {
    return (node->getIsZone() && nodeIndex != originIndex);
};

// 设置路径集合
void Astar::setPathSet(PathSet* pathSet) {
    assert(pathSet != NULL);
    pathSet_ = pathSet;
};

// 更新最佳路径成本
void Astar::updateBestKnownPathCost(int odPairIndex) {
    ODSet* odSet = pathSet_->getODSetByIndex(odPairIndex);
    bestKnownPathCost_ = odSet->getMinPathCost();
};
