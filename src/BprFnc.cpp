#include "BprFnc.h"

#include <cassert> // 引入断言库
#include <stdio.h> // 引入标准输入输出库
#include <math.h> // 引入数学库
#include <iostream> // 引入输入输出流库
#include <iomanip> // 引入格式化输出库

// 构造函数，初始化成员变量
BprFnc::BprFnc(FPType freeFlow, FPType B, FPType capacity, FPType power) : freeFlow_(freeFlow), 
				 B_(B), capacity_(capacity), power_(power) { 
};

// 析构函数
BprFnc::~BprFnc(){

};

// 计算BPR函数值
FPType BprFnc::evaluate(FPType linkFlow) const {
	if (power_ == 0.0) // 为避免浮点数运算问题
		return freeFlow_; // 返回自由流时间
	return freeFlow_ * (1 + B_ * pow(linkFlow / capacity_, power_)); // BPR函数公式
};

// 计算BPR函数的一阶导数
FPType  BprFnc::evaluateDerivative(FPType linkFlow) const {
	if (power_ == 0.0)  // 为避免浮点数运算问题
		return 0.0; // 返回0
	return freeFlow_ * B_ * power_ * pow(linkFlow / capacity_, power_ - 1) / capacity_; // 一阶导数公式
};

// 打印BPR函数的参数信息
void BprFnc::print() const{
	std::cout << "bpr " << std::endl; // 输出bpr标识
	std::cout << "freeFlow = " << freeFlow_ << " B = " << B_ << " capacity = " << capacity_ << 
		" power_ = " << power_ << std::endl; // 输出参数信息
};

// 计算BPR函数的二阶导数
FPType BprFnc::evaluateSecondDerivative(FPType linkFlow) const {
	if (power_ - 1e-16 < 2) return 0.0;  // 为避免浮点数运算问题
	return freeFlow_ * B_ * power_ / pow(capacity_, power_) * (power_ - 1) * pow(linkFlow, power_ - 2); // 二阶导数公式
};

// 同时计算BPR函数值和一阶导数
FPType BprFnc::evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
	if (power_ == 0.0) {
		der = 0.0; // 导数为0
		return freeFlow_; // 返回自由流时间
	}
	FPType A = freeFlow_ * B_ * pow(linkFlow / capacity_, power_ - 1) / capacity_; // 计算中间变量A
	der = power_ * A; // 计算导数
	return freeFlow_ + A * linkFlow; // 返回函数值
};

// 获取容量参数
FPType BprFnc::getCapacity(){
	return capacity_;
};

// 获取函数类型
FncType BprFnc::getType(){
	return BPRFNC;
};