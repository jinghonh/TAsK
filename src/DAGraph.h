#ifndef D_A_GRAPH
#define D_A_GRAPH

#include "UsedTypes.h"

#include <list>

class StarLink;
class StarNetwork;
class DAGraphNode;
class ODMatrix;

/** \brief 该类为有向无环图（DAG）的抽象基类，实现了DAG的基本功能，并为派生类提供接口以在每个DAG内执行流量移动。
*/
class DAGraph {
	
	public:
		
		virtual ~DAGraph(); // 虚析构函数，确保派生类析构时正确释放资源
		
		/** 如果link不在图中则添加，并分配对应的DAGraphNode对象（如果之前未分配）。
		*/
		bool addLink(StarLink *link);
		
		/** 删除未使用的link。若对应起点流量小于ZERO_FLOW且保持图的连通性，则该link被视为未使用。
		*/
		bool removeUnusedLinks();

		/** 也删除未使用的link，但遍历作为参数传递的link列表。
		*/
		bool removeUnusedLinks(const std::list<StarLink*> &links); 	

		/** 将引用listRef设置为索引为nodeIndex的节点的出边列表。
		*/
		void getOutLinks(int nodeIndex, std::list<StarLink*>& listRef);

		/** 将引用listRef设置为索引为nodeIndex的节点的入边列表。
		*/
		void getInLinks(int nodeIndex, std::list<StarLink*>& listRef);

		/** 返回索引为nodeIndex的节点的出边列表的拷贝。
		*/
		std::list<StarLink*> getOutLinksCopy(int nodeIndex) const;

		/** 返回索引为nodeIndex的节点的入边列表的拷贝。
		*/
		std::list<StarLink*> getInLinksCopy(int nodeIndex) const;
		
		/** 开始按拓扑升序遍历所有节点（即从起点开始）。
			@return 拓扑升序的第一个节点索引，如果列表为空则返回-1。
		*/
		int beginAscPass();

		/** 返回拓扑升序的当前节点索引，如果到达列表末尾则返回-1。
		*/
		int getNextAscPass();

		/** 开始按拓扑降序遍历所有节点（即从最远的目标节点开始）。
			@return 拓扑降序的第一个节点索引，如果列表为空则返回-1。
		*/
		int beginDescPass();

		/** 返回拓扑降序的当前节点索引，如果到达列表末尾则返回-1。
		*/
		int getNextDescPass();
		
		/** 创建DAG节点的拓扑序。
		*/
		bool topologicalSort();

		/** 为DAG的每个节点分配最小和最大距离。
			@param destIndex 目标节点索引。如果到达目标节点destIndex，算法将停止（因此，拓扑上在destIndex之后的节点的最小和最大距离不会被计算）。
			@param destIndex 如果必须为所有节点计算这些距离，则destIndex必须设置为-1。
			\warning 此方法假定拓扑序以起点节点开始。
		*/
		void buildMinMaxTrees(int destIndex);

		/** 实现向图中添加更优link的算法，同时保持无环性。算法详见Nie的论文
			"A class of bush-based algorithms for the traffic assignment problem",
			Transportation Research, 2010.
		*/
		bool addBetterLinks();
		
		/** 此方法必须由派生类实现。它必须通过将当前解更接近平衡状态来更改起点流量以及相应的路段流量和行程时间。
		*/
		virtual bool moveFlow() = 0;
		
		/** 此方法用于OriginSet中初始化所有bush，并在派生类中保持与执行的流量移动一致的起点流量。
			将需求添加到内部结构中，该结构存储每个bush链接的起始流量。
		*/
		void addOriginFlow(int linkIndex, FPType demand);

		/** 此方法仅在PAS类中使用，因为它在添加起始流量时如果链接不在bush中则添加链接。
		*/
		void addOriginFlowAndCreateLink(StarLink* link, FPType demand);

		/** 设置索引为index的链接的起始流量为flow。
		*/
		void setOriginFlow(int index, FPType flow);

		/** 将索引为index的链接的起始流量设置为零。
		*/
		void setOriginFlowToZero(int linkIndex);

		/** @return 返回由linkIndex指定的链接的起始流量。
		*/
		FPType getOriginFlow(int linkIndex) const;
		
		/** @return 返回该bush所属的起点索引。
		*/
		int getOriginIndex() const;
		
		/** 在屏幕上打印有关此bush的相关信息。
		*/
		void print() const;

		/** 在屏幕上打印起始流量。供调试使用。
		*/
		void printOriginFlow() const;
		
		/** For debugging.
		*/
		void printShPath(int node);
		
		/** For debugging.
		*/
		void printMaxShPath(int node);
		
		/** 检查起点流量是否满足网络流约束。
			@return 流量的最大偏差。
		*/
		FPType checkOFlowsFeasibility();
		
		/** @return 返回属于该bush的链接数量。
		*/
		FPType getNbLinksInBush() const;
		
	protected:
		
		// just for iteration without top.order in TAPAS
		int *nodeIndexes_;
		int nodeSize_;
		
		static StarNetwork * net_;
		static ODMatrix *mat_;
	
		static FPType zeroFlow_;

		/** @param net 指向网络的指针。
			@param mat 指向O-D矩阵的指针。
			@param zeroFlow 链路流量容忍度。
			@param originIndex 起点索引。
		*/
		DAGraph(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, int originIndex);
		
		/** @return 返回索引为nodeIndex的节点的需求量，如果给定节点不是目标节点，则返回零。
		*/
		FPType getDemand(int nodeIndex) const;
		
		/** @param index 获取指定索引的DAGraphNode对象
		*/
		DAGraphNode* getNode(int index) const;
		
		/** 如果DAG的连通性得以保留，则从DAG中移除由索引指定的链接。
			@return 如果链接已移除，则返回true，否则返回false（如果此操作后DAG未连接，则不会移除链接）。
		*/
		bool removeLink(int index);

		/** 如果检测到回边，则抛出异常。此方法可以被派生类重写，参见DAGraphTapas。
		*/
		virtual bool handleBackEdge(StarLink* link);

		/** 此方法是DAGraphTapas的钩子方法。如果没有被重写，则不执行任何操作。
		*/
		virtual void handleExploredLink(StarLink* link) {};

		/** 此方法总是返回true，除非被重写，参见DAGraphTapas获取细节。
		*/
		virtual bool checkPositiveFlow(int linkIndex) {return true;};
		
	private:
		const int originIndex_;
		
		FPType *originFlows_;
		DAGraphNode **nodes_;
		
		StarLink **links_;
		int linkSize_; 
		std::list<int> linkIndexes_;
		
		std::list<int> topOrder_;
		
		// for passes
		std::list<int>::iterator currNode_;
		std::list<int>::reverse_iterator currNodeDesc_;
		
		// for improving set
		std::list<int> p2Cont_;
		
		// for topological sort
		int clock_;
		
		// to keep track of initialisation of static variables
		static bool wasInitialised_; 
		
		/** @return 如果链路可以改进当前bush，则返回true，否则返回false。
		*/
		bool worthAdding(StarLink* link);

		/** 将所谓的P2集合中的链路添加到bush中。详见 Nie2010。
		*/
		bool addFromP2();
		
		/** @return 返回尚未在bush中的链路的索引。
			In order to find the first such link starIndex must be set to -1.
		 */
		int getNextNotInSet(int startIndex) const;

		/** @return index of the next link for which both head and tail nodes belong
			to the bush, -1 otherwise.
		*/
		bool isReachable(StarLink* link) const;
		
		/** 此方法在拓扑排序中使用。详见 Dasgupta2006。
		*/
		bool explore(int vertex, bool *visited);

		/** 此方法在拓扑排序中使用。详见 Dasgupta2006。
		*/
		inline void preVisit(int vertex);

		/** 此方法在拓扑排序中使用。详见 Dasgupta2006。
		*/
		inline void postVisit(int vertex);
		
		/** 初始化静态成员
		*/
		static void initialiseStaticMembers(StarNetwork *net, ODMatrix *mat, FPType zeroFlow);
		
};

#endif
