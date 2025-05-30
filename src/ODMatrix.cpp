#include "ODMatrix.h" // 包含OD矩阵头文件
#include "Origin.h" // 包含起点类头文件
#include "PairOD.h" // 包含OD对头文件

#include <cassert> // 包含断言功能
#include <iostream> // 包含输入输出流
#include <sstream> // 包含字符串流

ODMatrix::ODMatrix(int nbNodes, int nbOrigins) : nbNodes_(nbNodes), 
					nbODPairs_(0), odMatrix_(nbOrigins), demandByIndex_() { // 构造函数，初始化成员变量
	assert(nbNodes > 0); // 断言节点数大于0
};

ODMatrix::~ODMatrix(){ // 析构函数
	for (std::vector<Origin*>::iterator it = odMatrix_.begin(); it != odMatrix_.end(); ++it){ // 遍历所有起点
		delete *it; // 删除起点对象
	}
};

int ODMatrix::getNbOrigins() const{ // 获取起点数量的方法
	return odMatrix_.size(); // 返回OD矩阵的大小
};

void ODMatrix::addOrigin(Origin *origin){ // 添加起点的方法
	assert(getNbOrigins() <= nbNodes_ && origin->getIndex() >= 0); // 断言起点数不超过节点数且起点索引非负
	odMatrix_[origin->getIndex()] = origin; // 将起点对象存储到矩阵中对应索引位置
};

OriginIterator ODMatrix::begin(){ // 获取起点容器的开始迭代器
	return odMatrix_.begin(); // 返回内部向量的开始迭代器
};


OriginIterator ODMatrix::end(){ // 获取起点容器的结束迭代器
	return odMatrix_.end(); // 返回内部向量的结束迭代器
};

ConstOriginIterator ODMatrix::begin() const { // 获取起点容器的常量开始迭代器
	return odMatrix_.begin(); // 返回内部向量的常量开始迭代器
};

ConstOriginIterator ODMatrix::end() const { // 获取起点容器的常量结束迭代器
	return odMatrix_.end(); // 返回内部向量的常量结束迭代器
};

void ODMatrix::print(){ // 打印OD矩阵的方法
	std::cout << "nb pairs = " << nbODPairs_ << std::endl; // 打印OD对的数量
	for (OriginIterator it = begin(); it != end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取当前起点指针
		std::cout << "Origin: " << origin->getIndex() << std::endl; // 打印起点索引
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历当前起点的所有目的地
			PairOD* dest = *jt; // 获取当前目的地指针
			dest->print(); // 打印目的地信息
		}
	};
};

void ODMatrix::setIndexes(){ // 设置索引的方法
	int count = 0; // 计数器初始化为0
	int originIndex = -1; // 起点索引初始化为-1
	//
	for (OriginIterator it = begin(); it != end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取当前起点指针
		originIndex = origin->getIndex(); // 获取起点索引
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历当前起点的所有目的地
			PairOD* dest = *jt; // 获取当前目的地指针
			dest->setODIndex(count); // 设置OD对索引
			demandByIndex_.insert(std::make_pair<std::string, FPType>( // 在哈希表中插入键值对
					createKey(originIndex, dest->getIndex()), // 键是由起点和目的地索引创建的
							  dest->getDemand())); // 值是需求量
			++count; // 增加计数器
		}
	}
	nbODPairs_ = count; // 设置OD对的总数
	std::cout << "nbODPairs = " << nbODPairs_ << std::endl; // 打印OD对的数量
};

FPType ODMatrix::getDemandByIndex(int originIndex, int destIndex) const{ // 根据索引获取需求量的方法
	std::unordered_map<std::string, FPType>::const_iterator got = demandByIndex_.find(createKey(originIndex, destIndex)); // 在哈希表中查找键
	if (got == demandByIndex_.end()) return 0.0; // 如果未找到，返回0
	return got->second; // 返回找到的需求量
};

int ODMatrix::getNbODPairs() const{ // 获取OD对数量的方法
	return nbODPairs_; // 返回OD对的数量
};

std::string ODMatrix::createKey(int originIndex, int destIndex) const{ // 创建键的方法
	std::stringstream key; // 创建字符串流
	key << originIndex << "_" << destIndex; // 将起点和目的地索引组合成键
	return key.str(); // 返回字符串
};

Origin* ODMatrix::getOriginByIndex(int index) const { // 根据索引获取起点的方法
	assert(index >= 0 && index < getNbOrigins()); // 断言索引有效
	return odMatrix_[index]; // 返回对应索引的起点
};
