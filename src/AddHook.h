#ifndef ADD_HOOK_
#define ADD_HOOK_

#include "UsedTypes.h"

/** \brief 这是一个基类，用于在EqAlgo类中执行一些操作，这些操作可以使用算法每次迭代后获得的收敛信息。
	\details 通常这些操作包括屏幕或文件输出。这个类本身不执行任何操作。
	具体的操作在派生类中实现。
 */
class AddHook {
	public:
		
		AddHook(){};
		virtual ~AddHook(){};
		
		/** 此方法不执行任何操作。 */
		virtual void produceOut(FPType timePassed, FPType gap) {};
		
};

#endif
