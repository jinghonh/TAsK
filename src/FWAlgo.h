#ifndef FW_ALGO  // 防止头文件重复包含
#define FW_ALGO

#include "EqAlgo.h"  // 包含基类头文件

class LinkFlows;  // 前向声明LinkFlows类

/** \brief 该类实现了基于链路算法框架的主要步骤。
    \details 预期该类只会有一个实例。
 */
class FWAlgo : public EqAlgo {  // 定义FWAlgo类，继承自EqAlgo
	
	public:
	
		/** @param linkFlows 包含给定基于链路算法具体实现的对象
			@param component 执行收敛信息额外操作的对象，参见AddHook
			@param net 网络
			@param conv 收敛度量
			@param timeLimit 时间限制（秒）
		*/
		FWAlgo(LinkFlows *linkFlows, AddHook *component, StarNetwork *net, ConvMeasure* conv,
					FPType timeLimit);  // 构造函数
		~FWAlgo();  // 析构函数
		
	private:
		LinkFlows *linkFlows_;  // 链路流量对象指针

		virtual void initialise();  // 初始化虚函数
		virtual void performOneIteration();  // 执行一次迭代的虚函数
		virtual bool isConverged();  // 检查是否收敛的虚函数
};

#endif  // 结束头文件保护
