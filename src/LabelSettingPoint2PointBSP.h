#ifndef LABEL_CORRECTING_POINT_2_POINT_BSP  // 防止头文件重复包含的宏定义
#define LABEL_CORRECTING_POINT_2_POINT_BSP

#include "LabelSettingBSPBase.h"  // 包含标签设置BSP基类头文件
#include "Point2PointBiObjShPath.h"  // 包含点对点双目标最短路径头文件
#include "BoundsCalculatorForBSPBase.h"  // 包含BSP边界计算器基类头文件

class NonAdditivePC;  // 前向声明非加性路径成本类
class PathsAdder;  // 前向声明路径添加器类
class DominationByPathCostBase;  // 前向声明基于路径成本的支配规则基类

/** \brief 点对点双目标标签设置算法的具体实现。
*/
class LabelSettingPoint2PointBSP : public Point2PointBiObjShPath, 
					protected LabelSettingBSPBase	{  // 定义点对点标签设置BSP类，继承自Point2PointBiObjShPath和LabelSettingBSPBase
	public:

		/** @param net 网络对象
			@param tolls 包含链路通行费的容器
			@param bounds 知道如何计算通行费和时间下界的对象
			@param pathAdder 向目标节点添加路径的对象
			@param dominanceRule 知道如何应用路径成本支配规则的对象
		*/
		LabelSettingPoint2PointBSP(StarNetwork& net, const TollContainerType& tolls, 
						BoundsCalculatorForBSPBase& bounds,
						const PathsAdder& pathAdder, 
						DominationByPathCostBase& dominanceRule);  // 构造函数
		~LabelSettingPoint2PointBSP();  // 析构函数

		virtual void calculate(int originIndex, int destIndex, int odIndex);  // 计算函数
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path);  // 创建路径函数

	protected:

		int currOdIndex_;  // 当前OD对索引
		DominationByPathCostBase& dominanceRule_;  // 支配规则对象引用

		/** 实现点对点最短路径的特殊停止条件，参见\cite DemeyerGAPD13。
		*/
		virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;  // 检查是否继续寻找标签
		/** 计算成本向量[newTime, newToll]的估计时间和通行费，并调用支配规则，参见isWorth()。
		*/
		virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const;  // 检查是否值得添加标签
		/** @return 最近调用算法的目标节点索引。
		*/
		int getDestIndex() const;  // 获取目标节点索引

		/** @return 如果值得保留估计成本向量[timeLowerBound, tollLowerBound]的标签返回true，否则返回false。
		*/
		bool isWorth(FPType timeLowerBound, TollType tollLowerBound) const;  // 检查是否值得保留标签
		/** 调用pathAdder_对象的uploadPaths方法和dominanceRule_的updateBestKnown方法。
		*/
		virtual void uploadPaths();  // 上传路径
		/** 调用dominanceRule_的addLabelToLabelSet方法。
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
				BiObjLabel* label);  // 向标签集添加标签


	private:
		int destNode_;  // 目标节点
		BoundsCalculatorForBSPBase& bounds_;  // 边界计算器对象引用
		const PathsAdder& pathAdder_;  // 路径添加器对象常量引用

};

#endif  // 结束宏定义
