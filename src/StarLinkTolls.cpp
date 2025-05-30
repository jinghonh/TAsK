#include "StarLinkTolls.h" // 包含带收费的星形链接头文件

StarLinkTolls::StarLinkTolls(StarLink* link, TollType toll) : StarLink(link->getNodeFrom(), 
						link->getNodeTo(), link->getLinkFnc()->clone()), toll_(toll),
						originalLink_(link){ // 构造函数，初始化基类和成员变量
};

StarLinkTolls::~StarLinkTolls() { // 析构函数

};

FPType StarLinkTolls::getTime() const { // 重写获取时间方法
	return toll_; // 返回收费而不是时间
};

StarLink* StarLinkTolls::getForwardLink() { // 重写获取前向链接方法
	return originalLink_; // 返回原始链接
};
