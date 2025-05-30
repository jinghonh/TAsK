// 包含头文件
#include "LazyShortestPath.h"

// 构造函数，初始化成员变量
LazyShortestPath::LazyShortestPath(ShortestPath* shPath) : shPath_(shPath), 
					prevOriginIndex_(-1), originChanged_(true), newPathCost_(0){

};

// 析构函数，释放内存
LazyShortestPath::~LazyShortestPath() {
	delete shPath_;
};

// 计算从起点到所有终点的最短路径
void LazyShortestPath::calculate(int originIndex) {
	shPath_->calculate(originIndex);
	originChanged_ = true;
};  

// 计算从起点到指定终点的最短路径
void LazyShortestPath::calculate(int originIndex, int destIndex, int odPairIndex) {
	if (originIndex == prevOriginIndex_) {
		updatePathCost(destIndex);
		originChanged_ = false;
	} else {
		shPath_->calculate(originIndex);
		prevOriginIndex_ = originIndex;
		originChanged_ = true;
	}
}; 
		
// 获取到指定终点的路径成本
FPType LazyShortestPath::getCost(int destIndex) const {
	if (originChanged_) {
		return shPath_->getCost(destIndex);
	} else {
		return newPathCost_;
	}
};
		
// 获取到达指定终点的入边
StarLink* LazyShortestPath::getInComeLink(int destIndex) const {
	return shPath_->getInComeLink(destIndex);
}; 

// 更新路径成本
void LazyShortestPath::updatePathCost(int destIndex) {
	StarLink *link = shPath_->getInComeLink(destIndex);
	FPType pathCost = 0;
	while (link != NULL) {
		pathCost += link->getTime();
		link = shPath_->getInComeLink(link->getNodeFromIndex());
	}
	newPathCost_ = pathCost;
};