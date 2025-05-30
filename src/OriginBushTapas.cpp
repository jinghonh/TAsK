#include "OriginBushTapas.h" // 包含TAPAS起点菊花头文件
#include "DAGraphTapas.h" // 包含TAPAS有向无环图头文件

#include <stdlib.h> // 包含标准库

ShortestPath * OriginBushTapas::shPath_ = NULL; // 初始化静态最短路径指针为NULL
PASManager * OriginBushTapas::pasManager_ = NULL; // 初始化静态PAS管理器指针为NULL
		
OriginBushTapas::OriginBushTapas(ShortestPath *shPath, PASManager *pasManager) : dag_(NULL) { // 构造函数，初始化dag_指针
	shPath_ = shPath; // 设置静态最短路径指针
	pasManager_ = pasManager; // 设置静态PAS管理器指针
};

OriginBushTapas::~OriginBushTapas(){ // 析构函数

};
		
DAGraph* OriginBushTapas::createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									   FPType zeroFlow, FPType dirTol){ // 创建新DAG的方法实现
	dag_ = new DAGraphTapas(net, mat, zeroFlow, dirTol, index, shPath_, pasManager_); // 创建新的TAPAS有向无环图
	return dag_; // 返回创建的图
};

void OriginBushTapas::updateTopSort(){ // 重写更新拓扑排序方法
	dag_->removeCyclicFlows(); // 移除循环流量
};
