#ifndef SHORTEST_PATH_WITH_RAND_RETURN_WITH_FIXED_PROB // 防止头文件重复包含
#define SHORTEST_PATH_WITH_RAND_RETURN_WITH_FIXED_PROB 

#include "ShortestPathWithRandomReturn.h" // 包含带随机返回的最短路径头文件

/** \brief 随机流更新的固定概率版本，用于计算点对点最短路径的概率是固定的情况。

	详细信息，请参见ShortestPathWithRandomReturn。
*/
class ShortestPathWithRandomReturnWithFixedProbability : public ShortestPathWithRandomReturn { // 固定概率随机返回最短路径类
	public:
		/** @param shPath 将以一定概率调用的点对点最短路径算法。
			@param probability 计算点对点最短路径的固定概率。
		*/
		ShortestPathWithRandomReturnWithFixedProbability(ShortestPath* shPath, FPType probability); // 构造函数
		~ShortestPathWithRandomReturnWithFixedProbability(); // 析构函数

	protected:
		virtual FPType getProbability() const; // 重写获取概率的虚方法

	private:
		FPType probability_; // 固定概率值

};
#endif