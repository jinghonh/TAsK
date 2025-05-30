#include "PathBasedFlowMove.h"    // 包含路径流量移动头文件
#include "DescDirectionPath.h"     // 包含下降方向路径头文件

#include <stdlib.h>                // 包含标准库

// 构造函数：初始化路径流量移动类
// pathDirectionAlgo: 下降方向路径算法指针
PathBasedFlowMove::PathBasedFlowMove(DescDirectionPath* pathDirectionAlgo) : pathDirection_(NULL),
									 size_(0),            // 初始化路径方向数组大小为0
								     currDirection_(0),   // 初始化当前方向索引为0
								     pathDirectionAlgo_(pathDirectionAlgo) { // 初始化下降方向算法

};

// 计算路径方向
// paths: 路径列表
// 返回值: 是否已经平衡
bool PathBasedFlowMove::calculatePathDirection(const std::list<Path*> &paths){
	bool isEquilibrated = false;  // 初始化平衡状态为假
	pathDirection_ = pathDirectionAlgo_->calculatePathDirection(size_, paths, isEquilibrated); // 调用算法计算路径方向
	return isEquilibrated;        // 返回是否已平衡
};

// 获取路径方向数组的第一个元素
// 返回值: 第一个路径方向指针
PathAndDirection* PathBasedFlowMove::beginPathDirection() const {
	currDirection_ = 0;           // 重置当前方向索引为0
	if (size_ == 0) return NULL;  // 如果数组大小为0，返回NULL
	return pathDirection_[0];     // 返回第一个路径方向
};

// 获取路径方向数组的下一个元素
// 返回值: 下一个路径方向指针，如果没有更多则返回NULL
PathAndDirection* PathBasedFlowMove::getNextPathDirection() const {
	++currDirection_;             // 增加当前方向索引
	if (currDirection_ >= size_) return NULL; // 如果索引超出范围，返回NULL
	return  pathDirection_[currDirection_];   // 返回下一个路径方向
};
