#include "PathBasedFlowMoveGP.h"  // 包含广义投影路径流量移动头文件
#include "PathAndDirection.h"      // 包含路径及方向头文件
#include "Path.h"                 // 包含路径头文件

#include <stdlib.h>               // 包含标准库

// 构造函数：初始化广义投影路径流量移动类
// alpha: 步长参数
// pathDirectionAlgo: 下降方向路径算法指针
PathBasedFlowMoveGP::PathBasedFlowMoveGP(FPType alpha, DescDirectionPath* pathDirectionAlgo) : 
			PathBasedFlowMove(pathDirectionAlgo), // 调用父类构造函数
			alpha_(alpha),       // 初始化步长参数
			demand_(0.0) {       // 初始化需求量为0

};

// 析构函数，无具体实现
PathBasedFlowMoveGP::~PathBasedFlowMoveGP(){

};

// 设置需求量
// demand: 新的需求量
void PathBasedFlowMoveGP::setDemand(FPType demand){
	demand_ = demand;            // 设置需求量
};

// 执行流量移动
// 返回值: 是否已达到平衡状态(始终返回false)
bool PathBasedFlowMoveGP::executeFlowMove(){
	
	FPType totalFlowToMove = 0.0;  // 初始化总流量移动量为0
	Path *currPath = NULL;         // 当前路径指针
	FPType currDir = 0.0;          // 当前方向值
	FPType flow = 0.0;             // 流量变量
	for (int i = 0; i < size_ - 1; ++i) {  // 遍历除最后一个外的所有路径
		currPath = (pathDirection_[i])->getPath();  // 获取当前路径
		currDir = (pathDirection_[i])->getDirectionValue();  // 获取当前方向值
		if (currPath->getFlow() < -alpha_ * currDir) {  // 如果当前流量小于负步长乘方向值
			currPath->setFlow(0.0);  // 将流量设置为0
		} else {
			flow = currPath->getFlow() + alpha_ * currDir;  // 计算新流量
			currPath->setFlow(flow);  // 设置新流量
			totalFlowToMove += flow;  // 累加总流量
		}
	}
	Path *minPath = (pathDirection_[size_ - 1])->getPath();  // 获取最后一个路径
	minPath->setFlow(demand_ - totalFlowToMove);  // 将最后一个路径的流量设置为需求量减去其他路径的总流量
	return false;  // 返回未平衡
};
