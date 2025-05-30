#ifndef INDIFF_CURVE_CONTAINER_CONVEX_CURVE  // 防止头文件重复包含的宏定义开始
#define INDIFF_CURVE_CONTAINER_CONVEX_CURVE  // 定义宏

#include "IndiffCurveContainer.h"  // 包含基类头文件

/** \brief 凸标量化函数的容器类
*/
class IndiffCurveContainerConvexCurve : public IndiffCurveContainer {  // 定义凸曲线容器类，继承自IndiffCurveContainer
	public:
		IndiffCurveContainerConvexCurve(int nbOdPairs);  // 构造函数，参数为OD对数量
		~IndiffCurveContainerConvexCurve();  // 析构函数

	private:
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll,  // 创建曲线的虚函数
					FPType maxTime, FPType minTime);  // 参数包括最大点数、最大收费、最大和最小时间

};
#endif  // 防止头文件重复包含的宏定义结束