#ifndef LINK_FLOWS_CFW  // 防止头文件重复包含
#define LINK_FLOWS_CFW

#include "LinkFlows.h"  // 包含基类头文件

/** \brief 该类实现了共轭Frank-Wolf算法
	\details 详细信息请参见 \cite Mitradjieva2012
	\warning 当精度较高时，该算法会出现收敛问题。
	此外，当在巴塞罗那实例上使用递减因子为2的Armijo步长时，算法无法收敛。
*/
class LinkFlowsCFW : public LinkFlows {  // 定义LinkFlowsCFW类，继承自LinkFlows
	
	public:
		// 构造函数，接收网络、OD矩阵、最短路径、线搜索、零流量阈值和标签修正算法参数
		LinkFlowsCFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, 
			FPType zeroFlow, LabelCorrectingAl* LCShPath);
		virtual ~LinkFlowsCFW();  // 虚析构函数
		
	protected:
		
		FPType *linkFlowsCFW_; /**< CFW链路流数组 */
		int nbCalls_; /**< 下降方向计算的次数，用于初始迭代 */
		
		virtual void calculateDirection();  // 计算下降方向的虚函数
		
	private:
		/** 该方法计算CFW算法的特定参数，详见 \cite Mitradjieva2012
		*/
		FPType calculateAlpha();  // 计算alpha参数的函数
};

#endif  // 结束头文件保护
