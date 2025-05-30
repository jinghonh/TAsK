#ifndef BOUNDS_CALCULATOR_FOR_BSP
#define BOUNDS_CALCULATOR_FOR_BSP 

#include "LabelCorrectingAlWithoutZonesCheck.h" // 引入不带区域检查的标签修正算法头文件
#include "BoundsCalculatorForBSPBase.h" // 引入BSP边界计算器基类头文件

/**
	实现基于零流量的行程时间边界计算。

	本类还计算收费下界。
*/
class BoundsCalculatorForBSP : public BoundsCalculatorForBSPBase {
	public:
		BoundsCalculatorForBSP(StarNetwork& net, const TollContainerType& tolls); // 构造函数，初始化网络和收费

		/**
			计算所有边界（基于零流量的行程时间和收费）：
			从每个节点到每个目的节点。
		*/
		virtual void initializeBounds(); // 初始化所有边界

		~BoundsCalculatorForBSP(); // 析构函数

		/**
			子类用于在必要时更新行程时间边界的钩子方法。
		*/
		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex); // 更新指定目的节点和OD对的时间边界

		/**
			返回从节点 nodeIndex 到目的地 destNodeIndex 的收费下界。
		*/
		virtual TollType getTollLowerBound(int nodeIndex, int destNodeIndex); // 获取收费下界

		/**
			返回从节点 nodeIndex 到目的地 destNodeIndex 的零流量行程时间下界。
		*/
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex); // 获取零流量行程时间下界

	protected:
		LabelCorrectingAlWithoutZonesCheck* spp_; // 指向最短路径算法对象的指针
		std::vector<FPType> zeroFlowTimes_; // 存储零流量行程时间的数组
		std::vector<TollType> tolls_; // 存储收费的数组
		StarNetwork* reverseStar_; // 指向反向网络的指针
		StarNetwork* originalNet_; // 指向原始网络的指针
		const TollContainerType& linkTolls_; // 链路收费的引用

		/**
			为边界计算创建反向网络。
		*/
		virtual StarNetwork* getNetwork(StarNetwork& net); // 获取反向网络

		/**
			为边界计算创建以收费为链路代价的反向网络。
		*/
		virtual StarNetwork* getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls); // 获取带收费的反向网络

		/**
			返回指向网络的指针。
		*/
		virtual StarNetwork* getOppositeNet(); // 获取反向网络指针

};

#endif