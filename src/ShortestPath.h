#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include "StarLink.h"

#include <iostream>

/** \brief 这是一个抽象类，定义了单目标最短路径算法的接口。
 */
class ShortestPath {
	
	public:
		virtual ~ShortestPath(){};
		
		/** 计算从起点 \b originIndex 到所有其他可达节点的最短路径。
		*/
		virtual void calculate(int originIndex) =0;

		/** 计算从起点 \b originIndex 到终点 \b destIndex 的最短路径。
			如果子类没有重写此方法，则调用常规的从给定起点到所有其他节点的最短路径计算。
		*/
		virtual void calculate(int originIndex, int destIndex, int odPairIndex) {
			calculate(originIndex);
		}; 
		
		/** @return 到节点 \b destIndex 的最短路径距离 */
		virtual FPType getCost(int destIndex) const =0;
		
		/** @return 指向属于最短路径并指向节点 \b destIndex 的链接的指针，
			如果是起点或者目标节点从当前起点不可达，则返回NULL。
		*/
		virtual StarLink* getInComeLink(int destIndex) const =0;
		
		/** 用于调试。在屏幕上输出从起点到 destIndex 的最短路径。
		*/
		void printPath(int destIndex) const {
			StarLink *link = getInComeLink(destIndex);
			int nextDest = -1;
			std::string str;
			while (link != NULL) {
				str = link->toString() + " " + str;
				nextDest = link->getNodeFromIndex();
				link = getInComeLink(nextDest);
			}
			std::cout << str << std::endl;
		};
		
	protected:
		ShortestPath(){};

};

#endif
