#ifndef DOMINATION_BY_PATH_COST_BASE
#define DOMINATION_BY_PATH_COST_BASE 

#include "UsedTypes.h"  // 包含基本类型定义

class BiObjLabelContainer;  // 前向声明双目标标签容器类

class BiObjLabel;  // 前向声明双目标标签类

/** \brief 路径成本支配规则的基类。

    为派生类定义接口并提供不执行任何操作的默认实现。
*/
class DominationByPathCostBase {
	public:
		DominationByPathCostBase();  // 构造函数
		virtual ~DominationByPathCostBase();  // 虚析构函数

		/** 将标签添加到标签集合中。
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex);

		/** 不执行任何操作。
		*/
		virtual void resetBestKnownPathCost();

		/** 执行常规的支配检查。
			@return 如果基于timeLowerBound和tollLowerBound的标签不被目标节点中的任何标签支配，
			则返回true，否则返回false。
		*/
		virtual bool isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const;

		/** 不执行任何操作。
		*/
		virtual void updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex);

};
#endif