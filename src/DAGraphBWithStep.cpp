#include "DAGraphBWithStep.h" // 包含 DAGraphBWithStep 类的头文件
#include "LineSearch.h" // 包含 LineSearch 类的头文件
#include "Path.h" // 包含 Path 类的头文件
#include "StarNetwork.h" // 包含 StarNetwork 类的头文件
#include "Derivative.h" // 包含 Derivative 类的头文件 (可能用于线搜索的导数计算)
#include "StarLink.h" // 包含 StarLink 类的头文件

#include <limits> // 引入数值极限库
#include <cassert> // 引入断言库

LineSearch* DAGraphBWithStep::lineSearch_ = NULL; // 静态成员变量，指向 LineSearch 对象的指针，初始化为 NULL

// DAGraphBWithStep 类的构造函数
// net: 指向 StarNetwork 对象的指针
// mat: 指向 ODMatrix 对象的指针
// zeroFlow: 零流量阈值
// dirTol: 方向容差
// originIndex: 起点索引
// useMultiStep: 是否使用多步牛顿法
// lineSearch: 指向 LineSearch 对象的指针，用于线搜索计算步长
DAGraphBWithStep::DAGraphBWithStep(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol, 
					int originIndex, bool useMultiStep, LineSearch* lineSearch) : 
					DAGraphB(net, mat, zeroFlow, dirTol, originIndex, useMultiStep) { // 调用基类 DAGraphB 的构造函数
	lineSearch_ = lineSearch; // 初始化线搜索对象指针
}; // 构造函数结束

// DAGraphBWithStep 类的析构函数
DAGraphBWithStep::~DAGraphBWithStep(){

}; // 析构函数结束

// 计算流量移动的步长 (使用线搜索方法)
// minPath: 指向最短路径对象的指针
// maxPath: 指向最长路径对象的指针
// 返回计算出的流量步长
FPType DAGraphBWithStep::calcFlowStep(Path* minPath, Path* maxPath) const{
	int nbLinks = net_->getNbLinks(); // 获取网络中的路段总数
	FPType x[nbLinks]; // 存储各路段当前流量的数组
	FPType y[nbLinks]; // 存储各路段下降方向的数组 (d_k)
	int indexes[nbLinks]; // 存储参与计算的路段索引的数组
	
	// 初始化y数组中与最长路径和最短路径相关的元素为0
	for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){ // 遍历最长路径
		y[(*it)->getIndex()] = 0.0; // 将对应路段的y值初始化为0
	} // 循环结束
	for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) { // 遍历最短路径
		y[(*it)->getIndex()] = 0.0; // 将对应路段的y值初始化为0
	} // 循环结束
	
	int linkIndex = -1; // 初始化路段索引变量
	int size = 0; // 初始化参与计算的路段数量
	
	// 将路径方向投影到路段上
	FPType descDirection = minPath->getCurrCost() - maxPath->getCurrCost(); // 计算下降方向值 (最短路径成本 - 最长路径成本，应为负或零)

	FPType oFlow = 0.0; // 用于存储路段的起点流量
	FPType maxPathFlow = std::numeric_limits<FPType>::infinity( ); // 初始化最长路径上的最小流量（作为步长的隐式上界）
	for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){ // 遍历最长路径
		StarLink* link = *it; // 获取当前路段
		linkIndex = link->getIndex(); // 获取路段索引
		x[linkIndex] = link->getFlow(); // 存储当前路段流量到x数组
		y[linkIndex] += descDirection;    // 更新y数组，表示从该路段移出流量 (descDirection为负)
		indexes[size] = linkIndex; // 将路段索引加入到indexes数组
		++size; // 参与计算的路段数量加1
		oFlow = getOriginFlow(linkIndex); // 获取当前路段的起点流量
		if (oFlow < maxPathFlow) maxPathFlow = oFlow; // 更新最长路径上的最小流量
	} // 循环结束
	if (maxPathFlow == 0.0) { // 如果最长路径上存在流量为0的路段，则无法移出流量
		return 0.0; // 返回步长0
	} // 条件判断结束

	for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) { // 遍历最短路径
		StarLink* link = *it; // 获取当前路段
		linkIndex = link->getIndex(); // 获取路段索引
		x[linkIndex] = link->getFlow(); // 存储当前路段流量到x数组
		y[linkIndex] -= descDirection; // 更新y数组，表示向该路段移入流量 (descDirection为负，所以是减去负值，即增加)
		indexes[size] = linkIndex; // 将路段索引加入到indexes数组
		++size; // 参与计算的路段数量加1
	} // 循环结束
	
	(lineSearch_->getDerivative())->setDataPointers(size, x, y, indexes); // 设置线搜索导数计算所需的数据指针
	
	// 计算线搜索的上限
	FPType ub = -maxPathFlow / descDirection; // 步长的上限，确保最长路径上的流量不会变为负值 (descDirection为负，所以-maxPathFlow/descDirection为正)
	assert(ub > 0.0); // 断言上限大于0
	return (lineSearch_->execute(0.0, ub) * -descDirection); // 执行线搜索，返回最优步长乘以(-descDirection)得到实际流量移动量
}; // 函数结束