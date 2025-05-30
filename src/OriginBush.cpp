#include "OriginBush.h"
#include "PairOD.h"
#include "StarLink.h"
#include "DAGraph.h"

#include <stdlib.h>
#include <iostream>
#include <cassert>

void OriginBush::allocateDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol){
	assert(daGraph_ == NULL);
	daGraph_ = createNewDAG(index, net, mat, zeroFlow, dirTol); 
};

OriginBush::OriginBush() : daGraph_(NULL), topSortUpToDate_(false) { 
};

OriginBush::~OriginBush(){
	delete daGraph_;
};

void OriginBush::updateTopSort(){
	// 如果在此菊花的上一次mainLoop调用中移除了一些链接，则调用拓扑排序
	if (!topSortUpToDate_) {
		daGraph_->topologicalSort();
		topSortUpToDate_ = true;
	}
};

bool OriginBush::improve(){
	
	assert(topSortUpToDate_ == true);
	// 1. 计算最小和最大树 - 初始化 u_i 和 U_i（假定存在拓扑顺序）
	daGraph_->buildMinMaxTrees(-1);
	
	// 2. 遍历所有尚未在集合中的链接，检查是否值得添加
	bool wasImproved = daGraph_->addBetterLinks();
	
	// 3. 如果菊花已改进 - 执行拓扑排序 - 构建通路
	if (wasImproved) {
		daGraph_->topologicalSort();
		callBuildMinMaxTrees();
	}
	return wasImproved;
};

void OriginBush::callBuildMinMaxTrees(){
	daGraph_->buildMinMaxTrees(-1);
};

bool OriginBush::equilibrate(){
	return daGraph_->moveFlow();
};

void OriginBush::removeUnusedLinks(){
	// 从daGraph_中移除流量为零的链接，同时保持连通性
	if (daGraph_->removeUnusedLinks()) {
		topSortUpToDate_ = false;
	}
};

void OriginBush::addLinkAndUpdateFlow(StarLink *link, PairOD* dest){
	assert(link != NULL && dest != NULL);
	daGraph_->addLink(link);
	if (dest->getODIndex() != -1) {
		FPType demand = dest->getDemand();
		daGraph_->addOriginFlow(link->getIndex(), demand);
		link->addFlow(demand);
	}
};

void OriginBush::print(){
	std::cout << "Origin: " << daGraph_->getOriginIndex() << " daGraph_ = " << 
				daGraph_ << std::endl;
	daGraph_->print();
};

int OriginBush::getOriginIndex() const{
	return daGraph_->getOriginIndex();
};

void OriginBush::printOriginFlow() const{
	daGraph_->printOriginFlow();
};

FPType OriginBush::checkOFlowsFeasibility(){
	return daGraph_->checkOFlowsFeasibility();
}; 
