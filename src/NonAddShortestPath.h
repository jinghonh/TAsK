#ifndef NON_ADDITIVE_SH_PATH
#define NON_ADDITIVE_SH_PATH

#include "ShortestPath.h"

class NonAdditivePC;
class OneSourceBiObjShPath;
class Point2PointBiObjShPath;
class ODMatrix;
class BiObjLabel;
class BiObjLabelContainer;

#include <map>
#include <list>

/** \brief 此类实现非加性最短路径算法。
	\details 它基于双目标最短路径和封装在NonAdditivePC中的无差异曲线。
*/
class NonAddShortestPath : public ShortestPath {

	public:
		NonAddShortestPath(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							Point2PointBiObjShPath* point2pointSP,
							ODMatrix* mat);
		~NonAddShortestPath();

		virtual void calculate(int originIndex);
		virtual void calculate(int originIndex, int destIndex, int odPairIndex);
		virtual FPType getCost(int destIndex) const;

		/** \note 当前实现在路径所属链接数量上具有线性时间复杂度。
		*/
		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		std::vector<FPType> costs_;
		mutable std::list<StarLink*> path_;
		OneSourceBiObjShPath* oneSorceSP_;
		int curDestIndex_;
		BiObjLabel* minCostLabel_;
		NonAdditivePC *pathCost_;
		
		/** 根据最后一次非加性最短路径调用计算给定目的地的非加性成本。
			@return 路径成本和具有此成本的标签指针的对。
		*/
		std::pair<FPType, BiObjLabel*> calculateCostToDest(int destIndex, int odPairIndex, 
			const BiObjLabelContainer& labels);

		/** 构建路径。
		*/
		virtual void createPath() const;
		/** 派生类的钩子方法。不做任何事。
		*/
		virtual void doSomethingWithMinCostLabel(BiObjLabel* minCostLabel);

	private:
		
		Point2PointBiObjShPath* point2pointSP_;
		ODMatrix* mat_;
		mutable std::list<StarLink*>::const_iterator currLink_;
		
			
};
#endif
