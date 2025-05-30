#include "BoundsCalculatorForBackwardNet.h"  // 包含后向网络边界计算器头文件
#include "StarNetworkWithTolls.h"           // 包含带收费的星形网络头文件
#include "StarNetwork.h"                    // 包含星形网络头文件

// 构造函数：初始化后向网络边界计算器
BoundsCalculatorForBackwardNet::BoundsCalculatorForBackwardNet(StarNetwork& net, 
			const TollContainerType& tolls) : 
			BoundsCalculatorForBSP(net, tolls),  // 调用父类构造函数
			net_(&net), backwardNet_(NULL) {     // 初始化网络指针和后向网络指针

};

// 析构函数：释放后向网络资源
BoundsCalculatorForBackwardNet::~BoundsCalculatorForBackwardNet() {
	delete backwardNet_;  // 删除后向网络对象
};
		
// 获取网络：创建并返回后向网络
StarNetwork* BoundsCalculatorForBackwardNet::getNetwork(StarNetwork& net) {
	backwardNet_ = BoundsCalculatorForBSP::getNetwork(net);  // 调用父类方法获取网络
	std::cout << "为后向边界创建前向网络" << std::endl;      // 输出创建信息
	return net_;  // 返回网络指针
};

// 获取带收费的网络：创建并返回带收费的后向网络
StarNetwork* BoundsCalculatorForBackwardNet::getNetworkWithTolls(StarNetwork& net,
						const TollContainerType& tolls) {
	
	StarNetworkWithTolls netWithTolls(tolls);  // 创建带收费的网络对象
	std::cout << "为后向边界创建前向网络" << std::endl;      // 输出创建信息
	return netWithTolls.createNetWithTolls(net);  // 返回带收费的网络
};

// 获取对向网络：返回后向网络指针
StarNetwork* BoundsCalculatorForBackwardNet::getOppositeNet() {
	return backwardNet_;  // 返回后向网络指针
};
