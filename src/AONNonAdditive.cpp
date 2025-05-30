// 包含必要的头文件
#include "AONNonAdditive.h"  // 包含AONNonAdditive类的头文件
#include "NonAddShortestPathForAON.h"  // 包含非加性最短路径算法的头文件
#include "InitialiseSolution.h"  // 包含初始化解的头文件

// 构造函数实现
// @param mat OD矩阵对象
// @param shPath 非加性最短路径算法对象
AONNonAdditive::AONNonAdditive(const ODMatrix &mat, NonAddShortestPathForAON* shPath) : 
				AONUsual(mat, shPath){  // 调用基类构造函数初始化

};

// 析构函数实现
AONNonAdditive::~AONNonAdditive(){
	delete shPath_;  // 释放最短路径算法对象的内存
	shPath_ = NULL;  // 将指针设置为空
};

// 计算旅行时间
// @param dest 目的地对象
// @return 返回旅行时间(这里固定返回0.0)
FPType AONNonAdditive::calculateTravelTime(const PairOD& dest) const{
	return 0.0;  // 返回旅行时间
};