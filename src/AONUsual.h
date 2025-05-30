#ifndef AON_USUAL  // 防止头文件重复包含
#define AON_USUAL  // 定义宏

#include "AONAssignment.h"  // 包含AONAssignment基类头文件

class ShortestPath;  // 前向声明ShortestPath类
class PairOD;  // 前向声明PairOD类

/** \brief 为给定起点节点实现基本的全有或全无分配过程。
*/
class AONUsual : public AONAssignment {  // 定义AONUsual类，继承自AONAssignment
	public:
		AONUsual(const ODMatrix &mat, ShortestPath* shPath);  // 构造函数声明
		~AONUsual();  // 析构函数声明

	protected:
		/** 计算给定O-D对的旅行时间：需求乘以给定O-D对之间的最短路径距离。
			@param dest O-D对对象。
			@return 给定O-D对的总旅行时间。
		*/
		virtual FPType calculateTravelTime(const PairOD& dest) const;  // 虚函数声明，计算旅行时间

	private:
		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin);  // 虚函数声明，为起点执行分配
};

#endif  // 结束头文件保护