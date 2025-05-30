#ifndef ORIGIN_BASED_ALGO_EQII
#define ORIGIN_BASED_ALGO_EQII

#include "OriginBasedAlgo.h" // 包含基于起点算法基类

/** \brief 此类实现了基于起点算法的平衡II策略。
	\warning 在高精度要求下，平衡II可能不收敛。
*/
class OriginBasedAlgoEQII : public OriginBasedAlgo { // 平衡II基于起点算法类，继承自基于起点算法
	public:
		/** @param maxNbIter 平衡II中的最大迭代次数。
		*/
		OriginBasedAlgoEQII(OriginSet *originSet, StarNetwork *net, AddHook *component, 
							ConvMeasure* conv, FPType timeLimit, int maxNbIter); // 构造函数
		~OriginBasedAlgoEQII(); // 析构函数
	
	private:	
		const int maxNbIter_; // 最大迭代次数常量

		bool mainLoop(OriginBush *bush); // 重写的主循环方法
};

#endif
