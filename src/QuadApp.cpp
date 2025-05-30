#include "QuadApp.h" // 包含头文件
#include "StarNetwork.h" // 包含星形网络类
#include "Derivative.h" // 包含导数类
#include "StarLink.h" // 包含星形链接类

#include <stdlib.h> // 包含标准库

QuadApp::QuadApp(Derivative *der, StarNetwork* net) : 
					LineSearch(der), net_(net) { // 构造函数，初始化基类和成员变量
	
};

QuadApp::~QuadApp(){ // 析构函数

};

FPType QuadApp::execute(FPType a, FPType b){ // 执行二次近似计算的方法
	
	FPType sum1 = 0.0; // 初始化第一个求和变量
	FPType sum2 = 0.0; // 初始化第二个求和变量
	StarLink* link = NULL; // 初始化链接指针
	Derivative &der = *der_; // 获取导数引用
	int size = der.getSize(); // 获取导数大小
	FPType y = 0.0; // 初始化y变量
	for (int i = 0; i < size; ++i) { // 遍历所有元素
		link = net_->getLink(der.getLinkIndex(i)); // 获取链接
		y = der.getY(i); // 获取y值
		
		sum1 += link->getTime() * y; // 计算第一个求和
		sum2 += link->getDerivative() * y * y; // 计算第二个求和
	}
	if (sum1 >= 0.0) { // 如果第一个求和大于等于0
		
		return 0.0; // 返回0
	} 
	FPType returnVal = -sum1 / sum2; // 计算返回值
	if (returnVal > b) returnVal = b; // 如果返回值大于b，则返回b
	return returnVal; // 返回计算结果
};
