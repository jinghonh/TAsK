#include "PathBasedFlowMoveWithStep.h"  // 包含带步长的路径流量移动头文件
#include "PathAndDirection.h"          // 包含路径及方向头文件
#include "Path.h"                      // 包含路径头文件
#include "StepSize.h"                  // 包含步长计算头文件

#include <stdlib.h>                    // 包含标准库
#include <cassert>                     // 包含断言

// 构造函数：初始化带步长的路径流量移动类
// stepSize: 步长计算指针
// pathDirectionAlgo: 下降方向路径算法指针
// zeroFlow: 零流量阈值
PathBasedFlowMoveWithStep::PathBasedFlowMoveWithStep(StepSize *stepSize,
													 DescDirectionPath* pathDirectionAlgo, 
													 FPType zeroFlow):
													 PathBasedFlowMove(pathDirectionAlgo),  // 调用父类构造函数
													 stepSize_(stepSize), zeroFlow_(zeroFlow),  // 初始化步长计算和零流量阈值
													 dir_(NULL){  // 初始化下降方向指针为NULL

};

// 析构函数，无具体实现
PathBasedFlowMoveWithStep::~PathBasedFlowMoveWithStep(){

};

// 执行流量移动
// 返回值: 是否达到平衡状态
bool PathBasedFlowMoveWithStep::executeFlowMove(){
	assert(dir_ != NULL);  // 断言下降方向指针不为NULL
	FPType alpha = stepSize_->getStepSize(dir_);  // 获取步长
	if (alpha == 0.0) {  // 如果步长为0
		return true;     // 返回已平衡
	}
	Path* path = NULL;   // 路径指针
	FPType newFlow = 0.0;  // 新流量变量
	for(PathAndDirection *it = beginPathDirection(); it != NULL; it = getNextPathDirection()){  // 遍历所有路径方向
		path = it->getPath();  // 获取当前路径
		newFlow = path->getFlow() + alpha * it->getDirectionValue();  // 计算新流量
		if (newFlow < zeroFlow_) {  // 如果新流量小于零流量阈值
			newFlow = 0.0;          // 将新流量设置为0

		}
		path->setFlow(newFlow);     // 设置路径新流量
	}
	return false;                   // 返回未平衡
};

// 设置下降方向
// dir: 下降方向指针
void PathBasedFlowMoveWithStep::setDescDirection(DescDirection* dir){
	dir_ = dir;                     // 设置下降方向指针
};
