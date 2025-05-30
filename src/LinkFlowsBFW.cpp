#include "LinkFlowsBFW.h"  // 包含BFW算法头文件
#include "StarNetwork.h"   // 包含网络结构头文件

#include <math.h>          // 包含数学函数库

// BFW算法构造函数
LinkFlowsBFW::LinkFlowsBFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, 
		FPType zeroFlow, LabelCorrectingAl* LCShPath) : 
		LinkFlowsCFW(net, mat, shPath, lineSearch, zeroFlow, LCShPath) { 
	linkFlowsBFW_ = new FPType[nbLinks_];  // 为BFW流量分配数组分配内存
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsBFW_[i] = 0.0;  // 初始化BFW流量为0
	}
};

// 析构函数
LinkFlowsBFW::~LinkFlowsBFW(){
	delete[] linkFlowsBFW_;  // 释放BFW流量数组内存
};
		
// 计算搜索方向
void LinkFlowsBFW::calculateDirection(){
	
	if (nbCalls_ == 0) { // 第一次迭代使用FW方向
		LinkFlows::calculateDirection();  // 调用基类方法计算方向

		for (int i = 0; i < nbLinks_; ++i) {
			linkFlowsCFW_[i] =  linkFlowsAux_[i];  // 更新CFW流量
			linkFlowsBFW_[i] = 0.0;  // 重置BFW流量
		}

	} else if (nbCalls_ == 1) { // 第二次迭代使用CFW方向
		for (int i = 0; i < nbLinks_; ++i) {
			linkFlowsBFW_[i] = linkFlowsCFW_[i];  // 保存当前CFW流量到BFW
		}
		LinkFlowsCFW::calculateDirection();  // 调用CFW方法计算方向

	} else {
		calculateFWAux();  // 计算辅助FW流量
		
		FPType beta0 = 0.0;  // 初始化系数
		FPType beta1 = 0.0;
		FPType beta2 = 0.0;
		calcCoeff(beta0, beta1, beta2);  // 计算组合系数
		FPType promVal = 0.0;
		for (int i = 0; i < nbLinks_; ++i) {
			promVal = beta0 * linkFlowsAux_[i] + beta1 * linkFlowsCFW_[i] + beta2 * linkFlowsBFW_[i];  // 计算组合流量
			linkFlowsBFW_[i] = linkFlowsCFW_[i];  // 更新BFW流量
			linkFlowsCFW_[i] = promVal;  // 更新CFW流量
			direction_[i] = linkFlowsCFW_[i] - linkFlows_[i];  // 计算搜索方向
		}
	}
	++nbCalls_;  // 增加迭代次数计数
};

// 计算组合系数
void LinkFlowsBFW::calcCoeff(FPType &beta0, FPType &beta1, FPType &beta2){
	if ((fabs(1.0 - stepPrev_) <= zeroFlow_) || (fabs(1.0 - stepPrevPrev_) <= zeroFlow_) ) {
		beta0 = 1.0;  // 如果步长接近1，使用FW方向
		beta1 = 0.0;
		beta2 = 0.0;
	} else {
		FPType nk = 0.0;  // 初始化计算变量
		FPType dk = 0.0;
		FPType nnk = 0.0;
		FPType ddk = 0.0;
		FPType der = 0.0;
		FPType dir_fw = 0.0;
		FPType dir_2 = 0.0;
		FPType dir_1 = 0.0;
		for (int i = 0; i < nbLinks_; ++i) {
			der = (net_->getLink(i))->getDerivative();  // 获取链路导数
			dir_fw = linkFlowsAux_[i] - linkFlows_[i];  // 计算FW方向
			dir_2 = stepPrev_ * linkFlowsCFW_[i] - linkFlows_[i] + (1 - stepPrev_) * linkFlowsBFW_[i];  // 计算二阶方向
			dir_1 = linkFlowsCFW_[i] - linkFlows_[i];  // 计算一阶方向
			nk += dir_2 * der * dir_fw;  // 累加计算系数
			dk += dir_2 * der * (dir_2 - dir_1) / (1 - stepPrev_);
			nnk += dir_1 * der * dir_fw;
			ddk += dir_1 * dir_1 * der;
		}
		if ((fabs(dk) <= zeroFlow_) || (fabs(ddk) <= zeroFlow_) ) { 
			beta0 = 1.0;  // 如果分母接近0，使用FW方向
			beta1 = 0.0;
			beta2 = 0.0;
		} else {
			FPType mu = -nk / dk;  // 计算mu系数
			FPType eta = -nnk / ddk + (mu * stepPrev_) / (1.0 - stepPrev_);  // 计算eta系数
			beta0 = 1.0 / (1.0 + mu + eta);  // 计算beta0
			if ((beta0 >= 0.0) && (beta0 <= 1.0 - zeroFlow_)) {
				beta1 = eta * beta0;  // 计算beta1
				beta2 = mu * beta0;  // 计算beta2
				
				if ((beta1 < 0.0) || (beta1 > 1.0 - zeroFlow_) || (beta2 < 0.0) || (beta2 > 1.0 - zeroFlow_)) {
					beta0 = 1.0;  // 如果系数超出范围，使用FW方向
					beta1 = 0.0;
					beta2 = 0.0;
				}
			} else {
				beta0 = 1.0;  // 如果beta0超出范围，使用FW方向
				beta1 = 0.0;
				beta2 = 0.0;
			}
			
		}
	}
};
