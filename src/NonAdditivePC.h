#ifndef _NON_ADDITIVE_PATH_COST_
#define _NON_ADDITIVE_PATH_COST_

#include "PathCost.h" // 包含路径成本基类
#include "IndiffCurveContainer.h" // 包含无差异曲线容器

/** \brief 此类实现非加性路径成本计算。
*/
class NonAdditivePC : public PathCost { // 定义NonAdditivePC类，继承自PathCost
	public:
		NonAdditivePC(const IndiffCurveContainer& curves, const TollContainerType& tolls); // 构造函数，接收曲线容器和通行费容器
		~NonAdditivePC(); // 析构函数
		/** @return 给定路径和O-D对索引的非加性路径成本。
		*/
		FPType calculate(Path *path, int odIndex) const; // 计算路径成本，参数为路径指针和OD对索引
		/** @return 给定总旅行时间、通行费和O-D对索引的非加性路径成本。
		*/
		FPType calculate(FPType totalTime, TollType totalToll, int odIndex) const; // 重载计算函数，使用时间和通行费直接计算
		/** @return 给定通行费和O-D对索引的标量化函数值。
		*/
		FPType evaluateCurve(TollType totalToll, int odIndex) const; // 评估给定通行费和OD对索引的曲线值

	private:
		const IndiffCurveContainer& curves_; // 无差异曲线容器的引用
		const TollContainerType& tolls_; // 通行费容器的引用
};

#endif
