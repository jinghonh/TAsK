#ifndef DAGRAPH_LUCE
#define DAGRAPH_LUCE

#include "DAGraph.h"
#include "LineSearch.h"

#include <vector>

/** \brief 实现基于起点的 LUCE 算法。
	\details 详细信息请参见 \cite Gentile2014。
	\note 当前实现与论文中的描述略有不同。
	它是基于起点的，而不是基于终点的。
	在许多地方，算法遍历丛 (bushes) 而不是网络。
	此外，仅当链路上的流量发生变化时，才会动态更新链路的行程时间。
	\warning 当要求的精度较高时，此算法在收敛方面显示出一些问题。
	可能的原因是浮点运算的问题，多次迭代后可能会丢失大量流量。
	此算法的另一个有问题的问题与导数为零的退化情况有关。目前，通过将导数设置为一来解决此问题。
*/
class DAGraphLUCE : public DAGraph {
	public:

		DAGraphLUCE(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
					int originIndex, LineSearch* lineSearch);
		~DAGraphLUCE();
		
		/** 实现 LUCE 算法的流量移动。
			@return 总是返回 false，因为此算法并非旨在实现 Equilibration II。
		*/
		virtual bool moveFlow();
		
		/** 首先将导数设置为零，然后为属于丛的链路重新计算导数。
			它还计算流量分配和节点流量。
			要求存在拓扑顺序。
		*/
		void prepareData(); 
		
	private:
		
		const int nbNodes_;
		const int nbLinks_;
		std::vector<FPType> nodeFlowsInv_;
		std::vector<FPType> flowPortionsInv_;
		
		static LineSearch* lineSearch_;

		void calcFlowPortions();
		
		void computeAvCosts(std::vector<FPType> &C, std::vector<FPType> &G);
		void computeDirection(std::vector<FPType> &e_links, const std::vector<FPType> &C,
							  const std::vector<FPType> &G);
		FPType computeStepSize(const std::vector<FPType> &e_links);
		void assignNewFlow(FPType stepSize, const std::vector<FPType> &e_links);

};

#endif
