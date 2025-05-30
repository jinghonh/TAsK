#ifndef _ADDITIVE_PATH_COST_
#define _ADDITIVE_PATH_COST_

#include "PathCost.h"

/** \brief 加性路径成本 - 属于给定路径的链接成本之和。*/
class AdditivePC : public PathCost {
	public:
		AdditivePC();
		~AdditivePC();
		
		/** 计算加性路径成本。
			@param path 路径。
			@param odIndex O-D对索引。
		*/
		FPType calculate(Path *path, int odIndex) const;
};

#endif
