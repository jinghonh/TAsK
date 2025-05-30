#include "StarNetwork.h" // 包含StarNetwork头文件
#include "Error.h" // 包含Error头文件
#include "FileReader.h" // 包含FileReader头文件
#include "StarLink.h" // 包含StarLink头文件
#include "StarNode.h" // 包含StarNode头文件
#include "FileWriter.h" // 包含FileWriter头文件

#include <cassert> // 包含断言
#include <stdlib.h> // 包含标准库函数
#include <iostream> // 包含输入输出流
#include <fstream> // 包含文件流
#include <sstream> // 包含字符串流
#include <iomanip> // 包含输入输出操作格式控制
#include <math.h> // 包含数学函数
#include <algorithm> // 包含算法库

StarNetwork::StarNetwork(int nbNodes, int nbLinks, std::string &netName) : 
							netName_(netName), nbNodes_(nbNodes), nbLinks_(nbLinks),
							nodes_(nbNodes), links_(nbLinks), pointers_(nbNodes + 1), 
							size_(0), sizeLinks_(0), curNode_(-1), curLink_(-1), curOnlyLink_(-1),
							 linkAdded_(true){ // 构造函数，初始化网络参数
	for (int i = 0; i < nbNodes; ++i) { 
		nodes_[i] = nullptr; // 初始化节点指针为空
		pointers_[i] = -1; // 初始化指针数组为-1
	}
	pointers_[nbNodes] = -1; // 设置最后一个指针为-1
	
	for (int i = 0; i < nbLinks; ++i) {
		links_[i] = nullptr; // 初始化链接指针为空
		
	}
	std::cout << "link0 = " << links_[0] << std::endl; // 输出第一个链接的地址

};

StarNetwork::~StarNetwork(){ // 析构函数
	for (int i = 0; i < size_; ++i) delete nodes_[i]; // 删除所有节点
	for (int i = 0; i < nbLinks_; ++i) delete links_[i]; // 删除所有链接

};

void StarNetwork::linkNodes(){ // 连接节点函数
	assert(size_ > 0); // 断言节点数大于0
	if (pointers_[size_ - 1] == sizeLinks_) 
		throw Error("Last added node does not have out-going links"); // 如果最后添加的节点没有出链，抛出错误
	createIndexes(); // 创建索引
	bool nodeToFound = false; // 初始化目标节点查找标志
	for (StarLink *link = beginOnlyLink(); link != nullptr; link = getNextOnlyLink())  { // 遍历所有链接
		nodeToFound = false; // 重置目标节点查找标志
		for (StarNode *node = beginNode(); node != nullptr; node = getNextNode()) { // 遍历所有节点
			if (link->getNodeTo() == node->getID()){ // 如果链接的目标节点ID等于当前节点ID
				link->setNodeToIndex(node->getIndex()); // 设置链接的目标节点索引
				nodeToFound = true; // 标记已找到目标节点
				break; // 跳出循环
			}	
		}
		if (!nodeToFound){ // 如果没有找到目标节点
			link->setNodeToIndex(getNodeIndex(link->getNodeTo())); // 通过ID获取节点索引并设置
		}
	}
};
void StarNetwork::createIndexes(){ // 创建索引函数
	std::unordered_map<int, int>::const_iterator got; // 声明哈希表迭代器
	int id = -1; // 初始化ID变量
	int count = size_; // 初始化计数器为当前节点数
	for (StarLink *link = beginOnlyLink(); link != nullptr; link = getNextOnlyLink()){ // 遍历所有链接
		id = link->getNodeTo(); // 只考虑目标节点，因为所有有出链的节点都在构造过程中添加了
		got = idMap_.find(id); // 在ID映射中查找
		if (got == idMap_.end()) { // 如果没有找到
			idMap_.insert(std::make_pair(id, count)); // 将ID和索引插入映射
			++count; // 增加计数器
			assert(count <= nbLinks_); // 断言计数器不超过链接总数
		}
	}
};

void StarNetwork::addNode(StarNode *node){ // 添加节点函数
	if (size_ >= nbNodes_) throw 
			Error("Attemp to add a node when the number of nodes exceeds the total number of nodes"); // 如果节点数超过总数，抛出错误
	if (!linkAdded_)
		throw Error("Two nodes were added in a row. Only nodes with out-going links can be added"); // 如果上一次没有添加链接，抛出错误
	node->setIndex(size_); // 设置节点索引
	nodes_[size_] = node; // 存储节点指针
	idMap_.insert(std::make_pair(node->getID(), size_)); // 将节点ID和索引插入映射
	pointers_[size_] = sizeLinks_; // 设置当前节点的指针
	++size_; // 增加节点计数
	pointers_[size_] = nbLinks_; // 设置下一个节点的指针
	linkAdded_ = false; // 标记链接未添加
};

void StarNetwork::addLink(StarLink *link){ // 添加链接函数
	if (sizeLinks_ >= nbLinks_) throw 
		Error("Attemp to add a link when the number of links exceeds the total number of links"); // 如果链接数超过总数，抛出错误
	if (size_ == 0) throw Error("Node must be added first"); // 如果没有节点，抛出错误
	links_[sizeLinks_] = link; // 存储链接指针
	link->setIndex(sizeLinks_); // 设置链接索引
	link->setNodeFromIndex(size_ - 1); // 设置链接的起始节点索引
	++sizeLinks_; // 增加链接计数
	linkAdded_ = true; // 标记链接已添加
};
		
std::string StarNetwork::getNetName() const { // 获取网络名称
	return netName_; // 返回网络名称
};

int StarNetwork::getNbNodes() const { // 获取节点总数
	return nbNodes_; // 返回节点总数
};

int StarNetwork::getNbLinks() const { // 获取链接总数
	return nbLinks_; // 返回链接总数
};

StarNode* StarNetwork::beginNode() const { // 获取第一个节点
	curNode_ = 0; // 设置当前节点索引为0
	curLink_ = pointers_[curNode_]; // 设置当前链接索引
	return nodes_[curNode_]; // 返回第一个节点
};

StarLink* StarNetwork::getLink(int linkIndex) const{ // 通过索引获取链接
	assert((linkIndex >= 0) && (linkIndex < nbLinks_)); // 断言索引有效
	return links_[linkIndex]; // 返回指定索引的链接
};

StarNode* StarNetwork::beginNode(int index) const { // 从指定索引开始获取节点
	assert((index >= 0) && (index < nbNodes_)); // 断言索引有效
	if (index >= size_) return nullptr; // 如果索引超过当前节点数，返回空
	curNode_ = index; // 设置当前节点索引
	curLink_ = pointers_[curNode_]; // 设置当前链接索引
	return nodes_[curNode_]; // 返回指定索引的节点
};

StarNode* StarNetwork::getNextNode() const{ // 获取下一个节点
	++curNode_; // 增加当前节点索引
	if (curNode_ == size_) { // 如果到达节点末尾
		curLink_ = -1; // 重置当前链接索引
		return nullptr; // 返回空
	}
	curLink_ = pointers_[curNode_]; // 设置当前链接索引
	return nodes_[curNode_]; // 返回下一个节点
};

StarLink* StarNetwork::beginLink() const{ // 获取当前节点的第一个链接
	return links_[curLink_]; // 返回当前链接索引的链接
};

StarLink* StarNetwork::getNextLink() const{ // 获取当前节点的下一个链接
	++curLink_; // 增加当前链接索引
	if (curLink_ == pointers_[curNode_ + 1]) { // 如果到达当前节点的链接末尾
		return nullptr; // 返回空
	}
	return links_[curLink_]; // 返回下一个链接
};

StarLink* StarNetwork::beginOnlyLink() const { // 获取第一个链接（不考虑节点）
	curOnlyLink_ = 0; // 设置当前链接索引为0
	return links_[0]; // 返回第一个链接
};

StarLink* StarNetwork::getNextOnlyLink() const { // 获取下一个链接（不考虑节点）
	++curOnlyLink_; // 增加当前链接索引
	if (curOnlyLink_ == nbLinks_) return nullptr; // 如果到达链接末尾，返回空
	return links_[curOnlyLink_]; // 返回下一个链接
};
		
void StarNetwork::print(){ // 打印网络信息
	std::cout << "network name: " << getNetName() << " nbNodes = " << nbNodes_ 
			  << " nbLinks = " << nbLinks_	 << std::endl; // 输出网络名称、节点数和链接数
	for (StarNode *node = beginNode(); node != nullptr; node = getNextNode()){ // 遍历所有节点
	  	std::cout << "Node ID = " << node->getID()  << " node index = " << node->getIndex() 
	  			  << std::endl; // 输出节点ID和索引
		for (StarLink *link = beginLink(); link != nullptr; link = getNextLink()) { // 遍历当前节点的所有链接
		  std::cout << " link " << link->getIndex() << "  (" << link->getNodeFrom() << ", " << 
		  	link->getNodeTo() << ") cost-> (" << link->getTime() << ") flow = " 
			<< link->getFlow() << std::endl; // 输出链接信息
		}
	}
};

int StarNetwork::getNodeIndex(int id){ // 通过ID获取节点索引
	std::unordered_map<int, int>::const_iterator got = idMap_.find(id); // 在ID映射中查找
	if (got == idMap_.end()) { // 如果没有找到
		std::stringstream str; // 创建字符串流
		str << "Cannot find id = " << id; // 构建错误消息
		throw Error(str.str()); // 抛出错误
	}
	return got->second; // 返回节点索引
};

void StarNetwork::printToFile(const std::string &fileName, int precision){ // 将网络信息打印到文件
	
	FileWriter fw(fileName); // 创建文件写入器
	std::string line; // 声明行字符串
	std::ostringstream strs; // 创建输出字符串流
					
	for (StarLink *link = beginOnlyLink(); link != nullptr; link = getNextOnlyLink()) { // 遍历所有链接
		strs.str(""); // 清空字符串流
		strs << link->getNodeFrom() << " " << link->getNodeTo() << " " << 
			 std::setprecision(precision) << 
		link->getFlow() << " " << link->getTime() << "\n"; // 构建链接信息行
		line = strs.str(); // 获取字符串
		fw.writeLine(line); // 写入行到文件
	}	
	
};

bool StarNetwork::assignLinkFlow(const int nodeFrom, const int nodeTo, const FPType flow) const { // 分配链接流量
	for (StarLink *link = beginOnlyLink(); link != nullptr; link = getNextOnlyLink()) { // 遍历所有链接
		if ((link->getNodeFrom() == nodeFrom) && (link->getNodeTo() == nodeTo)){ // 如果找到匹配的链接
			link->setFlow(flow); // 设置流量
			return true; // 返回成功
		}
	}
	return false; // 未找到匹配的链接，返回失败
};

void StarNetwork::loadFromFile(const std::string& fileName){ // 从文件加载网络信息
	FileReader myfile(fileName); // 创建文件读取器
	int nodeFrom = -1; // 初始化起始节点ID
	int nodeTo = 1; // 初始化目标节点ID
	FPType flow = 0.0; // 初始化流量
	FPType tmp = 0.0; // 初始化临时变量
	while ( myfile.isGood() ){ // 当文件状态良好时循环
      	std::string line = myfile.getNextLine(); // 读取下一行
      	std::stringstream ss(line); // 创建字符串流
      	ss >> nodeFrom >> nodeTo >> flow >> tmp; // 从行中提取数据
      	if (!assignLinkFlow(nodeFrom, nodeTo, flow)) // 尝试分配链接流量
      		throw Error("This link does not exist in the network"); // 如果链接不存在，抛出错误
    }
};

void StarNetwork::calculateLinkCosts(){ // 计算链接成本
	for (StarLink *link = beginOnlyLink(); link != nullptr; link = getNextOnlyLink()) { // 遍历所有链接
		link->updateTime(); // 更新链接时间
	}
};

StarNode* StarNetwork::getNodeWithLinks(int index){ // 获取有链接的节点
	assert(index >= 0); // 断言索引非负
	if (index >= size_) return nullptr; // 如果索引超过当前节点数，返回空（表示该节点没有出链）
	return nodes_[index]; // 返回指定索引的节点
};


int StarNetwork::findID(int index) const { // 通过索引查找ID
	for(std::unordered_map<int, int>::const_iterator it = idMap_.begin();
			it != idMap_.end(); ++it){ // 遍历ID映射
		if (it->second == index) { // 如果找到匹配的索引
			return it->first; // 返回对应的ID
		}
	}
	return -1; // 未找到匹配的索引，返回-1
};
