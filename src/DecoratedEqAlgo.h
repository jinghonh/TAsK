#ifndef _DECOR_EQ_ALGO_
#define _DECOR_EQ_ALGO_

#include "UsedTypes.h"

/** \brief 此类是 EqAlgo 装饰器的基类。*/
class DecoratedEqAlgo {
	public:
		virtual ~DecoratedEqAlgo() {}; // 虚析构函数
		virtual int execute() = 0; // 纯虚函数，执行算法

		/** @return 检查算法收敛性所花费的总 CPU 时间（秒）。
		*/
		virtual FPType getTotalConvTime() = 0; // 纯虚函数，获取总收敛时间

		/** @return 算法每次迭代所花费的总 CPU 时间（秒）（不包括输出操作，仅为纯迭代时间）。
		*/
		virtual FPType getTotalPureIterTime() = 0; // 纯虚函数，获取总纯迭代时间

		virtual FPType getInitialisationTime() = 0; // 纯虚函数，获取初始化时间
		
	protected:
		DecoratedEqAlgo(){}; // 保护构造函数
};

#endif
