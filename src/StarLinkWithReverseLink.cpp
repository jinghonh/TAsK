#include "StarLinkWithReverseLink.h" // 包含带反向链接的星形链接头文件

#include <cassert> // 包含断言头文件

StarLinkWithReverseLink::StarLinkWithReverseLink(StarLink* link) : StarLink(link->getNodeTo(), 
						link->getNodeFrom(), link->getLinkFnc()->clone()), originalLink_(link) { // 构造函数，创建反向链接
	assert(link != NULL); // 断言链接不为空
};

StarLinkWithReverseLink::~StarLinkWithReverseLink() { // 析构函数

};

FPType StarLinkWithReverseLink::getTime() const { // 重写获取时间方法
	return originalLink_->getTime(); // 返回原始链接的时间
};

StarLink* StarLinkWithReverseLink::getForwardLink() { // 重写获取前向链接方法
	return originalLink_; // 返回原始链接
};