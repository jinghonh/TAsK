#ifndef LINEAR_FNC  // 防止头文件重复包含
#define LINEAR_FNC

#include "LinkFnc.h"  // 包含基类头文件

/** \brief 该类实现了线性链路成本函数
	\details t(x) = a*x + b，其中a和b为参数
 */
class LinearFnc : public LinkFnc {  // 定义LinearFnc类，继承自LinkFnc
	
	public:  // 公有成员
	
		LinearFnc(FPType a, FPType b);  // 构造函数，接收两个参数a和b
		~LinearFnc();  // 析构函数
	
		FPType evaluate(FPType arcFlow) const;  // 计算链路成本函数值
		FPType evaluateDerivative(FPType arcFlow) const;  // 计算链路成本函数的导数
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der);  // 同时计算函数值和导数

		void print() const;  // 打印函数信息

		virtual std::string toString() const {  // 返回函数类型的字符串表示
			return "linear fnc";
		};

		virtual FncType getType();  // 获取函数类型
	
	private:  // 私有成员
	
		const FPType _param1;  // 参数a
		const FPType _param2;  // 参数b
};

#endif  // 结束头文件保护
