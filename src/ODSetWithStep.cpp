#include "ODSetWithStep.h" // 包含ODSetWithStep头文件
#include "Utils.h" // 包含工具函数
#include "PathBasedFlowMoveWithStep.h" // 包含带步长的基于路径的流量移动类
#include "PathAndDirection.h" // 包含路径和方向类
#include "StarNetwork.h" // 包含星型网络类
#include "Path.h" // 包含路径类
#include "StarLink.h" // 包含星型链接类

#include <set> // 包含集合容器
#include <vector> // 包含向量容器

PathBasedFlowMoveWithStep* ODSetWithStep::flowMoveStep_ = NULL; // 初始化静态流量移动指针为空
FPType* ODSetWithStep::linkFlows_ = NULL; // 初始化静态链接流量数组为空
bool ODSetWithStep::wasInitialised_ = false; // 初始化静态初始化标志为假
FPType *ODSetWithStep::projDir_ = NULL; // 初始化静态投影方向数组为空
int * ODSetWithStep::indexes_ = NULL; // 初始化静态索引数组为空
int ODSetWithStep::size_ = 0; // 初始化静态大小为0
int ODSetWithStep::nbLinks_ = 0; // 初始化静态链接数为0

ODSetWithStep::ODSetWithStep(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
			ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove) : 
			ODSet(index, destIndex, originIndex, pathCost, net, shPath, zeroFlow, flowMove) { // 构造函数，调用父类构造函数
	flowMoveStep_ = flowMove; // 设置静态流量移动指针
	nbLinks_ = net->getNbLinks(); // 设置静态链接数
	if (!wasInitialised_) { // 如果尚未初始化
			linkFlows_ = new FPType[nbLinks_]; // 分配链接流量数组内存
			projDir_ = new FPType[nbLinks_]; // 分配投影方向数组内存
			indexes_ = new int[nbLinks_]; // 分配索引数组内存
			for (int i = 0; i < nbLinks_; ++i) { // 遍历所有链接
				linkFlows_[i] = 0.0; // 初始化链接流量为0
				projDir_[i] = 0.0; // 初始化投影方向为0
				indexes_[i] = -1; // 初始化索引为-1
			}
	}
	wasInitialised_ = true; // 设置初始化标志为真
}; 

ODSetWithStep::~ODSetWithStep() { // 析构函数
	delete[] linkFlows_; // 释放链接流量数组内存
	delete[] projDir_; // 释放投影方向数组内存
	delete[] indexes_; // 释放索引数组内存
	linkFlows_ = NULL; // 将链接流量指针设为空
	projDir_ = NULL; // 将投影方向指针设为空
	indexes_ = NULL; // 将索引指针设为空
}; 

FPType* ODSetWithStep::getProjectedDirection() { // 获取投影方向的方法
	 return projDir_; // 返回投影方向数组指针
}; 

FPType ODSetWithStep::getUpperBound() { // 获取上界的方法
	FPType ub = std::numeric_limits<FPType>::infinity(); // 初始化上界为无穷大
	FPType dir = 0.0; // 初始化方向值为0
	FPType ubTmp = 0.0; // 初始化临时上界为0
	for (PathAndDirection *pathDir = flowMoveStep_->beginPathDirection(); pathDir != NULL; 
		pathDir = flowMoveStep_->getNextPathDirection()) { // 遍历所有路径方向对
		dir = pathDir->getDirectionValue(); // 获取方向值
		if (dir < 0.0) { // 如果方向为负
			ubTmp = -(pathDir->getPath()->getFlow()) / dir; // 计算临时上界
			if (ubTmp < ub) { // 如果临时上界小于当前上界
				ub = ubTmp; // 更新上界
			}	
		}
	}
	return ub; // 返回上界
}; 

FPType* ODSetWithStep::getLinkFlows() { // 获取链接流量的方法
	 return linkFlows_; // 返回链接流量数组指针
}; 

int ODSetWithStep::getSize(){ // 获取大小的方法
	return size_; // 返回大小
};

int* ODSetWithStep::getIndexes(){ // 获取索引的方法
	return indexes_; // 返回索引数组指针
};

void ODSetWithStep::calculateProjectedDirection() { // 计算投影方向的方法
	// 将所有值设为0
	for (int i = 0; i < nbLinks_; ++i) { // 遍历所有链接
		projDir_[i] = 0.0; // 初始化投影方向为0
	}
	
	// 将基于路径的方向投影到链接上
	Path *path = NULL; // 初始化路径指针为空
	int linkIndex = -1; // 初始化链接索引为-1

	for (PathAndDirection *pathDir = flowMoveStep_->beginPathDirection(); pathDir != NULL; pathDir = flowMoveStep_->getNextPathDirection()) { // 遍历所有路径方向对
		path = pathDir->getPath(); // 获取路径指针
		
		for(StarLinkIterator it = path->begin(); it != path->end(); ++it){ // 遍历路径中的所有链接
			StarLink* link = *it; // 获取链接指针
			linkIndex = link->getIndex(); // 获取链接索引
			projDir_[linkIndex] += pathDir->getDirectionValue(); // 累加方向值到对应链接的投影方向
			linkFlows_[linkIndex] = link->getFlow(); // 设置链接流量
		}
	}
	
	int count = 0; // 初始化计数器为0
	for (int i = 0; i < nbLinks_; ++i) { // 遍历所有链接
		if (projDir_[i] != 0.0) { // 如果投影方向不为0
			indexes_[count] = i; // 记录索引
			++count; // 增加计数器
		}
	}

	size_ = count; // 设置大小
	flowMoveStep_->setDescDirection(this); // 设置下降方向
}; 

int ODSetWithStep::getOdIndex() const { // 获取OD索引的方法
	return getIndex(); // 返回索引
};

PathBasedFlowMove* ODSetWithStep::getPathBasedFlowMove() const { // 获取基于路径的流量移动的方法
	return flowMoveStep_; // 返回流量移动指针
};