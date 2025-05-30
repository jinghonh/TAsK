#ifndef DOMINATION_BY_PATH_COST  // 防止头文件重复包含
#define DOMINATION_BY_PATH_COST 

#include "DominationByPathCostBase.h"  // 包含基类头文件

class NonAdditivePC;  // 前向声明非加性路径成本类

/** \brief 该类实现了基于路径成本的支配规则
*/
class DominationByPathCost : public DominationByPathCostBase {  // 继承自基类的路径成本支配类

	public:
		/** @param pathCost 实现非加性路径成本计算的对象
		*/
		DominationByPathCost(const NonAdditivePC& pathCost);  // 构造函数
		~DominationByPathCost();  // 析构函数

		/** 将标签添加到标签集中，如果节点索引等于目标索引且路径成本低于已知最佳路径成本，
			则更新已知最佳路径成本
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex);  // 添加标签到标签集的方法
		/** 将已知最佳路径成本初始化为无穷大
		*/
		virtual void resetBestKnownPathCost();  // 重置最佳路径成本的方法
		/** @return 如果基于时间下界和收费下界的路径成本小于已知最佳路径成本则返回true，
			否则返回false
		*/
		virtual bool isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const;  // 判断是否值得的方法
		/** 更新已知最佳路径成本：遍历存储在目标节点中的所有标签，
			检查这些标签中是否有任何路径成本低于已知最佳路径成本，
			如果有则更新已知最佳路径成本
		*/
		virtual void updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex);  // 更新最佳路径成本的方法

	private:
		const NonAdditivePC& pathCost_;  // 路径成本对象的引用
		FPType bestKnownPathCost_;  // 已知最佳路径成本
};
#endif