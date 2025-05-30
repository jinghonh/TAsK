#include "ShortestPathAdder.h" // 包含最短路径添加器头文件
#include "PathSet.h" // 包含路径集合头文件
#include "BiObjLabelContainer.h" // 包含双目标标签容器头文件
#include "Path.h" // 包含路径头文件
#include "ODSet.h" // 包含起终点集合头文件

#include <limits> // 包含数值限制头文件

ShortestPathAdder::ShortestPathAdder(const TollContainerType& tolls) : CurrentPathsAdder(tolls) { // 构造函数，初始化收费容器

};

ShortestPathAdder::~ShortestPathAdder() { // 析构函数

};

void ShortestPathAdder::uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const { // 上传路径方法的实现
	ODSet* set = paths_->getODSetByIndex(odIndex); // 获取指定索引的OD集合
	FPType minCost = std::numeric_limits<FPType>::infinity(); // 初始化最小成本为无穷大
	Path* minPath = NULL; // 初始化最小成本路径为NULL
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) { // 遍历OD集合中的所有路径
		Path* path = *pathIt; // 获取当前路径
		FPType pathCost = path->getCurrCost(); // 获取路径当前成本
		if (pathCost < minCost) { // 如果路径成本小于当前最小成本
			minCost = pathCost; // 更新最小成本
			minPath = path; // 更新最小成本路径
		}
	}
	if (minPath != NULL) { // 如果找到了最小成本路径
		FPType pathTime = 0; // 初始化路径时间为0
		TollType pathToll = 0; // 初始化路径收费为0
		for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) { // 遍历路径中的所有链接
			StarLink* link = *it; // 获取当前链接
			pathTime += link->getTime(); // 累加链接时间
			pathToll += tolls_[link->getIndex()]; // 累加链接收费
		}
		BiObjLabel* label = new BiObjLabel(pathTime, pathToll, destIndex, NULL, NULL); // 创建新的双目标标签
		labels.pushBackLabel(destIndex, label); // 将标签添加到标签容器中
	}
	
};