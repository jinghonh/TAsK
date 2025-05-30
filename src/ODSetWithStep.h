#ifndef OD_SET_WITH_STEP
#define OD_SET_WITH_STEP

#include "ODSet.h"
#include "DescDirection.h"

class PathBasedFlowMoveWithStep;

/** \brief 此类实现了当步长需要计算时移动流量的策略。
	\details 用于基于路径算法的方法1和方法2。
*/
class ODSetWithStep : public ODSet, public DescDirection {
	public:
		ODSetWithStep(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove);
		~ODSetWithStep();
		
		// 来自DescDirection接口
		FPType* getProjectedDirection();
		FPType getUpperBound();
		FPType* getLinkFlows();
		int getSize();
		int* getIndexes();
		virtual int getOdIndex() const;
		virtual PathBasedFlowMove* getPathBasedFlowMove() const;
		
	private:
		static FPType *projDir_; /**< 下降投影方向 */
		static int *indexes_;
		static int size_;
		static PathBasedFlowMoveWithStep* flowMoveStep_;
		static FPType *linkFlows_;
		static bool wasInitialised_;
		static int nbLinks_;
		
		void calculateProjectedDirection();
};

#endif
