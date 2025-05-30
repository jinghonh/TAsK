#ifndef PARSE_INDIFF_CURVES
#define PARSE_INDIFF_CURVES 

#include "UsedTypes.h" // 包含自定义类型定义

#include <string> // 包含字符串库
#include <list> // 包含链表容器库

class IndiffCurveContainer; // 前向声明无差异曲线容器类
class IndiffCurve; // 前向声明无差异曲线类

/** \brief 分段线性归一化函数的解析器，也是其他类型函数解析器的基类。
*/
class ParseIndiffCurves { // 无差异曲线解析器基类
	public:
		ParseIndiffCurves(); // 构造函数
		virtual ~ParseIndiffCurves(); // 虚析构函数
		/** @param fileName 含有归一化函数的文件路径。
			@param container 将添加归一化函数的容器引用。
		*/
		void parse(const std::string& fileName, IndiffCurveContainer& container); // 解析文件并填充容器

	protected:
		typedef std::list<std::pair<FPType, FPType> > PointsContainer; // 定义点容器类型，存储无差异曲线的点
		/** @return 从行中提取的OD对索引。
		*/
		int getODIndex(const std::string& line); // 获取OD对索引
		/** @return 从行中提取的收费值。
		*/
		FPType getFirst(const std::string& line); // 获取第一个值（收费）
		/** @return 从行中提取的旅行时间值。
		*/
		FPType getSecond(const std::string& line); // 获取第二个值（旅行时间）
		/** 从行中提取分段线性归一化函数的点，并将其存储在points中。
			@return 提取的点总数。
		*/
		int getCurvePoints(const std::string& line, PointsContainer& points); // 获取曲线点
		/** 根据points为分段线性归一化函数分配内存并返回指针。
			@param nbPoints 点数。
			@param points 所有点。
			@points curveType 曲线类型。
		*/
		virtual IndiffCurve* createCurve(int nbPoints, const PointsContainer& points, 
			const std::string& curveType); // 创建曲线（虚函数）
		/** @return 描述曲线类型的字符串，如果不是线性、凸或凹曲线，则返回空字符串。
		*/
		std::string getCurveType(const std::string& line); // 获取曲线类型
};

#endif