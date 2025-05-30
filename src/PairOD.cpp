#include "PairOD.h" // 包含OD对头文件

#include <iostream> // 包含输入输出流头文件

PairOD::PairOD(int index, FPType demand) : index_(index), demand_(demand),  odIndex_(-1) { // 构造函数，初始化索引、需求和OD索引

};

PairOD::~PairOD(){ // 析构函数

};

void PairOD::print(){ // 打印方法
	std::cout << "odIndex = " << odIndex_ << " destIndex = " << index_ << " demand = " 
		<< demand_  << std::endl; // 输出OD索引、目的地索引和需求
};

FPType PairOD::getDemand() const{ // 获取需求的方法
	return demand_; // 返回需求值
};

int PairOD::getIndex() const{ // 获取目的地索引的方法
	return index_; // 返回目的地索引
};
	
int PairOD::getODIndex() const{ // 获取OD对索引的方法
	return odIndex_; // 返回OD对索引
};

void PairOD::setODIndex(int index){ // 设置OD对索引的方法
	odIndex_ = index; // 设置OD对索引
};
