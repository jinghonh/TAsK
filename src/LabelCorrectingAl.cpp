#include "LabelCorrectingAl.h"  // 包含标签修正算法头文件
#include "StarNetwork.h"        // 包含星形网络头文件
#include "StarNode.h"           // 包含星形节点头文件

#include <stdlib.h>             // 包含标准库
#include <cassert>              // 包含断言
#include <iostream>             // 包含输入输出流
#include <limits>               // 包含数值限制
#include <sstream>              // 包含字符串流

// 构造函数实现
LabelCorrectingAl::LabelCorrectingAl(StarNetwork *netPointer) : 
							netPointer_(netPointer), 
							nbNodes_(netPointer->getNbNodes()), originIndex_(-1) {

	nodeList_ = new nodeInfo[nbNodes_];  // 创建节点信息数组
	for (int i = 0; i < nbNodes_; ++i) {  // 初始化每个节点的信息
		nodeList_[i].dist = 0.0;          // 设置距离为0
		nodeList_[i].linkIndex = -1;      // 设置链接索引为-1
	}
	
};

// 析构函数实现
LabelCorrectingAl::~LabelCorrectingAl(){
	delete[] nodeList_;  // 释放节点信息数组
};

// 获取目标节点成本
FPType LabelCorrectingAl::getCost(int destIndex) const{
	assert((destIndex >= 0) && (destIndex < nbNodes_));  // 确保目标索引有效
	return nodeList_[destIndex].dist;	
};

// 获取目标节点的入边
StarLink* LabelCorrectingAl::getInComeLink(int destIndex) const {
	assert((destIndex >= 0) && (destIndex < nbNodes_));  // 确保目标索引有效
	int linkIndex = nodeList_[destIndex].linkIndex;      // 获取链接索引
	if (linkIndex == -1) return NULL;                    // 如果没有入边返回NULL
	return netPointer_->getLink(linkIndex);              // 返回对应的链接
};

// 获取当前源节点
int LabelCorrectingAl::getCurrentOrigin() const {
	return originIndex_;
}; 

// 计算最短路径
void LabelCorrectingAl::calculate(int originIndex){

	originIndex_ = originIndex;
	
	int sequenceList[nbNodes_ + 1]; // +1 - 因为我们有一个额外的指针:
					// sequenceList[nbNodes_] - 包含序列列表中当前最后一个节点的索引
	int firstInSequence = -1;
	sequenceList[0] = -1;
	for (int i = 0; i < nbNodes_; ++i){  // 初始化所有节点
		nodeList_[i].dist = std::numeric_limits<FPType>::infinity();  // 设置距离为无穷大
		nodeList_[i].linkIndex = -1;      // 设置链接索引为-1
		sequenceList[i + 1] = -1;         // 初始化序列列表
	}
	nodeList_[originIndex].dist = 0.0;    // 源节点距离设为0
	firstInSequence = originIndex;        // 设置序列第一个节点为源节点
	sequenceList[originIndex] = nbNodes_; // 源节点指向序列末尾
	sequenceList[nbNodes_] = originIndex; // 序列末尾指向源节点
	
	int topNode = -1;                     // 当前处理的节点
	StarLink *nextLink = NULL;            // 下一个链接
	StarNode *curNode = NULL;             // 当前节点
	int nextNodeIndex = -1;               // 下一个节点索引
	FPType distToNextNode = 0;            // 到下一个节点的距离
	
	// 主循环
	while (firstInSequence != nbNodes_) { // 当序列列表不为空时
		
		topNode = firstInSequence;        // 获取序列中的第一个节点
		curNode = netPointer_->beginNode(topNode);  // 获取当前节点
		distToNextNode = 0;
		nextNodeIndex = -1;
				
		if (proceed(curNode, topNode)) {  // 对区域节点的特殊条件
			for (nextLink = netPointer_->beginLink(); nextLink != NULL; nextLink = netPointer_->getNextLink()) {
				
				nextNodeIndex = nextLink->getNodeToIndex();  // 获取下一个节点索引
				
				distToNextNode = nodeList_[topNode].dist + nextLink->getTime();  // 计算到下一个节点的距离
				if ( distToNextNode < nodeList_[nextNodeIndex].dist) {  // 如果找到更短的路径
					nodeList_[nextNodeIndex].dist = distToNextNode;     // 更新距离
					nodeList_[nextNodeIndex].linkIndex = nextLink->getIndex();  // 更新链接索引
					if (sequenceList[nextNodeIndex] < 0) {  // 如果下一个节点不在序列列表中
						if (sequenceList[nextNodeIndex] == -1) {  // 如果下一个节点从未在序列列表中
							// 将下一个节点放在序列列表底部
							sequenceList[nextNodeIndex] = nbNodes_;  // 下一个节点是序列列表中的最后一个
							sequenceList[sequenceList[nbNodes_]] = nextNodeIndex;  // 之前最后一个节点现在指向下一个节点
							sequenceList[nbNodes_] = nextNodeIndex;  // 最后一个指针指向下一个节点索引
						} else {
							// 将下一个节点放在topNode之后
							sequenceList[nextNodeIndex] = sequenceList[topNode];  // 下一个节点现在指向topNode之前指向的节点
							sequenceList[topNode] = nextNodeIndex;  // topNode现在指向下一个节点
							if (sequenceList[nextNodeIndex] == nbNodes_) {  // 如果topNode是最后一个
								sequenceList[nbNodes_] = nextNodeIndex;  // 更新最后一个指针
							}				
						}
					}
				}
			}
		} 

		// topNode没有出边需要检查
		firstInSequence = sequenceList[topNode];  // 更新序列中的第一个节点
		sequenceList[topNode] = -2;              // 标记topNode已处理
	}
};

// 检查是否继续处理节点
bool LabelCorrectingAl::proceed(StarNode* curNode, int topNode) const {
	return ((curNode != NULL) && (!curNode->getIsZone() || (topNode == originIndex_)));  // 检查节点是否有效且不是区域节点或是否为源节点
};
