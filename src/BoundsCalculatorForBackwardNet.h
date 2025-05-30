#ifndef BOUNDS_CALCULATOR_FOR_BACKWARD_NET  // 防止头文件重复包含的宏定义
#define BOUNDS_CALCULATOR_FOR_BACKWARD_NET 

#include "BoundsCalculatorForBSP.h"  // 包含基类头文件

/** \brief 实现反向网络的旅行时间和收费边界计算
*/
class BoundsCalculatorForBackwardNet : public BoundsCalculatorForBSP {  // 定义反向网络边界计算器类，继承自BSP边界计算器
	public:
		BoundsCalculatorForBackwardNet(StarNetwork& net, const TollContainerType& tolls);  // 构造函数，接收网络和收费参数
		~BoundsCalculatorForBackwardNet();  // 析构函数

	protected:

		/** @return 创建并返回反向网络
		*/
		virtual StarNetwork* getNetwork(StarNetwork& net);  // 虚函数，获取网络
		/** 基于收费作为链路成本创建反向网络
		*/
		virtual StarNetwork* getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls);  // 虚函数，获取带收费的网络
		/** @return 反向网络
		*/
		virtual StarNetwork* getOppositeNet();  // 虚函数，获取反向网络

	private:
		StarNetwork* net_;  // 原始网络指针
		StarNetwork* backwardNet_;  // 反向网络指针
};
#endif