#include "ReverseNetwork.h" // 包含头文件
#include "StarLinkWithReverseLink.h" // 包含带反向链接的星形链接类
#include "StarNetwork.h" // 包含星形网络类
#include "StarNode.h" // 包含星形节点类

#include <vector> // 包含向量容器
#include <list> // 包含列表容器
#include <cassert> // 包含断言
#include <iostream> // 包含输入输出流

ReverseNetwork::ReverseNetwork(){}; // 构造函数

ReverseNetwork::~ReverseNetwork(){}; // 析构函数

StarNetwork* ReverseNetwork::createReverseStar(const StarNetwork & net){ // 创建反向星形网络的方法

	std::string net_Name = net.getNetName(); // 获取网络名称
	std::string & netName = net_Name; // 创建引用

	StarNetwork* reversed = new StarNetwork(net.getNbNodes(), net.getNbLinks(), netName); // 创建新的星形网络

	std::vector<int> nodeID(net.getNbNodes(), 0); // 创建节点ID向量
	std::vector<bool> nodeIsZone(net.getNbNodes(), false); // 创建节点是否为区域的向量
	std::vector<std::list<StarLink*> > links(net.getNbNodes()); // 创建链接列表的向量

	for(StarNode* node = net.beginNode(); node != NULL; node = net.getNextNode()){ // 遍历所有节点
	 	
	 	nodeIsZone[node->getIndex()] = node->getIsZone(); // 设置节点是否为区域
		for(StarLink* link = net.beginLink(); link != NULL; link = net.getNextLink()){ // 遍历所有链接
			links[link->getNodeToIndex()].push_back(link); // 将链接添加到目标节点的链接列表
			nodeID[link->getNodeToIndex()] = link->getNodeTo(); // 设置节点ID
		}
	}

	for(int i = 0; i < net.getNbNodes(); ++i){ // 遍历所有节点索引
		if(!links[i].empty()){ // 如果节点有链接
			assert(nodeID[i] != 0); // 断言节点ID不为0
			reversed->addNode(new StarNode(nodeID[i], nodeIsZone[i])); // 添加新节点
			for (std::list<StarLink*>::iterator it = links[i].begin(); it != links[i].end(); ++it){ // 遍历节点的所有链接
				StarLink* currentLink = *it; // 获取当前链接
				StarLink* newLink = createLink(currentLink); // 创建反向链接
				reversed->addLink(newLink); // 添加链接到反向网络
			}
		}
	}

	reversed->linkNodes(); // 连接节点

	// 节点按照与原网络相同的顺序添加 ->
	// 因此，所有节点索引都相同
	assert(checkNodeIndexes(net, reversed)); // 检查节点索引

	return reversed; // 返回反向网络
};

StarLink* ReverseNetwork::createLink(StarLink* link) { // 创建链接的方法
	return new StarLinkWithReverseLink(link); // 返回带反向链接的星形链接
};

bool ReverseNetwork::checkNodeIndexes(const StarNetwork & net, StarNetwork* reversedNet) { // 检查节点索引的方法
	int nbNodes = net.getNbNodes(); // 获取节点数量
	for (int forwardNodeIndex = 0; forwardNodeIndex < nbNodes; ++forwardNodeIndex) { // 遍历所有节点索引
		int nodeID = net.findID(forwardNodeIndex); // 获取节点ID
		if (nodeID != -1) { // 如果节点存在
			int reversedIndex = reversedNet->getNodeIndex(nodeID); // 获取反向网络中的节点索引
			if (forwardNodeIndex != reversedIndex) { // 如果索引不匹配
				std::cout << "AHA--------------- forwardNodeIndex = " << forwardNodeIndex <<
						" nodeID = " << nodeID << " reversedIndex = " << reversedIndex << std::endl; // 输出调试信息
				return false; // 返回检查失败
			}
		}
	}
	return true; // 返回检查成功
};