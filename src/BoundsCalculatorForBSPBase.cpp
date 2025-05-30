#include "BoundsCalculatorForBSPBase.h"

// 构造函数
BoundsCalculatorForBSPBase::BoundsCalculatorForBSPBase() {

};

// 析构函数
BoundsCalculatorForBSPBase::~BoundsCalculatorForBSPBase(){

};

// 更新指定目的节点和OD对的时间边界
void BoundsCalculatorForBSPBase::updateTimeBounds(int destNodeIndex, int odPairIndex) {

};

// 获取从节点 nodeIndex 到目的地 destNodeIndex 的收费下界
TollType BoundsCalculatorForBSPBase::getTollLowerBound(int nodeIndex, int destNodeIndex) {
	return 0; // 返回收费下界，基类默认返回0
};

// 获取从节点 nodeIndex 到目的地 destNodeIndex 的零流量行程时间下界
FPType BoundsCalculatorForBSPBase::getTimeLowerBound(int nodeIndex, int destNodeIndex) {
	return 0; // 返回零流量行程时间下界，基类默认返回0
};

// 初始化所有边界
void BoundsCalculatorForBSPBase::initializeBounds() {

};