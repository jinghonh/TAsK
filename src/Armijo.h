#ifndef _ARMIJO_
#define _ARMIJO_

#include "LineSearch.h"

class Derivative;

/** \brief 这个类实现了Armijo-like规则来计算步长。
*/
class Armijo : public LineSearch {
	
	public:
		/** 
			@param decrement 每次迭代时步长减小的因子。
			@param der 实现导数计算的对象。
		*/	
		Armijo(FPType decrement, Derivative* der);
		~Armijo();
		
		FPType execute(FPType a, FPType b);
	
		/** @return 步长减小的因子。
		*/
		FPType getDecriment() const;
	
	private:
	
		const FPType decriment_;
	
};

#endif

