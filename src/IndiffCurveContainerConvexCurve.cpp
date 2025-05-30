// 包含必要的头文件
#include "IndiffCurveContainerConvexCurve.h"
#include "ConvexIndiffCurve.h"

// 构造函数：初始化凸无差异曲线容器
// 参数：nbOdPairs - OD对的数量
IndiffCurveContainerConvexCurve::IndiffCurveContainerConvexCurve(int nbOdPairs) : 
		IndiffCurveContainer(nbOdPairs) {

};

// 析构函数：清理凸无差异曲线容器
IndiffCurveContainerConvexCurve::~IndiffCurveContainerConvexCurve() {

};

// 创建新的凸无差异曲线
// 参数：
// maxNbPointsPerCurve - 每条曲线的最大点数
// maxToll - 最大收费
// maxTime - 最大时间
// minTime - 最小时间
// 返回：新创建的凸无差异曲线指针
IndiffCurve* IndiffCurveContainerConvexCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new ConvexIndiffCurve(minTime, maxTime, maxToll);
};