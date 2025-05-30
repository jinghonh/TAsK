#include "LinkFlowsCFW.h"  // 包含LinkFlowsCFW类的头文件
#include "StarNetwork.h"   // 包含StarNetwork类的头文件

#include <math.h>          // 包含数学函数库

// 构造函数：初始化LinkFlowsCFW对象
LinkFlowsCFW::LinkFlowsCFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
		LineSearch* lineSearch, FPType zeroFlow, LabelCorrectingAl* LCShPath) : 
		LinkFlows(net, mat, shPath, lineSearch, zeroFlow, LCShPath), nbCalls_(0) {
	linkFlowsCFW_ = new FPType[nbLinks_];  // 为CFW链路流分配内存
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsCFW_[i] = 0.0;  // 初始化所有链路流为0
	}
};

// 析构函数：释放动态分配的内存
LinkFlowsCFW::~LinkFlowsCFW(){
	delete[] linkFlowsCFW_;  // 释放CFW链路流数组
};


// 计算方向的方法
void LinkFlowsCFW::calculateDirection(){
	
	calculateFWAux();  // 计算辅助流量
	
	FPType alpha = 0.0;  // 初始化alpha参数
	if (nbCalls_ > 0) alpha = calculateAlpha();  // 如果不是第一次调用，计算alpha
	FPType beta = 1.0 - alpha;  // 计算beta参数
	
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsCFW_[i] = alpha * linkFlowsCFW_[i] + beta * linkFlowsAux_[i];  // 更新CFW链路流
		direction_[i] = linkFlowsCFW_[i] - linkFlows_[i];  // 计算方向
	}
	++nbCalls_;  // 增加调用次数
	
};

// 计算alpha参数的方法
FPType LinkFlowsCFW::calculateAlpha(){

	FPType numerator = 0.0;    // 初始化分子
    FPType denominator = 0.0;  // 初始化分母
    for (int i = 0; i < nbLinks_; ++i) {
      	FPType const dkfw = linkFlowsAux_[i] - linkFlows_[i];          // d_k^FW = y_k^FW - x_k
    	FPType const dkMinus1_bar = linkFlowsCFW_[i] - linkFlows_[i];  // d_{k-1}^BAR = s_{k-1}^CFW  - x_k
    	FPType const hk = net_->getLink(i)->getDerivative();           // 获取链路导数
   		numerator += dkMinus1_bar * hk * dkfw;                        // 累加分子项
    	denominator += dkMinus1_bar * hk * (dkfw - dkMinus1_bar);     // 累加分母项
  	}
  	FPType const val = numerator / denominator;  // 计算alpha值
	if (val > 1.0 - zeroFlow_) return 1.0 - zeroFlow_;  // 如果值过大，返回上限
	if (val >= 0.0) return val;  // 如果值在有效范围内，返回该值
	return 0.0;	 // 如果值为负，返回0
};
