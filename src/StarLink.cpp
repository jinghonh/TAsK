#include "StarLink.h" // 包含星形链接头文件

#include <sstream> // 包含字符串流头文件

StarLink::StarLink(int nodeFrom, int nodeTo, LinkFnc *fnc) : index_(-1),  
				  nodeFrom_(nodeFrom), nodeTo_(nodeTo),
				  nodeFromIndex_(-1), nodeToIndex_(-1), flow_(0.0), fnc_(fnc), der_(0.0) {  // 构造函数，初始化所有成员变量
	updateTime(); // 更新时间
};

StarLink::~StarLink(){ // 析构函数
	delete fnc_; // 释放链接函数对象
	fnc_ = NULL; // 设置指针为NULL
};

int StarLink::getNodeFrom() const{ // 获取起始节点ID的方法
	return nodeFrom_; // 返回起始节点ID
};

int StarLink::getNodeTo() const{ // 获取目标节点ID的方法
	return nodeTo_; // 返回目标节点ID
};

LinkFnc* StarLink::getLinkFnc() const{ // 获取链接函数对象的方法
	return fnc_; // 返回链接函数对象
};

FPType StarLink::getFlow() const{ // 获取流量的方法
	return flow_; // 返回当前流量
};

void StarLink::setFlow(FPType flow){ // 设置流量的方法
	flow_ = flow; // 设置流量值
};

int StarLink::getIndex() const{ // 获取索引的方法
	return index_; // 返回索引
};

void StarLink::setIndex(int index){ // 设置索引的方法
	index_ = index; // 设置索引值
};

int StarLink::getNodeFromIndex() const{ // 获取起始节点内部索引的方法
	return nodeFromIndex_; // 返回起始节点内部索引
};

int StarLink::getNodeToIndex() const{ // 获取目标节点内部索引的方法
	return nodeToIndex_; // 返回目标节点内部索引
};

FPType StarLink::getTime() const{ // 获取时间的方法
	return time_; // 返回时间
};
		
void StarLink::setNodeToIndex(int index){ // 设置目标节点内部索引的方法
	nodeToIndex_ = index; // 设置目标节点内部索引
};

void StarLink::setNodeFromIndex(int index){ // 设置起始节点内部索引的方法
	nodeFromIndex_ = index; // 设置起始节点内部索引
};

void StarLink::addFlow(FPType flow){ // 添加流量的方法
	flow_ += flow; // 增加流量
};

void StarLink::updateTime(){ // 更新时间的方法
	time_ = fnc_->evaluateTimeAndDerivative(flow_, der_);  // 评估时间和导数
};

void StarLink::setFlowToZero(){ // 将流量设置为零的方法
	flow_ = 0.0; // 设置流量为0
};

std::string StarLink::toString() const{ // 转换为字符串的方法
	std::stringstream ss; // 创建字符串流
	ss << "Link: " << index_ << " [" << nodeFromIndex_ << ", " << nodeToIndex_ << "]" 
		<<	" [" << nodeFrom_ << ", " << nodeTo_ << "]"; // 构造字符串
	return ss.str(); // 返回字符串表示
};

FPType StarLink::getDerivative() const{ // 获取导数的方法
	return der_; // 返回导数
};

StarLink* StarLink::getForwardLink() { // 获取前向链接的方法
	return this; // 返回自身
};
