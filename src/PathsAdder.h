#ifndef PATHS_ADDER // 防止头文件重复包含
#define PATHS_ADDER // 定义PATHS_ADDER宏

class BiObjLabelContainer; // 前向声明BiObjLabelContainer类

/** \brief 双目标标签设置算法中已知路径添加加速技术的基类。
*/
class PathsAdder { // 路径添加器抽象基类
	public:
		virtual ~PathsAdder() {}; // 虚析构函数，确保正确释放派生类资源
		/** 上传已知路径到目标节点。
			@param odIndex 起始点-目的地对索引。
			@param destIndex 目标节点索引。
			@param labels 非支配标签的容器。
		*/
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const = 0; // 纯虚函数，上传路径

	protected:
		PathsAdder() {}; // 受保护的构造函数，防止直接实例化此抽象类

};
#endif // 结束头文件保护