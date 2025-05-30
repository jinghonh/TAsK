#ifndef ORIGIN_BASED_ALGO_TAPAS
#define ORIGIN_BASED_ALGO_TAPAS

#include "OriginBasedAlgo.h" // 包含基于起点算法基类
#include "PASManager.h" // 包含PAS管理器

/** \brief 此类通过添加TAPAS所需的功能扩展OriginBasedAlgo。
*/
class OriginBasedAlgoTapas : public OriginBasedAlgo { // TAPAS基于起点算法类，继承自基于起点算法
	public:
		explicit OriginBasedAlgoTapas(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
				AddHook *component, ConvMeasure* conv, FPType timeLimit); // 显式构造函数
		~OriginBasedAlgoTapas(); // 析构函数
	
	private:
		PASManager* pasSet_; // PAS管理器指针
		
		/** 从PASManager中移除未使用的PAS，并在未删除的PAS内移动流量。
		*/
		void doSmthAfterOrigins(); // 重写的钩子方法
};

#endif
