#ifndef QUADRATIC_APP // 防止头文件重复包含
#define QUADRATIC_APP // 定义QUADRATIC_APP宏

#include "LineSearch.h" // 包含线搜索类

class StarNetwork; // 前向声明星形网络类

/** \brief 此类实现了用于步长计算的二次近似。
*/
class QuadApp : public LineSearch { // 二次近似类，继承自LineSearch
	
	public:
		
		/** @param der 导数计算。
			@param net 网络。
		*/
		QuadApp(Derivative *der, StarNetwork* net); // 构造函数，接收导数计算和网络参数
		~QuadApp(); // 析构函数
	
		virtual FPType execute(FPType a, FPType b); // 重写execute方法，执行二次近似计算

	private:
		StarNetwork* net_; // 星形网络指针

};

#endif // 结束头文件保护

