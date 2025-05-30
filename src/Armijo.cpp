#include "Armijo.h"  // 包含Armijo类的头文件
#include "Derivative.h"  // 包含Derivative类的头文件

#include <cassert>  // 包含断言功能的头文件
#include <cstdlib>  // 包含标准库头文件

// 构造函数实现
// @param decriment 递减因子
// @param der 导数计算器指针
Armijo::Armijo(FPType decriment, Derivative *der) : 
			LineSearch(der), decriment_(decriment) {
	assert(decriment > 0.0 && der != NULL);  // 确保递减因子为正且导数计算器不为空
};

// 析构函数实现
Armijo::~Armijo(){

};

// 执行Armijo线搜索算法
// @param a 区间左端点
// @param b 区间右端点
// @return 返回满足Armijo条件的步长
FPType Armijo::execute(FPType a, FPType b){
	assert(a <= b);  // 确保区间端点有序
	FPType alpha = b;  // 初始化步长为右端点
	if (der_->calculate(0.0) >= 0.0) {
		return 0.0; // 当在0.0处的导数为正时的病态情况
	}
	FPType der = 0.0;  // 初始化导数值
	while (true) {
		der = der_->calculate(alpha);  // 计算当前步长处的导数值
		if (der > 0.0) {
			alpha /= decriment_;  // 如果导数为正，则减小步长
		} else {
			return alpha;  // 如果导数为负，返回当前步长
		}
	}
	return alpha;  // 返回最终步长
};

// 获取递减因子
// @return 返回递减因子的值
FPType Armijo::getDecriment() const{
	return decriment_;  // 返回递减因子
};
