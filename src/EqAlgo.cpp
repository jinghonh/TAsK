#include "EqAlgo.h"     // 包含平衡算法头文件
#include "AddHook.h"    // 包含添加钩子组件头文件
#include "Timer.h"      // 包含计时器头文件
#include "ConvMeasure.h"// 包含收敛性度量头文件

#include <cassert>      // 包含断言功能
#include <iostream>     // 包含标准输入输出流


// 构造函数: 初始化平衡算法
// component: 添加钩子组件
// net: 星型网络指针
// timeLimit: 时间限制
// conv: 收敛性度量指针
EqAlgo::EqAlgo(AddHook *component, StarNetwork* net, FPType timeLimit, 
		ConvMeasure* conv) : 
		conv_(conv), net_(net), component_(component), timeLimit_(timeLimit), totalConvTime_(0.0),
		totalPureIterTime_(0.0), initialisationTime_(0.0) {
			assert(net_ != nullptr);  // 断言确保网络指针不为空
			
		};


EqAlgo::~EqAlgo() = default;  // 析构函数，无具体实现
		
// 获取总收敛检查时间
FPType EqAlgo::getTotalConvTime(){
	return totalConvTime_;
};

// 获取总纯迭代计算时间
FPType EqAlgo::getTotalPureIterTime(){
	return totalPureIterTime_;
};

// 获取初始化所用时间
FPType EqAlgo::getInitialisationTime() {
	return initialisationTime_;
};

// 执行平衡算法的主函数
// 返回值: 迭代次数
int EqAlgo::execute() {
	std::cout << "Initialisation" << std::endl;  // 输出初始化开始信息
	Timer initTimer;                             // 创建初始化计时器
	initTimer.start();                           // 开始计时初始化过程
	initialise();                                // 调用初始化方法
	initialisationTime_ = initTimer.getTimePassed();  // 记录初始化所用时间
	std::cout << "Initialisation done" << std::endl;  // 输出初始化完成信息
	
	int nbIter = 0;                              // 迭代计数器初始化为0
	Timer timer;                                 // 创建总时间计时器
	FPType timePassed = 0.0;                     // 初始化已用时间为0
	Timer convTimer;                             // 创建收敛检查计时器
	FPType convTime = 0.0;                       // 初始化收敛检查总时间为0
	bool algoConverged = false;                  // 初始化算法收敛状态为false
	while (true) {                               // 开始主迭代循环
		
		++nbIter;                                // 迭代次数加1
		
		// 全局收敛性检查
		convTimer.start();                       // 开始收敛检查计时
		algoConverged = isConverged();           // 检查算法是否已收敛
		convTime += convTimer.getTimePassed();   // 累加收敛检查所用时间
		if (algoConverged){                      // 如果算法已收敛
			break;                               // 跳出循环
		}
		
		timer.start();                           // 开始迭代计时
		performOneIteration();                   // 执行一次迭代计算

		timePassed += timer.getTimePassed();     // 累加迭代计算所用时间

		// 计算链路流量变化
		// 和其他计数器(同时以字符串格式存储)
		component_->produceOut(timePassed, conv_->getGap());  // 调用组件的输出方法

		// 检查是否达到时间限制
		if (convTime + timePassed >= timeLimit_) {  // 如果总用时达到或超过时间限制
			convTimer.start();                      // 进行最后一次收敛检查
			algoConverged = isConverged();          // 检查算法是否已收敛
			convTime += convTimer.getTimePassed();  // 累加收敛检查时间
			break;                                  // 跳出循环
		}
	}
	component_->produceOut(timePassed, conv_->getGap());  // 最后一次调用组件的输出方法
	totalConvTime_ = convTime;                     // 记录总收敛检查时间
	totalPureIterTime_ = timePassed;               // 记录总纯迭代时间
	return nbIter;                                 // 返回总迭代次数
};

// 检查算法是否已收敛
// 返回值: 是否收敛的布尔值
bool EqAlgo::isConverged(){
	return conv_->isConverged();  // 调用收敛度量对象的检查方法
}
