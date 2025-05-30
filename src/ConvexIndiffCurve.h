#ifndef CONVEX_INDIFF_CURVE
#define CONVEX_INDIFF_CURVE 

#include "IndiffCurve.h"

// 实现凸型无差异曲线的类
class ConvexIndiffCurve : public IndiffCurve {
	public:
		// 构造函数，toll1始终为0，函数形式为 f(toll) = a * toll^{3} + time1
		// 其中 a = (time2 - time1) / toll2^{3}
		ConvexIndiffCurve(FPType time1, FPType time2, TollType toll2);
		// 构造函数，已知a和b，函数形式为 f(toll) = a * toll^{3} + b
		ConvexIndiffCurve(FPType a, FPType b);
		~ConvexIndiffCurve(); // 析构函数

		virtual FPType evaluate(TollType toll); // 计算无差异曲线在toll处的值

		virtual std::string toString(); // 返回曲线参数的字符串表示

	private:
		FPType a_; // 系数a
		FPType b_; // 系数b

};
#endif