#ifndef PATH_BASED_ALGO_EQII
#define PATH_BASED_ALGO_EQII

#include "PathBasedAlgo.h"  // 包含基于路径的算法基类

/** \brief 这个类为基于路径的算法实现平衡II。
	\warning 平衡II在高精度情况下可能不会收敛。
*/
class PathBasedAlgoEqII : public PathBasedAlgo {
public:
	/** @param pathSet 所有路径的集合。
		@param component 执行额外收敛操作的对象。
		@param conv 收敛度量。
		@param net 网络。
		@param timeLimit 算法允许的最大执行时间(秒)。
		@param maxNbIter 平衡II的最大迭代次数。
		@param mat 起点-终点矩阵。
	*/
	PathBasedAlgoEqII(PathSet *pathSet, AddHook *component, ConvMeasure *conv, 
					StarNetwork *net, FPType timeLimit, 
					int maxNbIter, ODMatrix* mat);
	~PathBasedAlgoEqII();  // 析构函数

private:
	const int maxNbIter_;  // 最大迭代次数

	virtual bool mainLoop(ODSet *odSet);  // 重写主循环方法

};
#endif