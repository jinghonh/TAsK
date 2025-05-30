// 包含必要的头文件
#include "LinkFncContainer.h"
#include "StarNetwork.h"
#include "StarLink.h"

// 构造函数：初始化LinkFncContainer对象
LinkFncContainer::LinkFncContainer(StarNetwork *net) : links_(net->getNbLinks())
					{
	
	// 获取网络中的链接数量
	int nbLinks = net->getNbLinks();
	// 初始化所有链接函数指针为NULL
	for (int i = 0; i < nbLinks; ++i) {
		links_[i] = NULL;
	}
	// 遍历网络中的所有链接，获取每个链接的函数指针
	for (StarLink *link = net->beginOnlyLink(); link != NULL; link = net->getNextOnlyLink()){
		links_[link->getIndex()] = link->getLinkFnc();
	}
};

// 析构函数：清理LinkFncContainer对象
LinkFncContainer::~LinkFncContainer(){
};

// 返回链接函数容器的起始迭代器
LinkIterator LinkFncContainer::begin(){
	return links_.begin();
};

// 返回链接函数容器的结束迭代器
LinkIterator LinkFncContainer::end(){
	return links_.end();
};

// 根据索引获取对应的链接函数指针
LinkFnc* LinkFncContainer::getLinkFnc(int index){
	return links_[index];
};
