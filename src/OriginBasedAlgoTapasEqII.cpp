#include "OriginBasedAlgoTapasEqII.h" // 包含TAPAS平衡II算法头文件
#include "OriginBush.h" // 包含起点菊花头文件

OriginBasedAlgoTapasEqII::OriginBasedAlgoTapasEqII(OriginSet *originSet, StarNetwork *net, 
				PASManager* pasSet, AddHook *component, 
				ConvMeasure* conv, FPType timeLimit, int maxNbIter) :
				OriginBasedAlgoTapas(originSet, net, pasSet, component, 
				conv, timeLimit), maxNbIter_(maxNbIter) { // 构造函数，初始化基类并设置最大迭代次数

};
	
OriginBasedAlgoTapasEqII::~OriginBasedAlgoTapasEqII(){ // 析构函数

};

bool OriginBasedAlgoTapasEqII::mainLoop(OriginBush *bush){ // 重写主循环方法
	int nbIter = 0; // 初始化迭代计数器
	while (true) { // 无限循环
		++nbIter; // 增加迭代计数
		bool canMoveFlow = OriginBasedAlgo::mainLoop(bush); // 调用基类的主循环方法
		if (!canMoveFlow) return false; // 如果不能移动流量，返回false
		bush->updateTopSort(); // 更新拓扑排序
		if (nbIter >= maxNbIter_) return false; // 如果达到最大迭代次数，返回false
	}; 
	return false; // 永远不会执行到这里，但需要返回值
};