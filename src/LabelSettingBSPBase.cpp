// 包含必要的头文件
#include "LabelSettingBSPBase.h"
#include "Error.h"
#include "UtilsForDomination.h"
#include "LatexNetOutputLinksExploredInBSP.h"
#include "StarLink.h"

#include <list>
#include <algorithm> 
#include <sstream>
#include <cassert>

// 构造函数实现
LabelSettingBSPBase::LabelSettingBSPBase(StarNetwork& net, const TollContainerType& tolls) : 
				net_(net), tolls_(tolls) 
				, seenLinks_(net.getNbLinks()) {
	// 初始化所有seenLinks_为NULL
	for (int i = 0; i < net.getNbLinks(); ++i) {
		seenLinks_[i] = NULL;
	}
};

// 析构函数实现
LabelSettingBSPBase::~LabelSettingBSPBase() {};

// 计算BSP的主要方法
void LabelSettingBSPBase::calculateBSP(int originIndex, BiObjLabelContainer& labels) {
	
	// 创建起点标签
	BiObjLabel* originLabel = new BiObjLabel(0, 0, originIndex, NULL, NULL);

	BiObjLabelContainer* labelsPointer = &labels;

	// 清除所有现有标签
	labelsPointer->deleteAllLabels();

	uploadPaths();

	// 将起点标签添加到容器中
	labelsPointer->pushBackLabel(originIndex, originLabel);
	
	QueueType* tmpLabels = initLabels(originLabel, labels);

	while (stillLookingForLabels(tmpLabels_)) { // 停止条件必须始终使用原始的tmpLabels_

		tmpLabels = changeDirectionIfNeeded(); // 可能会将tmpLabels_更改为双向版本的不同集合
		labelsPointer = changeLabelsIfNeeded(labelsPointer);

		QueueType& tmpLabelsRef = *tmpLabels;
		
		// 获取并移除下一个标签
		BiObjLabel* currentLabel = getNextLabelAndRemoveItFromList(tmpLabelsRef);
		int owner = currentLabel->getOwner();
		
		StarNode* curNode = beginNode(owner);
		if ((curNode != NULL)) { 
			// && (!curNode->getIsZone() || (owner == originIndex))) { // 区域的特殊条件
			for (StarLink* link = beginLink(); link != NULL; link = getNextLink()) { 
				
				assert(currentLabel != NULL);
				// 计算新的费用和时间
				TollType newToll = currentLabel->getToll() + tolls_[link->getForwardLink()->getIndex()];
				FPType newTime = currentLabel->getTime() + link->getTime();
					
				int linkHead = link->getNodeToIndex();
				// 检查是否值得添加新标签且不被支配
				if ( isWorthAdding(newTime, newToll, linkHead) && 
						isNonDominated(newTime, newToll, *labelsPointer, linkHead)) {
					BiObjLabel* newLabel = 
								new BiObjLabel(newTime, newToll, linkHead, currentLabel, link);
					
					mergeLabels(newLabel, *labelsPointer, linkHead, tmpLabelsRef);
					tmpLabelsRef.insert(newLabel);
					combine(newLabel);
				
				} 
			}
		}
	};
};

// 合并标签的虚函数
void LabelSettingBSPBase::combine(BiObjLabel* newLabel) {
};

// 上传路径的虚函数
void LabelSettingBSPBase::uploadPaths() {
};

// 初始化标签队列
QueueType* LabelSettingBSPBase::initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels) {
	tmpLabels_.clear();
	tmpLabels_.insert(originLabel);
	return &tmpLabels_;
};

// 检查是否还需要继续寻找标签
bool LabelSettingBSPBase::stillLookingForLabels(const QueueType& tmpLabels) const {
	return !tmpLabels.empty();
};

// 如果需要改变方向则改变
QueueType* LabelSettingBSPBase::changeDirectionIfNeeded() {
	return &tmpLabels_;
};

// 如果需要改变标签则改变
BiObjLabelContainer* LabelSettingBSPBase::changeLabelsIfNeeded(BiObjLabelContainer* labels) {
	return labels;
};

// 获取指定所有者的起始节点
StarNode* LabelSettingBSPBase::beginNode(int owner) const {
	return net_.beginNode(owner);
};

// 获取起始链接
StarLink* LabelSettingBSPBase::beginLink() const {
	return net_.beginLink();
};

// 获取下一个链接
StarLink* LabelSettingBSPBase::getNextLink() const {
	return net_.getNextLink();
};

// 从列表中获取并移除下一个标签
BiObjLabel* LabelSettingBSPBase::getNextLabelAndRemoveItFromList(QueueType& tmpLabels) {
	QueueType::iterator lexMin = tmpLabels.begin();
	assert(lexMin != tmpLabels.end());
	BiObjLabel* returnVal = *lexMin;
	tmpLabels.erase(lexMin);
	return returnVal;
};

// 创建新标签
BiObjLabel* LabelSettingBSPBase::createLabel(BiObjLabel* currentLabel, StarLink* link) const {
	TollType newToll = currentLabel->getToll() + tolls_[link->getIndex()];
	FPType newTime = currentLabel->getTime() + link->getTime();
	return new BiObjLabel(newTime, newToll, link->getNodeToIndex(), currentLabel, link);
};

// 检查新标签是否不被支配
bool LabelSettingBSPBase::isNonDominated(FPType newTime, TollType newToll, const BiObjLabelContainer& labels, 
				int linkHead) const {
	for (LabelsIterator it = labels.begin(linkHead); it != labels.end(linkHead); ++it) {
		if (UtilsForDomination::isDominated(newTime, newToll, *it)) return false;
	}
	return true;
};

// 合并标签
void LabelSettingBSPBase::mergeLabels(BiObjLabel* label, BiObjLabelContainer& labels, int linkHead,
				QueueType& tmpLabels) {
	for (LabelsIterator it = labels.begin(linkHead); it != labels.end(linkHead); ++it) {
		BiObjLabel* labelInNode = *it;
		if (UtilsForDomination::isDominated(labelInNode, label)) {
			
			deleteFromTmpLabels(tmpLabels, labelInNode);

			delete labelInNode;
			it = labels.erase(linkHead, it);
			--it;
		}
	}
	addLabelToLabelSet(labels, linkHead, label);
};

// 将标签添加到标签集合中
void LabelSettingBSPBase::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex,
		BiObjLabel* label) {
	labels.pushBackLabel(nodeIndex, label);
};

// 从临时标签列表中删除标签
void LabelSettingBSPBase::deleteFromTmpLabels(QueueType& tmpLabels, BiObjLabel* labelInNode){
	// 查找相等的值范围
	std::pair<QueueType::iterator, QueueType::iterator> equalValues = 
								tmpLabels.equal_range(labelInNode);
	QueueType::iterator elemToDelete = equalValues.first;
	for (; elemToDelete != equalValues.second; ++elemToDelete) {
	 	if (*elemToDelete == labelInNode){
	 		break;
	 	}
	}
	if (elemToDelete != equalValues.second) tmpLabels.erase(elemToDelete); 
};
