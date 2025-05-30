#ifndef PAS_
#define PAS_

#include "UsedTypes.h"

#include <list>
#include <set>

class DAGraphTapas;
class StarLink;

/** \brief 此类表示备选路段对（PAS）。
	\details 详细信息请参见 \cite Hillel2010。
	\note PAS未使用的定义与论文中不同，请参见isUnused()。
*/
class PAS {
	public:
		explicit PAS(FPType zeroFlow, FPType dirTol);
		virtual ~PAS();
		
		/** @return 如果PAS未使用则返回true，否则返回false。
			当没有移动流量或昂贵路段的整个起点流量已被移动时，PAS视为未使用。
			\note 此未使用PAS的定义与论文中的不同
			（根据数值实验，它运行得更快 - 更频繁地删除PAS更好）。
		*/
		bool isUnused();
		
		/** 此方法用于创建当前PAS的路段。将链接添加到廉价成本路段的前端。
		*/
		void pushFrontToCheap(StarLink *link);
		/** 此方法用于创建当前PAS的路段。将链接添加到昂贵成本路段的前端。
		*/
		void pushFrontToExp(StarLink *link);
		/** 此方法用于创建当前PAS的路段。将链接添加到廉价成本路段的后端。
		*/
		void pushBackToCheap(StarLink *link);
		/** 此方法用于创建当前PAS的路段。将链接添加到昂贵成本路段的后端。
		*/
		void pushBackToExp(StarLink *link);
		
		/** @return 廉价路段中的最后一个链接。如果路段为空，则返回NULL。
			此方法用于PASManager检查PAS是否已存在。
		*/
		StarLink* getLastCheapLink();
		/** @return 昂贵路段中的最后一个链接。如果路段为空，则返回NULL。
			此方法用于PASManager检查PAS是否已存在。
		*/
		StarLink* getLastExpLink();
		

		/** 将菊花 \b dag 添加到此PAS的相关起点集合中。
			如果此菊花已在集合中，则不会添加。
			@return 如果起点被添加则返回true，否则返回false。
			\note 存储整个菊花的指针而不是起点索引，以便于访问PAS所需的一些信息。
		*/
		bool addOrigin(DAGraphTapas *dag);
		
		/** 在PAS内执行流量移动，更新相应的链接流量和旅行时间。
			@return 如果实际移动了流量则返回true，否则返回false。
		*/
		bool moveFlow();

		/** 重新计算廉价和昂贵路段的成本，并更新指向廉价路段的相应指针
			（经过多次迭代后，廉价路段可能变成昂贵的）。
			@return 路段成本差异
		*/
		FPType recalcPASCosts();
		
		/** 检查当前PAS是否有流量且具有成本效益（详情请参见 \cite Hillel2010）。
			@param cost 降低的成本。
			@param v TAPAS的特定参数，用于考虑PAS是否具有流量效益。
			@param index 正在调查的菊花的昂贵链接的索引。
			@param dag 指向正在调查的菊花的指针。
			@return 如果PAS有效则返回true，否则返回false。
		*/
		bool checkIfEffective(FPType cost, FPType v, int index, DAGraphTapas* dag);
		
		/** 用于调试。在屏幕上打印路段。 */
		void print() const;
		/** @return 属于两个路段的链接总数。 */
		int getNbLinks() const;
		/** @return 与此PAS关联的起点总数。
		*/
		int getNbOrigins() const;
		/** @return 两个路段之间的成本差异。
		*/
		FPType getCostDiff();

	protected:

		std::list<StarLink*> segments_[2];
		FPType cheapCost_;
		FPType expCost_;
		FPType totalShift_;
		int cheapSegm_;

		/** 计算必须从昂贵路段移动到廉价路段的流量移动。
		*/
		virtual FPType getFlowShift();
		
	private:
		
		typedef std::set<DAGraphTapas*> SetType;
		SetType relevantOrigins_;
									/**< 使用集合以确保插入的对数复杂度。 */
									/**< 同时确保唯一性。 */
		static FPType zeroFlow_;
		static FPType dirTol_;
		
		int nbFlowMoves_;
		
		/** 计算每个起点的最小可能移动和最小可能总移动。
			它还调用getFlowShift()并确保它不超过最小可能总移动。
		*/
		FPType calculateFlowShift();

		/** 计算给定索引的路段成本。
		*/
		FPType calcSegCost(int index);
		
		/** 检查当前PAS是否具有成本效益。
			@param cost 降低的成本。
			@return 如果PAS具有成本效益则返回true，否则返回false。
		*/
		inline bool checkIfCostEffective(FPType cost) const;
		/** 检查当前PAS是否具有流量效益。
			@param v TAPAS的特定参数。
			@param index 正在调查的菊花的昂贵链接的索引。
			@param dag 指向正在调查的菊花的指针。
			@return 如果PAS具有流量效益则返回true，否则返回false。
		*/
		bool checkIfFlowEffective(FPType v, int index, DAGraphTapas* dag) const;
		
};
#endif
