#ifndef LINE_SEARCH  // 防止头文件重复包含
#define LINE_SEARCH

#include "UsedTypes.h"  // 包含类型定义头文件

class Derivative;  // 前向声明Derivative类

/** \brief 这个类是不同线搜索策略的基类。
*/
class LineSearch {
	
	public:
	
		virtual ~LineSearch(){};  // 虚析构函数

		/** 执行线搜索的方法
			@param a 线搜索的下界
			@param b 线搜索的上界
		*/
		virtual FPType execute(FPType a, FPType b) = 0;  // 纯虚函数，执行线搜索

		/** @return 返回导数对象的指针。算法有时需要它来设置Derivative的数据指针。
		*/
		Derivative* getDerivative() const {return der_;};  // 获取导数对象指针
		
	protected:
		
		Derivative *der_;  // 导数对象指针

		LineSearch(Derivative *der) : der_(der) {};  // 构造函数，初始化导数对象指针

};

#endif  // 结束头文件保护
