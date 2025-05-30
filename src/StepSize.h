#ifndef STEP_SIZE
#define STEP_SIZE

#include "UsedTypes.h"

class LineSearch;
class DescDirection;

/** \brief 此类用于在基于路径的算法中，这些算法使用线搜索来计算步长。
	该类提供了步长计算的接口。
*/
class StepSize {
	public:
		
		virtual ~StepSize();
		
		/** 此方法返回计算出的步长，给定一个DescDirection对象，
			该对象提供访问线搜索所需数据的接口。
		*/
		FPType getStepSize(DescDirection *algo);
		
	protected:
		
		LineSearch *lineSearch_;

		StepSize(LineSearch *lineSearch);

		/** 此方法初始化线搜索中使用的导数。
		*/
		virtual void initialiseDerivative(DescDirection *algo) = 0;
};

#endif
