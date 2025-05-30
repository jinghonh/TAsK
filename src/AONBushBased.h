#ifndef AON_BUSH_BASED  // 防止头文件重复包含
#define AON_BUSH_BASED value  // 定义宏

#include "AONAssignment.h"  // 包含AONAssignment基类头文件
class ShortestPath;  // 前向声明ShortestPath类

/** \brief 基于树状结构的全有或全无分配算法。

    在基于树状结构的方法中，AON必须遍历所有节点，
    而不仅仅是目的地节点，以计算最小和最大成本。
*/
class AONBushBased : public AONAssignment {  // 定义AONBushBased类，继承自AONAssignment
	public:
		AONBushBased(const ODMatrix &mat, ShortestPath* shPath, int nbNodes);  // 构造函数声明
		~AONBushBased();  // 析构函数声明

	protected:
		/** @return 始终返回零，因为对于基于树状结构的方法，我们不计算总最小时间。*/
		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, 
			Origin* origin);  // 虚函数声明，为起点执行分配

	private:
		int nbNodes_;  // 节点数量
};

#endif  // 结束头文件保护