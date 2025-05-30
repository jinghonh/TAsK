#include "StepSize.h" // 包含步长头文件
#include "LineSearch.h" // 包含线搜索头文件
#include "DescDirection.h" // 包含下降方向头文件

StepSize::StepSize(LineSearch *lineSearch) : lineSearch_(lineSearch) { // 构造函数，初始化线搜索对象
	
};

StepSize::~StepSize(){ // 析构函数
	
};

FPType StepSize::getStepSize(DescDirection *algo){ // 获取步长的方法
	initialiseDerivative(algo); // 初始化导数
	return  lineSearch_->execute(0.0, algo->getUpperBound()); // 执行线搜索并返回结果
};

