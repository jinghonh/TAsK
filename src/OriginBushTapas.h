#ifndef ORIGIN_BUSH_TAPAS
#define ORIGIN_BUSH_TAPAS

#include "OriginBush.h" // 包含起点菊花基类

class DAGraphTapas; // 前向声明TAPAS有向无环图类
class PASManager; // 前向声明PAS管理器类
class ShortestPath; // 前向声明最短路径类

/** \brief 此类负责DAGraphTapas的创建。
	它还重写了OriginBush的一些对TAPAS算法
	不同的方法。
*/
class OriginBushTapas : public OriginBush { // TAPAS起点菊花类，继承自OriginBush
	public:
		OriginBushTapas(ShortestPath *shPath, PASManager *pasManager); // 构造函数，接受最短路径和PAS管理器参数
		~OriginBushTapas(); // 析构函数
		
		/** 移除循环流量。
		*/
		virtual void updateTopSort(); // 重写更新拓扑排序方法
		
		/** 什么都不做，始终返回false（TAPAS的平衡II在 
			OriginBasedAlgoTapasEqII中实现）。
		*/
		virtual bool improve() {return false;}; // 重写的空改进方法
		
		/** 什么都不做。*/
		virtual void removeUnusedLinks() {}; // 重写的空移除未使用链接方法
	
	private:
		DAGraphTapas *dag_; // TAPAS有向无环图指针
		static ShortestPath *shPath_; // 静态最短路径指针
		static PASManager *pasManager_; // 静态PAS管理器指针

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									  FPType zeroFlow, FPType dirTol); // 创建新DAG的虚函数实现
};

#endif
