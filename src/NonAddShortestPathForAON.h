#ifndef NON_ADD_SHORTEST_PATH_FOR_AON
#define NON_ADD_SHORTEST_PATH_FOR_AON 

#include "NonAddShortestPath.h"

/** \brief 实现全有或全无(all-or-nothing)的非加性最短路径。

	非加性单源最短路径不存储全有或全无所需的路径。
	该类的职责是在计算非加性最短路径后为每个O-D对创建路径。
*/
class NonAddShortestPathForAON : public NonAddShortestPath {
	public:
		NonAddShortestPathForAON(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							ODMatrix* mat);
		~NonAddShortestPathForAON();

		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		/** 将最小成本标签minCostLabel存储到内部数据结构中。
		*/
		virtual void doSomethingWithMinCostLabel(BiObjLabel* minCostLabel);

	private:
		std::vector<BiObjLabel*> minCostLabels_;
		mutable bool shoulResetPath_;
		mutable std::list<StarLink* > currPath_;
		mutable std::list<StarLink* >::const_iterator linkIt_;


};

#endif