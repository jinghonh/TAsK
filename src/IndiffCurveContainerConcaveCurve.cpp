// 包含必要的头文件
#include "IndiffCurveContainerConcaveCurve.h"
#include "ConcaveIndiffCurve.h"

// 构造函数：初始化凹曲线容器，参数为OD对数量
IndiffCurveContainerConcaveCurve::IndiffCurveContainerConcaveCurve(int nbOdPairs) : 
				IndiffCurveContainer(nbOdPairs) {

};

// 析构函数：清理资源
IndiffCurveContainerConcaveCurve::~IndiffCurveContainerConcaveCurve() {

};

// 创建曲线方法：根据给定的参数创建新的凹曲线
// 参数说明：
// maxNbPointsPerCurve: 每条曲线的最大点数
// maxToll: 最大收费
// maxTime: 最大时间
// minTime: 最小时间
IndiffCurve* IndiffCurveContainerConcaveCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new ConcaveIndiffCurve(minTime, maxTime, maxToll);
};