#ifndef _CONV_MEASURE_
#define _CONV_MEASURE_

#include "UsedTypes.h"

#include <stdlib.h> // 引入标准库
#include <iostream> // 引入输入输出流库

// 抽象基类，表示收敛性度量
class ConvMeasure {
	public:
		virtual ~ConvMeasure() {};
		
		// 检查算法的全局收敛性，若满足收敛准则返回true，否则返回false
		bool isConverged() {
			gap_ = calculateGap(); // 计算收敛性度量值
			return (gap_ <= precision_);
		} ;
		
		// 检查算法的全局收敛性，带最小旅行时间参数
		bool isConverged(FPType minTravelTime) {
			gap_ = calculateGap(minTravelTime);
			return (gap_ <= precision_);
		};
		
		FPType getPrecision() const {return precision_;}; // 获取精度
		FPType getGap() const {return gap_;}; // 获取当前gap值
	
	protected:
		// 构造函数，设置收敛精度
		ConvMeasure(FPType precision) : precision_(precision), gap_(0.0) {};
		
		// 计算收敛性度量值，纯虚函数
		virtual FPType calculateGap() = 0;
		// 计算收敛性度量值，可带最小旅行时间参数，默认调用calculateGap
		virtual FPType calculateGap(FPType minTravelTime) {return calculateGap();};
	
	private:
		const FPType precision_; // 收敛精度
		FPType gap_; // 当前gap值
};

#endif
