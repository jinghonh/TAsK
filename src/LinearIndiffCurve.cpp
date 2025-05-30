// 包含线性无差异曲线类的头文件
#include "LinearIndiffCurve.h"

// 包含字符串流和断言功能的头文件
#include <sstream>
#include <cassert>

// 构造函数：通过两个时间点和一个收费点构造线性无差异曲线
// time1: 第一个时间点
// time2: 第二个时间点
// toll2: 对应的收费点
LinearIndiffCurve::LinearIndiffCurve(FPType time1, FPType time2, TollType toll2) :
	a_((time2 - time1) / toll2), b_(time1) {
	// 确保输入参数的有效性：收费必须为正，时间必须非负
	assert(toll2 > 0 && time1 >= 0 && time2 >= 0);
};

// 构造函数：直接通过斜率和截距构造线性无差异曲线
// a: 斜率
// b: 截距
LinearIndiffCurve::LinearIndiffCurve(FPType a, FPType b) : a_(a), b_(b) {

};

// 析构函数
LinearIndiffCurve::~LinearIndiffCurve() {

};

// 计算给定收费下的时间值
// toll: 输入收费值
// 返回: 对应的时间值
FPType LinearIndiffCurve::evaluate(TollType toll) {
	return a_ * toll + b_;
};

// 将线性无差异曲线的参数转换为字符串表示
// 返回: 包含斜率和截距的字符串
std::string LinearIndiffCurve::toString() {
	std::stringstream ss;
	ss << "Linear: (" << a_ << ", " << b_ << ")";
	return ss.str();
};