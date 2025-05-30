#include "DAGraph.h" // 包含有向无环图头文件
#include "Error.h" // 包含错误处理头文件
#include "StarNetwork.h" // 包含星型网络头文件
#include "DAGraphNode.h" // 包含有向无环图节点头文件
#include "ODMatrix.h" // 包含OD矩阵头文件
#include "Origin.h" // 包含起点头文件
#include "PairOD.h" // 包含OD对头文件
#include "StarNode.h" // 包含星型节点头文件

#include <cassert> // 包含断言头文件
#include <math.h> // 包含数学函数头文件
#include <stdlib.h> // 包含标准库头文件
#include <iostream> // 包含输入输出流头文件
#include <limits> // 包含限制头文件

StarNetwork* DAGraph::net_ = NULL; // 初始化静态网络指针为NULL
ODMatrix* DAGraph::mat_ = NULL; // 初始化静态OD矩阵指针为NULL
bool DAGraph::wasInitialised_ = false; // 初始化静态初始化标志为false
FPType DAGraph::zeroFlow_ = 0.0; // 初始化静态零流量阈值为0.0

DAGraph::DAGraph(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, int originIndex) : nodeSize_(0), 
			originIndex_(originIndex),   linkSize_(0), clock_(1) { // 构造函数，初始化成员变量
	
	if (!wasInitialised_) initialiseStaticMembers(net, mat, zeroFlow); // 如果未初始化，则初始化静态成员
	wasInitialised_ = true; // 设置已初始化标志
	
	int nbNodes = net->getNbNodes(); // 获取节点数量
	
	nodes_ = new DAGraphNode*[nbNodes]; // 分配节点指针数组
	nodeIndexes_ = new int[nbNodes]; // 分配节点索引数组
	for (int i = 0; i < nbNodes; ++i) { // 遍历所有节点
		nodes_[i] = NULL; // 初始化节点指针为NULL
		nodeIndexes_[i] = -1; // 初始化节点索引为-1
	}
	
	int nbLinks = net->getNbLinks(); // 获取链接数量
	originFlows_ = new FPType[nbLinks]; // 分配起点流量数组
	links_ = new StarLink*[nbLinks]; // 分配链接指针数组
	
	for (int i = 0; i < nbLinks; ++i) { // 遍历所有链接
		links_[i] = NULL; // 初始化链接指针为NULL
		originFlows_[i] = 0.0; // 初始化起点流量为0.0
	}
	
	currNode_ = topOrder_.begin(); // 初始化当前节点迭代器为拓扑顺序开始
	currNodeDesc_ = topOrder_.rbegin(); // 初始化当前降序节点迭代器为拓扑顺序逆序开始
	
};

DAGraph::~DAGraph(){ // 析构函数
	for (int i = 0; i < nodeSize_; ++i) { // 遍历所有节点
		delete nodes_[nodeIndexes_[i]]; // 删除节点
	}
	delete[] nodes_; // 删除节点数组
	delete[] nodeIndexes_; // 删除节点索引数组
	delete[] links_; // 删除链接数组
	delete[] originFlows_; // 删除起点流量数组
};

void DAGraph::initialiseStaticMembers(StarNetwork *net, ODMatrix *mat, FPType zeroFlow){ // 初始化静态成员方法
	net_ = net; // 设置网络指针
	mat_ = mat; // 设置OD矩阵指针
	zeroFlow_ = zeroFlow; // 设置零流量阈值
};

FPType DAGraph::getDemand(int nodeIndex) const{ // 获取需求方法
	return (nodes_[nodeIndex])->demand; // 返回节点的需求
};

void DAGraph::setOriginFlowToZero(int linkIndex){ // 设置起点流量为零方法
	originFlows_[linkIndex] = 0.0; // 将链接的起点流量设为零
};

int DAGraph::getOriginIndex() const{ // 获取起点索引方法
	return originIndex_; // 返回起点索引
};

void DAGraph::addOriginFlowAndCreateLink(StarLink* link, FPType demand){ // 添加起点流量并创建链接方法
	assert(link != NULL); // 断言链接非空
	int index = link->getIndex(); // 获取链接索引
	if (links_[index] == NULL) { // 如果链接不存在
		addLink(link); // 添加链接
	}
	addOriginFlow(index, demand); // 添加起点流量
};

void DAGraph::addOriginFlow(int linkIndex, FPType demand){ // 添加起点流量方法
	assert(linkIndex >= 0 && linkIndex < net_->getNbLinks() && links_[linkIndex] != NULL); // 断言链接索引有效且链接存在
	originFlows_[linkIndex] += demand; // 增加链接的起点流量
};


bool DAGraph::handleBackEdge(StarLink* link){ // 处理后向边方法
	std::cout << "Back egde detected in DAG Link = " <<  link->getIndex() << " oflow = " << 
		getOriginFlow(link->getIndex()) << link->toString() << std::endl; // 打印后向边信息
	throw Error("Back egde detected in DAG"); // 抛出后向边错误
	return true; // 返回true（实际上不会执行到这里）
};

bool DAGraph::explore(int vertex, bool *visited){ // 探索节点方法（用于拓扑排序）
	
	visited[vertex] = true; // 标记节点已访问
	preVisit(vertex); // 前置访问节点
	std::list<StarLink*> &linksList = (nodes_[vertex])->outLinks; // 获取节点的出链接列表
	int index = -1; // 初始化索引为-1
	bool backEdgeDetected = false; // 初始化后向边检测标志为false
	StarLink* link = NULL; // 初始化链接指针为NULL
	for(std::list<StarLink*>::iterator it = linksList.begin(); it != linksList.end(); ++it){ // 遍历所有出链接
		link = *it; // 获取当前链接
		index = link->getNodeToIndex(); // 获取链接的终点索引
		if (checkPositiveFlow(link->getIndex())) { // 如果链接有正流量
			handleExploredLink(link); // 处理已探索链接
			if ((nodes_[index])->pre == 0) { // 如果终点未被前置访问
				backEdgeDetected = explore(index, visited); // 递归探索终点
				if (backEdgeDetected) return true; // 如果检测到后向边，返回true
			}
			if ((nodes_[index])->pre > 0 && (nodes_[index])->post == 0) { // 如果终点已前置访问但未后置访问
				return handleBackEdge(link); // 处理后向边
			}
		} 
	}
	postVisit(vertex); // 后置访问节点
	return false; // 未检测到后向边
};

void DAGraph::preVisit(int vertex){ // 前置访问节点方法
	(nodes_[vertex])->pre = clock_; // 设置节点的前置访问时间戳
	++clock_; // 增加时钟值
};

void DAGraph::postVisit(int vertex){ // 后置访问节点方法
	(nodes_[vertex])->post = clock_; // 设置节点的后置访问时间戳
	++clock_; // 增加时钟值
	topOrder_.push_front(vertex); // 将节点添加到拓扑顺序的前端
};
		
bool DAGraph::topologicalSort(){ // 拓扑排序方法

	clock_ = 1; // 重置时钟为1
	topOrder_.clear(); // 清空拓扑顺序
	int nbNodes = net_->getNbNodes(); // 获取节点数量
	bool visited[nbNodes]; // 创建访问标记数组
	for (int i = 0; i < nbNodes; ++i) { // 遍历所有节点
		visited[i] = false; // 初始化访问标记为false
	}
	int index = -1; // 初始化索引为-1
	for (int i = 0; i < nodeSize_; ++i) { // 遍历所有节点
		index = nodeIndexes_[i]; // 获取节点索引
		(nodes_[index])->pre = 0; // 重置前置访问时间戳为0
		(nodes_[index])->post = 0; // 重置后置访问时间戳为0
	}
	
	
	for (int i = 0; i < nodeSize_; ++i) { // 遍历所有节点
		index = nodeIndexes_[i]; // 获取节点索引
		if (!visited[index]) { // 如果节点未被访问
			bool tmp = explore(index, visited); // 探索节点
			if (tmp == true) { // 如果检测到后向边
				return true; // 返回true，表示图中有环
			}
		}
	}
	return false; // 返回false，表示图是无环的
	
};

void DAGraph::buildMinMaxTrees(int destIndex){ // 构建最小最大树方法
	int index = -1; // 初始化索引为-1

	for (int i = 0; i < nodeSize_; ++i) { // 遍历所有节点
		index = nodeIndexes_[i]; // 获取节点索引
		nodes_[index]->minDist = std::numeric_limits<FPType>::infinity( ); // 设置最小距离为无穷大
		nodes_[index]->maxDist = 0.0; // 设置最大距离为0
	}
	nodes_[originIndex_]->minDist = 0.0; // 将起点的最小距离设为0
	
	// 找出具有正流量的最短和最长路径
	FPType dist = 0.0; // 初始化距离为0
	FPType time = 0.0; // 初始化时间为0
	int i = beginAscPass(); // 开始升序遍历
	
	assert(originIndex_ == i); //-- TODO: 理论上可能存在
					// 一个以另一个节点开始的替代拓扑顺序 -- 
					// 在这种情况下不清楚该怎么做
	StarLink* link = NULL; // 初始化链接指针为NULL
	for (; i != -1; i = getNextAscPass()) { // 遍历所有节点（按拓扑顺序升序）
		std::list<StarLink*> &linksList = nodes_[i]->incomeLinks; // 获取节点的入链接列表
		for (std::list<StarLink*>::iterator it = linksList.begin(); it != linksList.end(); ++it) { // 遍历所有入链接
			link = *it; // 获取当前链接
			index = link->getNodeFromIndex(); // 获取链接的起点索引
			assert(nodes_[index] != NULL); // 断言起点存在
			time = link->getTime(); // 获取链接的时间
			dist = nodes_[index]->minDist + time; // 计算最小距离
			if (dist < nodes_[i]->minDist) { // 如果找到更短的路径
				nodes_[i]->minDist = dist; // 更新最小距离
				nodes_[i]->minLink = link; // 更新最小链接
			}
			dist = nodes_[index]->maxDist + time; // 计算最大距离
			if (dist >= nodes_[i]->maxDist) { // 如果找到更长的路径
				nodes_[i]->maxDist = dist; // 更新最大距离
				nodes_[i]->maxLink = link; // 更新最大链接
			}
		}
		
		if (i == destIndex) break; // 如果到达目标节点，则退出循环
	}
};

bool DAGraph::addBetterLinks(){ // 添加更好的链接方法
	bool wasImproved = false; // 初始化改进标志为false
	bool newLinkAdded = false; // 初始化新链接添加标志为false
	p2Cont_.clear(); // 清空P2容器
	for (int linkIndex = getNextNotInSet(-1); linkIndex != -1;
			linkIndex = getNextNotInSet(linkIndex)) { // 遍历所有不在集合中的链接
		StarLink *link = net_->getLink(linkIndex); // 获取链接
		if (isReachable(link) && worthAdding(link)) { // 如果链接可达且值得添加
			if (addLink(link)) newLinkAdded = true; // 添加链接，如果成功则设置新链接添加标志
			wasImproved = true; // 设置改进标志
		}
	}
	if (!wasImproved) { // 如果没有改进
		newLinkAdded = addFromP2(); // 从P2添加链接
	}

	return newLinkAdded; // 返回是否添加了新链接
};

bool DAGraph::addFromP2(){ // 从P2添加链接方法
	bool addedFromP2 = false; // 初始化从P2添加标志为false
	if (!p2Cont_.empty()) { // 如果P2容器非空
		for (std::list<int>::iterator it = p2Cont_.begin(); it != p2Cont_.end(); ++it) { // 遍历P2容器
			if (addLink(net_->getLink(*it))) addedFromP2 = true; // 添加链接，如果成功则设置从P2添加标志
		}
	}
	return addedFromP2; // 返回是否从P2添加了链接
};

bool DAGraph::worthAdding(StarLink* link){ // 判断链接是否值得添加的方法
	assert(link); // 断言链接非空
	FPType linkTime = link->getTime(); // 获取链接时间
	int nodeFromIndex = link->getNodeFromIndex(); // 获取链接的起点索引
	int nodeToIndex = link->getNodeToIndex(); // 获取链接的终点索引
	assert(nodes_[nodeFromIndex] && nodes_[nodeToIndex]); // 断言起点和终点都存在
	if ((nodes_[nodeFromIndex])->maxDist + linkTime < (nodes_[nodeToIndex])->maxDist) { // 检查P2条件
		p2Cont_.push_back(link->getIndex()); // 将链接添加到P2容器
		if (linkTime + (nodes_[nodeFromIndex])->minDist < (nodes_[nodeToIndex])->minDist) { // 检查P1条件
			return true; // 如果同时满足P1条件，则返回true
		}
	}
	return false; // 否则返回false
};

bool DAGraph::isReachable(StarLink* link) const { // 判断链接是否可达的方法
	assert(link); // 断言链接非空
	return (nodes_[link->getNodeFromIndex()] != NULL && nodes_[link->getNodeToIndex()] != NULL); // 返回起点和终点是否都存在
};

// 返回不在集合中的链接后，始终检查给定起点是否可到达两个节点！！！！
int DAGraph::getNextNotInSet(int startIndex) const{ // 获取下一个不在集合中的链接的方法
	int nbLinks = net_->getNbLinks(); // 获取链接总数
	assert(startIndex >= -1 && startIndex < nbLinks); // 断言起始索引有效
	if (startIndex == nbLinks - 1) return -1; // 如果已经是最后一个链接，则返回-1
	int index = startIndex + 1; // 初始化索引为起始索引加1
	while (links_[index] != NULL) { // 当链接存在时
		++index; // 增加索引
		if (index == nbLinks) return -1; // 如果到达链接总数，则返回-1
	}
	return index; // 返回找到的索引
};

bool DAGraph::addLink(StarLink *link){ // 添加链接的方法
	assert(link !=  NULL); // 断言链接非空
	int index = link->getIndex(); // 获取链接索引
	bool wasAdded = false; // 初始化添加标志为false
	if (links_[index] == NULL) { // 如果这个链接尚未被添加
		
		int nodeFromIndex = link->getNodeFromIndex(); // 获取链接的起点索引
		int nodeToIndex = link->getNodeToIndex(); // 获取链接的终点索引
		StarNode *nodeFrom = net_->getNodeWithLinks(nodeFromIndex); // 获取起点节点
		StarNode *nodeTo = net_->getNodeWithLinks(nodeToIndex); // 获取终点节点
		FPType demandTo = 0.0; // 初始化终点需求为0.0
		if (nodes_[nodeToIndex] == NULL) { // 如果终点节点不存在
			demandTo = mat_->getDemandByIndex(originIndex_, nodeToIndex); // 从OD矩阵获取终点需求
		} else { // 否则
			demandTo = getDemand(nodeToIndex); // 从节点获取终点需求
		}
		
		if ((nodeTo == NULL || !nodeTo->getIsZone() || demandTo > 0.0) && 
				(nodeFrom == NULL || !nodeFrom->getIsZone() 
					|| nodeFrom->getIndex() == originIndex_)) { // 如果满足添加条件
		
			links_[index] = link; // 设置链接
			wasAdded = true; // 设置添加标志
			++linkSize_; // 增加链接数量
			linkIndexes_.push_back(index); // 将索引添加到链接索引列表
			
			if (nodes_[nodeFromIndex] == NULL) { // 如果起点节点不存在
				FPType demandFrom = mat_->getDemandByIndex(originIndex_, nodeFromIndex); // 从OD矩阵获取起点需求
				DAGraphNode *newNode = new DAGraphNode(nodeFromIndex, demandFrom); // 创建新的有向无环图节点 
				nodes_[nodeFromIndex] = newNode; // 设置节点
				nodeIndexes_[nodeSize_] = nodeFromIndex; // 设置节点索引
				++nodeSize_; // 增加节点数量
			}
			if (nodes_[nodeToIndex] == NULL) { // 如果终点节点不存在
				DAGraphNode *newNode = new DAGraphNode(nodeToIndex, demandTo); // 创建新的有向无环图节点
				nodes_[nodeToIndex] = newNode; // 设置节点
				nodeIndexes_[nodeSize_] = nodeToIndex; // 设置节点索引
				++nodeSize_; // 增加节点数量
			} 
			(nodes_[nodeToIndex])->incomeLinks.push_back(link); // 将链接添加到终点节点的入链接列表
			(nodes_[nodeFromIndex])->outLinks.push_back(link); // 将链接添加到起点节点的出链接列表
		}
	}
	return wasAdded;
};

bool DAGraph::removeLink(int index){ // 移除链接的方法
	
	if (links_[index] != NULL) { // 如果链接存在
		if (fabs(originFlows_[index]) <= zeroFlow_){ // 如果链接的起点流量小于或等于零流量阈值
			int nodeToIndex = (links_[index])->getNodeToIndex(); // 获取链接的终点索引
			if ((nodes_[nodeToIndex])->incomeLinks.size() > 1) { // 如果终点节点的入链接数量大于1
				(nodes_[nodeToIndex])->incomeLinks.remove(links_[index]); // 从终点节点的入链接列表中移除链接
				(nodes_[(links_[index])->getNodeFromIndex()])->outLinks.remove(links_[index]); // 从起点节点的出链接列表中移除链接
				links_[index] = NULL; // 设置链接为NULL
				--linkSize_; // 减少链接数量
				return true; // 返回true，表示链接被成功移除
			}
		}
	}
	return false; // 返回false，表示链接未被移除
};

bool DAGraph::removeUnusedLinks(const std::list<StarLink*> &links){ // 移除未使用的链接（给定链接列表）的方法
	int index = -1; // 初始化索引为-1
	bool wasDeleted = false; // 初始化删除标志为false
	for(std::list<StarLink*>::const_iterator it = links.begin(); it != links.end(); ++it){ // 遍历所有链接
		index = (*it)->getIndex(); // 获取链接索引
		if (removeLink(index)) { // 如果成功移除链接
			wasDeleted = true; // 设置删除标志
			linkIndexes_.remove(index); // 从链接索引列表中移除索引
		}
	}
	return wasDeleted; // 返回是否删除了链接
	
};

// 目前没有维护拓扑顺序！
bool DAGraph::removeUnusedLinks(){ // 移除未使用的链接的方法
	bool wasDeleted = false; // 初始化删除标志为false
	std::list<int>::iterator it = linkIndexes_.begin(); // 初始化迭代器为链接索引列表的开始
    while (it != linkIndexes_.end()){ // 当迭代器未到达列表末尾时
        if (removeLink(*it)) { // 如果成功移除链接
            it = linkIndexes_.erase(it); // 从链接索引列表中删除当前索引并获取下一个迭代器
            wasDeleted = true; // 设置删除标志
        } else { // 否则
            ++it; // 移动到下一个索引
        }
    }
    return wasDeleted; // 返回是否删除了链接
};

std::list<StarLink*> DAGraph::getOutLinksCopy(int nodeIndex) const{ // 获取出链接副本的方法
	return (nodes_[nodeIndex])->outLinks; // 返回节点的出链接列表副本
};

std::list<StarLink*> DAGraph::getInLinksCopy(int nodeIndex) const{ // 获取入链接副本的方法
	return (nodes_[nodeIndex])->incomeLinks; // 返回节点的入链接列表副本
};

void DAGraph::getOutLinks(int nodeIndex, std::list<StarLink*>& listRef) { // 获取出链接的方法
	listRef = (nodes_[nodeIndex])->outLinks; // 将节点的出链接列表赋值给传入的引用
};

void DAGraph::getInLinks(int nodeIndex, std::list<StarLink*>& listRef) { // 获取入链接的方法
	listRef = (nodes_[nodeIndex])->incomeLinks; // 将节点的入链接列表赋值给传入的引用
};


int DAGraph::beginAscPass(){ // 开始升序遍历的方法
	if (nodeSize_ == 0) return -1; // 如果节点数量为0，则返回-1
	currNode_ = topOrder_.begin(); // 将当前节点迭代器设为拓扑顺序的开始
	if (currNode_ == topOrder_.end()) return -1; // 如果迭代器已到达末尾，则返回-1
	return *currNode_; // 返回当前节点
};

int DAGraph::getNextAscPass(){ // 获取下一个升序遍历节点的方法
	++currNode_; // 移动当前节点迭代器
	if (currNode_ == topOrder_.end()) return -1; // 如果迭代器已到达末尾，则返回-1
	return *currNode_; // 返回当前节点
};

int DAGraph::beginDescPass(){ // 开始降序遍历的方法
	if (nodeSize_ == 0) return -1; // 如果节点数量为0，则返回-1
	currNodeDesc_ = topOrder_.rbegin(); // 将当前降序节点迭代器设为拓扑顺序的反向开始
	if (currNodeDesc_ == topOrder_.rend()) return -1; // 如果迭代器已到达反向末尾，则返回-1
	return *currNodeDesc_; // 返回当前降序节点
};

int DAGraph::getNextDescPass(){ // 获取下一个降序遍历节点的方法
	++currNodeDesc_; // 移动当前降序节点迭代器
	if (currNodeDesc_ == topOrder_.rend()) return -1; // 如果迭代器已到达反向末尾，则返回-1
	return *currNodeDesc_; // 返回当前降序节点
};

void DAGraph::print() const{ // 打印有向无环图信息的方法
	int index = -1; // 初始化索引为-1
	std::cout << "ORIGIN = " << getOriginIndex() << std::endl; // 打印起点索引
	for (int i = 0; i < nodeSize_; ++i) { // 遍历所有节点
		index = nodeIndexes_[i]; // 获取节点索引
		std::cout << "Node " << (nodes_[index])->nodeIndex; // 打印节点索引
		std::cout << " in-links: "; // 打印入链接标题
		std::list<StarLink*> &links2 = (nodes_[index])->incomeLinks; // 获取节点的入链接列表引用
		for(std::list<StarLink*>::iterator it = links2.begin(); it != links2.end(); ++it){ // 遍历所有入链接
			std::cout << "link " << (*it)->getIndex() << " (" << (*it)->getNodeFromIndex() << 
				", " << (*it)->getNodeToIndex() << ") OriginFlow = " << 
				originFlows_[(*it)->getIndex()] << " "; // 打印链接信息
		}
		std::cout << std::endl; // 换行
		
	}
	
};

void DAGraph::printOriginFlow() const{ // 打印起点流量的方法
	std::cout << "Origin = " << originIndex_ << " nbLinks = " << net_->getNbLinks() << std::endl; // 打印起点索引和链接数量
	for (int i = 0; i < net_->getNbLinks(); ++i) { // 遍历所有链接
		std::cout << originFlows_[i] << " "; // 打印链接的起点流量
	}
	std::cout << std::endl; // 换行
};

void DAGraph::setOriginFlow(int index, FPType flow){ // 设置起点流量的方法
	assert(index >= 0 && index < net_->getNbLinks()); // 断言索引有效
	originFlows_[index] = flow; // 设置链接的起点流量
};

FPType DAGraph::getOriginFlow(int linkIndex) const{ // 获取起点流量的方法
	//assert(linkIndex >= 0 && linkIndex < net_->getNbLinks());
	return originFlows_[linkIndex]; // 返回链接的起点流量
};

void DAGraph::printShPath(int node){ // 打印最短路径的方法
	StarLink *prevMin = (nodes_[node])->minLink; // 获取节点的最小链接
	int prevNode = -1; // 初始化前一个节点索引为-1
	std::cout << "Cost = " << (nodes_[node])->minDist << std::endl; // 打印成本
	FPType cost = 0.0; // 初始化成本为0.0
	while (prevMin != NULL){ // 当前一个最小链接非空时
		prevNode = prevMin->getNodeFromIndex(); // 获取前一个节点索引
		cost += prevMin->getTime(); // 增加成本
		std::cout << "[" << prevMin->getNodeToIndex() << ", " << prevNode << "] " 
				<< prevMin->getFlow() << "-"; // 打印链接信息
		prevMin = (nodes_[prevNode])->minLink; // 更新前一个最小链接
	}
	std::cout << std::endl; // 换行
};

void DAGraph::printMaxShPath(int node){ // 打印最大最短路径的方法
	StarLink *prevMax = (nodes_[node])->maxLink; // 获取节点的最大链接
	int prevNode = -1; // 初始化前一个节点索引为-1
	std::cout << "Cost = " << (nodes_[node])->maxDist << std::endl; // 打印成本
	FPType cost = 0.0; // 初始化成本为0.0
	while (prevMax != NULL){ // 当前一个最大链接非空时
		prevNode = prevMax->getNodeFromIndex(); // 获取前一个节点索引
		cost += prevMax->getFlow() * prevMax->getTime(); // 增加成本
		std::cout << "[" << prevMax->getNodeToIndex() << ", " << prevNode << "] " 
					<< prevMax->getFlow() << " - "; // 打印链接信息
		prevMax = (nodes_[prevNode])->maxLink; // 更新前一个最大链接
	}
	std::cout << std::endl; // 换行
};	

DAGraphNode* DAGraph::getNode(int index) const{ // 获取节点的方法
	assert(nodes_[index] != NULL); // 断言节点非空
	return nodes_[index]; // 返回节点
};

FPType DAGraph::checkOFlowsFeasibility(){ // 检查起点流量可行性的方法
	int nbNodes = net_->getNbNodes(); // 获取节点数量
	FPType total[nbNodes]; // 创建总流量数组
	for (int i = 0; i < nbNodes; ++i) { // 遍历所有节点
		total[i] = 0.0; // 初始化总流量为0.0
	}
	// 加载需求
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取当前起点
		if (origin->getIndex() == originIndex_){ // 如果是当前起点
			for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历所有OD对
				PairOD* dest = *jt; // 获取当前目的地
				FPType demand = dest->getDemand(); // 获取需求
				total[origin->getIndex()] += demand; // 增加起点的总流量
				total[dest->getIndex()] -= demand; // 减少目的地的总流量
			}
			break; // 找到当前起点后退出循环
		}
	}
	//遍历网络并检查
	
	int i = -1; // 初始化索引为-1
	StarLink* link = NULL; // 初始化链接指针为NULL
	std::list<StarLink*> inLinks; // 创建入链接列表
	for (int j = 0; j < nodeSize_; ++j) { // 遍历所有节点
    	i = nodeIndexes_[j]; // 获取节点索引
    	getInLinks(i, inLinks); // 获取节点的入链接
    	for (std::list<StarLink*>::iterator it = inLinks.begin(); it != inLinks.end(); ++it){ // 遍历所有入链接
      		link = *it; // 获取当前链接
			total[link->getNodeFromIndex()] -= getOriginFlow(link->getIndex()); // 减少链接起点的总流量
			total[link->getNodeToIndex()] += getOriginFlow(link->getIndex()); // 增加链接终点的总流量
		}
	
	}
	
	FPType max = 0.0; // 初始化最大值为0.0
		
	for (int i = 0; i < net_->getNbNodes(); ++i) { // 遍历所有节点
		if (fabs(total[i]) > max) {	// 如果节点的总流量绝对值大于最大值
			max = fabs(total[i]); // 更新最大值
		}
	}
	return max; // 返回最大值
};

FPType DAGraph::getNbLinksInBush() const{ // 获取有向无环图中链接数量的方法
	return linkSize_; // 返回链接数量
};
