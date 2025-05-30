#ifndef ALGO_DECORATOR_BASE  // 防止头文件重复包含
#define ALGO_DECORATOR_BASE

#include "DecoratedEqAlgo.h"  // 包含被装饰算法类的头文件

/** \brief 算法装饰器的基类。为派生类定义通用方法。
*/
class AlgoDecoratorBase : public DecoratedEqAlgo {  // 定义装饰器基类，继承自被装饰算法类
	public:
		
		virtual ~AlgoDecoratorBase() {delete component_;};  // 虚析构函数，释放component_指针
		virtual int execute() = 0;  // 纯虚函数，执行算法

		/** 从DecoratedEqAlgo继承的方法。重定向到component_。*/
		virtual FPType getTotalConvTime() {return component_->getTotalConvTime();};  // 获取总收敛时间
		/** 从DecoratedEqAlgo继承的方法。重定向到component_。*/
		virtual FPType getTotalPureIterTime() {return component_->getTotalPureIterTime();};  // 获取总纯迭代时间
		/** 从DecoratedEqAlgo继承的方法。重定向到component_。*/
		virtual FPType getInitialisationTime() {return component_->getInitialisationTime();};  // 获取初始化时间

	protected:

		DecoratedEqAlgo* component_; /**< 用于装饰的算法，它在这个类中被释放。*/
		/** @param component 用于装饰的算法。*/
		AlgoDecoratorBase(DecoratedEqAlgo* component): component_(component) {};  // 构造函数，初始化component_成员

};
#endif