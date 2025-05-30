#ifndef AON_NON_ADDITIVE  // 防止头文件重复包含
#define AON_NON_ADDITIVE value  // 定义宏

#include "AONUsual.h"  // 包含AONUsual基类头文件

class NonAddShortestPathForAON;  // 前向声明非加性最短路径类

/** \brief 非加性情况下的全有或全无分配过程。
*/
class AONNonAdditive : public AONUsual {  // 定义AONNonAdditive类，继承自AONUsual
	public:
		AONNonAdditive(const ODMatrix &mat, NonAddShortestPathForAON* shPath);  // 构造函数声明

		/** \warning 释放shPath的内存。*/
		~AONNonAdditive();  // 析构函数声明

	protected:
		/** 在非加性情况下，我们跳过计算给定O-D对的总最小时间。
			@return 始终返回零。
		*/
		virtual FPType calculateTravelTime(const PairOD& dest) const;  // 虚函数声明，计算旅行时间
		
};

#endif  // 结束头文件保护