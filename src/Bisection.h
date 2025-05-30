#ifndef NEW_BISECTION_  // 防止头文件重复包含的宏定义开始
#define NEW_BISECTION_  // 定义宏

#include "LineSearch.h"  // 包含线搜索基类头文件

class Derivative;  // 前向声明导数计算类

/** \brief 该类实现了用于步长计算的二分法
*/
class Bisection : public LineSearch {  // 二分法类，继承自线搜索基类
	
	public:
		
		/** 
			@param precision 二分法的精度
			@param der 指向实现导数计算对象的指针
		*/
		Bisection(FPType precision, Derivative *der);  // 构造函数
		~Bisection();  // 析构函数
	
		FPType execute(FPType a, FPType b);  // 执行二分法计算的方法

		/** @return 返回二分法的精度
		*/
		FPType getPrecision() const;  // 获取精度的方法
	
	private:
		const FPType precision_;  // 存储二分法精度的常量成员变量
};

#endif  // 防止头文件重复包含的宏定义结束
