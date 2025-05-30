#ifndef BPR_FNC_
#define BPR_FNC_

#include "LinkFnc.h"

// 该类实现了BPR路段费用函数
// 公式为：t(x) = freeFlow * (1 + B * (x / capacity) ^ power)
// 为避免在计算一阶和二阶导数时出现浮点数问题，部分数值常量直接写入代码
class BprFnc : public LinkFnc {
	public:
		BprFnc(FPType freeFlow, FPType B, FPType capacity, FPType power); // 构造函数，初始化参数
		~BprFnc(); // 析构函数

		FPType evaluate(FPType linkFlow) const; // 计算BPR函数值
		FPType evaluateDerivative(FPType linkFlow) const; // 计算BPR函数一阶导数
		FPType evaluateSecondDerivative(FPType linkFlow) const; // 计算BPR函数二阶导数

		void print() const; // 打印函数参数信息

		virtual std::string toString() const {
			return "BPR fnc"; // 返回函数类型字符串
		};
		
		LinkFnc* clone() const {return new BprFnc(*this);} // 克隆当前对象

		FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der); // 同时计算函数值和导数

		virtual FncType getType(); // 获取函数类型

		virtual FPType getCapacity(); // 获取容量参数
	private:
		const FPType freeFlow_; // 自由流时间
		const FPType B_; // B参数
		const FPType capacity_; // 容量参数
		const FPType power_; // 幂参数
};

#endif
