// 包含必要的头文件
#include "IndiffCurveContainerLinearCurve.h"
#include "LinearIndiffCurve.h"

// 构造函数：初始化线性无差异曲线容器
// 参数：nbOdPairs - OD对的数量
IndiffCurveContainerLinearCurve::IndiffCurveContainerLinearCurve(int nbOdPairs) : 
							IndiffCurveContainer(nbOdPairs) {

};

// 析构函数：清理线性无差异曲线容器
IndiffCurveContainerLinearCurve::~IndiffCurveContainerLinearCurve() {

};

// 创建无差异曲线的方法
// 参数：
// maxNbPointsPerCurve - 每条曲线的最大点数
// maxToll - 最大收费
// maxTime - 最大时间
// minTime - 最小时间
// 返回：新创建的线性无差异曲线对象
IndiffCurve* IndiffCurveContainerLinearCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new LinearIndiffCurve(minTime, maxTime, maxToll);
};
