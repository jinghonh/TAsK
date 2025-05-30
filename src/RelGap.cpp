#include "RelGap.h" // 包含头文件
#include "Error.h" // 包含错误处理类
#include "Origin.h" // 包含起点类
#include "PairOD.h" // 包含OD对类
#include "ODMatrix.h" // 包含OD矩阵类
#include "ShortestPath.h" // 包含最短路径类
#include "StarNetwork.h" // 包含星形网络类

#include <stdlib.h> // 包含标准库
#include <iostream> // 包含输入输出流
#include <math.h> // 包含数学函数

RelGap::RelGap(FPType precision, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath) : ConvMeasure(precision),
										net_(net), mat_(mat), shPath_(shPath) { // 构造函数，初始化基类和成员变量

};

RelGap::~RelGap(){ // 析构函数

};

FPType RelGap::getMinTravelTime(){ // 获取最小旅行时间的方法
	FPType minTravelTime = 0.0; // 初始化最小旅行时间
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取起点
		shPath_->calculate(origin->getIndex()); // 计算最短路径
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历所有目的地
			PairOD* dest = *jt; // 获取目的地
			minTravelTime += dest->getDemand() * shPath_->getCost(dest->getIndex());  // 累加需求乘以最短路径成本
		}
	}
	return minTravelTime; // 返回最小旅行时间
};

FPType RelGap::getTotalTravelTime() { // 获取总旅行时间的方法
	FPType totalTravelTime = 0.0; // 初始化总旅行时间
	for (StarLink *link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) { // 遍历所有链接
		totalTravelTime += link->getFlow() * link->getTime(); // 累加流量乘以时间
		if (totalTravelTime != totalTravelTime) { // 检查NaN（不是数字）
			std::cout << "totalTravelTime = " << totalTravelTime << " flow = " << 
				link->getFlow() << " time = " << link->getTime() << std::endl; // 输出调试信息
			throw Error("totalTravelTime是NaN。出现严重错误。"); // 抛出错误
		}
	}
	return totalTravelTime; // 返回总旅行时间
};

FPType RelGap::calculateGapValue(FPType minTravelTime, FPType totalTravelTime) { // 计算差距值的方法
	if (fabs(totalTravelTime) <= 1e-25) { // 如果总旅行时间接近于0
		return std::numeric_limits<FPType>::infinity( ); // 返回无穷大
	}
	FPType gap = 1.0 - minTravelTime / totalTravelTime; // 计算差距值
	return gap; // 返回差距值
};


FPType RelGap::calculateGap(){ // 计算差距的方法
	return calculateGapValue(getMinTravelTime(), getTotalTravelTime()); // 返回差距值
};

FPType RelGap::calculateGap(FPType minTravelTime) { // 使用给定最小旅行时间计算差距的方法
	return calculateGapValue(minTravelTime, getTotalTravelTime()); // 返回差距值
};