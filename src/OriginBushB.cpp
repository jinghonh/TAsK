#include "OriginBushB.h" // 包含OriginBushB头文件
#include "DAGraphBWithStep.h" // 包含带步长的DAGraphB头文件
#include "DAGraphB.h" // 包含DAGraphB头文件

bool OriginBushB::useMultiStep_ = false; // 初始化静态多步骤标志为false
LineSearch* OriginBushB::lineSearch_ = NULL; // 初始化静态线搜索指针为NULL

OriginBushB::OriginBushB(bool useMultiStep, LineSearch* lineSearch) { // 构造函数
	useMultiStep_ = useMultiStep; // 设置是否使用多步骤
	lineSearch_ = lineSearch; // 设置线搜索对象
};

OriginBushB::~OriginBushB(){ // 析构函数

};
		
DAGraph* OriginBushB::createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
								   FPType dirTol){ // 重写创建新DAG的工厂方法
	if (lineSearch_ == NULL) // 如果线搜索为空
		return new DAGraphB(net, mat, zeroFlow, dirTol, index, useMultiStep_); // 返回新的DAGraphB
	return new DAGraphBWithStep(net, mat, zeroFlow, dirTol, index, useMultiStep_, lineSearch_); // 否则返回带步长的DAGraphB
};
