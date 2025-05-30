#include "ParseSpecificIndiffCurves.h" // 包含特定无差异曲线解析器头文件
#include "LinearIndiffCurve.h" // 包含线性无差异曲线类
#include "ConvexIndiffCurve.h" // 包含凸无差异曲线类
#include "ConcaveIndiffCurve.h" // 包含凹无差异曲线类
#include "Error.h" // 包含错误处理类

#include <cassert> // 包含断言头文件

ParseSpecificIndiffCurves::ParseSpecificIndiffCurves() { // 构造函数实现

};

ParseSpecificIndiffCurves::~ParseSpecificIndiffCurves() { // 析构函数实现

};

IndiffCurve* ParseSpecificIndiffCurves::createCurve(int nbPoints, const PointsContainer& points, 
	const std::string& curveType) { // 创建特定类型无差异曲线的方法实现
	assert(nbPoints == 1); // 断言点数为1，因为特定曲线只需一个点
	std::pair<FPType, FPType> values = *points.begin(); // 获取第一个点的值对
	if (curveType == "Linear") { // 如果是线性曲线
		return new LinearIndiffCurve(values.first, values.second); // 创建线性无差异曲线
	} else if (curveType == "Convex") { // 如果是凸曲线
		return new ConvexIndiffCurve(values.first, values.second); // 创建凸无差异曲线
	} else if (curveType == "Concave") { // 如果是凹曲线
		return new ConcaveIndiffCurve(values.first, values.second); // 创建凹无差异曲线
	} else { // 如果是其他类型
		throw Error("Unexpected indifference curve type"); // 抛出错误，不支持的无差异曲线类型
	}
	return NULL; // 返回空指针（实际上不会执行到这一步）

};