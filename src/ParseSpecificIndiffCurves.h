#ifndef PARSE_SPECIFIC_INDIFF_CURVES
#define PARSE_SPECIFIC_INDIFF_CURVES 

#include "ParseIndiffCurves.h" // 包含无差异曲线解析基类

/** \brief 线性、凸和凹无差异函数的解析器。
*/
class ParseSpecificIndiffCurves : public ParseIndiffCurves { // 特定无差异曲线解析器类，继承自无差异曲线解析器基类
	public:
		ParseSpecificIndiffCurves(); // 构造函数
		~ParseSpecificIndiffCurves(); // 析构函数

	protected:
		/** 为curveType指定的无差异函数分配内存。
			如果curveType不是线性、凸或凹，则抛出错误。
		*/
		virtual IndiffCurve* createCurve(int nbPoints, const PointsContainer& points,
			const std::string& curveType); // 重写的创建曲线方法，根据曲线类型创建对应的无差异曲线

};
#endif