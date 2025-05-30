#ifndef CONCAVE_INDIFF_CURVE
#define CONCAVE_INDIFF_CURVE 

#include "IndiffCurve.h"

// 实现凹型无差异曲线的类
class ConcaveIndiffCurve : public IndiffCurve {
	public:
		// 构造函数，toll1始终为0，函数形式为 f(toll) = a * toll^{1/3} + time1
		// 其中 a = (time2 - time1) / toll2^{1/3}
		ConcaveIndiffCurve(FPType time1, FPType time2, TollType toll2);
		// 构造函数，已知a和b，函数形式为 f(toll) = a * toll^{1/3} + b
		ConcaveIndiffCurve(FPType a, FPType b);
		~ConcaveIndiffCurve(); // 析构函数

		virtual FPType evaluate(TollType toll); // 计算无差异曲线在toll处的值

		virtual std::string toString(); // 返回曲线参数的字符串表示

	private:
		FPType a_; // 系数a
		FPType b_; // 系数b

};
#endif