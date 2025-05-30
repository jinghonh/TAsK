#ifndef NO_PATHS_ADDER
#define NO_PATHS_ADDER 

#include "PathsAdder.h" // 包含路径添加器基类

/** \brief 实现一种情况，即双目标标签设置算法不向目标节点添加任何路径。
*/
class NoPathsAdder : public PathsAdder { // 定义NoPathsAdder类，继承自PathsAdder
	public:
		NoPathsAdder(); // 构造函数
		~NoPathsAdder(); // 析构函数

		/** 不执行任何操作。
		*/
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const; // 重写上传路径的方法，但不执行任何操作

};
#endif