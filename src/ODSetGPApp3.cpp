#include "ODSetGPApp3.h" // 包含ODSetGPApp3头文件
#include "PathBasedFlowMoveGP.h" // 包含基于路径的梯度投影流量移动类
#include "ODMatrix.h" // 包含OD矩阵类

#include <cassert> // 包含断言功能

ODMatrix* ODSetGPApp3::mat_ = NULL; // 初始化静态OD矩阵指针为空
PathBasedFlowMoveGP* ODSetGPApp3::flowMoveGP_ = NULL; // 初始化静态流量移动指针为空

ODSetGPApp3::ODSetGPApp3(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net, 
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveGP* flowMove, ODMatrix *mat) : 
				ODSet(index, destIndex, originIndex, pathCost, net, shPath, zeroFlow, flowMove) { // 构造函数调用父类构造函数
	mat_ = mat; // 设置静态OD矩阵指针
	flowMoveGP_ = flowMove; // 设置静态流量移动指针
}; 

ODSetGPApp3::~ODSetGPApp3() { // 析构函数

}; 

void ODSetGPApp3::calculateProjectedDirection() { // 实现计算投影方向的方法
	FPType demand = mat_->getDemandByIndex(originIndex_, destIndex_); // 获取当前OD对的需求量
	assert(demand > 0.0); // 断言需求量为正
	flowMoveGP_->setDemand(demand); // 设置梯度投影流量移动的需求量
}; 

