#include "Bisection.h"  // 包含二分法类的头文件
#include "Derivative.h" // 包含导数计算类的头文件

#include <cassert>      // 包含断言宏
#include <math.h>       // 包含数学函数库
#include <stdlib.h>     // 包含标准库函数

// 构造函数：初始化二分法对象
Bisection::Bisection(FPType precision, Derivative *der) : 
						LineSearch(der), precision_(precision){
	assert(precision > 0.0 && der != NULL);  // 确保精度为正且导数计算器不为空
};

// 析构函数
Bisection::~Bisection(){

};

// 执行二分法搜索
FPType Bisection::execute(FPType a, FPType b){
	
	assert(a <= b);  // 确保区间起点小于等于终点

	FPType alpha = 0.0;  // 初始化搜索点
	FPType der = der_->calculate(b);  // 计算区间右端点的导数
	if (der <= 0.0) {  // 如果右端点导数小于等于0
		return b;      // 直接返回右端点
	}
	if (der_->calculate(0.0) >= 0.0) {  // 如果0点的导数大于等于0
		return 0.0;                     // 直接返回0
	}
	
	while (true) {  // 开始二分迭代
		alpha = (a + b) / 2.0;  // 计算区间中点
		if (alpha == a || alpha == b) {  // 如果中点与端点重合
			
			return alpha;  // 防止死循环，返回当前点
		}
		der = der_->calculate(alpha);  // 计算中点的导数
		if (der <= 0.0) {  // 如果导数小于等于0
			a = alpha;     // 更新左端点
		} else {           // 如果导数大于0
			b = alpha;     // 更新右端点
		};
		if (((b - a) <= 2 * precision_)){  // 如果区间长度小于等于2倍精度
			break;                         // 退出循环
		}
	}
	alpha = (a + b) / 2.0;  // 计算最终的中点
	return alpha;           // 返回最终结果
};

// 获取精度值
FPType Bisection::getPrecision() const {
	return precision_;  // 返回精度值
};
