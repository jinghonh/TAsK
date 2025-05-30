#ifndef _INITIALISE_SOLUTION_  // 防止头文件重复包含的宏定义开始
#define _INITIALISE_SOLUTION_  // 定义宏

#include "ShortestPath.h"  // 包含最短路径类的头文件
#include "PairOD.h"  // 包含OD对类的头文件

#include <stdlib.h>  // 包含标准库头文件
#include <cassert>  // 包含断言头文件

/** \brief 定义用于初始化均衡算法中不同容器对象的抽象类
*/
class InitialiseSolution {
	public:
		virtual ~InitialiseSolution(){};  // 虚析构函数
		
		/** 初始化容器对象（属于一个OD对的路径集合、一个bush等）
			@param link 图中的链路
			@param dest 目标节点
			@param originIndex 起点的索引
		*/
		virtual void initialiseItself(StarLink* link, PairOD *dest, int originIndex) = 0;  // 纯虚函数，用于初始化自身
		
		/** 此方法在AONAssignment中被调用。它遍历从\b originIndex到目标\b dest的最短路径上的所有链路，
			并对每个链路调用initialiseItself()方法
		*/
		void initialiseSolution(PairOD *dest, ShortestPath *shPath, int originIndex){  // 初始化解决方案的方法

			StarLink *link = shPath->getInComeLink(dest->getIndex());  // 获取目标节点的入边
			assert(link != NULL);  // 断言确保链路不为空
			int nextDest = link->getNodeFromIndex();  // 获取链路的起始节点

			while (link != NULL) {  // 当链路不为空时循环
				initialiseItself(link, dest, originIndex);  // 初始化当前链路
				nextDest = link->getNodeFromIndex();  // 更新下一个目标节点
				link = shPath->getInComeLink(nextDest);  // 获取下一个链路的入边
			}
		};
		
		
	protected:
		InitialiseSolution(){};  // 受保护的构造函数
		
};

#endif  // 防止头文件重复包含的宏定义结束
