#include "PASManager.h" // 包含PAS管理器头文件
#include "DAGraphTapas.h" // 包含TAPAS有向无环图头文件
#include "PAS.h" // 包含PAS（备选路段对）头文件
#include "ShortestPath.h" // 包含最短路径头文件

#include <cassert> // 包含断言头文件
#include <math.h> // 包含数学函数头文件
#include <iostream> // 包含输入输出流头文件
#include <stdlib.h> // 包含标准库头文件

PASManager::PASManager(const ShortestPath &shPath, FPType dirTol, int nbNodes, FPType mu, FPType v, 
						FPType zeroFlow) : 
						zeroFlow_(zeroFlow), dirTol_(dirTol), shPath_(shPath), 
						listOfPas_(),  nbNodes_(nbNodes), 
						mu_(mu), v_(v), setOfNodes_(), nbIter_(1) { // 构造函数，初始化所有成员变量
	srand (time(NULL)); // 初始化随机数生成器

}; 

PASManager::~PASManager() { // 析构函数
	for (std::list<PAS*>::iterator it = listOfPas_.begin(); it != listOfPas_.end(); ++it){ // 遍历所有PAS
		delete *it; // 删除PAS
	}
}; 

void PASManager::recalculatePASCosts(){ // 重新计算所有PAS成本的方法
	for (CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){ // 遍历所有PAS
		(*pasIt)->recalcPASCosts(); // 重新计算当前PAS的成本
	}
};

PAS* PASManager::pasExist(StarLink* cheapLink, StarLink* expLink){ // 检查是否存在特定PAS的方法
	assert(cheapLink != NULL && expLink != NULL); // 断言参数非空
	for (CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){ // 遍历所有PAS
		PAS* pas = *pasIt; // 获取当前PAS
		if (pas->getLastExpLink() == expLink && pas->getLastCheapLink() == cheapLink) { // 如果找到匹配的PAS
			return pas; // 返回找到的PAS
		}
	}
	return NULL; // 如果未找到，返回NULL
}

bool PASManager::nodeBelongsToShPath(int nodeIndex){ // 检查节点是否属于最短路径的方法
	return (setOfNodes_.find(nodeIndex) != setOfNodes_.end()); // 检查节点是否在节点集合中
};

void PASManager::createExpSegment(PAS *pas, const std::vector<StarLink*> &checkedLinks,
								  int divergeNode, int mergingNodeIndex){ // 创建昂贵路段的方法
	int size = checkedLinks.size(); // 获取检查链接的数量
	assert(divergeNode >= 0 && divergeNode <  size && mergingNodeIndex >= 0 &&
		   mergingNodeIndex < size); // 断言参数有效
	int start = divergeNode; // 设置起始节点为分歧节点
	StarLink* link = NULL; // 初始化链接指针为空
	while (start != mergingNodeIndex){ // 直到到达合并节点
		link = checkedLinks[start]; // 获取当前节点的链接
		pas->pushBackToExp(link); // 将链接添加到昂贵路段
		start = link->getNodeToIndex(); // 更新当前节点为链接的终点
	}
};

void PASManager::createCheapSegment(PAS *pas, int divergeNode, int mergingNodeIndex){ // 创建廉价路段的方法
	StarLink *link = shPath_.getInComeLink(mergingNodeIndex); // 获取合并节点的入链接
	int nextDest = link->getNodeFromIndex(); // 获取链接的起点
	while (link != NULL) { // 直到没有更多链接
		pas->pushFrontToCheap(link); // 将链接添加到廉价路段的前端
		nextDest = link->getNodeFromIndex(); // 更新目的地为链接的起点
		if (nextDest == divergeNode) return; // 如果到达分歧节点，则返回
		link = shPath_.getInComeLink(nextDest); // 获取下一个入链接
	}
};

void PASManager::prepareShPathStructure(int mergingNodeIndex){ // 准备最短路径结构的方法
 	setOfNodes_.clear(); // 清空节点集合
 	StarLink *link = shPath_.getInComeLink(mergingNodeIndex); // 获取合并节点的入链接
	int nextDest = link->getNodeFromIndex(); // 获取链接的起点
	while (true) { // 无限循环
		setOfNodes_.insert(nextDest); // 将节点添加到集合
		link = shPath_.getInComeLink(nextDest); // 获取下一个入链接
		if (link == NULL) break; // 如果没有更多链接，则退出循环
		nextDest = link->getNodeFromIndex(); // 更新目的地为链接的起点
	}
};

PAS* PASManager::allocatePAS(){ // 分配PAS的方法
	return new PAS(zeroFlow_, dirTol_); // 创建并返回新PAS
};

PAS* PASManager::createPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex, 
							bool (*checkPtr)(FPType originFlow, FPType thr)){ // 创建PAS的方法
	// BFS的数据
	std::list<std::pair<int, StarLink*> > bfsQueue; // BFS队列
	bfsQueue.push_back(std::make_pair(expLink->getNodeFromIndex(), expLink)); // 将起点和昂贵链接添加到队列
	std::vector<int> nodeState(nbNodes_, -1); // -1 - 节点未被调查
	std::vector<StarLink*> checkedLinks(nbNodes_); // 检查链接向量
	for (int i = 0; i < nbNodes_; ++i) // 遍历所有节点
		checkedLinks[i] = NULL; // 初始化检查链接为NULL
	
	prepareShPathStructure(mergingNodeIndex); // 准备最短路径结构
	std::list<StarLink*> inLinks; // 入链接列表
	StarLink* link = NULL; // 初始化链接指针为空
	int divergeNode = -1; // 初始化分歧节点为-1
	FPType thr = v_ * dag->getOriginFlow(expLink->getIndex()); // 计算阈值
	bool pasCreated = true; // 初始化PAS创建标志为true
	bool canStop = false; // 初始化停止标志为false
	while (!canStop) { // 直到可以停止
		// 从队列中弹出第一个元素进行调查
		if (bfsQueue.empty()) { // 如果队列为空
			pasCreated = false; // 有时无法创建PAS
			break; // 退出循环
		}
		std::pair<int, StarLink*> firstInQueue(bfsQueue.front()); // 获取队列中的第一个元素
	
		// 将弹出的元素复制到checkedLinks
		checkedLinks[firstInQueue.first] = firstInQueue.second; // 存储链接
		bfsQueue.pop_front(); // 从队列中移除元素
			
		// 检查是否可以停止
		if (nodeBelongsToShPath(firstInQueue.first)) { // 如果节点属于最短路径
			divergeNode = firstInQueue.first; // 设置分歧节点
			break; // 退出循环
		}
		
		// 将已使用链接的新节点添加到队列
		dag->getInLinks(firstInQueue.first, inLinks); // 获取节点的入链接
		for (std::list<StarLink*>::iterator it = inLinks.begin(); it != inLinks.end(); ++it){ // 遍历所有入链接
     		link = *it; // 获取当前链接
     		FPType originFlow = dag->getOriginFlow(link->getIndex()); // 获取链接的起点流量
     		if (checkPtr(originFlow, thr)) { // 如果流量满足条件
	     		int nodeFromIndex = link->getNodeFromIndex(); // 获取链接的起点索引
	     		if (originFlow > zeroFlow_ && nodeState[nodeFromIndex] == -1) { // 如果流量大于零且节点未被访问
					bfsQueue.push_back(std::make_pair(nodeFromIndex, link)); // 将节点添加到队列
					// 更新节点状态
					nodeState[nodeFromIndex] = 1; // 节点在队列中
					// 将弹出的元素复制到checkedLinks
					checkedLinks[nodeFromIndex] = link; // 存储链接
				
					// 检查是否可以停止
					if (nodeBelongsToShPath(nodeFromIndex)) { // 如果节点属于最短路径
						// 这个检查是多余的。TODO: 考虑如何改进
						divergeNode = nodeFromIndex; // 设置分歧节点
						canStop = true; // 设置可以停止
						break; // 退出循环
					}
				}
			}
		}
		
		// 更新节点状态
		nodeState[firstInQueue.first] = 0; // 节点已访问
	
	}
	
	if (pasCreated){ // 如果可以创建PAS	
		PAS* pas = allocatePAS(); // 分配PAS
	 	createExpSegment(pas, checkedLinks, divergeNode, mergingNodeIndex); // 创建昂贵路段
		createCheapSegment(pas, divergeNode, mergingNodeIndex); // 创建廉价路段
		if (pas->getCostDiff() < dirTol_) { // 如果成本差异小于方向容差
			delete pas; // 删除PAS
			pas = NULL; // 设置指针为NULL
		} else { // 否则

			pas->addOrigin(dag); // 将菊花添加到PAS的相关起点集合
			listOfPas_.push_back(pas); // 将PAS添加到列表

		}
		return pas; // 返回创建的PAS
		
	}

	return NULL; // 如果未创建PAS，返回NULL
};

FPType PASManager::calculateReducedCost(StarLink *expLink){ // 计算降低成本的方法
	return shPath_.getCost(expLink->getNodeFromIndex()) + expLink->getTime()
						   - shPath_.getCost(expLink->getNodeToIndex()); // 返回降低成本
};

FPType PASManager::calcThreshold(){ // 计算阈值的方法
	return 10.0 * pow(10, -nbIter_); // 返回阈值
}; 

bool checkThreshold(FPType originFlow, FPType thr) {return (originFlow >= thr);}; // 检查阈值的函数
		
bool checkThresholdFake(FPType originFlow, FPType thr) {return true;}; // 假检查阈值的函数，总是返回true
		
void PASManager::createNewPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex) { // 创建新PAS的方法
	PAS* foundPAS = pasExist(shPath_.getInComeLink(mergingNodeIndex), expLink); // 检查是否存在PAS
	bool isEffective = false; // 初始化有效标志为false
	FPType reducedCost = calculateReducedCost(expLink); // 计算降低成本
	int expIndex = expLink->getIndex(); // 获取昂贵链接索引
	bool (*fncPtr)(FPType originFlow, FPType thr); // 声明函数指针
	FPType redVal = mu_ * reducedCost; // 计算降低值
	if (foundPAS != NULL) { // 如果找到PAS
		foundPAS->addOrigin(dag); // 将菊花添加到PAS
		isEffective = foundPAS->checkIfEffective(redVal, v_, expIndex, dag); // 检查PAS是否有效
	} else { // 我们需要创建新的PAS并将其添加到listOfPas_
		fncPtr = &checkThresholdFake; // 使用假检查阈值函数	
		PAS* pasTmp = createPAS(dag, expLink, mergingNodeIndex, fncPtr); // 创建临时PAS
		if (pasTmp != NULL) { // 如果成功创建
			isEffective = pasTmp->checkIfEffective(redVal, v_, expIndex, dag); // 检查PAS是否有效
		} 
	}
		
	if (reducedCost > calcThreshold() && !isEffective) { // 检查链接expLink是否是问题OLC
		fncPtr = &checkThreshold; // 使用真实检查阈值函数
		createPAS(dag, expLink, mergingNodeIndex, fncPtr); // 创建PAS
	}
}; 


void PASManager::deleteUnusedPASAndMoveFlow(){ // 删除未使用的PAS并移动流量的方法
	++nbIter_; // 每次循环后调用此方法 -> 我们可以在这里计算迭代次数
	PAS* pas = NULL; // 初始化PAS指针为空
	
	for (std::list<PAS*>::iterator it = listOfPas_.begin(); it != listOfPas_.end(); ++it){ // 遍历所有PAS
		pas = *it; // 获取当前PAS
		pas->moveFlow(); // 移动流量
		if (pas->isUnused()) { // 如果PAS未使用
			delete *it; // 删除PAS
			it = listOfPas_.erase(it); // 从列表中移除PAS
			--it; // 回退迭代器
		}	
	}
}; 

CostPasIterator PASManager::begin() const { // 获取开始迭代器的方法
	return listOfPas_.begin(); // 返回列表的开始迭代器
}; 

CostPasIterator PASManager::end() const { // 获取结束迭代器的方法
	return listOfPas_.end(); // 返回列表的结束迭代器
}; 
		
int PASManager::getNbPAS()const  { // 获取PAS数量的方法
	return listOfPas_.size(); // 返回列表大小
}; 

void PASManager::print()const  { // 打印所有PAS的方法
	for(CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){ // 遍历所有PAS
		(*pasIt)->print(); // 打印当前PAS
	}
}; 
