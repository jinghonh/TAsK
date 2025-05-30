#ifndef SHORTEST_PATH_WITH_RANDOM_RETURN
#define SHORTEST_PATH_WITH_RANDOM_RETURN 

#include "ShortestPath.h"

/** \brief 实现基于路径算法的随机流更新策略。

	部分点对点最短路径计算会被跳过。执行点对点最短路径计算的概率取决于
	当前迭代：对于第一次迭代，概率等于1，对于后续迭代等于 \f$0.1 + 1/iter\f$。
	该类可以与任何类型的点对点最短路径算法一起工作：A*、非加性
	最短路径等。
*/
class ShortestPathWithRandomReturn : public ShortestPath {
	public:
		/** @param shPath 将以一定概率调用的点对点最短路径算法。
		*/
		ShortestPathWithRandomReturn(ShortestPath* shPath);
		~ShortestPathWithRandomReturn();

		virtual void calculate(int originIndex);  
		virtual void calculate(int originIndex, int destIndex, int odPairIndex); 
		virtual FPType getCost(int destIndex) const;
		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		/** @return 计算点对点最短路径的概率。
		*/
		virtual FPType getProbability() const;

	private:
		ShortestPath* shPath_;
		bool returnInf_;
		int nbIters_;

};
#endif