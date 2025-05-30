// 包含头文件
#include "LabelSettingOneSourceBSP.h"

// 包含输入输出流头文件
#include <iostream>

// 构造函数实现
// 参数: net - 网络对象引用, tolls - 收费容器常量引用
// 初始化列表调用父类构造函数
LabelSettingOneSourceBSP::LabelSettingOneSourceBSP(StarNetwork& net, 
		const TollContainerType& tolls) : OneSourceBiObjShPath(net.getNbNodes()), 
		LabelSettingBSPBase(net, tolls) {

		// 输出创建对象的信息
		std::cout << "LabelSettingOneSourceBSP is created" << std::endl;

};

// 析构函数实现
LabelSettingOneSourceBSP::~LabelSettingOneSourceBSP() {

};

// 计算函数实现
// 参数: originIndex - 源节点索引
void LabelSettingOneSourceBSP::calculate(int originIndex){
	// 调用基类的BSP计算方法
	calculateBSP(originIndex, labels_);
};