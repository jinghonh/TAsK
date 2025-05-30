#ifndef BOUNDS_CALCULATOR_WITH_TIME_UPDATE
#define BOUNDS_CALCULATOR_WITH_TIME_UPDATE 

#include "BoundsCalculatorForBSP.h"

#include <list>

/**
	基于当前流量实现行程时间边界计算
*/
class BoundsCalculatorWithTimeUpdate : public BoundsCalculatorForBSP {
	public:
		BoundsCalculatorWithTimeUpdate(StarNetwork& net, const TollContainerType& tolls); // 构造函数，初始化网络和收费
		~BoundsCalculatorWithTimeUpdate(); // 析构函数

		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex); // 更新指定目的节点和OD对的时间边界
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex); // 获取从节点nodeIndex到目的地destNodeIndex的零流量行程时间下界

		/**
			返回指向最短路径上指向节点destIndex的链路指针
		*/
		StarLink* getInComeLink(int destIndex) const; // 获取进入指定目的节点的链路指针

	private:
		int destNode_; // 当前目的节点索引
		int prevODIndex_; // 上一个OD对索引
};
#endif