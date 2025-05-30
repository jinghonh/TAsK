#include "ReverseNetworkWithTolls.h" // 包含带收费的反向网络头文件
#include "StarLinkWithTolls.h" // 包含带收费的星形链接头文件

ReverseNetworkWithTolls::ReverseNetworkWithTolls(const TollContainerType& tolls) : tolls_(tolls){ // 构造函数，初始化收费容器

};

ReverseNetworkWithTolls::~ReverseNetworkWithTolls(){ // 析构函数

};

StarLink* ReverseNetworkWithTolls::createLink(StarLink* link) { // 重写创建链接的方法
	return new StarLinkWithTolls(link, tolls_[link->getIndex()]); // 创建并返回一个新的带收费的星形链接
};