// 包含头文件
#include "LabelCorrectingAlWithoutZonesCheck.h"

// 构造函数实现，初始化基类
LabelCorrectingAlWithoutZonesCheck::LabelCorrectingAlWithoutZonesCheck(StarNetwork *netPointer) :
					LabelCorrectingAl(netPointer) {

};

// 析构函数实现
LabelCorrectingAlWithoutZonesCheck::~LabelCorrectingAlWithoutZonesCheck(){

};

// proceed方法实现，检查当前节点是否为空
bool LabelCorrectingAlWithoutZonesCheck::proceed(StarNode* curNode, int topNode) const {
	return (curNode != NULL);
};
