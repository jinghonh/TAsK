#ifndef DAGRAPH_B
#define DAGRAPH_B

#include "DAGraph.h"

class Path;

/** \brief 该类实现基于起点的算法B。
	\details 详细内容见 \cite Dial_2006.
*/
class DAGraphB : public DAGraph {
	public: 
		
		/** 
			@param net 指向 StarNetwork 对象的指针
			@param mat 指向 ODMatrix 对象的指针
			@param zeroFlow 零流量阈值
			@param dirTol 方向容差，用于判断下降方向是否有效
			@param originIndex 起点索引
			@param useMultiStep 为true时表示应用多步牛顿法，为false时表示不应用
		*/
		DAGraphB(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
				 int originIndex, bool useMultiStep);
		~DAGraphB();
		
		bool moveFlow();
		
	protected:
		/** 实现牛顿法流量步长计算。见DAGraphBWithStep为其他 
			step size实现。
		*/
		virtual FPType calcFlowStep(Path* minPath, Path* maxPath) const;

		int nbIter;
	private:
		
		static bool useMultiStep_;
		static FPType dirTol_;

		
		/** 执行一次流量移动：将流量从属于bush且起于origin、止于index节点的最长路径移动到最短路径 
		 	假设minPath 和maxPath无公共边。
		*/
		bool performFlowMove(int index, int origin);
		
		/** 给link增加流量dFlow，更新对应的起点流量并
			重新计算link的旅行时间。
		*/
		void addFlow(StarLink *link, FPType dFlow);

		/** 调试用，打印到指定节点index的最短路径上的各链接的起点流量。 */
		void printMinPathOFlows(int index) const;
		/** 调试用，打印到指定节点index的最长路径上的各链接的起点流量。 */
		void printMaxPathOFlows(int index) const;
		
};

#endif
