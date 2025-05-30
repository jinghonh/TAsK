#ifndef EQ_ALGO
#define EQ_ALGO
#include "DecoratedEqAlgo.h"  // 包含装饰模式的均衡算法基类
#include "UsedTypes.h"        // 包含自定义类型定义

class AddHook;               // 前向声明钩子组件类
class StarNetwork;           // 前向声明星型网络类
class ConvMeasure;           // 前向声明收敛度量类

/** \brief 这个类是所有交通分配算法的基类。
*/
class EqAlgo : public DecoratedEqAlgo {
	
	public: 
		virtual ~EqAlgo();    // 虚析构函数
		
		/** @return 总收敛检查时间。
		*/
		virtual FPType getTotalConvTime();

		/** @return 总纯迭代计算时间。
		*/
		virtual FPType getTotalPureIterTime();

		/** @return 执行全有或全无分配所需的初始化时间。
		*/
		virtual FPType getInitialisationTime() ;

		/** 执行算法。
			@return 收敛到给定精度水平所需的总迭代次数。
		*/
		int execute();
		
	protected:
		ConvMeasure* conv_;   // 收敛度量指针

		/**	@param component 执行额外收敛信息操作的对象，
				参见AddHook。
			@param net 网络
			@param timeLimit 算法在达到所需精度水平之前必须停止的时间限制(秒)。
			@param conv 收敛度量。
		*/
		EqAlgo(AddHook *component, StarNetwork* net, FPType timeLimit, 
				ConvMeasure* conv);

		/** 初始化算法的解结构。通常执行AON分配。
		*/		
		virtual void initialise() = 0;

		/** 执行算法的一次迭代。
		*/
		virtual void performOneIteration() = 0;

		/** 收敛性检查可能被子类重写。例如，参见FWAlgo。 
		*/
		virtual bool isConverged();

		StarNetwork *net_;    // 星型网络指针
	private:	
		AddHook *component_;  // 钩子组件指针
		
		const FPType timeLimit_;     // 时间限制
		FPType totalConvTime_;       // 总收敛检查时间
		FPType totalPureIterTime_;   // 总纯迭代时间
		FPType initialisationTime_;  // 初始化时间
};

#endif
