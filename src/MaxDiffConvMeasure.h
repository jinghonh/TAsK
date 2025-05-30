#ifndef MAX_DIFF_CONV_MEASURE  // 防止头文件重复包含
#define MAX_DIFF_CONV_MEASURE

#include "ConvMeasure.h"  // 包含基类头文件

class ShortestPath;  // 前向声明最短路径类
class ODMatrix;      // 前向声明OD矩阵类
class PathSet;       // 前向声明路径集合类

/** \brief 最大差异收敛度量类
*/
class MaxDiffConvMeasure : public ConvMeasure {  // 继承自ConvMeasure类
	public:
		// 构造函数，初始化收敛度量器
		MaxDiffConvMeasure(FPType precision, ShortestPath* shPath, 
						ODMatrix* mat, PathSet* pathSet);
		~MaxDiffConvMeasure();  // 析构函数

	private:
		
		ShortestPath* shPath_;  // 最短路径对象指针
		ODMatrix* mat_;         // OD矩阵对象指针
		PathSet* pathSet_;      // 路径集合对象指针

		/** @return 返回最大差异收敛度量的值
		*/
		virtual FPType calculateGap();  // 计算收敛间隙的虚函数

		/** @return 返回指定OD对索引对应的所有路径中最长路径的代价
		*/
		FPType calculateMaxPathCost(int odIndex);  // 计算最大路径代价的函数
};

#endif  // 结束头文件保护