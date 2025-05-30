#include "PathBasedFlowMovePE.h"  // 包含投影平衡路径流量移动头文件
#include "PathAndDirection.h"      // 包含路径及方向头文件
#include "Path.h"                 // 包含路径头文件

// 构造函数：初始化投影平衡路径流量移动类
// pathDirectionAlgo: 下降方向路径算法指针
PathBasedFlowMovePE::PathBasedFlowMovePE(DescDirectionPath* pathDirectionAlgo):
										 PathBasedFlowMove(pathDirectionAlgo){  // 调用父类构造函数

};

// 析构函数，无具体实现
PathBasedFlowMovePE::~PathBasedFlowMovePE(){

};
		
// 执行流量移动
// 返回值: 是否达到平衡状态(始终返回false)
bool PathBasedFlowMovePE::executeFlowMove(){
	Path *maxPath = (pathDirection_[0])->getPath();  // 获取第一个路径（最大成本路径）
	Path *minPath = (pathDirection_[1])->getPath();  // 获取第二个路径（最小成本路径）
	FPType maxDir = (pathDirection_[0])->getDirectionValue();  // 获取最大成本路径的方向值
	FPType flow = maxPath->getFlow();  // 获取最大成本路径的当前流量
	if (flow < -maxDir) {  // 如果流量小于负方向值
		maxPath->setFlow(0.0);  // 将最大成本路径的流量设置为0
		minPath->setFlow(minPath->getFlow() + flow);  // 将最大成本路径的流量转移到最小成本路径
	} else {
		maxPath->setFlow(flow + maxDir);  // 更新最大成本路径的流量
		minPath->setFlow(minPath->getFlow() - maxDir);  // 对应减少最小成本路径的流量
	}
	return false;  // 返回未平衡
};
