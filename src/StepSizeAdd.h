#ifndef STEP_SIZE_ADDITIVE // 防止头文件重复包含
#define STEP_SIZE_ADDITIVE

#include "StepSize.h" // 包含步长基类头文件

class Derivative; // 前向声明导数类

/** \brief 此类实现使用加性路径成本函数时的步长计算。
*/
class StepSizeAdd : public StepSize { // 加性步长类，继承自StepSize
	public:
		StepSizeAdd(LineSearch *lineSearch, Derivative *der); // 构造函数
		~StepSizeAdd(); // 析构函数
		
		void initialiseDerivative(DescDirection *algo); // 实现初始化导数方法
	private:
		Derivative *der_; // 导数对象指针
};

#endif
