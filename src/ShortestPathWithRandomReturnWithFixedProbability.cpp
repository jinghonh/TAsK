#include "ShortestPathWithRandomReturnWithFixedProbability.h" // 包含固定概率随机返回最短路径头文件

ShortestPathWithRandomReturnWithFixedProbability::
			ShortestPathWithRandomReturnWithFixedProbability(ShortestPath* shPath, FPType probability):
			ShortestPathWithRandomReturn(shPath), probability_(probability) { // 构造函数，初始化概率值和基类

};

ShortestPathWithRandomReturnWithFixedProbability::~ShortestPathWithRandomReturnWithFixedProbability() { // 析构函数

};

FPType ShortestPathWithRandomReturnWithFixedProbability::getProbability() const { // 获取概率方法的实现
	return probability_; // 返回固定的概率值
};