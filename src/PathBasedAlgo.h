#ifndef _PATH_BASED_ALGO_
#define _PATH_BASED_ALGO_

#include "EqAlgo.h"              // 包含均衡算法基类
#include "PathSetPrinterToFile.h" // 包含路径集合文件打印器

class PathSet;                   // 前向声明路径集合类

/** \brief 这个类实现了基于路径算法的框架。
*/
class PathBasedAlgo : public EqAlgo {
	
	public:
		/** @param pathSet 所有路径的集合。
			@param component 执行额外收敛操作的对象。
			@param conv 收敛度量。
			@param net 网络。
			@param timeLimit 算法允许的最大执行时间(秒)。
			@param mat 起点-终点矩阵。
		*/
		PathBasedAlgo(PathSet *pathSet, AddHook *component, ConvMeasure *conv, 
					StarNetwork *net, FPType timeLimit, 
					ODMatrix* mat); 
		virtual ~PathBasedAlgo();  // 虚析构函数
		
	protected:
		/** 对给定的OD对执行一次迭代中的主要操作。
			它实现了平衡I。平衡II重写了此方法。
			详细信息参见PathBasedAlgoEQII。
			@return 如果当前OD对已平衡则返回true，否则返回false。
			返回值仅对平衡II有用。
		*/
		virtual bool mainLoop(ODSet *odSet);

	private:
		PathSet *pathSet_;           // 路径集合指针
		ODMatrix* mat_{};              // OD矩阵指针
		PathSetPrinterToFile printPathSet_;  // 路径集合文件打印器

		virtual void initialise();           // 初始化方法
		virtual void performOneIteration();  // 执行一次迭代方法
		
};

#endif
