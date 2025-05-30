#ifndef DAGRAPH_TAPAS
#define DAGRAPH_TAPAS

#include "DAGraph.h"

class ShortestPath;
class PASManager;

/** \brief 实现 TAPAS 算法的主要步骤。
	\details 详细信息请参见 \cite{Hillel2010}。
	\note TAPAS 的此实现不是随机的，
	它没有实现流量比例条件。
	此外，为了确定 PAS 是否处于非活动状态，使用了不同的条件
	（如果在上一次迭代期间没有移动流量，则 PAS 处于非活动状态）。
	在有效 PAS 创建的情况下使用以下阈值：\f$ 10 \cdot 10^{-iter}\f$，其中
	\f$ iter \f$ 是当前迭代次数。算法的性能取决于此度量。
	仅实现了有效 PAS 的创建。未实现分支转移。
*/
class DAGraphTapas : public DAGraph {
	public:
  		explicit DAGraphTapas(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
  							  int originIndex, ShortestPath *shPath, PASManager *pasManager);
		~DAGraphTapas();
		
		/** 实现 TAPAS 的主要步骤，包括创建 PAS 和在所有活动 PAS 内移动流量。
		*/
		bool moveFlow();

		/** 存储最小转移量的值。它在 PAS 中计算和使用。*/
		void setMinShift(FPType value);

		/** @return 可能的最小转移量的值。*/
		FPType getMinShift();
		
		/** 此方法移除循环流。*/
		void removeCyclicFlows();		
		
	private:
		static ShortestPath *shPath_;
		static PASManager *pasManager_;
		FPType minShift_;

		
		std::list<StarLink*> exploredLinks_; /**< 存储在拓扑排序期间探索过的链路。
								在 handleBackEdge() 中使用。*/
		
		/** 此方法在 topologicalSort() 中调用。当发现后向边时，
		 	它会移除循环流并更新相应的链路行程时间。
			@return 总是返回 true。它用于停止 topologicalSort() 中的递归调用。
		*/
		bool handleBackEdge(StarLink* link);

		/** 此方法将探索过的链路存储到内部数据结构中。*/
		void handleExploredLink(StarLink* link);

		/** topologicalSort() 中的附加条件。在 TAPAS 的情况下，topologicalSort()
			必须仅检查此起点使用的链路，即具有正起点流量的链路。
			@return 如果传递的链路的起点流量大于 zeroFlow_，则返回 true，否则返回 false。
		*/
		bool checkPositiveFlow(int linkIndex);
		
};

#endif
