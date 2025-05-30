#ifndef INDIFF_CURVE_CONTAINER_LINEAR_CURVE  // 防止头文件重复包含的宏定义
#define INDIFF_CURVE_CONTAINER_LINEAR_CURVE 

#include "IndiffCurveContainer.h"  // 包含基类头文件

/** \brief 线性标量化函数的容器类
*/
class IndiffCurveContainerLinearCurve : public IndiffCurveContainer {  // 定义线性曲线容器类，继承自IndiffCurveContainer
	public:
		IndiffCurveContainerLinearCurve(int nbOdPairs);  // 构造函数，参数为OD对数量
		~IndiffCurveContainerLinearCurve();  // 析构函数

	private:
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll,  // 创建曲线的虚函数
					FPType maxTime, FPType minTime);  // 参数包括最大点数、最大收费、最大和最小时间

};
#endif  // 结束宏定义