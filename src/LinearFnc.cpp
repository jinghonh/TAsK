// 包含必要的头文件
#include "LinearFnc.h"
#include <stdlib.h>
#include <iostream>

// 构造函数，初始化线性函数的参数a和b
LinearFnc::LinearFnc(FPType a, FPType b) : _param1(a), _param2(b){

};

// 析构函数
LinearFnc::~LinearFnc(){

};

// 计算线性函数值：f(x) = ax + b
FPType LinearFnc::evaluate(FPType arcFlow) const {
	return _param1 * arcFlow + _param2;
};

// 计算线性函数的导数：f'(x) = a
FPType  LinearFnc::evaluateDerivative(FPType arcFlow) const {
	return _param1;
};

// 打印函数参数a和b的值
void LinearFnc::print() const{
	std::cout << "a = " << _param1 << " b = " << _param2 << std::endl;
};

// 获取函数类型，返回线性函数类型
FncType LinearFnc::getType(){
	return LINEARFNC;
};

// 同时计算函数值和导数
FPType LinearFnc::evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
	der = _param1;  // 设置导数值
	return evaluate(linkFlow);  // 返回函数值
};
