#ifndef BOUNDS_CALCULATOR_FOR_BSP_BASE
#define BOUNDS_CALCULATOR_FOR_BSP_BASE 

#include "UsedTypes.h"

/**
	定义其他边界计算器的接口，实现不计算任何边界的情况。
*/
class BoundsCalculatorForBSPBase {
	public:
		BoundsCalculatorForBSPBase(); // 构造函数
		virtual ~BoundsCalculatorForBSPBase(); // 虚析构函数

		/**
			不执行任何操作。
		*/
		virtual void initializeBounds(); // 初始化所有边界（无操作）

		/**
			不执行任何操作。
		*/
		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex); // 更新指定目的节点和OD对的时间边界（无操作）

		/**
			始终返回零。
			@return 总是返回0
		*/
		virtual TollType getTollLowerBound(int nodeIndex, int destNodeIndex); // 获取从节点nodeIndex到目的地destNodeIndex的收费下界（总是0）

		/**
			始终返回零。
			@return 总是返回0
		*/
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex); // 获取从节点nodeIndex到目的地destNodeIndex的零流量行程时间下界（总是0）

};
#endif