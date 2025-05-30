#include "Error.h"  // 包含错误处理头文件
#include "LabelSettingPoint2PointBSP.h"  // 包含点对点标签设置BSP算法的头文件
#include "PathsAdder.h"  // 包含路径添加器的头文件
#include "UtilsForDomination.h"  // 包含支配关系工具的头文件
#include "DominationByPathCostBase.h"  // 包含基于路径成本的支配规则基类的头文件

#include <limits>  // 包含数值限制的头文件
#include <algorithm>  // 包含算法库的头文件

// 构造函数：初始化点对点标签设置BSP算法
LabelSettingPoint2PointBSP::LabelSettingPoint2PointBSP(StarNetwork& net, 
	const TollContainerType& tolls, BoundsCalculatorForBSPBase& bounds,
	 const PathsAdder& pathAdder,
	 DominationByPathCostBase& dominanceRule):
	Point2PointBiObjShPath(net.getNbNodes()),  // 初始化点对点双目标最短路径基类
	LabelSettingBSPBase(net, tolls),  // 初始化标签设置BSP基类
	currOdIndex_(-1), dominanceRule_(dominanceRule),  // 初始化当前OD对索引和支配规则
	destNode_(-1), bounds_(bounds),  // 初始化目标节点和边界计算器
	pathAdder_(pathAdder){  // 初始化路径添加器
};

// 析构函数
LabelSettingPoint2PointBSP::~LabelSettingPoint2PointBSP(){
};

// 计算从起点到终点的路径
void LabelSettingPoint2PointBSP::calculate(int originIndex, int destIndex, int odIndex){
	destNode_ = destIndex;  // 设置目标节点
	currOdIndex_ = odIndex;  // 设置当前OD对索引

	dominanceRule_.resetBestKnownPathCost();  // 重置已知最佳路径成本
	bounds_.updateTimeBounds(destIndex, odIndex);  // 更新时间边界

	calculateBSP(originIndex, labels_);  // 计算BSP
};

// 将标签添加到标签集合中
void LabelSettingPoint2PointBSP::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label) {
	dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, destNode_, currOdIndex_);
};

// 上传路径
void LabelSettingPoint2PointBSP::uploadPaths() {
	pathAdder_.uploadPaths(currOdIndex_, destNode_, labels_);  // 上传路径
	dominanceRule_.updateBestKnown(labels_, destNode_, currOdIndex_);  // 更新已知最佳路径
};

// 从标签创建路径
void LabelSettingPoint2PointBSP::createPath(BiObjLabel *labelWithMinCost, 
	std::list<StarLink*>& path){
	UtilsForDomination::createPathFromLabel(labelWithMinCost, path);
};

// 检查是否继续寻找标签
bool LabelSettingPoint2PointBSP::stillLookingForLabels(const QueueType& tmpLabels) const{
	// 重要：假设tmpLabels按字典序排序
	if (tmpLabels.empty()) return false;  // 如果临时标签为空，返回false
	FPType minTime = (*tmpLabels.begin())->getTime();  // 获取最小时间
	TollType minToll = 0;  // 通常我们总是有一个零收费的标签
	// 即使没有，它也是一个合适的下界
	return !UtilsForDomination::isDominatedByLabelInDestNode(labels_, destNode_, minTime, minToll);
	// 这个策略似乎能加快计算速度
	// return dominanceRule_.stillLookingForLabels(minTime, minToll, currOdIndex_);
};

// 检查是否值得添加新标签
bool LabelSettingPoint2PointBSP::isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const {
	FPType timeLowerBound = newTime + bounds_.getTimeLowerBound(nodeIndex, destNode_);  // 计算时间下界
	TollType tollLowerBound = newToll + bounds_.getTollLowerBound(nodeIndex, destNode_);  // 计算收费下界
	return isWorth(timeLowerBound, tollLowerBound);  // 检查是否值得
};

// 检查是否值得添加标签
bool LabelSettingPoint2PointBSP::isWorth(FPType timeLowerBound, TollType tollLowerBound) const {
	return dominanceRule_.isWorth(labels_, destNode_, timeLowerBound, 
				tollLowerBound, currOdIndex_);
};

// 获取目标节点索引
int LabelSettingPoint2PointBSP::getDestIndex() const{
	return destNode_;
};
