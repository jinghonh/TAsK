#ifndef _ORIGIN_BUSH_
#define _ORIGIN_BUSH_

#include "UsedTypes.h"

class DAGraph;
class PairOD;
class ODMatrix;
class StarNetwork;
class StarLink;

/** \brief 此类实现了菊花 - 基于起点算法的基本元素。
	\details 此类实现了工厂方法 - 子类分配必要的
	DAGraph对象。不直接使用DAGraph而是通过OriginBush，因为
	一些操作是通用的（所以减少了代码重复），同时一些
	操作可以被子类重写。
	例如，参见OriginBushTapas。 */
class OriginBush {
	
	public:
		
		/** 此类负责释放DAGraph对象。
		*/
		virtual ~OriginBush();
		
		/** 为DAGraph分配内存。 
			\note 必须在每个OriginBush创建后调用。
		*/
		void allocateDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol);
		
		/** 如果菊花在上一次迭代中发生变化，则重新计算拓扑顺序。
		*/
		virtual void updateTopSort();
		
		/** 根据某些基于起点的算法执行流量移动。
			所有新的流量都分配给StarNetwork对象，
			链接旅行时间也会更新
			（这个责任属于DAGraph类）。
		*/
		bool equilibrate();
		
		/** 尝试通过添加更好的链接来改进当前菊花。如果新链接被添加到菊花中，
			也会执行拓扑排序和最小最大树的构建。
			@return 如果当前菊花得到改进则返回true，否则返回false。
		*/
		virtual bool improve();
		
		/** 从菊花中移除未使用的链接。如果链接流量小于
			预定义常量ZERO_FLOW，则认为该链接未被使用。
		*/
		virtual void removeUnusedLinks();
		
		/** 将链接 \b link 添加到菊花。此方法还负责更新
			起点流量和链接流量。此方法在OriginSet中调用，
			以初始化菊花。
		*/
		void addLinkAndUpdateFlow(StarLink *link, PairOD* dest);
		
		/** 在屏幕上打印菊花信息。
		*/
		void print();
		/** @return 起点索引。
		*/
		int getOriginIndex() const;
		/** 用于调试。在屏幕上打印起点流量。
		*/
		void printOriginFlow() const;
		
		/** 用于调试。检查起点流量是否可行。
			@return 流量的最大偏差。
		*/
		FPType checkOFlowsFeasibility();
		
	protected:
		OriginBush();
			
	private:
		
		DAGraph *daGraph_;
		bool topSortUpToDate_;
		
		/** 用于创建DAGraph的工厂方法。
		*/
		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, 
			FPType dirTol) = 0;

		/** 在improve()方法中调用buildMinMaxTrees()。如果不需要，
			此方法可以被子类重写。例如，参见OriginBushLUCE。
		*/
		virtual void callBuildMinMaxTrees();
		
};

#endif
