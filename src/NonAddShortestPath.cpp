#include "NonAddShortestPath.h" // 包含非可加最短路径头文件
#include "ODMatrix.h" // 包含起点终点矩阵头文件
#include "OneSourceBiObjShPath.h" // 包含单起点双目标最短路径头文件
#include "Point2PointBiObjShPath.h" // 包含点对点双目标最短路径头文件
#include "Error.h" // 包含错误处理头文件
#include "Timer.h" // 包含计时器头文件
#include "NonAdditivePC.h" // 包含非可加路径成本头文件
#include "Origin.h" // 包含起点头文件
#include "PairOD.h" // 包含起点终点对头文件

#include <limits> // 包含限制头文件，用于获取数值类型的极限值
#include <cstdlib> // 包含标准库头文件
#include <sstream> // 包含字符串流头文件
#include <cassert> // 包含断言头文件

NonAddShortestPath::NonAddShortestPath(NonAdditivePC *pathCost, int nbNodes, 
					OneSourceBiObjShPath* oneSorceSP, 
					Point2PointBiObjShPath* point2pointSP, ODMatrix* mat) : // 构造函数
					
					costs_(nbNodes, std::numeric_limits<FPType>::infinity()), // 初始化成本向量为无穷大
					oneSorceSP_(oneSorceSP), // 初始化单起点双目标最短路径指针
					curDestIndex_(-1), // 初始化当前终点索引为-1
					minCostLabel_(NULL), // 初始化最小成本标签为空
					pathCost_(pathCost), // 初始化路径成本指针
					point2pointSP_(point2pointSP), // 初始化点对点双目标最短路径指针
					mat_(mat) // 初始化起点终点矩阵指针
					 {
	assert(pathCost != NULL && oneSorceSP != NULL && mat != NULL); // 断言参数非空
	currLink_ = path_.begin(); // 初始化当前链接迭代器为路径起始位置
};

NonAddShortestPath::~NonAddShortestPath(){ // 析构函数
};

void NonAddShortestPath::calculate(int originIndex){ // 计算从指定起点到所有终点的最短路径
	curDestIndex_ = -1; // 重置当前终点索引
	oneSorceSP_->calculate(originIndex); // 计算从起点到所有节点的双目标最短路径
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取当前起点
		if (origin->getIndex() == originIndex) { // 如果起点索引匹配
			for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历起点的所有终点
				PairOD* dest = *jt; // 获取当前终点
				int destIndex = dest->getIndex(); // 获取终点索引
				std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, 
					dest->getODIndex(), oneSorceSP_->getLabels()); // 计算到终点的成本和标签
				costs_[destIndex] = costAndLabel.first; // 保存到终点的成本
				doSomethingWithMinCostLabel(costAndLabel.second); // 对最小成本标签执行操作
			}
			break; // 找到匹配起点后退出循环
		}
	}
};

void NonAddShortestPath::doSomethingWithMinCostLabel(BiObjLabel* minCostLabel) { // 对最小成本标签执行操作的虚函数
	// 基类中为空实现，子类会重写此方法
};

std::pair<FPType, BiObjLabel*> NonAddShortestPath::calculateCostToDest(int destIndex, 
				int odPairIndex, const BiObjLabelContainer& labels){ // 计算到指定终点的成本和标签
	FPType minCost = std::numeric_limits<FPType>::infinity(); // 初始化最小成本为无穷大
	BiObjLabel* labelWithMinCost = NULL; // 初始化最小成本标签为空
	for (LabelsIterator labelIt = labels.begin(destIndex); labelIt != labels.end(destIndex); ++labelIt){ // 遍历终点的所有标签
		BiObjLabel* label = *labelIt; // 获取当前标签
		FPType cost = pathCost_->calculate(label->getTime(), label->getToll(), odPairIndex); // 计算标签的成本
		if (cost < minCost) { // 如果成本小于当前最小成本
			minCost = cost; // 更新最小成本
			labelWithMinCost = label; // 更新最小成本标签
		}
	}
	assert(labelWithMinCost != NULL && minCost != std::numeric_limits<FPType>::infinity()); // 断言找到了有效的最小成本标签
	return std::make_pair(minCost, labelWithMinCost); // 返回最小成本和对应的标签
};

void NonAddShortestPath::calculate(int originIndex, int destIndex, int odPairIndex){ // 计算从指定起点到指定终点的最短路径
	assert(originIndex >= 0 && destIndex >= 0); // 断言起点和终点索引有效
	curDestIndex_ = destIndex; // 设置当前终点索引

	point2pointSP_->calculate(originIndex, destIndex, odPairIndex); // 计算从起点到终点的双目标最短路径
	
	std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, odPairIndex, 
			point2pointSP_->getLabels()); // 计算到终点的成本和标签

	path_.clear(); // 清空路径
		
	minCostLabel_ = costAndLabel.second; // 设置最小成本标签
	
	costs_[destIndex] = costAndLabel.first; // 保存到终点的成本

};

FPType NonAddShortestPath::getCost(int destIndex) const{ // 获取到指定终点的最短路径成本
	return costs_[destIndex]; // 返回成本
};

StarLink* NonAddShortestPath::getInComeLink(int destIndex) const{ // 获取到达指定节点的入链接
	assert(destIndex >= 0 && curDestIndex_ != -1); // 断言终点索引有效且当前终点索引已设置
	
	if (path_.empty()) { // 如果路径为空
		assert(minCostLabel_ != NULL); // 断言最小成本标签非空
		createPath(); // 创建路径
		currLink_ = path_.begin(); // 重置当前链接迭代器
		assert(currLink_ != path_.end()); // 断言路径非空
	}

	if (currLink_ == path_.end()) { // 如果已到达路径末尾
		return NULL; // 返回空指针
	}
	StarLink* link = *currLink_; // 获取当前链接
	if (link->getNodeToIndex() == destIndex) { // 如果链接的终点是目标节点
		++currLink_; // 移动迭代器到下一个链接
		return link; // 返回当前链接
	} 
	throw Error("Should never end up here. Non-additive shortest path was called with path that was not created."); // 抛出错误，路径创建错误
	
	return NULL; // 返回空指针（不会执行到这里）
};

void NonAddShortestPath::createPath() const { // 创建路径
	point2pointSP_->createPath(minCostLabel_, path_); // 从最小成本标签创建路径
}

