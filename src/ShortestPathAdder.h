#ifndef SHORTEST_PATH_ADDER // 防止头文件重复包含
#define SHORTEST_PATH_ADDER 

#include "CurrentPathsAdder.h" // 包含当前路径添加器头文件

/** \brief 实现已知路径技术的添加，用于从路径集合中添加
	具有最小非加性路径成本的路径。
*/
class ShortestPathAdder : public CurrentPathsAdder { // 最短路径添加器类，继承自CurrentPathsAdder
	public:
		ShortestPathAdder(const TollContainerType& tolls); // 构造函数，接收收费容器
		~ShortestPathAdder(); // 析构函数

		void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const; // 上传路径的方法实现

};
#endif