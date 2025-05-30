#include "OriginBasedAlgoTapas.h" // 包含TAPAS基于起点算法的头文件


OriginBasedAlgoTapas::OriginBasedAlgoTapas(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
						AddHook *component, ConvMeasure* conv, FPType timeLimit) : 
						OriginBasedAlgo(originSet, net, component, conv, timeLimit), 
						pasSet_(pasSet) { // 构造函数，初始化基类并设置PAS管理器

}; 

OriginBasedAlgoTapas::~OriginBasedAlgoTapas() { // 析构函数

}; 

void OriginBasedAlgoTapas::doSmthAfterOrigins() { // 重写钩子方法
	pasSet_->deleteUnusedPASAndMoveFlow(); // 删除未使用的PAS并移动流量
}; 

