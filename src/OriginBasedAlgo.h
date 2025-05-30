#ifndef _ORIGIN_BASED_ALGO_
#define _ORIGIN_BASED_ALGO_

#include "EqAlgo.h"

class OriginSet;
class StarNetwork;
class OriginBush;

/** \brief 此类实现了基于起点算法框架的主要步骤。
*/
class OriginBasedAlgo : public EqAlgo {
	public:
		/** @param originSet 指向存储所有菊花的对象的指针。
			@param net 网络。
			@param component 指向执行收敛数据额外操作的对象的指针。
			@param conv 收敛度量。
			@param timeLimit 允许的最大计算时间。
		*/
		OriginBasedAlgo(OriginSet *originSet, StarNetwork *net, AddHook *component, 
						ConvMeasure* conv, FPType timeLimit);
		virtual ~OriginBasedAlgo();
		
	protected:
		/** 这是一个默认情况下不执行任何操作的钩子方法。它在每次迭代后执行。
			它在OriginBasedAlgoTapas中被重新实现。
		*/
		virtual void doSmthAfterOrigins(){};

		/** 为给定起点执行一次迭代中的主要操作。
			它实现平衡I。平衡II重写此方法。
			详细信息请参见OriginBasedAlgoEQII。
			@return 如果当前菊花已平衡则返回true，否则返回false。返回值仅对平衡II有用。
		*/
		virtual bool mainLoop(OriginBush *bush);
		
	private:
		OriginSet *originSet_;

		virtual void initialise();
		virtual void performOneIteration();
};

#endif
