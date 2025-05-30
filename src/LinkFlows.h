#ifndef LINK_FLOWS_  // 防止头文件重复包含
#define LINK_FLOWS_

#include "InitialiseSolution.h"  // 包含初始化解决方案的头文件
#include "AONUsual.h"  // 包含全有或全无分配的头文件

class LineSearch;  // 前向声明线搜索类
class ODMatrix;  // 前向声明OD矩阵类
class LabelCorrectingAl;  // 前向声明标签修正算法类
class StarNetwork;  // 前向声明星形网络类
class ShortestPath;  // 前向声明最短路径类

/** \brief 该类实现了基于链路的算法和Frank-Wolfe算法所需的基本操作
	\details 详细信息请参见 \cite Sheffi1985
	\warning 当精度较高时，该算法会出现收敛问题
*/
class LinkFlows : public InitialiseSolution {  // 定义LinkFlows类，继承自InitialiseSolution
	public:
		// 构造函数，初始化网络、OD矩阵、最短路径等参数
		LinkFlows(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
					LineSearch* lineSearch, FPType zeroFlow, 
					LabelCorrectingAl* LCShPath);
		virtual ~LinkFlows();  // 虚析构函数
		
		/** 计算链路成本，调用全有或全无分配，并用AON流量初始化链路流量（存储在数组中）
		*/
		void initialise();
		
		/** 计算下降方向和步长。然后通过修改内部数据结构计算新的链路流量
		*/
		void equilibrate();
		
		/** 将链路流量从内部数据结构加载到StarNetwork对象并更新链路成本
		*/
		void updateLinkFlows();

		/** @return 返回在全有或全无分配中计算的最小行程时间
		*/
		FPType getMinTravelTime() const;
		
		/** 通过将目的地\b dest的需求添加到现有链路流量中来初始化链路流量
		*/
	 	void initialiseItself(StarLink* link, PairOD *dest, int originIndex);
	 	
	protected:
		
		const int nbLinks_;  // 链路数量
		FPType *linkFlows_; /**< 当前链路流量 */
		FPType *linkFlowsAux_; /**< 当前全有或全无流量 */
		
		FPType stepPrev_; /**< 从线搜索获得的当前步长 */
		FPType stepPrevPrev_; /**< 前一步长，BFW算法需要用到 */

		const FPType zeroFlow_;  // 零流量阈值

		LineSearch* lineSearch_; /**< 线搜索指针是protected的，因为CFW和BFW需要访问链路成本函数以评估导数 */

		FPType *direction_; /**< 当前下降方向 \note 必须由子类直接修改 */

		StarNetwork *net_;  // 网络对象指针
		
		/** 计算全有或全无流量并将其存储到linkFlowsAux_中
		*/
		void calculateFWAux();

		/** 计算下降方向
		*/
		virtual void calculateDirection();

	private:
		AONUsual aon_;  // 全有或全无分配对象
		
		int *indexes_;  // 索引数组
		FPType minTravelTime_;  // 最小行程时间
		
};

#endif
