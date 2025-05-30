#ifndef POINT_2_POINT_BI_OBJ_SH_PATH // 防止头文件重复包含
#define POINT_2_POINT_BI_OBJ_SH_PATH // 定义宏 

#include "BiObjLabelContainer.h" // 包含双目标标签容器类

#include <list> // 包含标准库列表容器

class BiObjLabel; // 前向声明双目标标签类
class StarLink; // 前向声明星形链接类

/** \brief 定义点对点双目标最短路径算法的接口。
*/
class Point2PointBiObjShPath { // 点对点双目标最短路径算法基类
	public:
		
		virtual ~Point2PointBiObjShPath() {}; // 虚析构函数
		/** 求解点对点双目标最短路径问题。
			@param originIndex 起点索引。
			@param destIndex 目标点索引。
			@param odIndex 起始点-目的地对索引。
		*/
		virtual void calculate(int originIndex, int destIndex, int odIndex) = 0; // 计算最短路径的纯虚函数
		/** 从给定标签labelWithMinCost回溯路径
			并将路径存储在path中。
		*/
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path) = 0; // 创建路径的纯虚函数
		/** @return 对非支配标签容器的引用。
		*/
		const BiObjLabelContainer& getLabels(){ // 获取标签容器
			return labels_;
		};

	protected:
		BiObjLabelContainer labels_; // 非支配标签容器
		Point2PointBiObjShPath(int nbNodes) : labels_(nbNodes) {}; // 构造函数，初始化标签容器

};

#endif // 结束头文件保护