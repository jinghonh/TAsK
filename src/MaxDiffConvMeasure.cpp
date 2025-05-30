// 包含必要的头文件
#include "MaxDiffConvMeasure.h"
#include "PathSet.h"
#include "ShortestPath.h"
#include "Origin.h"
#include "Path.h"
#include "ODSet.h"
#include "ODMatrix.h"

#include <cassert>
#include <limits>

// 构造函数：初始化最大差异收敛度量
MaxDiffConvMeasure::MaxDiffConvMeasure(FPType precision, ShortestPath* shPath,
		ODMatrix* mat, PathSet* pathSet) : 
		ConvMeasure(precision), shPath_(shPath), mat_(mat), pathSet_(pathSet) {
	assert(shPath != NULL); // 确保最短路径对象不为空
};

// 析构函数
MaxDiffConvMeasure::~MaxDiffConvMeasure(){

};

// 计算给定OD对的最大路径成本
FPType MaxDiffConvMeasure::calculateMaxPathCost(int odIndex){
	ODSet* set = pathSet_->getODSetByIndex(odIndex); // 获取OD对对应的路径集合
	set->updateSet(); // 更新路径集合
 	FPType maxCost = 0.0; // 初始化最大成本
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) {
		FPType cost = (*pathIt)->getCurrCost(); // 获取当前路径成本
		if (cost > maxCost) maxCost = cost; // 更新最大成本
	}
	return maxCost;
};

// 计算最大差异（收敛间隙）
FPType MaxDiffConvMeasure::calculateGap(){
	FPType maxDiff = 0.0; // 初始化最大差异
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){ // 遍历所有起点
		Origin* origin = *it;
		shPath_->calculate(origin->getIndex()); // 计算从当前起点出发的最短路径
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历所有终点
			PairOD* dest = *jt;
			FPType minPathCost = shPath_->getCost(dest->getIndex()); // 获取最短路径成本
			assert(minPathCost != std::numeric_limits<FPType>::infinity()); // 确保最短路径存在
			FPType diff = calculateMaxPathCost(dest->getODIndex()) - minPathCost; // 计算当前OD对的最大差异
			if (diff > maxDiff) { // 更新全局最大差异
				maxDiff = diff;
			}
		}
	}

	return maxDiff;
};
