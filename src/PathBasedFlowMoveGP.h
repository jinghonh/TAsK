#ifndef PATH_BASED_FLOW_MOVE_GP
#define PATH_BASED_FLOW_MOVE_GP

#include "PathBasedFlowMove.h"  // 包含路径流量移动基类
#include "UsedTypes.h"          // 包含自定义类型定义

/** \brief 这个类实现了梯度投影算法(方法3)的基于路径的流量移动。
	\details 详细信息参见 \cite Jayakrishnan1994。 
*/
class PathBasedFlowMoveGP : public PathBasedFlowMove {
	public:
		
		/** @param alpha 算法特定的常数，影响其收敛性。
		*/
		PathBasedFlowMoveGP(FPType alpha, DescDirectionPath* pathDirectionAlgo);
		~PathBasedFlowMoveGP();  // 析构函数
		
		/** @return 总是返回false。
		*/
		bool executeFlowMove();
		/** 将需求设置为当前OD对的需求值。
			\note 每次考虑新的OD对时都必须执行此方法。
		*/
		void setDemand(FPType demand);
		
	private:
		const FPType alpha_;  // 步长参数
		FPType demand_;       // 需求量
};

#endif
