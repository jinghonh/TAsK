#include "ODSet.h"
#include "DescDirectionPathPE.h"
#include "DescDirectionPathGP.h"
#include "DescDirectionPathPG.h"
#include "Error.h"
#include "Utils.h"
#include "PathBasedFlowMovePE.h"
#include "PathBasedFlowMoveGP.h"
#include "PathCost.h"
#include "ShortestPath.h"

// #include <stdlib.h>
#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <iomanip> 

// 静态成员变量初始化
StarNetwork *ODSet::net_ = nullptr;       // 星型网络指针，静态成员
ShortestPath *ODSet::shPath_ = nullptr;   // 最短路径算法指针，静态成员
FPType ODSet::zeroFlow_ = 0.0;         // 零流量阈值，静态成员
PathCost *ODSet::pathCost_ = nullptr;     // 路径成本计算指针，静态成员
PathBasedFlowMove *ODSet::flowMove_ = nullptr; // 路径流量移动指针，静态成员

// 构造函数：初始化OD集合
// index: OD对的索引
// destIndex: 目的地索引
// originIndex: 起点索引
// pathCost: 路径成本计算指针
// net: 星型网络指针
// shPath: 最短路径算法指针
// zeroFlow: 零流量阈值
// flowMove: 路径流量移动指针
ODSet::ODSet(const int index, const int destIndex, const int originIndex, PathCost *pathCost, StarNetwork *net,
		ShortestPath *shPath, const FPType zeroFlow, PathBasedFlowMove* flowMove) :
		 destIndex_(destIndex), originIndex_(originIndex), index_(index){
	net_ = net;                    // 设置网络指针
	shPath_ = shPath;              // 设置最短路径算法指针
	
	pathCost_ = pathCost;          // 设置路径成本计算指针
	zeroFlow_ = zeroFlow;          // 设置零流量阈值
	
	minDist_ = std::numeric_limits<FPType>::infinity( ); // 初始化最小距离为无穷大
	
	flowMove_ = flowMove;          // 设置路径流量移动指针
};


// 析构函数：释放所有路径内存
ODSet::~ODSet(){
	for (const auto & allPath : allPaths_) {
		delete allPath;               // 删除每个路径对象
	}
};

// 获取OD对的索引
int ODSet::getIndex() const{
	return index_;
};

// 获取目的地索引
int ODSet::getDestIndex() const{
	return destIndex_;
};

// 获取起点索引
int ODSet::getOriginIndex() const{
	return originIndex_;
};

// 添加路径到OD集合
void ODSet::addPath(Path *path){
	allPaths_.push_back(path);      // 将路径添加到路径列表
	const FPType pathCost = path->getCurrCost(); // 获取路径当前成本
	if (pathCost < minDist_) minDist_ = pathCost; // 更新最小距离
};
		
// 获取路径迭代器的起点
PathIterator ODSet::begin() const{
	return allPaths_.begin();
};

// 获取路径迭代器的终点
PathIterator ODSet::end() const{
	return allPaths_.end();
};

// 打印OD集合信息
void ODSet::print() const{
	std::cout << "OD pair# " << getIndex() << " origin = " << getOriginIndex() << " dest = " 
		<< getDestIndex()  << std::endl;  // 打印OD对的索引、起点和终点
	for (const auto pathIt : *this) {
		pathIt->print();       // 打印每条路径的信息
	}
	std::cout << std::endl;
};

// 更新OD集合中所有路径的成本
void ODSet::updateSet(){
	
	FPType dist = 0.0;            // 初始化距离为0
	minDist_ = std::numeric_limits<FPType>::infinity( ); // 重置最小距离为无穷大
	for (auto path : *this) {
			dist = pathCost_->calculate(path, index_); // 计算路径成本
		path->setCurrCost(dist);   // 设置路径当前成本
		if (dist < minDist_) {     // 如果发现更小的成本
			minDist_ = dist;       // 更新最小距离
		}
	}
};

// 尝试通过添加新路径来改进OD集合
bool ODSet::improveSet(){
	shPath_->calculate(originIndex_, destIndex_, index_); // 计算从起点到终点的最短路径
	const FPType minDist = shPath_->getCost(destIndex_);        // 获取最短路径的成本
	if (minDist - minDist_ < -zeroFlow_) { // 如果找到的路径比当前最小路径更好（考虑误差）
		const auto path = new Path();           // 创建新路径
		path->setCurrCost(minDist);        // 设置路径成本
		StarLink *link = shPath_->getInComeLink(destIndex_); // 获取到达终点的链路
		int nextDest = link->getNodeFromIndex();            // 获取链路起点
		while (link != nullptr) {             // 从终点向起点回溯构建路径
			path->addLinkToPath(link);     // 将链路添加到路径
			nextDest = link->getNodeFromIndex(); // 获取链路起点
			link = shPath_->getInComeLink(nextDest); // 获取到达该点的链路
		}
		addPath(path);                     // 将新路径添加到OD集合
		return true;                       // 返回已改进
	}
	return false;                          // 返回未改进
};

// 保存所有路径的当前流量到上一状态
void ODSet::setPrevFlows() const {
	for (const auto path : *this) {
			path->setPrevFlow(path->getFlow()); // 将当前流量设置为上一流量
	}
};

// 平衡OD集合中的路径流量
bool ODSet::equilibrate(){
	//save current flow to prev flow
	setPrevFlows();                       // 保存当前流量
	
	bool isEquilibrated = false;          // 初始化平衡状态为假
	isEquilibrated = flowMove_->calculatePathDirection(allPaths_); // 计算路径方向
	if (isEquilibrated) {                 // 如果已经平衡
		return true;                      // 返回已平衡
	}
	
	calculateProjectedDirection();        // 计算投影方向（子类中实现的钩子方法）
	isEquilibrated = flowMove_->executeFlowMove(); // 执行流量移动
	
	if (isEquilibrated) {                 // 如果现在已经平衡
		return true;                      // 返回已平衡
	}
	
	return false;                         // 返回未平衡
};

// 获取OD集合中的路径数量
int ODSet::getNbPaths() const{
	return allPaths_.size(); 
};
		
// 移除未使用的路径（流量小于等于零流量阈值）
void ODSet::removeUnusedPaths(){
	const Path* path = nullptr;
	for (auto it = allPaths_.begin(); it != allPaths_.end(); ++it) {
		path = *it;
		if (path->getFlow() <= zeroFlow_) { // 如果路径流量小于等于零流量阈值
			delete path;                    // 删除路径对象
			it = allPaths_.erase(it);       // 从列表中移除并更新迭代器
			--it;                           // 迭代器回退以适应删除
		}
	}
};


// 将路径流量变化投影到所有链路上
void ODSet::projectPathFlowOnLinks(){
	
	FPType flow = 0.0;                    // 初始化流量为0
	for (const auto path : *this) {
			flow = path->getFlow() - path->getPrevFlow(); // 计算路径流量变化
		if (fabs(flow) > zeroFlow_) {     // 如果流量变化大于零流量阈值
			for (const auto & it : *path) {
				it->addFlow(flow);     // 将流量变化添加到路径上的每个链路
			}
		}
	}
	for (const auto path : *this) {
			for (const auto link : *path) {
				if (link->getFlow() < zeroFlow_) { // 如果链路流量小于零流量阈值
					link->setFlow(0.0);       // 将链路流量设置为0
			}
			link->updateTime();              // 更新链路时间
		}
	}

};

// 获取OD集合中的最小路径成本
FPType ODSet::getMinPathCost() const {
	return minDist_;
};