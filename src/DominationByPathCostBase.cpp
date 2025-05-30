// 包含必要的头文件
#include "DominationByPathCostBase.h"
#include "BiObjLabelContainer.h"
#include "UtilsForDomination.h"

// 构造函数
DominationByPathCostBase::DominationByPathCostBase() {

};

// 析构函数
DominationByPathCostBase::~DominationByPathCostBase() {

};

// 向标签集合中添加标签
void DominationByPathCostBase::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex) {
	labels.pushBackLabel(nodeIndex, label);  // 将标签添加到指定节点的标签集合末尾
};

// 重置最佳已知路径成本
void DominationByPathCostBase::resetBestKnownPathCost() {

};

// 判断路径是否值得保留
bool DominationByPathCostBase::isWorth(const BiObjLabelContainer& labels, int destIndex,
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const {
	return !UtilsForDomination::isDominatedByLabelInDestNode(labels, destIndex, 
				timeLowerBound, tollLowerBound);;  // 检查路径是否被目标节点中的其他标签支配
};

// 更新最佳已知路径
void DominationByPathCostBase::updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex) {

};
