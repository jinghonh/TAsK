#include "ConcaveIndiffCurve.h"

#include <sstream> // 引入字符串流库
#include <math.h> // 引入数学库
#include <cassert> // 引入断言库

// 构造函数，给定两个时间和一个toll，计算a和b
ConcaveIndiffCurve::ConcaveIndiffCurve(FPType time1, FPType time2, TollType toll2) :
	a_((time2 - time1) / pow(toll2, 1.0/3)), b_(time1) {
	assert(toll2 > 0 && time1 >= 0 && time2 >= 0); // 保证参数有效
};

// 构造函数，直接给定a和b
ConcaveIndiffCurve::ConcaveIndiffCurve(FPType a, FPType b) : a_(a), b_(b) {

};

// 析构函数
ConcaveIndiffCurve::~ConcaveIndiffCurve() {

};

// 计算无差异曲线在toll处的值
FPType ConcaveIndiffCurve::evaluate(TollType toll) {
	return a_ * pow(toll, 1.0/3) + b_;
};

// 返回曲线参数的字符串表示
std::string ConcaveIndiffCurve::toString() {
	std::stringstream ss;
	ss << "Concave: (" << a_ << ", " << b_ << ")"; 
	return ss.str();
};