#ifndef LABEL_CORR_AL  // 防止头文件重复包含
#define LABEL_CORR_AL

#include "ShortestPath.h"  // 包含最短路径基类

class StarNetwork;  // 前向声明星形网络类
class StarNode;     // 前向声明星形节点类

/** \brief 该类实现了标签修正算法，用于寻找从给定起点到网络中所有其他节点的最短路径。
    \note 算法实现了交通分配的特殊功能：
        最短路径不允许经过区域节点。
    \details 详细信息请参见 \cite Sheffi1985。
 */
class LabelCorrectingAl : public ShortestPath {  // 继承自最短路径基类
	
	public:
		LabelCorrectingAl(StarNetwork *netPointer);  // 构造函数
		~LabelCorrectingAl();  // 析构函数
		
		/** 计算从originIndex到网络中所有其他节点的最短路径。
		*/
		virtual void calculate(int originIndex);  // 计算最短路径的虚函数
		
		/** @return 从给定起点到destIndex的最短路径成本。
		*/
		virtual FPType getCost(int destIndex) const;  // 获取路径成本的虚函数
		/** @return 指向属于从给定起点到destIndex的最短路径且指向destIndex的链路的指针。
		*/
		virtual StarLink* getInComeLink(int destIndex) const;  // 获取入边链路的虚函数
		
		/** @return 最近计算最短路径的起点索引。
		*/
		int getCurrentOrigin() const;  // 获取当前起点的函数

	protected:
		/** 禁止最短路径经过区域的特殊条件。
			@return 如果允许访问curNode则返回true。
		*/
		virtual bool proceed(StarNode* curNode, int topNode) const;  // 判断是否继续的虚函数
	
	private:
		/** 内部工具结构，通常用于存储最短路径问题的解。
			由于它仅在此类中使用，因此该结构的定义直接在此头文件中。
 		*/
		struct nodeInfo {  // 节点信息结构体
			FPType dist; /**< 到节点的当前最短距离*/
			int linkIndex;  // 链路索引
		};
		
		StarNetwork *netPointer_;  // 网络指针
		nodeInfo *nodeList_;       // 节点信息列表
		const int nbNodes_;        // 节点数量
		int originIndex_;          // 起点索引
		
};

#endif
