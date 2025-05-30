#include "PathBasedAlgo.h"  // 包含基于路径的算法头文件
#include "ODSet.h"        // 包含起点-终点集合头文件
#include "PathSet.h"      // 包含路径集合头文件

// 构造函数: 初始化基于路径的算法
// pathSet: 路径集合指针
// component: 添加钩子组件指针
// conv: 收敛性度量指针
// net: 星型网络指针
// timeLimit: 时间限制
// mat: OD矩阵指针
PathBasedAlgo::PathBasedAlgo(PathSet *pathSet, AddHook *component, ConvMeasure *conv,
						StarNetwork *net, const FPType timeLimit, ODMatrix* mat) : EqAlgo(component, net, timeLimit, conv),
						                                                     // 调用父类构造函数
						                                                     pathSet_(pathSet), mat_(nullptr),
						                                                     printPathSet_(mat) {
	// 初始化路径集合和打印路径集合
};

// 析构函数，无具体实现
PathBasedAlgo::~PathBasedAlgo()= default;

// 主循环函数，处理单个OD集合
// odSet: 起点-终点集合指针
// 返回值: 是否达到平衡状态的布尔值
bool PathBasedAlgo::mainLoop(ODSet *odSet){
	odSet->updateSet();                     // 更新OD集合
	const bool wasImproved = odSet->improveSet(); // 尝试改进OD集合，返回是否有改进
	if (wasImproved || (odSet->getNbPaths() > 1)) {  // 如果有改进或路径数大于1
		if (odSet->equilibrate()) {         // 尝试使OD集合平衡
			odSet->removeUnusedPaths();     // 移除未使用的路径
			return true;                    // 返回已达到平衡
		}
		odSet->projectPathFlowOnLinks();    // 将路径流量投影到链路上
		odSet->removeUnusedPaths();         // 移除未使用的路径
	} else { 
		return true;                        // 如果没有改进且路径数不大于1，则已平衡
	}
	return false;                           // 未达到平衡状态
};

// 初始化函数，由父类EqAlgo调用
void PathBasedAlgo::initialise(){
	pathSet_->initialise();                 // 初始化路径集合
	
};

// 执行一次迭代的函数，由父类EqAlgo调用
void PathBasedAlgo::performOneIteration(){
	for (const auto odSetIt : *pathSet_){
		mainLoop(odSetIt);                 // 对每个OD集合执行主循环
	}
};