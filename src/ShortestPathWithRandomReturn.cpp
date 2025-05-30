#include "ShortestPathWithRandomReturn.h" // 包含带随机返回的最短路径头文件
#include "Utils.h" // 包含工具类头文件

#include <time.h> // 包含时间相关头文件
#include <stdlib.h> // 包含标准库头文件
#include <limits> // 包含数值限制头文件
#include <cassert> // 包含断言头文件

ShortestPathWithRandomReturn::ShortestPathWithRandomReturn(ShortestPath* shPath) : 
								shPath_(shPath), returnInf_(false), nbIters_(1) { // 构造函数，初始化成员变量
	srand(time(NULL)); // 初始化随机数生成器
};

ShortestPathWithRandomReturn::~ShortestPathWithRandomReturn() { // 析构函数
};

void ShortestPathWithRandomReturn::calculate(int originIndex) { // 实现计算方法
	returnInf_ = false; // 设置returnInf_为false
	shPath_->calculate(originIndex); // 调用内部最短路径算法的计算方法
	if (originIndex == 0) ++nbIters_; // 如果起点索引为0，迭代次数加1
}; 

FPType ShortestPathWithRandomReturn::getProbability() const { // 获取概率方法的实现
	return 0.1 + 1.0/nbIters_; // 返回当前迭代的概率值
}; 

void ShortestPathWithRandomReturn::calculate(int originIndex, int destIndex, int odPairIndex) { // 实现点对点计算方法
	if (Utils::generateRndNumber(1.0) < getProbability()) { // 如果生成的随机数小于概率值
		shPath_->calculate(originIndex, destIndex, odPairIndex); // 调用内部最短路径算法的点对点计算方法
		returnInf_ = false; // 设置returnInf_为false
	} else {
		returnInf_ = true; // 否则设置returnInf_为true
	}
};

FPType ShortestPathWithRandomReturn::getCost(int destIndex) const { // 获取成本方法的实现
	if (returnInf_) return std::numeric_limits<FPType>::infinity(); // 如果returnInf_为true，返回无穷大
	return shPath_->getCost(destIndex); // 否则返回内部最短路径算法的成本
};

StarLink* ShortestPathWithRandomReturn::getInComeLink(int destIndex) const { // 获取入边方法的实现
	assert(returnInf_ == false); // 断言returnInf_为false
	return shPath_->getInComeLink(destIndex); // 返回内部最短路径算法的入边
};