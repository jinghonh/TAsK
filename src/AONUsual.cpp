// 包含必要的头文件
#include "AONUsual.h"  // 包含AONUsual类的头文件
#include "InitialiseSolution.h"  // 包含初始化解的头文件
#include "ShortestPath.h"  // 包含最短路径算法的头文件
#include "Origin.h"  // 包含起点类的头文件

// 构造函数实现
// @param mat OD矩阵对象
// @param shPath 最短路径算法对象
AONUsual::AONUsual(const ODMatrix &mat, ShortestPath* shPath) : AONAssignment(mat, shPath) {
};

// 析构函数实现
AONUsual::~AONUsual(){

};

// 计算旅行时间
// @param dest 目的地对象
// @return 返回旅行时间(需求乘以路径成本)
FPType AONUsual::calculateTravelTime(const PairOD& dest) const {
	return dest.getDemand() * shPath_->getCost(dest.getIndex());  // 返回需求乘以路径成本
};

// 为给定起点执行分配
// @param solution 初始化解对象
// @param origin 起点对象
// @return 返回最小旅行时间
FPType AONUsual::performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin) {
	int originIndex = origin->getIndex();  // 获取起点索引
	shPath_->calculate(originIndex);  // 计算从起点到所有目的地的最短路径
	FPType minTravelTime = 0.0;  // 初始化最小旅行时间为0
	for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {  // 遍历所有目的地
		PairOD* dest = *jt;  // 获取当前目的地
		solution->initialiseSolution(dest, shPath_, originIndex);  // 初始化解
		minTravelTime += calculateTravelTime(*dest);  // 累加旅行时间
	}
	return minTravelTime;  // 返回总的最小旅行时间
};