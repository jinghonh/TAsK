#include "NonAddShortestPathWithLazyBSP.h" // 包含带惰性双目标最短路径的非可加最短路径头文件
#include "UtilsForDomination.h" // 包含支配关系工具头文件
#include "OneSourceBiObjShPath.h" // 包含单起点双目标最短路径头文件

NonAddShortestPathWithLazyBSP::NonAddShortestPathWithLazyBSP(
			NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
			ODMatrix* mat) : NonAddShortestPath(pathCost, nbNodes, oneSorceSP, 
			NULL, mat), prevOriginIndex_(-1) { // 构造函数
	std::cout << "NonAddShortestPathWithLazyBSP*******************" << std::endl; // 打印初始化信息
};

NonAddShortestPathWithLazyBSP::~NonAddShortestPathWithLazyBSP() { // 析构函数

};

void NonAddShortestPathWithLazyBSP::calculate(int originIndex, int destIndex, int odPairIndex) { // 计算从指定起点到指定终点的最短路径
	curDestIndex_ = destIndex; // 设置当前终点索引
	if (originIndex == prevOriginIndex_) { // 如果起点索引与前一个相同
		updateLabelTravelTimes(oneSorceSP_->getLabels()); // 更新标签的旅行时间
	} else { // 否则
		oneSorceSP_->calculate(originIndex); // 重新计算从起点到所有节点的双目标最短路径
		prevOriginIndex_ = originIndex; // 更新前一个起点索引
	}
	std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, odPairIndex, 
			oneSorceSP_->getLabels()); // 计算到终点的成本和标签

	path_.clear(); // 清空路径	
	minCostLabel_ = costAndLabel.second; // 设置最小成本标签
	costs_[destIndex] = costAndLabel.first; // 保存到终点的成本
};

void NonAddShortestPathWithLazyBSP::updateLabelTravelTimes(const BiObjLabelContainer& labels) { // 更新标签的旅行时间
	for (LabelsIterator labelIt = labels.begin(curDestIndex_); labelIt != labels.end(curDestIndex_); ++labelIt){ // 遍历当前终点的所有标签
		BiObjLabel* label = *labelIt; // 获取当前标签
		label->setTime(recalculatePathTime(label)); // 重新计算并设置标签的旅行时间
	}
};

FPType NonAddShortestPathWithLazyBSP::recalculatePathTime(BiObjLabel* destLabel) const { // 重新计算路径时间
	BiObjLabel* label = destLabel; // 从目标标签开始
	FPType travelTime = 0; // 初始化旅行时间为0
	while (label != NULL) { // 当标签非空时
		if (label->getPrevLink() != NULL) travelTime += label->getPrevLink()->getTime(); // 如果有前一个链接，则添加其时间
		label = label->getPrevLabel(); // 移动到前一个标签
	}
	return travelTime; // 返回总旅行时间
};

void NonAddShortestPathWithLazyBSP::createPath() const { // 创建路径
	UtilsForDomination::createPathFromLabel(minCostLabel_, path_); // 从最小成本标签创建路径
};