#include "ConstFnc.h"

#include <stdlib.h> // 引入标准库
#include <iostream> // 引入输入输出流库

// 构造函数，初始化常数参数
ConstFnc::ConstFnc(FPType param) : param_(param){

};

// 析构函数
ConstFnc::~ConstFnc(){

};
		
// 计算常数费用函数值，始终返回param_
FPType ConstFnc::evaluate(FPType linkFlow) const {
	return param_;
};

// 计算常数费用函数的一阶导数，恒为0
FPType ConstFnc::evaluateDerivative(FPType linkFlow) const {
	return 0.0;
};

// 打印函数类型和参数信息
void ConstFnc::print() const{
	std::cout << "ConstFnc" << std::endl; // 输出函数类型
	std::cout << "param = " << param_ << std::endl; // 输出参数
};

// 获取函数类型，返回CONSTFNC
FncType ConstFnc::getType(){
	return CONSTFNC;
};

// 同时计算函数值和导数，导数恒为0，函数值为param_
FPType ConstFnc::evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
	der = 0.0;
	return param_;
};
