#ifndef ASTAR_WITHOUT_ZONE_SKIP  // 防止头文件重复包含
#define ASTAR_WITHOUT_ZONE_SKIP 

#include "Astar.h"  // 包含A*算法基类头文件

/** \brief 执行常规A*算法，不跳过区域节点。
*/
class AstarWithoutZoneSkip : public Astar {  // 定义不跳过区域节点的A*算法类，继承自Astar
	public:
		AstarWithoutZoneSkip(StarNetwork* netPointer, ODMatrix* odMatrix);  // 构造函数，接收网络指针和OD矩阵指针
		~AstarWithoutZoneSkip();  // 析构函数

	protected:
		/** @return 始终返回false。
		*/
        virtual bool shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const;  // 虚函数，判断是否应该跳过区域节点

};
#endif