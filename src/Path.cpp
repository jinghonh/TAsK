#include "Path.h"       // 包含路径类头文件
#include "StarLink.h"    // 包含星型链路头文件

#include <iostream>      // 包含输入输出流

// 构造函数：初始化路径对象
// 将流量、上一步流量和当前成本都初始化为0
Path::Path() : flow_(0.0), prevFlow_(0.0), currCost_(0.0){ 

};

// 析构函数，使用默认实现
Path::~Path()= default;

// 设置上一步流量值
// prevFlow: 上一步流量值
void Path::setPrevFlow(const FPType prevFlow){
	prevFlow_ = prevFlow;  // 保存上一步流量值
};

// 设置当前流量值
// flow: 当前流量值
void Path::setFlow(const FPType flow){
	flow_ = flow;          // 保存当前流量值
};

// 设置当前路径成本
// cost: 路径成本值
void Path::setCurrCost(const FPType cost){
	currCost_ = cost;      // 保存当前路径成本
};

// 在路径前端添加一个链路
// link: 要添加的链路指针
void Path::addLinkToPath(StarLink *link){
	links_.push_front(link);  // 在链表前端添加链路
};

// 在路径后端添加一个链路
// link: 要添加的链路指针
void Path::pushBackLink(StarLink* link) {
	links_.push_back(link);   // 在链表后端添加链路
};
		
// 获取当前路径流量
// 返回值: 当前流量
FPType Path::getFlow() const{
	return flow_;           // 返回流量值
};

// 获取当前路径成本
// 返回值: 当前路径成本
FPType Path::getCurrCost() const{
	return currCost_;       // 返回当前路径成本
};

// 获取链路列表的起始迭代器(非常量版本)
// 返回值: 链路列表起始迭代器
StarLinkIterator Path::begin() {
	return links_.begin();   // 返回链表起始迭代器
};

// 获取链路列表的结束迭代器(非常量版本)
// 返回值: 链路列表结束迭代器
StarLinkIterator Path::end() {
	return links_.end();     // 返回链表结束迭代器
};

// 获取链路列表的起始迭代器(常量版本)
// 返回值: 常量链路列表起始迭代器
ConstStarLinkIterator Path::begin() const{
	return links_.begin();   // 返回常量链表起始迭代器
};

// 获取链路列表的结束迭代器(常量版本)
// 返回值: 常量链路列表结束迭代器
ConstStarLinkIterator Path::end() const {
	return links_.end();     // 返回常量链表结束迭代器
};

// 打印路径信息，包括流量、成本和组成路径的所有链路
void Path::print(){
	std::cout << " flow = " << flow_  << " cost = " << currCost_ << std::endl;  // 打印流量和成本
	for (const auto link : *this) {  // 使用范围for循环遍历路径中的所有链路
			const StarLink* forwardLink = link->getForwardLink();  // 获取前向链路
		std::cout << forwardLink->getIndex() << " [" << forwardLink->getTime() << ", " << forwardLink->getFlow() <<
		 "] ";  // 打印链路索引、时间和流量
	}
	std::cout << std::endl;  // 换行
};

// 获取上一步路径流量
// 返回值: 上一步路径流量
FPType Path::getPrevFlow() const{
	return prevFlow_;        // 返回上一步流量
};

// 检查路径是否为空
// 返回值: 路径是否为空的布尔值
bool Path::isEmpty() const{
	return  links_.empty();  // 检查链路列表是否为空
};

// 获取路径中的链路数量
// 返回值: 链路数量
int Path::size() const{
	return links_.size();    // 返回链路列表大小
};

// 清空路径中的所有链路
void Path::clear() {
	links_.clear();          // 清空链路列表
};

// 计算路径当前时间(累加所有链路的时间)
// 返回值: 路径的总时间
FPType Path::calcCurrTime() const {
	FPType pathTime = 0.0;   // 初始化路径时间为0
	for (const auto it : *this) {  // 遍历所有链路
		pathTime += it->getTime();  // 累加链路时间
	}
	return pathTime;         // 返回总路径时间
};