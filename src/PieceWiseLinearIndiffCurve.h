#ifndef PIECEWISE_LINEAR_INDIFF_CURVE // 防止头文件重复包含
#define PIECEWISE_LINEAR_INDIFF_CURVE // 定义PIECEWISE_LINEAR_INDIFF_CURVE宏

#include "IndiffCurve.h" // 包含无差异曲线类

/** \brief 此类实现分段线性标量化函数。 */
class PieceWiseLinearIndiffCurve : public IndiffCurve { // 分段线性无差异曲线类，继承自无差异曲线

	public:
		PieceWiseLinearIndiffCurve(int nbPoints); // 构造函数，指定点数
		~PieceWiseLinearIndiffCurve(); // 析构函数

		/** 此方法用于创建标量化函数。
			添加点(\b toll, \b value)到标量化函数。
			\note 添加的点必须按照toll值递增顺序。不能添加具有相同toll值的点。
			所有值必须非负。第一个toll值必须为零。
			如果违反这些条件，将抛出错误。
		*/
		void addPoint(TollType toll, FPType value); // 添加点到标量化函数

		/** 根据给定的toll值评估标量化函数。
			如果toll值在两个已知点之间，返回值对应于这两点之间的线。
			如果toll大于最后一个已知点，则返回FPType的最大值。
			如果toll为负，程序将断言终止。
		*/
		virtual FPType evaluate(TollType toll); // 评估标量化函数
		/** @return 标量化函数的字符串表示。
		*/
		virtual std::string toString(); // 获取字符串表示
		/** @return 标量化函数的断点总数。
		*/
		int getNbPoints() const; // 获取点数

		/** 扰动标量化函数的旅行时间值。
			@param percentage 扰动百分比，必须在区间[0, 1]内。
			@param probOfPerturbation 扰动概率，必须在区间[0, 1]内。
		*/
		virtual void perturbTime(FPType percentage, FPType probOfPerturbation); // 扰动时间值
		/** 扰动标量化函数的通行费值。
			@param percentage 扰动百分比，必须在区间[0, 1]内。
			@param probOfPerturbation 扰动概率，必须在区间[0, 1]内。
		*/
		virtual void perturbToll(FPType percentage, FPType probOfPerturbation); // 扰动通行费值
		/** 恢复未扰动的标量化函数。
		*/
		virtual void restoreOriginalCurve(); // 恢复原始曲线

	private:
		const int nbPoints_; // 点的数量，常量
		int size_; // 当前大小
		std::vector<FPType> values_; // 值向量
		std::vector<TollType> tolls_; // 通行费向量
		std::vector<FPType> valuesCopy_; // 值副本向量
		std::vector<TollType> tollsCopy_; // 通行费副本向量
	
};

#endif // 结束头文件保护