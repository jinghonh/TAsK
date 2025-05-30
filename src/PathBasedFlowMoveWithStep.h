#ifndef PATH_BASED_FLOW_MOVE_WITH_STEP
#define PATH_BASED_FLOW_MOVE_WITH_STEP

#include "PathBasedFlowMove.h"  // 包含路径流量移动基类
#include "UsedTypes.h"          // 包含自定义类型定义

class StepSize;                 // 前向声明步长类
class DescDirection;            // 前向声明下降方向类

/** \brief 这个类实现带有步长计算线性搜索的平衡算法(取决于DescDirectionPath类)的基于路径的流量移动(方法1和2)。
*/
class PathBasedFlowMoveWithStep : public PathBasedFlowMove {
	public:
		/** @param stepSize 实现步长计算的对象。
			@param pathDirectionAlgo 基于路径的下降方向。
			@param zeroFlow 链路流量容差。
		*/
		PathBasedFlowMoveWithStep(StepSize *stepSize, DescDirectionPath* pathDirectionAlgo,
								  FPType zeroFlow);
		~PathBasedFlowMoveWithStep();  // 析构函数
		
		/** @return 如果计算的步长为零则返回true，否则返回false。
		*/
		bool executeFlowMove();
		
		/** 设置当前下降方向。 
			\note 每次考虑新的OD对时都必须执行此方法。
		*/
		void setDescDirection(DescDirection* dir);
		
	private:
		StepSize *stepSize_;     // 步长计算指针
		const FPType zeroFlow_;  // 零流量阈值
		DescDirection* dir_;     // 下降方向指针
};

#endif
