#ifndef PATH_COST // 防止头文件重复包含
#define PATH_COST // 定义PATH_COST宏

#include "UsedTypes.h" // 包含自定义类型定义

class Path; // 前向声明Path类

/** \brief 这个类是路径成本计算的接口。
*/
class PathCost { // 路径成本抽象基类
	public:
		virtual ~PathCost(){}; // 虚析构函数，确保正确释放派生类资源
		/** @param path 存储路径的对象。
			@param odIndex 起始点-目的地对索引。
		*/
		virtual FPType calculate(Path *path, int odIndex) const = 0; // 纯虚函数，计算给定路径的成本
	protected:
		PathCost(){}; // 受保护的构造函数，防止直接实例化此抽象类
};

#endif // 结束头文件保护