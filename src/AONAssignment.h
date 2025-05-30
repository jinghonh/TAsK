#ifndef _AON_ASSIGNMENT_  // 防止头文件重复包含
#define _AON_ASSIGNMENT_

#include "UsedTypes.h"  // 包含基本类型定义

class InitialiseSolution;  // 前向声明初始化解类
class ODMatrix;           // 前向声明OD矩阵类
class Origin;             // 前向声明起点类
class ShortestPath;       // 前向声明最短路径类

/** \brief 全有或全无分配的抽象类。

    该类定义了全有或全无(AON)过程的一般结构。
 */
class AONAssignment {
	public:
		
		virtual ~AONAssignment();  // 虚析构函数
		
		/** 定义AON框架并初始化\b solution。
			@param solution InitialiseSolution类的实例，它实现了依赖于解类型的特定初始化例程。
			@return 最小行程时间。
		*/
		FPType execute(InitialiseSolution *solution);  // 执行AON分配
		
	protected:
		
		ShortestPath* shPath_; /**< 指向最短路径算法的指针 */

		/** 构造函数。
			@param mat OD矩阵的引用。
		*/
		AONAssignment(const ODMatrix &mat, ShortestPath* shPath);  // 构造函数

		/**	为给定的起点节点执行\b solution的初始化。
			@param solution 指向解的指针。
			@param origin 指向起点节点的指针。
			@return 为给定起点节点计算的最小行程时间。
		*/
		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin) = 0;  // 纯虚函数，为起点执行分配

	private:
		const ODMatrix &mat_; /**< OD矩阵的引用。*/
};

#endif  // 结束头文件保护
