#ifndef OD_SET
#define OD_SET

#include "UsedTypes.h"        // 包含自定义类型定义

#include <stdlib.h>           // 包含标准库
#include <list>               // 包含链表容器

class PathCost;               // 前向声明路径成本类
class Path;                   // 前向声明路径类
class StarNetwork;            // 前向声明星型网络类
class ShortestPath;           // 前向声明最短路径类
class PathBasedFlowMove;      // 前向声明基于路径的流量移动类

typedef std::list<Path*>::const_iterator PathIterator;  // 定义路径迭代器类型

/** \brief 这个类表示一个OD路径集合。它是PathSet类的基本元素。
*/
class ODSet { 
	public:
		/** @param index OD对索引。
			@param destIndex 目的地索引。
			@param originIndex 起点索引。
			@param pathCost 知道如何计算路径成本的对象。
			@param net 网络。
			@param shPath 最短路径算法。
			@param zeroFlow 链路流量容差。
			@param flowMove 计算流量移动的算法。
		*/
		ODSet(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net, 
			ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMove* flowMove);
		virtual ~ODSet();  // 虚析构函数

		/** @return 当前最小路径成本。
		*/
		FPType getMinPathCost() const;
		
		/** @return OD对索引。 */
		int getIndex() const;
		/** @return 目的地节点索引。*/
		int getDestIndex() const;
		/** @return 起点节点索引。*/
		int getOriginIndex() const;
		
		/** @return 存储在此OD集合中的路径数量。*/
		int getNbPaths() const;
		
		/** 向路径集合添加路径。
		*/
		void addPath(Path *path);
		
		/** @return 指向路径容器开头的迭代器。*/
		PathIterator begin() const;
		
		/** @return 容器的末尾。*/
		PathIterator end() const;
		
		/** 遍历此OD集合中存储的所有路径并重新计算它们的成本。
			它还更新当前最小路径成本。
		*/
		void updateSet();

		/** 在路径流量改变前存储它们。
		*/
		void setPrevFlows() const;
		
		/** 计算当前OD对的最短路径，并检查
			它是否比集合中当前最短路径更短。如果更短，则
			将其添加到集合中。
			@return 如果路径被添加到集合中则返回true，否则返回false。
		*/
		bool improveSet();
		
		/** 计算路径流量的下降方向，并根据某种策略移动流量，
			该策略取决于算法。
			有关算法，请参见PathBasedFlowMove类。
		*/
		bool equilibrate();
		
		/** 将路径流量投影到链路上并更新它们的成本。
		*/
		void projectPathFlowOnLinks();
		
		/** 从集合中移除未使用的路径。如果路径流量小于ZERO_FLOW，则认为该路径未使用。
		*/
		void removeUnusedPaths();
		
		void print() const;  // 打印OD集合信息
		
	protected:
		
		/** 默认情况下不执行任何操作 - 这是ODSetWithStep的钩子方法。 
		*/
		virtual void calculateProjectedDirection() {}; 

		const int destIndex_;    // 目的地索引
		const int originIndex_;  // 起点索引
			
	private:
		static StarNetwork *net_;      // 静态星型网络指针
		static ShortestPath *shPath_;  // 静态最短路径指针
		
		const int index_;              // OD对索引
		
		static PathCost *pathCost_;    // 静态路径成本指针
		FPType minDist_;               // 最小路径距离
		static FPType zeroFlow_;       // 静态零流量阈值
		
		std::list<Path*> allPaths_;    // 所有路径的链表
		
		static PathBasedFlowMove *flowMove_;  // 静态基于路径的流量移动指针
		
};

#endif
