#ifndef LINEAR_INDIFF_CURVE  // 防止头文件重复包含
#define LINEAR_INDIFF_CURVE 

#include "IndiffCurve.h"  // 包含基类头文件

/** \brief 实现线性标量化函数
*/
class LinearIndiffCurve : public IndiffCurve {  // 定义线性无差异曲线类，继承自IndiffCurve
	public:
		/** \note toll1始终为零
			函数形式为 \f$f(toll) = a * toll + time1\f$ 其中
			\f$a = (time2 - time1) / toll2\f$
		**/
		LinearIndiffCurve(FPType time1, FPType time2, TollType toll2);  // 构造函数，通过时间点和收费点计算系数
		/** 如果系数a和b已知，则
			函数形式为 \f$f(toll) = a * toll + b\f$
		*/
		LinearIndiffCurve(FPType a, FPType b);  // 构造函数，直接指定系数
		~LinearIndiffCurve();  // 析构函数

		virtual FPType evaluate(TollType toll);  // 计算给定收费值对应的时间值

		virtual std::string toString();  // 将对象转换为字符串表示

	private:
		FPType a_;  // 线性函数的斜率
		FPType b_;  // 线性函数的截距
};

#endif