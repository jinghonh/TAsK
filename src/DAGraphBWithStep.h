#ifndef DAGRAPH_B_WITH_STEP
#define DAGRAPH_B_WITH_STEP

#include "DAGraphB.h"

class LineSearch;

/** \brief 该类实现了算法B的另一种计算步长的方法，基于线搜索。
*/
class DAGraphBWithStep : public DAGraphB {
	public:
		// 构造函数
		// @param net 指向 StarNetwork 对象的指针
		// @param mat 指向 ODMatrix 对象的指针
		// @param zeroFlow 零流量阈值
		// @param dirTol 方向容差
		// @param originIndex 起点索引
		// @param useMultiStep 是否使用多步牛顿法（继承自 DAGraphB）
		// @param lineSearch 指向 LineSearch 对象的指针，用于执行线搜索
		DAGraphBWithStep(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol, 
						int originIndex, bool useMultiStep, LineSearch* lineSearch);
		~DAGraphBWithStep();

	private:

		static LineSearch* lineSearch_;

		// 重写基类的 calcFlowStep 方法，使用线搜索计算流量步长
		// @param minPath 指向最短路径对象的指针
		// @param maxPath 指向最长路径对象的指针
		// @return 计算出的流量步长
		virtual FPType calcFlowStep(Path* minPath, Path* maxPath) const;
};

#endif