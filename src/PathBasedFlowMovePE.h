#ifndef PATH_BASED_FLOW_MOVE_PE
#define PATH_BASED_FLOW_MOVE_PE

#include "PathBasedFlowMove.h"  // 包含路径流量移动基类

/** \brief 这个类实现了路径平衡算法(方法3)的基于路径的流量移动。
	\details 详细信息参见 \cite Florian1995。 
*/
class PathBasedFlowMovePE : public PathBasedFlowMove {
	public:
		PathBasedFlowMovePE(DescDirectionPath* pathDirectionAlgo);  // 构造函数
		~PathBasedFlowMovePE();  // 析构函数
		
		/** @return 总是返回false。
		*/
		bool executeFlowMove();
};

#endif
