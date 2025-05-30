// 包含必要的头文件
#include "DominationByPathCost.h"
#include "NonAdditivePC.h"
#include "BiObjLabelContainer.h"

#include <limits>

// 构造函数：初始化路径成本和最佳已知路径成本
DominationByPathCost::DominationByPathCost(const NonAdditivePC& pathCost) : 
			pathCost_(pathCost), bestKnownPathCost_(std::numeric_limits<FPType>::infinity()) {

};

// 析构函数
DominationByPathCost::~DominationByPathCost() {

};

// 向标签集合中添加新标签
void DominationByPathCost::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex) {
	if (nodeIndex == destIndex) {
		// 计算当前路径成本
		FPType pathCost = pathCost_.calculate(label->getTime(), label->getToll(), odIndex);
		if (pathCost < bestKnownPathCost_) {
			// 如果当前路径成本更优，更新最佳已知路径成本
			bestKnownPathCost_ = pathCost;
		} else {
			return; // 如果新标签的路径成本大于最佳已知值，则不添加到标签集合中
		}
	}

	// 将标签添加到标签集合中
	labels.pushBackLabel(nodeIndex, label);
};

// 重置最佳已知路径成本为无穷大
void DominationByPathCost::resetBestKnownPathCost() {
	bestKnownPathCost_ = std::numeric_limits<FPType>::infinity();
};

// 判断是否值得继续搜索
bool DominationByPathCost::isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const {

	// 计算路径成本的下界
	FPType pathCostLowerBound = pathCost_.calculate(timeLowerBound, tollLowerBound, odIndex);
	if (pathCostLowerBound >= bestKnownPathCost_) return false;	
	return true;
};

// 更新最佳已知路径成本
void DominationByPathCost::updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
				int odIndex) {
	// 遍历目标节点的所有标签
	for (LabelsIterator it = labels.begin(destIndex); it != labels.end(destIndex); ++it) {
		// 计算每个标签的路径成本
		FPType pathCost = pathCost_.calculate((*it)->getTime(), (*it)->getToll(), odIndex);
		if (pathCost < bestKnownPathCost_) {
			// 如果找到更优的路径成本，更新最佳已知值
			bestKnownPathCost_ = pathCost;
		}
	}
};
