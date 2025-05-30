#include "StarNetworkWithTolls.h" // 包含带收费的星形网络头文件
#include "StarLinkTolls.h" // 包含带收费的星形链接头文件
#include "StarNode.h" // 包含星形节点头文件
#include "StarNetwork.h" // 包含星形网络头文件

#include <cassert> // 包含断言头文件

StarNetworkWithTolls::StarNetworkWithTolls(const TollContainerType& tolls) : tolls_(tolls){ // 构造函数，初始化收费容器

};

StarNetworkWithTolls::~StarNetworkWithTolls(){ // 析构函数

};

StarNetwork* StarNetworkWithTolls::createNetWithTolls(const StarNetwork & net){ // 创建带收费的网络方法
	
	std::string net_Name = net.getNetName(); // 获取原网络的名称
	std::string & netName = net_Name; // 创建引用
	
	StarNetwork* netWithTolls = new StarNetwork(net.getNbNodes(), net.getNbLinks(),  netName); // 创建新的星形网络	
	
	for (StarNode* node = net.beginNode(); node != NULL; node = net.getNextNode()) { // 遍历原网络的所有节点
		netWithTolls->addNode(new StarNode(node->getID(), node->getIsZone())); // 添加新节点到带收费的网络		
		for(StarLink* link = net.beginLink();link != NULL; link = net.getNextLink()) { // 遍历当前节点的所有链接
			StarLink* newLink = createLink(link); // 创建新的链接
			netWithTolls->addLink(newLink); // 添加新链接到带收费的网络
		}
	}

	netWithTolls->linkNodes(); // 链接节点
	
	for (StarNode* node = netWithTolls->beginNode(); node != NULL; node = netWithTolls->getNextNode()) { // 遍历新网络的所有节点	
		for(StarLink* link = netWithTolls->beginLink();link != NULL; link = netWithTolls->getNextLink()) { // 遍历当前节点的所有链接
			assert(link->getIndex() == link->getForwardLink()->getIndex()); // 断言链接索引等于前向链接索引
		}
	}

	return netWithTolls; // 返回带收费的网络
};


StarLink* StarNetworkWithTolls::createLink(StarLink* link) { // 创建链接方法
	return new StarLinkTolls(link, tolls_[link->getIndex()]); // 创建并返回一个新的带收费的星形链接
};
