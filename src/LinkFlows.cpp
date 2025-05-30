// 包含必要的头文件
#include "LinkFlows.h"
#include "StarNetwork.h"
#include "LabelCorrectingAl.h"
#include "LineSearch.h"
#include "Derivative.h"

#include <stdlib.h>
#include <iostream>

// 构造函数实现
LinkFlows::LinkFlows(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			LineSearch* lineSearch, FPType zeroFlow,
			LabelCorrectingAl* LCShPath) :
			nbLinks_(net->getNbLinks()), stepPrev_(0.0), stepPrevPrev_(0.0), zeroFlow_(zeroFlow), 
			lineSearch_(lineSearch), net_(net), aon_(*mat, LCShPath), minTravelTime_(0.0) {
	
	// 分配内存并初始化数组
	linkFlows_ = new FPType[nbLinks_];        // 存储链路流量
	linkFlowsAux_ = new FPType[nbLinks_];     // 辅助数组，用于临时存储流量
	indexes_ = new int[nbLinks_];             // 存储链路索引
	direction_ = new FPType[nbLinks_];        // 存储搜索方向
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] = 0.0;                  // 初始化链路流量为0
		linkFlowsAux_[i] = 0.0;               // 初始化辅助流量为0
		indexes_[i] = i;                      // 设置索引值
		direction_[i] = 0.0;                  // 初始化方向为0
	}
};

// 析构函数实现
LinkFlows::~LinkFlows(){
	delete[] linkFlows_;      // 释放链路流量数组
	delete[] linkFlowsAux_;   // 释放辅助流量数组
	delete[] indexes_;        // 释放索引数组
	delete[] direction_;      // 释放方向数组
};

// 获取最小旅行时间
FPType LinkFlows::getMinTravelTime() const{
	return minTravelTime_;
};

// 初始化函数
void LinkFlows::initialise(){
	minTravelTime_ = aon_.execute(this);      // 执行全有全无分配
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] = linkFlowsAux_[i];     // 更新链路流量
	}
};

// 更新链路流量
void LinkFlows::updateLinkFlows(){
	for (StarLink *link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		link->setFlow(linkFlows_[link->getIndex()]);    // 设置链路流量
		link->updateTime();                             // 更新时间
	}
};

// 计算辅助流量
void LinkFlows::calculateFWAux(){
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlowsAux_[i] = 0.0;                        // 重置辅助流量
	}
	minTravelTime_ = aon_.execute(this);               // 执行全有全无分配
};

// 计算搜索方向
void LinkFlows::calculateDirection(){
	calculateFWAux();                                  // 计算辅助流量
	for(int i = 0; i < nbLinks_; ++i) {
		direction_[i] = linkFlowsAux_[i] - linkFlows_[i];    // 计算方向向量
	}
};
		
// 平衡流量分配
void LinkFlows::equilibrate(){
	calculateDirection();                              // 计算搜索方向
	
	stepPrevPrev_ = stepPrev_;                         // 保存前两步长
	(lineSearch_->getDerivative())->setDataPointers(nbLinks_, linkFlows_, direction_, indexes_);  // 设置导数计算所需数据
	stepPrev_ = lineSearch_->execute(0.0, 1.0);        // 执行线搜索
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] += stepPrev_ *  direction_[i];   // 更新链路流量
		if (linkFlows_[i] < zeroFlow_) {               // 处理小于零流量的情况
			linkFlows_[i] = 0.0;
		}
	}
};

// 初始化单个链路
void LinkFlows::initialiseItself(StarLink* link, PairOD *dest, int originIndex){
	linkFlowsAux_[link->getIndex()] += dest->getDemand();    // 增加需求到辅助流量
};
