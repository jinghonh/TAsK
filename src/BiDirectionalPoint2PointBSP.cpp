#include "BiDirectionalPoint2PointBSP.h"  // 包含双向点对点BSP算法头文件
#include "ReverseNetwork.h"               // 包含反向网络头文件
#include "BoundsCalculatorForBackwardNet.h" // 包含反向网络边界计算器头文件
#include "UtilsForDomination.h"           // 包含支配关系工具头文件
#include "DominationByPathCostBase.h"     // 包含基于路径成本的支配规则头文件

#include <cassert>                        // 包含断言头文件

// 构造函数：初始化双向点对点BSP算法
BiDirectionalPoint2PointBSP::BiDirectionalPoint2PointBSP(StarNetwork& net,
					const TollContainerType& tolls, 
					BoundsCalculatorForBSPBase& bounds,
					const PathsAdder& pathAdder,
					BoundsCalculatorForBackwardNet& backwardBounds,
					DominationByPathCostBase& dominanceRule) :
					
					LabelSettingPoint2PointBSP(net, tolls, bounds, pathAdder, 
						dominanceRule), 
					forwardList_(NULL),    // 初始化前向标签列表为空
					backwardLabels_(net.getNbNodes()), forwardLabels_(NULL), // 初始化后向标签容器
					curDirectionIsBackward_(true), backwardOriginIndex_(-1), // 设置初始方向为后向
					backwardBounds_(backwardBounds) {  // 初始化后向边界计算器
	ReverseNetwork createNet;             // 创建反向网络对象
	reversedNet_ = createNet.createReverseStar(net);  // 创建反向星型网络
};

// 析构函数：释放反向网络内存
BiDirectionalPoint2PointBSP::~BiDirectionalPoint2PointBSP() {
	delete reversedNet_;                  // 删除反向网络对象
};

// 初始化标签：设置起点和终点的标签
QueueType* BiDirectionalPoint2PointBSP::initLabels(BiObjLabel* originLabel,
										BiObjLabelContainer& labels){
	curDirectionIsBackward_ = true;       // 设置当前方向为后向
	int destIndex = getDestIndex();       // 获取目标节点索引
	BiObjLabel* destLabel = new BiObjLabel(0, 0, destIndex, NULL, NULL);  // 创建目标节点标签
	
	/*将目标标签添加到后向列表*/
	backwardList_.clear();                // 清空后向列表
	backwardList_.insert(destLabel);      // 插入目标标签
	
	/*将目标标签添加到后向标签容器*/
	backwardLabels_.deleteAllLabels();    // 删除所有后向标签
	backwardLabels_.pushBackLabel(destIndex, destLabel);  // 添加目标标签
	
	forwardLabels_ = &labels;             // 设置前向标签容器
	forwardList_ = LabelSettingBSPBase::initLabels(originLabel, labels);  // 初始化前向标签列表

	backwardOriginIndex_ = originLabel->getOwner();  // 设置后向起点索引
	
	return forwardList_;                  // 返回前向标签列表
};

// 检查是否继续寻找标签
bool BiDirectionalPoint2PointBSP::stillLookingForLabels(const QueueType& tmpLabels) const{

	if (tmpLabels.empty() || backwardList_.empty()) {  // 如果临时标签或后向列表为空
		return false;                   // 返回false
	};

	if (!forwardLabels_->empty(getDestIndex())) {  // 如果目标节点有前向标签

		QueueType::iterator lexMinForward = tmpLabels.begin();  // 获取前向最小标签
		BiObjLabel* returnValForward = *lexMinForward;

		QueueType::iterator lexMinBackward = backwardList_.begin();  // 获取后向最小标签
		BiObjLabel* returnValBackward = *lexMinBackward;	
		
		TollType newToll = 0;           // 初始化新费用
		FPType newTime = returnValForward->getTime() + returnValBackward->getTime();  // 计算新时间
		
		return !UtilsForDomination::isDominatedByLabelInDestNode(*forwardLabels_, getDestIndex(),
					newTime, newToll);  // 检查是否被支配
	}

	return true;                        // 返回true继续寻找
};

// 根据需要改变搜索方向
QueueType* BiDirectionalPoint2PointBSP::changeDirectionIfNeeded(){
	
	if (curDirectionIsBackward_ == false) {  // 如果当前是前向
		curDirectionIsBackward_ = true;      // 改为后向
		return &backwardList_;               // 返回后向列表
	}
	
	curDirectionIsBackward_ = false;         // 改为前向
	return forwardList_;                     // 返回前向列表
};

// 根据需要改变标签容器
BiObjLabelContainer* BiDirectionalPoint2PointBSP::changeLabelsIfNeeded(BiObjLabelContainer* labels){
	if (curDirectionIsBackward_ == true) {   // 如果当前是后向
		return &backwardLabels_;             // 返回后向标签容器
	}
	return forwardLabels_;                   // 返回前向标签容器
};

// 获取起始节点
StarNode* BiDirectionalPoint2PointBSP::beginNode(int owner) const {
	if (curDirectionIsBackward_ == true)  return reversedNet_->beginNode(owner);  // 返回反向网络起始节点
	return net_.beginNode(owner);          // 返回正向网络起始节点
};

// 获取起始边
StarLink* BiDirectionalPoint2PointBSP::beginLink() const {
	if (curDirectionIsBackward_ == true) return reversedNet_->beginLink();  // 返回反向网络起始边
	return net_.beginLink();               // 返回正向网络起始边
};

// 获取下一条边
StarLink* BiDirectionalPoint2PointBSP::getNextLink() const {
	if (curDirectionIsBackward_ == true) return reversedNet_->getNextLink();  // 返回反向网络下一条边
	return net_.getNextLink();             // 返回正向网络下一条边
};

// 检查是否值得添加新标签
bool BiDirectionalPoint2PointBSP::isWorthAdding(FPType newTime, TollType newToll,
												int nodeIndex) const {

	if (curDirectionIsBackward_ == false) {  // 如果是前向搜索
		return LabelSettingPoint2PointBSP::isWorthAdding(newTime, newToll, nodeIndex);  // 调用基类方法
	}
	
	FPType timeLowerBound = newTime + backwardBounds_.getTimeLowerBound(nodeIndex, 
								backwardOriginIndex_);  // 计算时间下界
	TollType tollLowerBound = newToll + backwardBounds_.getTollLowerBound(nodeIndex, 
								backwardOriginIndex_);  // 计算费用下界
	
	return isWorth(timeLowerBound, tollLowerBound);	 // 检查是否值得
};

// 将标签添加到标签集合
void BiDirectionalPoint2PointBSP::addLabelToLabelSet(BiObjLabelContainer& labels, 
			int nodeIndex, 
			BiObjLabel* label) {
	if (curDirectionIsBackward_ == false) {  // 如果是前向搜索
		dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, getDestIndex(),
					 currOdIndex_);  // 添加前向标签
	} else {
		dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, backwardOriginIndex_,
					 currOdIndex_);  // 添加后向标签
	}
};

// 合并标签
void BiDirectionalPoint2PointBSP::combine(BiObjLabel* newLabel){
	
	BiObjLabelContainer* currentLabels = forwardLabels_;  // 获取当前标签容器
	int destIndex = getDestIndex();                      // 获取目标节点索引
	int ownerInCurrLabel = newLabel->getOwner();         // 获取当前标签所有者
	if (curDirectionIsBackward_ == false) {              // 如果是前向搜索
		currentLabels = &backwardLabels_;                // 使用后向标签容器
	}
	
	if (ownerInCurrLabel != destIndex && !currentLabels->empty(ownerInCurrLabel)) {  // 如果当前节点不是目标且标签不为空
		int forwardDestIndex = getDestIndex();           // 获取前向目标索引
		for (LabelsIterator labelIt = currentLabels->begin(ownerInCurrLabel); 
						labelIt != currentLabels->end(ownerInCurrLabel); ++labelIt) {  // 遍历标签
			
			TollType newToll = newLabel->getToll() + (*labelIt)->getToll();  // 计算新费用
			FPType newTime = newLabel->getTime() + (*labelIt)->getTime();    // 计算新时间
			
			if (isNonDominated(newTime, newToll, (*forwardLabels_),	forwardDestIndex)) {
				BiObjLabel* tmpResult = NULL;
				if (curDirectionIsBackward_ == false) {
					tmpResult = new BiObjLabel(newTime, newToll, forwardDestIndex, newLabel, NULL);
				} else {
					tmpResult = new BiObjLabel(newTime, newToll, forwardDestIndex, (*labelIt), NULL);
				}
			
				mergeLabels(tmpResult, (*forwardLabels_), forwardDestIndex, (*forwardList_));
			} 

		}
	}
	
};

void BiDirectionalPoint2PointBSP::createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path) {
	
	BiObjLabel* label = labelWithMinCost;
	std::list<StarLink*> reversePathToDest;

	if (label->getPrevLink() == NULL && label->getPrevLabel() != NULL) {
		bool labelFound = false;
		int nodeIndex = reversedNet_->getNodeIndex(label->getPrevLabel()->getPrevLink()->getNodeTo());
		for (LabelsIterator it = backwardLabels_.begin(nodeIndex); 
							it != backwardLabels_.end(nodeIndex); ++it) {
			BiObjLabel* backwardLabel = *it;
			if (label->getPrevLabel()->getToll() + backwardLabel->getToll() == label->getToll() && 
					label->getPrevLabel()->getTime( )+ backwardLabel->getTime() == label->getTime()) {
				LabelSettingPoint2PointBSP::createPath(backwardLabel, reversePathToDest);
				labelFound = true;
				break;
			}
		}
		assert(labelFound);
	} else {
		path.push_back(label->getPrevLink());
	}
	
	for (std::list<StarLink*>::reverse_iterator rit=reversePathToDest.rbegin(); 
				rit!=reversePathToDest.rend(); ++rit) {
		path.push_back((*rit)->getForwardLink());
	}

	LabelSettingPoint2PointBSP::createPath(label->getPrevLabel(), path);
};
