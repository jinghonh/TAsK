#include "PathSet.h"           // 包含路径集合头文件
#include "ODSetWithStep.h"      // 包含带步长的OD集合头文件
#include "ODSetGPApp3.h"        // 包含GPAPP3 OD集合头文件
#include "Error.h"              // 包含错误处理头文件
#include "Origin.h"             // 包含起点头文件
#include "Path.h"               // 包含路径头文件
#include "ODMatrix.h"           // 包含OD矩阵头文件
#include "AONAssignment.h"      // 包含全有或全无分配头文件

#include <cassert>              // 包含断言
#include <sstream>              // 包含字符串流

// 构造函数：初始化路径集合
// setType: OD集合类型
// net: 星型网络指针
// mat: OD矩阵指针
// shPath: 最短路径算法指针
// pathCost: 路径成本计算指针
// zeroFlow: 零流量阈值
// flowMove: 路径流量移动指针
// flowMoveWithStep: 带步长的路径流量移动指针
// flowMoveGP: 广义投影路径流量移动指针
// aon: 全有或全无分配指针
PathSet::PathSet(ODSetType setType, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove,  
			PathBasedFlowMoveWithStep* flowMoveWithStep, PathBasedFlowMoveGP* flowMoveGP,
			AONAssignment* aon) : 
			odSetList_(mat->getNbODPairs()),  // 初始化OD集合列表大小
			size_(mat->getNbODPairs()),       // 设置集合大小
			currPath_(NULL),                  // 当前路径指针为NULL
			aon_(aon){                        // 设置全有或全无分配

	int odIndex = -1;                        // 初始化OD索引为-1
	for (auto it = mat->begin(); it != mat->end(); ++it){  // 遍历所有起点
		Origin* origin = *it;                // 获取当前起点
		for (auto jt = origin->begin(); jt != origin->end(); ++jt) {  // 遍历起点的所有目的地
			const PairOD* dest = *jt;              // 获取当前目的地
			odIndex = dest->getODIndex();    // 获取OD索引
			if (setType == PEAPP3) {         // 如果是PEAPP3类型
				odSetList_[odIndex] = new ODSet(odIndex, dest->getIndex(), origin->getIndex(),
												pathCost, net, shPath,  // 创建普通OD集合
					zeroFlow, flowMove);
			} else if (setType == WITH_STEP) {  // 如果是WITH_STEP类型
				odSetList_[odIndex] = new ODSetWithStep(odIndex, dest->getIndex(), origin->getIndex(),
														pathCost, net, shPath, zeroFlow,  // 创建带步长的OD集合
														flowMoveWithStep);
			} else if (setType == GPAPP3) {    // 如果是GPAPP3类型
				odSetList_[odIndex] = new ODSetGPApp3(odIndex, dest->getIndex(), origin->getIndex(),
													  pathCost, net, shPath, zeroFlow,  // 创建GPAPP3 OD集合
													  flowMoveGP, mat);
			} else {
				throw Error("Unexpected OD set type.");  // 抛出错误：意外的OD集合类型
			}
		}
	}
};

// 创建PEAPP3类型的路径集合
// net: 星型网络指针
// mat: OD矩阵指针
// shPath: 最短路径算法指针
// pathCost: 路径成本计算指针
// zeroFlow: 零流量阈值
// flowMove: 路径流量移动指针
// aon: 全有或全无分配指针
PathSet* PathSet::createSetPEAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove, AONAssignment* aon){
	return new PathSet(PEAPP3, net, mat, shPath, pathCost, zeroFlow, flowMove, NULL, NULL, aon);  // 创建并返回PEAPP3类型的路径集合
};

// 创建带步长的路径集合
// net: 星型网络指针
// mat: OD矩阵指针
// shPath: 最短路径算法指针
// pathCost: 路径成本计算指针
// zeroFlow: 零流量阈值
// flowMove: 带步长的路径流量移动指针
// aon: 全有或全无分配指针
PathSet* PathSet::createSetWithStep(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove, AONAssignment* aon){
	return new PathSet(WITH_STEP, net, mat, shPath, pathCost, zeroFlow, NULL, flowMove, NULL, aon);  // 创建并返回带步长的路径集合
};

// 创建GPAPP3类型的路径集合
// net: 星型网络指针
// mat: OD矩阵指针
// shPath: 最短路径算法指针
// pathCost: 路径成本计算指针
// zeroFlow: 零流量阈值
// flowMove: 广义投影路径流量移动指针
// aon: 全有或全无分配指针
PathSet* PathSet::createSetGPAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveGP* flowMove, AONAssignment* aon){
	return new PathSet(GPAPP3, net, mat, shPath, pathCost, zeroFlow, NULL, NULL, flowMove, aon);  // 创建并返回GPAPP3类型的路径集合
};

// 析构函数：释放所有OD集合和全有或全无分配
PathSet::~PathSet(){
	for (int i = 0; i < size_; ++i) {  // 遍历所有OD集合
		delete odSetList_[i];           // 删除OD集合
	}
	delete aon_;                        // 删除全有或全无分配
};

// 初始化路径集合
void PathSet::initialise(){
	aon_->execute(this);                // 执行全有或全无分配
	for (auto odSetIt : *this){  // 遍历所有OD集合
		odSetIt->projectPathFlowOnLinks();  // 对每个OD集合，将路径流量投影到链路上
	}
};

// 初始化单个路径
// link: 链路指针
// dest: OD对指针
// originIndex: 起点索引
void PathSet::initialiseItself(StarLink* link, PairOD *dest, int originIndex){

	const int odIndex = dest->getODIndex();   // 获取OD索引
	// 检查是否需要创建新路径
	if (link->getNodeToIndex() == (odSetList_[odIndex])->getDestIndex()) {  // 如果链路终点是OD集合的目的地
		currPath_ = new Path();         // 创建新路径
		currPath_->setFlow(dest->getDemand());  // 设置路径流量为需求量
		(odSetList_[odIndex])->addPath(currPath_);  // 将路径添加到OD集合
	}
	currPath_->addLinkToPath(link);     // 将链路添加到当前路径
	
};


// 获取OD集合迭代器的起点
ConstODSetIterator PathSet::begin() const{
	return odSetList_.begin();
};

// 获取OD集合迭代器的终点
ConstODSetIterator PathSet::end() const {
	return odSetList_.end();
};

// 打印所有OD集合
void PathSet::print() const {
	for (auto odSetIt : *this) {  // 遍历所有OD集合
		odSetIt->print();           // 打印OD集合
	}
};

// 通过索引获取OD集合
// odIndex: OD索引
// 返回值: OD集合指针
ODSet* PathSet::getODSetByIndex(int odIndex) const{
	assert(odIndex >= 0 && odIndex < size_);  // 断言OD索引在有效范围内
	return odSetList_[odIndex];               // 返回OD集合
};
