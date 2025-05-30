#include "CurrentPathsAdder.h" // 包含 CurrentPathsAdder 类的头文件
#include "BiObjLabelContainer.h" // 包含 BiObjLabelContainer 类的头文件
#include "PathSet.h" // 包含 PathSet 类的头文件
#include "Path.h" // 包含 Path 类的头文件
#include "ODSet.h" // 包含 ODSet 类的头文件

// CurrentPathsAdder 类的构造函数
// tolls: 收费容器的常量引用
CurrentPathsAdder::CurrentPathsAdder(const TollContainerType& tolls) : 
			paths_(NULL), tolls_(tolls) { // 初始化路径集指针为NULL，收费容器为传入的tolls

}; // 构造函数结束

// CurrentPathsAdder 类的析构函数
CurrentPathsAdder::~CurrentPathsAdder() {

}; // 析构函数结束

// 设置路径集的函数
// paths: 指向 PathSet 对象的指针
void CurrentPathsAdder::setPathSet(PathSet* paths) {
	paths_ = paths; // 将成员变量 paths_ 指向传入的 paths 对象
}; // 函数结束

// 上传路径的函数，将路径信息添加到标签容器中
// odIndex: OD对的索引
// destIndex: 目标节点的索引
// labels: BiObjLabelContainer 对象的引用，用于存储标签
void CurrentPathsAdder::uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const {
	ODSet* set = paths_->getODSetByIndex(odIndex); // 根据 OD 对索引从路径集中获取 ODSet 对象
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) { // 遍历 ODSet 中的所有路径
		Path* path = *pathIt; // 获取当前路径的指针
		FPType pathTime = 0; // 初始化路径时间为0
		TollType pathToll = 0; // 初始化路径收费为0
		for (ConstStarLinkIterator it = path->begin(); it != path->end(); ++it) { // 遍历路径中的所有路段
			StarLink* link = *it; // 获取当前路段的指针
			pathTime += link->getTime(); // 累加路段的行驶时间到路径时间
			pathToll += tolls_[link->getIndex()]; // 累加路段的收费到路径收费
		} // 内部循环结束
		BiObjLabel* label = new BiObjLabel(pathTime, pathToll, destIndex, NULL, NULL); // 创建一个新的 BiObjLabel 对象，包含路径时间和收费信息
		labels.pushBackLabel(destIndex, label); // 将新创建的标签添加到标签容器中
	} // 外部循环结束
}; // 函数结束