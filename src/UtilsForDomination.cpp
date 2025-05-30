#include "UtilsForDomination.h" // 包含UtilsForDomination头文件
#include "BiObjLabelContainer.h" // 包含BiObjLabelContainer头文件

const FPType UtilsForDomination::tolerance_ = 1e-15; // 定义支配判断的容差值

bool UtilsForDomination::isDominated(BiObjLabel* first, BiObjLabel* second) {
	return (second->getTime() < first->getTime() + tolerance_  && 
			second->getToll() <= first->getToll()); // 如果second的时间小于first的时间（加容差）且second的费用小于等于first的费用，则first被second支配
};

bool UtilsForDomination::isDominated(FPType newTime, TollType newToll, BiObjLabel* second) {
	return (second->getTime() < newTime + tolerance_  && 
			second->getToll() <= newToll); // 如果second的时间小于newTime（加容差）且second的费用小于等于newToll，则[newTime,newToll]被second支配
};

bool UtilsForDomination::isDominatedByLabelInDestNode(const BiObjLabelContainer& labels, 
			int destIndex, FPType newTime, TollType newToll) {
	for (LabelsIterator it = labels.begin(destIndex); it != labels.end(destIndex); ++it) {
		if (isDominated(newTime, newToll, *it)) {
			return true; // 如果被目标节点中的任一标签支配，返回true
		}
	}
	return false; // 没有被任何标签支配，返回false
};

void UtilsForDomination::createPathFromLabel(BiObjLabel* destLabel, std::list<StarLink*> &path) {
	BiObjLabel* label = destLabel; // 从目标标签开始
	
	while (label != NULL) {
		if (label->getPrevLink() != NULL) path.push_back(label->getPrevLink()); // 将前一个链接添加到路径中
		label = label->getPrevLabel(); // 向前移动到前一个标签

	}
};