#include "AstarWithoutZoneSkip.h"  // 包含AstarWithoutZoneSkip类的头文件

// 构造函数实现
// @param net 网络指针
// @param odMatrix OD矩阵指针
AstarWithoutZoneSkip::AstarWithoutZoneSkip(StarNetwork* net,
			ODMatrix* odMatrix) : Astar(net, odMatrix) {
};

// 析构函数实现
AstarWithoutZoneSkip::~AstarWithoutZoneSkip() {

};

// 判断是否应该跳过该区域
// @param node 节点指针
// @param nodeIndex 节点索引
// @param originIndex 起点索引
// @return 返回false表示不跳过任何区域
bool AstarWithoutZoneSkip::shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const {
	return false;
};