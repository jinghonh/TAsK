// 包含必要的头文件
#include "NonAdditivePC.h"
#include "StarLink.h"
#include "Path.h"

#include <cassert>

// 构造函数：初始化无差异曲线容器和收费容器
NonAdditivePC::NonAdditivePC(const IndiffCurveContainer& curves, const TollContainerType& tolls) : 
						curves_(curves), tolls_(tolls) {

};

// 析构函数
NonAdditivePC::~NonAdditivePC(){

};

// 计算路径的成本函数
FPType NonAdditivePC::calculate(Path *path, int odIndex) const{
	assert(path != NULL && odIndex >=0);  // 确保路径指针非空且OD索引有效
	TollType totalToll = 0.0;  // 初始化总收费
	FPType totalTime = 0.0;    // 初始化总时间
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {  // 遍历路径上的所有路段
		StarLink* link = *it;  // 获取当前路段
		totalTime += link->getTime();  // 累加路段时间
		totalToll += tolls_[link->getIndex()];  // 累加路段收费
	}
	return calculate(totalTime, totalToll, odIndex);  // 返回总成本
};

// 计算给定时间和收费下的成本
FPType NonAdditivePC::calculate(FPType totalTime, TollType totalToll, int odIndex) const{
	return totalTime + evaluateCurve(totalToll, odIndex);  // 返回总时间加上无差异曲线评估值
};

// 评估无差异曲线
FPType NonAdditivePC::evaluateCurve(TollType totalToll, int odIndex) const {
	return (curves_.getCurve(odIndex))->evaluate(totalToll);  // 返回对应OD对的无差异曲线评估值
};
