#ifndef LAZY_SHORTEST_PATH  // 防止头文件重复包含
#define LAZY_SHORTEST_PATH 

#include "ShortestPath.h"  // 包含最短路径基类头文件

/** \brief 单目标最短路径算法的惰性最短路径实现。

    详细信息请参见 \cite Chen1998。
*/
class LazyShortestPath : public ShortestPath {  // 定义惰性最短路径类，继承自最短路径基类
	public:
		LazyShortestPath(ShortestPath* shPath);  // 构造函数，接收一个最短路径对象指针
		~LazyShortestPath();  // 析构函数

		virtual void calculate(int originIndex);  // 计算从指定起点出发的最短路径

		virtual void calculate(int originIndex, int destIndex, int odPairIndex);  // 计算指定起终点对之间的最短路径
		
		virtual FPType getCost(int destIndex) const;  // 获取到指定终点的路径成本
		
		virtual StarLink* getInComeLink(int destIndex) const;  // 获取到达指定终点的入边

	private:
		ShortestPath* shPath_;  // 最短路径对象指针
		int prevOriginIndex_;  // 前一个起点索引
		bool originChanged_;  // 起点是否改变标志
		FPType newPathCost_;  // 新路径成本

		/** 遍历存储的到目标点destIndex的最短路径
			并在流量转移后更新可能发生变化的最短路径距离
		*/
		void updatePathCost(int destIndex);  // 更新路径成本的方法

};
#endif