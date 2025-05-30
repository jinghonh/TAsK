#ifndef INDEFF_CURVE_CONTAINER  // 防止头文件重复包含
#define INDEFF_CURVE_CONTAINER

#include "IndiffCurve.h"  // 包含无差异曲线类的头文件

#include <vector>  // 包含向量容器
#include <set>    // 包含集合容器

class ODMatrix;  // 前向声明OD矩阵类
class OneSourceBiObjShPath;  // 前向声明单源双目标最短路径类

/** \brief 这个类是标量化函数的容器。
    它负责管理这些函数的释放。
*/
class IndiffCurveContainer {
	public:
		explicit IndiffCurveContainer(int nbODpairs);  // 构造函数，参数为OD对数量
		virtual ~IndiffCurveContainer();  // 虚析构函数

		/** 此方法将曲线\b curve添加到内部容器的\b odPairIndex索引位置。
			如果该索引无效（负数或大于OD对数量）或者该索引位置
			已分配了标量化函数，断言将停止程序。
		*/
		void addCurve(int odPairIndex, IndiffCurve* curve);  // 添加曲线到指定OD对

		/** @return 返回OD对\b odPairIndex的标量化函数指针。
		*/
		IndiffCurve* getCurve(int odPairIndex) const;  // 获取指定OD对的曲线

		/** 释放之前的标量化函数（如果有的话）并生成
			随机的新函数。
			@param maxNbPointsPerCurve 每条曲线的最大断点数。
			@param maxToll 最大收费值。
			@param maxTime 最大时间值。
		*/
		void generateRandomCurves(int maxNbPointsPerCurve, TollType maxToll, FPType maxTime);  // 生成随机曲线

		/** 释放之前的标量化函数（如果有的话）并基于
			有效路径生成随机的新函数。
			@param maxNbPointsPerCurve 每条曲线的最大断点数。
			@param shPath 单源双目标最短路径算法。
			@param mat OD矩阵。
		*/
		void generateRandomCurvesWithBSP(int maxNbPointsPerCurve, OneSourceBiObjShPath& shPath,
				const ODMatrix& mat);  // 基于最短路径生成随机曲线

		/** 在屏幕上打印所有标量化函数。
		*/
		void print() const;  // 打印所有曲线
		/** 将标量化函数写入到fileName指定的文件中。
		*/
		void writeToFile(const std::string& fileName, const ODMatrix& mat) const;  // 将曲线写入文件

	private:

		std::vector<IndiffCurve*> curves_;  // 存储曲线的向量容器

		/** 释放所有标量化函数。
			\warning 如果同一个指针被添加两次，则无法正确工作。
		*/
		void deallocateCurves();  // 释放所有曲线

		/** 创建标量化函数。
			@param maxNbPointsPerCurve 每条曲线的最大断点数。
			@param maxToll 最大收费值。
			@param maxTime 最大时间值。
			@param minTime 最小时间值。
			@return 分配的标量化函数。
		*/
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime);  // 创建曲线

		/** 生成随机收费值并写入randomTolls。所有收费值都在
			区间[1, maxToll]内。所有收费值都是唯一的。
			@param nbPoints 必须生成的收费值数量。
		*/
		void generateRandomTolls(std::set<TollType>& randomTolls, TollType maxToll, int nbPoints);  // 生成随机收费值
};

#endif