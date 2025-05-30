#include "NonAddShortestPathForAON.h" // 包含用于全有或全无(AON)分配的非可加最短路径头文件
#include "UtilsForDomination.h" // 包含支配关系工具头文件
#include "Error.h" // 包含错误处理头文件
#include "BiObjLabel.h" // 包含双目标标签头文件

#include <cassert> // 包含断言头文件

NonAddShortestPathForAON::NonAddShortestPathForAON(
						NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
						ODMatrix* mat) : NonAddShortestPath(pathCost, nbNodes, oneSorceSP, NULL,
						mat), minCostLabels_(nbNodes), 
						shoulResetPath_(true) { // 构造函数
	for (int i = 0; i < nbNodes; ++i) { // 遍历所有节点
		minCostLabels_[i] = NULL; // 初始化每个节点的最小成本标签为空
	}
	linkIt_ = currPath_.begin(); // 初始化链接迭代器为路径起始位置
};

NonAddShortestPathForAON::~NonAddShortestPathForAON() { // 析构函数

};

StarLink* NonAddShortestPathForAON::getInComeLink(int destIndex) const { // 获取到达指定节点的入链接
	if (shoulResetPath_) { // 如果需要重置路径
		currPath_.clear(); // 清空当前路径
		assert(minCostLabels_[destIndex] != NULL); // 断言目标节点的最小成本标签非空
		UtilsForDomination::createPathFromLabel(minCostLabels_[destIndex], currPath_); // 从最小成本标签创建路径
		shoulResetPath_ = false; // 设置不需要重置路径
		linkIt_ = currPath_.begin(); // 重置链接迭代器为路径起始位置
		assert(linkIt_ != currPath_.end()); // 断言路径非空
	}
	if (linkIt_ == currPath_.end()) { // 如果已到达路径末尾
		shoulResetPath_ = true; // 设置需要重置路径
		return NULL; // 返回空指针
	}
	StarLink* link = *linkIt_; // 获取当前链接
	if (link->getNodeToIndex() == destIndex) { // 如果链接的终点是目标节点
		++linkIt_; // 移动迭代器到下一个链接
		return link; // 返回当前链接
	}
	throw Error("Should never end up here. Non-additive for AON shortest path was called with path that was not created."); // 抛出错误，路径创建错误
	return NULL; // 返回空指针（不会执行到这里）
};

void NonAddShortestPathForAON::doSomethingWithMinCostLabel(BiObjLabel* minCostLabel) { // 重写对最小成本标签执行操作的方法
	minCostLabels_[minCostLabel->getOwner()] = minCostLabel; // 保存节点的最小成本标签
};