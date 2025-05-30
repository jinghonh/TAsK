#include "OriginBasedAlgo.h" // 包含基于起点算法的头文件
#include "OriginSet.h" // 包含起点集合头文件
#include "OriginBush.h" // 包含起点菊花头文件

// #include <stdlib.h> // 注释掉的标准库引用

OriginBasedAlgo::OriginBasedAlgo(OriginSet *originSet, StarNetwork *net, AddHook *component, 
					ConvMeasure* conv, const FPType timeLimit) :
				 	EqAlgo(component, net, timeLimit, conv), originSet_(originSet) { // 构造函数，初始化基类并设置起点集合

};

OriginBasedAlgo::~OriginBasedAlgo()= default; // 默认析构函数
		
void OriginBasedAlgo::initialise() { // 初始化方法实现
	originSet_->initialise(); // 调用起点集合的初始化方法
};

void OriginBasedAlgo::performOneIteration(){ // 执行一次迭代的方法实现
	for (const auto bush : *originSet_){ // 遍历所有起点菊花
			bush->updateTopSort(); // 更新拓扑排序
		bush->improve(); // 改进当前菊花
		mainLoop(bush); // 对当前菊花执行主循环
	}
	doSmthAfterOrigins(); // 钩子方法 - 默认不执行任何操作
};

bool OriginBasedAlgo::mainLoop(OriginBush *bush){ // 主循环方法实现

	const bool canMoveFlow = bush->equilibrate(); // 平衡当前菊花的流量
	bush->removeUnusedLinks(); // 移除未使用的链接
	return canMoveFlow; // 返回是否可以移动流量
};
