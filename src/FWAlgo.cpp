#include "FWAlgo.h"  // 包含FWAlgo类的头文件
#include "Timer.h"   // 包含计时器类的头文件
#include "ConvMeasure.h"  // 包含收敛度量类的头文件
#include "LinkFlows.h"    // 包含链路流量类的头文件

// 构造函数：初始化FWAlgo对象
FWAlgo::FWAlgo(LinkFlows *linkFlows, AddHook *component, 
			StarNetwork *net, ConvMeasure* conv, FPType timeLimit) :
			EqAlgo(component, net, timeLimit, conv),  // 调用父类构造函数
			linkFlows_(linkFlows) {  // 初始化链路流量指针

};

// 析构函数
FWAlgo::~FWAlgo(){

};

// 初始化函数：初始化链路流量
void FWAlgo::initialise(){
	linkFlows_->initialise();
};

// 执行一次迭代
void FWAlgo::performOneIteration(){
	// 更新链路流量
	linkFlows_->updateLinkFlows();
	// 方向寻找
	linkFlows_->equilibrate(); // 线搜索和流量转移
};

// 检查是否收敛
bool FWAlgo::isConverged(){
	return conv_->isConverged(linkFlows_->getMinTravelTime());
};