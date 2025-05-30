// 包含必要的头文件
#include "Derivative.h"
#include "LinkFncContainer.h"
#include "LinkFnc.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cassert>

// 构造函数：初始化导数计算器
// 参数：zeroFlow - 零流量阈值，fnc - 链接函数容器
Derivative::Derivative(FPType zeroFlow, LinkFncContainer *fnc) : 
						size_(0), x_(NULL), y_(NULL),  indexes_(NULL),
						zeroFlow_(zeroFlow), fnc_(fnc) {
	
};

// 析构函数
Derivative::~Derivative(){

};

// 计算导数值
// 参数：alpha - 步长因子
// 返回：导数值
FPType Derivative::calculate(FPType alpha) const {
	
	FPType sum = 0.0;          // 初始化求和变量
	FPType inputFlow = 0.0;    // 初始化输入流量
	LinkFnc *linkFnc = NULL;   // 链接函数指针
	int index = -1;            // 索引变量
	
	for (int i = 0; i < size_; ++i){	
		index = indexes_[i];    // 获取当前索引
		assert(index != -1);    // 确保索引有效
		inputFlow = x_[index] + alpha * y_[index];  // 计算输入流量
		if (inputFlow <= zeroFlow_) inputFlow = 0.0;  // 如果流量小于等于零流量阈值，则设为0
		linkFnc = fnc_->getLinkFnc(index);  // 获取对应的链接函数
		sum += y_[index] * linkFnc->evaluate(inputFlow);  // 累加导数值
	}
	return sum;
};

// 设置数据指针
// 参数：size - 数据大小，x - x值数组，y - y值数组，indexes - 索引数组
void Derivative::setDataPointers(int size, FPType *x, FPType *y, int *indexes){
	size_ = size;      // 设置大小
	x_ = x;            // 设置x数组指针
	y_ = y;            // 设置y数组指针
	indexes_ = indexes;  // 设置索引数组指针
};

// 获取数据大小
int Derivative::getSize() const{
	return size_;
};

// 获取指定索引的x值
FPType Derivative::getX(int index) const{
	assert((index >= 0) && (index < size_));  // 确保索引在有效范围内
	return x_[indexes_[index]];
};
		
// 获取指定索引的y值
FPType Derivative::getY(int index) const{
	assert((index >= 0) && (index < size_));  // 确保索引在有效范围内
	return y_[indexes_[index]];
};

// 获取指定索引的链接函数
LinkFnc* Derivative::getLinkFnc(int index) const{
	assert((index >= 0) && (index < size_));  // 确保索引在有效范围内
	return  fnc_->getLinkFnc(indexes_[index]);
};

// 获取指定索引的链接索引
int Derivative::getLinkIndex(int index) const{
	assert((index >= 0) && (index < size_));  // 确保索引在有效范围内
	return indexes_[index];
};

// 获取零流量阈值
FPType Derivative::getZeroFlow() const{
	return zeroFlow_;
};
