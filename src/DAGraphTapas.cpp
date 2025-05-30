#include "DAGraphTapas.h"
#include "Utils.h" 
#include "Error.h"
#include "PASManager.h"
#include "ShortestPath.h"
#include "PAS.h"

#include <cassert>
#include <math.h>

ShortestPath* DAGraphTapas::shPath_ = NULL; // 最短路径对象指针
PASManager* DAGraphTapas::pasManager_ = NULL; // PAS管理器对象指针

DAGraphTapas::DAGraphTapas(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
							int originIndex, 
							ShortestPath *shPath, PASManager *pasManager) :
							DAGraph(net, mat, zeroFlow, originIndex), 
							minShift_(0.0), exploredLinks_() {
	 shPath_ = shPath; // 初始化最短路径对象
	 pasManager_ = pasManager; // 初始化PAS管理器对象
}; 

DAGraphTapas::~DAGraphTapas() {

}; 

bool DAGraphTapas::checkPositiveFlow(int linkIndex){
	return (getOriginFlow(linkIndex) > zeroFlow_); // 检查链路流量是否为正
};

bool DAGraphTapas::moveFlow() {
  shPath_->calculate(getOriginIndex()); // 计算最短路径
  pasManager_->recalculatePASCosts(); // 重新计算PAS成本
  // 查找被起点使用但不在最小成本树中的链路
  std::list<StarLink*> inLinks;
  StarLink* link = NULL;
  StarLink* shPathLink = NULL;
  int i = -1;

  for (int j = 0; j < nodeSize_; ++j) { //不进行拓扑排序，遍历节点
    i = nodeIndexes_[j];
    getInLinks(i, inLinks);
    shPathLink = shPath_->getInComeLink(i); 
    
    if (shPathLink != NULL) {
	    for (std::list<StarLink*>::iterator it = inLinks.begin();
	    		it != inLinks.end(); ++it){
	     	link = *it;
	      	// 检查当前链路是否不属于最小成本树
		    if (getOriginFlow(link->getIndex()) > zeroFlow_ && link != shPathLink){
				pasManager_->createNewPAS(this, link, i);
		    }
		}
    }
  }

  // 目前我们只遍历所有活动的PAS
  bool flowWasMoved = false;
  for (CostPasIterator pasIt = pasManager_->begin(); pasIt != pasManager_->end(); ++pasIt){
	if ((*pasIt)->moveFlow()) {

		flowWasMoved = true;
	}
   }

  return flowWasMoved; 
}; 

void DAGraphTapas::setMinShift(FPType value){
	assert(value >= 0.0);
	minShift_ = value; // 设置最小偏移量
};

FPType DAGraphTapas::getMinShift(){
	return minShift_; // 获取最小偏移量
};


void DAGraphTapas::removeCyclicFlows(){
	while (true) {
		exploredLinks_.clear();
		if (topologicalSort() == false) break; // 如果拓扑排序失败，则退出循环
	};
};


// 总是返回 true 
bool DAGraphTapas::handleBackEdge(StarLink* link){
	
	int linkInd = link->getIndex();
	int nextNode = link->getNodeFromIndex();
	int termNode = link->getNodeToIndex();
	StarLink* nextLink = NULL;
	std::list<StarLink*> cycle;
	cycle.push_back(link);
	FPType flow = 0.0;
	FPType minFlow = getOriginFlow(linkInd);
	for (std::list<StarLink*>::const_reverse_iterator it = exploredLinks_.rbegin();
			it != exploredLinks_.rend(); ++it) {
		nextLink = *it;
		if (nextLink->getNodeToIndex() == nextNode){
			flow = getOriginFlow(nextLink->getIndex());
			cycle.push_back(nextLink);
			if (flow < minFlow) minFlow = flow;
			nextNode = nextLink->getNodeFromIndex();
			if (nextNode == termNode) break;
		}
	}
	
	// 将环上的流量减少 minFlow
	StarLink *linkTmp = NULL;
	int linkTmpIndex = -1;
	for(std::list<StarLink*>::iterator it = cycle.begin(); it != cycle.end(); ++it) {
		linkTmp = *it;
		linkTmpIndex = linkTmp->getIndex();
		if (getOriginFlow(linkTmpIndex) - minFlow < zeroFlow_) {
			setOriginFlowToZero(linkTmpIndex);
		} else {
			addOriginFlow(linkTmpIndex, -minFlow);
		}
		if (linkTmp->getFlow() - minFlow < zeroFlow_) {
			linkTmp->setFlowToZero();
		} else {
			linkTmp->addFlow(-minFlow);
		}
		linkTmp->updateTime();
	}
	return true;	
};

void DAGraphTapas::handleExploredLink(StarLink* link){
	exploredLinks_.push_back(link); // 将已探索的链路添加到列表中
};

