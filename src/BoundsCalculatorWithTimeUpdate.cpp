#include "BoundsCalculatorWithTimeUpdate.h"

#include <cassert>

// 构造函数，初始化网络、收费、目的节点和OD对索引
BoundsCalculatorWithTimeUpdate::BoundsCalculatorWithTimeUpdate(StarNetwork& net,
			const TollContainerType& tolls) : 
			BoundsCalculatorForBSP(net, tolls), destNode_(-1), prevODIndex_(-1) {
	// 初始化成员变量
};

// 析构函数
BoundsCalculatorWithTimeUpdate::~BoundsCalculatorWithTimeUpdate() {
	// 无需特殊处理
};

// 更新指定目的节点和OD对的时间边界
void BoundsCalculatorWithTimeUpdate::updateTimeBounds(int destNodeIndex, int odPairIndex) {
	// 如果OD对索引发生变化，则更新目的节点和OD对索引，并重新计算最短路径
	if (odPairIndex != prevODIndex_) {
		destNode_ = destNodeIndex;      // 更新目的节点索引
		prevODIndex_ = odPairIndex;     // 更新OD对索引
		spp_->calculate(destNode_);     // 计算到目的节点的最短路径
	}
};

// 获取从节点 nodeIndex 到目的地 destNodeIndex 的零流量行程时间下界
FPType BoundsCalculatorWithTimeUpdate::getTimeLowerBound(int nodeIndex, int destNodeIndex) {
	assert(destNode_ == destNodeIndex);         // 确保目的节点索引一致
	return spp_->getCost(nodeIndex);            // 返回节点到目的地的最短路径代价
};

// 获取进入指定目的节点的链路指针
StarLink* BoundsCalculatorWithTimeUpdate::getInComeLink(int destIndex) const {
	return spp_->getInComeLink(destIndex);      // 返回进入目的节点的链路
};