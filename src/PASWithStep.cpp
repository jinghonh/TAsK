#include "PASWithStep.h" // 包含带步长的PAS头文件
#include "Derivative.h" // 包含导数头文件
#include "StarLink.h" // 包含星型链接头文件
#include "LineSearch.h" // 包含线搜索头文件

#include <stdlib.h> // 包含标准库头文件

PASWithStep::PASWithStep(FPType zeroFlow, FPType dirTol, LineSearch* lineSearch, 
				int nbLinks) : 
				PAS(zeroFlow, dirTol), lineSearch_(lineSearch), nbLinks_(nbLinks) { // 构造函数

};

PASWithStep::~PASWithStep(){ // 析构函数

};

FPType PASWithStep::getFlowShift(){ // 获取流量移动量的方法
	FPType x[nbLinks_]; // 创建流量数组
	FPType y[nbLinks_]; // 创建方向数组
	int indexes[nbLinks_]; // 创建索引数组

	int size = 0; // 初始化大小为0
	FPType dir[2]; // 创建方向数组
	dir[cheapSegm_] = expCost_ - cheapCost_; // 廉价路段的方向是成本差异
	dir[1 - cheapSegm_] = -dir[cheapSegm_]; // 昂贵路段的方向是反向成本差异
	StarLink* link = NULL; // 初始化链接指针为空
	int linkIndex = -1; // 初始化链接索引为-1
	for (int i = 0; i < 2; ++i) { // 遍历两个路段
		for (std::list<StarLink*>::const_iterator it = segments_[i].begin();
			 it != segments_[i].end(); ++it) { // 遍历路段中的所有链接
			link = *it; // 获取当前链接
			linkIndex = link->getIndex(); // 获取链接索引
			x[linkIndex] = link->getFlow(); // 设置流量
			y[linkIndex] = dir[i]; // 设置方向
			indexes[size] = linkIndex; // 设置索引
			++size; // 增加大小
		}
	}
	(lineSearch_->getDerivative())->setDataPointers(size, x, y, indexes); // 设置导数数据指针
	return lineSearch_->execute(0.0, totalShift_ / dir[cheapSegm_]) * dir[cheapSegm_]; // 执行线搜索并返回流量移动量

};