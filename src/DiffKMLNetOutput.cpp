// 包含必要的头文件
#include "DiffKMLNetOutput.h"
#include "StarLink.h"
#include "StarNetwork.h"

#include <sstream>      // 用于字符串流处理
#include <limits>       // 用于数值限制
#include <stdlib.h>     // 标准库函数
#include <iostream>     // 输入输出流
#include <math.h>       // 数学函数
#include <cassert>      // 断言

// 构造函数：初始化网络、流量差异和容差
DiffKMLNetOutput::DiffKMLNetOutput(StarNetwork* net, const std::vector<FPType>& flowDiff,
					FPType tolerance) : 
					KMLNetOutput(net), flowDiff_(flowDiff), scaleFactor_(1),
					tolerance_(tolerance) {
	calcScalingFactor();  // 计算缩放因子
};

// 计算缩放因子的函数
void DiffKMLNetOutput::calcScalingFactor(){
	FPType maxDiff = 0.0;  // 最大差异值
	FPType minDiff = std::numeric_limits<FPType>::infinity();  // 最小差异值
	for (size_t i = 0; i < flowDiff_.size(); ++i){
		FPType absDiff = fabs(flowDiff_[i]);  // 计算绝对值
		if (absDiff > maxDiff) maxDiff = absDiff;  // 更新最大差异
		if (absDiff < minDiff) minDiff = absDiff;  // 更新最小差异
	}
	std::cout << "maxDiff = " << maxDiff << " minDiff = " << minDiff << std::endl;
	scaleFactor_ = maxDiff / maxLength_;  // 计算缩放因子
	assert(scaleFactor_ != 0);  // 确保缩放因子不为0
};

// 带日志文件的构造函数
DiffKMLNetOutput::DiffKMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs,
					const std::vector<FPType>& flowDiff, FPType tolerance) : 
					KMLNetOutput(net, logFileForMissingIDs),
					flowDiff_(flowDiff), tolerance_(tolerance) {

};

// 析构函数
DiffKMLNetOutput::~DiffKMLNetOutput(){

};

// 计算线宽的函数
FPType DiffKMLNetOutput::calcLineWidth(StarLink* link){
	return flowDiff_[link->getIndex()] / scaleFactor_;  // 返回基于流量差异的线宽
};

// 根据线宽创建颜色的函数
std::string DiffKMLNetOutput::createColorFromLineWidth(FPType lineWidth){
	if (lineWidth == 0) return getGreenColor();  // 零差异显示为绿色
	if (lineWidth < 0.0) return "50F0C814";  // 负差异显示为蓝色
	return "501478FF";  // 正差异显示为红色
};

// 创建线宽字符串的函数
std::string DiffKMLNetOutput::createLineWidth(FPType lineWidth){
	if (lineWidth == 0) lineWidth = 3;  // 零差异使用默认线宽
	std::stringstream ss;
    ss << fabs(lineWidth);  // 转换为绝对值字符串
    return ss.str();
};

// 判断是否创建地标的函数
bool DiffKMLNetOutput::shouldCreatePlacemark(StarLink* link) {
	return true;  // 始终创建地标
};

// 创建描述字符串的函数
std::string DiffKMLNetOutput::createDescriptionString(StarLink* link) {
	std::stringstream ss;
	ss << "difference: " << flowDiff_[link->getIndex()];  // 显示流量差异
	return ss.str();
};