#ifndef ORIGIN_BUSH_LUCE
#define ORIGIN_BUSH_LUCE

#include "OriginBush.h" // 包含起点菊花基类

class DAGraphLUCE; // 前向声明LUCE有向无环图类
class LineSearch; // 前向声明线搜索类

/** \brief 此类负责DAGraphLUCE的创建。 
	\details 它还重写了callBuildMinMaxTrees()方法，以避免不必要的
	最小和最大树的计算。
*/
class OriginBushLUCE : public OriginBush { // LUCE起点菊花类，继承自OriginBush
	public:
		explicit OriginBushLUCE(LineSearch* lineSearch); // 显式构造函数，接受线搜索参数
		~OriginBushLUCE(); // 析构函数
		
		/** 调用OriginBush的improve()方法和额外的方法来准备LUCE算法
			特定的数据。
		*/
		virtual bool improve(); // 重写改进方法
		
	private:
		DAGraphLUCE* dag_; // LUCE有向无环图指针
		static LineSearch* lineSearch_; // 静态线搜索指针

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
									  FPType dirTol); // 创建新DAG的虚函数实现
		
		/** 什么都不做，因为LUCE只需要拓扑顺序而不需要最小或
			最大树。
		*/
		virtual void callBuildMinMaxTrees(){}; // 重写的空方法，避免构建最小最大树
};

#endif
