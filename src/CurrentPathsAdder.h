#ifndef CURRENT_PATHS_ADDER // 防止头文件重复包含
#define CURRENT_PATHS_ADDER // 定义头文件宏

#include "PathsAdder.h" // 包含 PathsAdder 类的头文件
#include "UsedTypes.h" // 包含自定义类型定义

class PathSet; // PathSet 类的声明

// 实现已知路径添加技术的类，适用于添加所有正流量路径
class CurrentPathsAdder : public PathsAdder { // CurrentPathsAdder 类定义，继承自 PathsAdder
	public:
		// 构造函数，tolls为收费容器
		CurrentPathsAdder(const TollContainerType& tolls);
		~CurrentPathsAdder(); // 析构函数

		// 存储所有正流量路径的路径集指针
		// 注意：对象创建后需调用一次，不能在构造函数中调用，因为PathSet间接依赖本类
		void setPathSet(PathSet* paths);

		// 向目标节点destIndex添加所有已知路径的标签
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const;

	protected:
		PathSet* paths_; // 路径集指针
		const TollContainerType& tolls_; // 收费容器引用

}; // 类定义结束
#endif // CURRENT_PATHS_ADDER