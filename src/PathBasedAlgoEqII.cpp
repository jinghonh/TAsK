#include "PathBasedAlgoEqII.h"  // 包含PathBasedAlgoEqII类头文件

// 构造函数：初始化基于路径的平衡算法II
// pathSet: 路径集合指针
// component: 钩子组件指针
// conv: 收敛性度量指针
// net: 星型网络指针
// timeLimit: 时间限制
// maxNbIter: 最大迭代次数
// mat: OD矩阵指针
PathBasedAlgoEqII::PathBasedAlgoEqII(PathSet *pathSet, AddHook *component, 
			ConvMeasure *conv, StarNetwork *net, 
			FPType timeLimit, int maxNbIter, ODMatrix* mat) : 
			PathBasedAlgo(pathSet, component, conv, net, timeLimit, mat), // 调用父类构造函数
			maxNbIter_(maxNbIter) {  // 初始化最大迭代次数

}; 

// 析构函数，无具体实现
PathBasedAlgoEqII::~PathBasedAlgoEqII() {

}; 

// 主循环函数，在给定的OD集合上执行算法迭代直到平衡或达到最大迭代次数
// odSet: 需要平衡的OD集合指针
// 返回值: 是否达到平衡状态
bool PathBasedAlgoEqII::mainLoop(ODSet *odSet) {
	bool isEquilibrated = false;  // 初始化平衡状态为假
	int nbIter = 0;               // 迭代计数器初始化为0
	while (!isEquilibrated) {     // 循环直到达到平衡状态
		++nbIter;                 // 增加迭代计数器
		isEquilibrated = PathBasedAlgo::mainLoop(odSet);  // 调用父类的mainLoop方法
		if (nbIter >= maxNbIter_) return false;  // 如果达到最大迭代次数，返回未平衡
	}
	return isEquilibrated;        // 返回平衡状态
}; 

