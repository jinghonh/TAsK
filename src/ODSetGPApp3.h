#ifndef OD_SET_GP_APP3
#define OD_SET_GP_APP3

#include "ODSet.h"

class PathBasedFlowMoveGP;
class ODMatrix;

/** \brief 此类扩展了ODSet，用于实现论文 \cite Jayakrishnan1994 中描述的梯度投影法。
	\details 详细信息见 \cite Jayakrishnan1994。 
*/
class ODSetGPApp3 : public ODSet{
	public:
		ODSetGPApp3(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveGP* flowMove, ODMatrix *mat);
		~ODSetGPApp3();
		
	private:
		static ODMatrix* mat_;
		static PathBasedFlowMoveGP* flowMoveGP_;
		
		/** 设置当前OD对的需求值，PathBasedFlowMoveGP需要此值。
		*/
		virtual void calculateProjectedDirection();
		
};

#endif
