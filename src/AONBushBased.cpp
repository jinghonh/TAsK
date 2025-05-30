// 包含必要的头文件
#include "AONBushBased.h"  // 包含AONBushBased类的头文件
#include "Origin.h"        // 包含Origin类的头文件
#include "ShortestPath.h"  // 包含ShortestPath类的头文件
#include "InitialiseSolution.h"  // 包含InitialiseSolution类的头文件

#include <limits>  // 包含数值限制相关的头文件

// 构造函数实现
// @param mat OD矩阵对象
// @param shPath 最短路径算法对象
// @param nbNodes 节点数量
AONBushBased::AONBushBased(const ODMatrix &mat, ShortestPath* shPath, int nbNodes) : 
                    AONAssignment(mat, shPath), nbNodes_(nbNodes) {
};

// 析构函数实现
AONBushBased::~AONBushBased(){
};

// 为指定起点执行分配
// @param solution 初始解对象
// @param origin 起点对象
// @return 返回分配结果
FPType AONBushBased::performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin){
    int originIndex = origin->getIndex();  // 获取起点索引
    shPath_->calculate(originIndex);       // 计算从起点到所有节点的最短路径
    int nodes[nbNodes_];                   // 创建节点数组
    for (int i = 0; i < nbNodes_; ++i){    // 初始化节点数组
        nodes[i] = -1;                     // 将所有节点标记为未访问
    }
    nodes[originIndex] = 1;                // 将起点标记为已访问
    for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {  // 遍历所有目的地
        PairOD* dest = *jt;                // 获取当前目的地
        solution->initialiseSolution(dest, shPath_, originIndex);  // 初始化目的地解
        nodes[dest->getIndex()] = 1;       // 将目的地标记为已访问
    }
    // 遍历其他非目的地节点
    FPType dist = 0.0;                     // 初始化距离变量
    for (int i = 0; i < nbNodes_; ++i){    // 遍历所有节点
        if (nodes[i] == -1){               // 如果节点未被访问
            dist = shPath_->getCost(i);     // 获取到该节点的距离
            if (dist != std::numeric_limits<FPType>::infinity()) {  // 如果距离不是无穷大（即不是另一个起点）
                PairOD fakeDest(i, 0.0);    // 创建虚拟目的地
                solution->initialiseSolution(&fakeDest, shPath_, originIndex);  // 初始化虚拟目的地解
                nodes[i] = 1;               // 将节点标记为已访问
            }
        }
    }
    return 0.0;                            // 返回分配结果
};