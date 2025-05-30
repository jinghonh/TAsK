#include "StepSizeAdd.h" // 包含加性步长头文件
#include "DescDirection.h" // 包含下降方向头文件
#include "Derivative.h" // 包含导数头文件

StepSizeAdd::StepSizeAdd(LineSearch *lineSearch, Derivative *der) : 
			StepSize(lineSearch), der_(der){ // 构造函数，初始化基类和导数对象

};

StepSizeAdd::~StepSizeAdd(){ // 析构函数

};

void StepSizeAdd::initialiseDerivative(DescDirection *algo){ // 初始化导数方法的实现
	der_->setDataPointers(algo->getSize(), algo->getLinkFlows(), // 设置导数对象的数据指针
						  algo->getProjectedDirection(), algo->getIndexes());
};
