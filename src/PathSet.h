#ifndef PATH_SET
#define PATH_SET

#include "InitialiseSolution.h"  // 包含初始化解决方案基类

#include <vector>                // 包含向量容器

class ODSet;                     // 前向声明OD集合类
class ODMatrix;                  // 前向声明OD矩阵类
class PathCost;                  // 前向声明路径成本类
class PathBasedFlowMove;         // 前向声明基于路径的流量移动类
class PathBasedFlowMoveWithStep; // 前向声明带步长的基于路径的流量移动类
class PathBasedFlowMoveGP;       // 前向声明广义投影的基于路径的流量移动类
class AONAssignment;             // 前向声明全有或全无分配类
class Path;                      // 前向声明路径类
class ShortestPath;              // 前向声明最短路径类
class StarNetwork;               // 前向声明星型网络类

typedef std::vector<ODSet*>::const_iterator ConstODSetIterator;  // 定义OD集合常量迭代器类型
/** \brief 这个类表示ODSet对象的容器。
*/
class PathSet : public InitialiseSolution {
	public:

		/** 对于实现方法1或2的算法，必须调用此方法。
		*/
		static PathSet* createSetWithStep(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove, AONAssignment* aon);

		/** 对于路径平衡算法方法3，必须调用此方法。
		*/		
		static PathSet* createSetPEAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove, AONAssignment* aon);
		
		/** 对于梯度投影算法方法3，必须调用此方法。
		*/
		static PathSet* createSetGPAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveGP* flowMove, AONAssignment* aon);
		~PathSet();  // 析构函数
		
		void initialise();  // 初始化方法
		
		void initialiseItself(StarLink* link, PairOD *dest, int originIndex);  // 自身初始化方法
		
		/** @return 指向第一个OD集合的迭代器。
		*/
		ConstODSetIterator begin() const;
		/** @return 容器的末尾。
		*/
		ConstODSetIterator end() const;

		/** @return 给定OD对索引odIndex的OD集合指针。
		*/
		ODSet* getODSetByIndex(int odIndex) const;
		/** 在屏幕上打印路径集合。
		*/
		void print() const;
		
	private:
		std::vector<ODSet*> odSetList_;  // OD集合列表
		const int size_;                 // 集合大小
		Path *currPath_;                 // 当前路径指针
		
		AONAssignment* aon_;             // 全有或全无分配指针

		PathSet(ODSetType setType, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove,  
			PathBasedFlowMoveWithStep* flowMoveWithStep, PathBasedFlowMoveGP* flowMoveGP,
			AONAssignment* aon);  // 私有构造函数
};

#endif
