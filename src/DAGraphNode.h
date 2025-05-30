#ifndef _DAGRAPH_NODE_
#define _DAGRAPH_NODE_

#include "StarLink.h"

#include <list>

/** \brief 该类是DAGraph中的基本元素，代表图的一个节点。
	\details 所有变量均为public，以便更快访问。
*/
class DAGraphNode {
	public:
		
		// 构造函数
		// @param index 节点的索引
		// @param demandVal 节点的需求量 (如果不是目标节点则为0)
		DAGraphNode(int index, FPType demandVal) : minLink(NULL), maxLink(NULL),
					demand(demandVal), nodeIndex(index), 
					pre(0), post(0), minDist(0.0), maxDist(0.0){};
		~DAGraphNode(){};
		
		// 指向该节点的入边列表。
		std::list<StarLink*> incomeLinks;
		// 从该节点出发的出边列表。
		std::list<StarLink*> outLinks;
		
		// 指向属于终止于此节点的最短路径的路段的指针。
		StarLink* minLink;
		// 指向属于终止于此节点的最长路径的路段的指针。
		StarLink* maxLink;
		// 此节点的需求量，如果此节点不是目标节点，则为零。
		// 为更快访问而显式存储。
		const FPType demand;
		// 此节点的索引。
		const int nodeIndex;
		// 用于拓扑排序的特殊数字。它表示对此节点的访问何时开始。
		// 参见 Dasgupta2006。
		int pre;
		// 用于拓扑排序的特殊数字。它表示对此节点的访问何时结束。
		// 参见 Dasgupta2006。
		int post;
		// 到此节点的最短距离。
		FPType minDist;
		// 到此节点的最长距离。
		FPType maxDist;
};

#endif
