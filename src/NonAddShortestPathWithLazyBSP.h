#ifndef NON_ADD_SHORTEST_PATH_WITH_LAZY_BSP
#define NON_ADD_SHORTEST_PATH_WITH_LAZY_BSP 

#include "NonAddShortestPath.h" // 包含非加性最短路径基类

/** \brief 非加性惰性最短路径。

	详细信息，参见 \cite Chen1998。
*/
class NonAddShortestPathWithLazyBSP : public NonAddShortestPath { // 定义NonAddShortestPathWithLazyBSP类，继承自NonAddShortestPath
	public:
		NonAddShortestPathWithLazyBSP(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							ODMatrix* mat); // 构造函数，接收多个参数
		~NonAddShortestPathWithLazyBSP(); // 析构函数

		virtual void calculate(int originIndex, int destIndex, int odPairIndex); // 计算特定OD对的最短路径
		virtual void createPath() const; // 创建路径

	private:
		int prevOriginIndex_; // 前一个起点索引

		void updateLabelTravelTimes(const BiObjLabelContainer& labels); // 更新标签旅行时间
		FPType recalculatePathTime(BiObjLabel* destLabel) const; // 重新计算路径时间

};
#endif