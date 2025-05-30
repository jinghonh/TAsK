#ifndef CONST_FNC
#define CONST_FNC

#include "LinkFnc.h"

// 该类实现了常数路段费用函数
// 公式为：c_a(f_a) = param
class ConstFnc : public LinkFnc {
	public:
		ConstFnc(FPType param); // 构造函数，初始化参数
		~ConstFnc(); // 析构函数
		
		FPType evaluate(FPType linkFlow) const; // 计算常数费用函数值
		FPType evaluateDerivative(FPType linkFlow) const; // 计算常数费用函数的一阶导数
		void print() const; // 打印函数信息
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der); // 同时计算函数值和导数
		
		virtual FncType getType(); // 获取函数类型

		virtual std::string toString() const {
			return "constant fnc"; // 返回函数类型字符串
		};
		
		LinkFnc* clone() const {return new ConstFnc(*this);} // 克隆当前对象
	private:
		const FPType param_; // 常数参数
};

#endif
