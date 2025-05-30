#ifndef ORIGIN_BASED_ALGO_TAPAS_EQII
#define ORIGIN_BASED_ALGO_TAPAS_EQII

#include "OriginBasedAlgoTapas.h" // 包含TAPAS基于起点算法头文件

/** \brief 为TAPAS实现平衡II。
*/
class OriginBasedAlgoTapasEqII : public OriginBasedAlgoTapas { // TAPAS平衡II算法类，继承自TAPAS基于起点算法
public:
	
	OriginBasedAlgoTapasEqII(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
				AddHook *component, ConvMeasure* conv, FPType timeLimit, 
				int maxNbIter); // 构造函数
	
	~OriginBasedAlgoTapasEqII(); // 析构函数

private:	
		const int maxNbIter_; // 最大迭代次数常量

		bool mainLoop(OriginBush *bush); // 重写的主循环方法
};

#endif