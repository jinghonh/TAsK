#ifndef PATH_AND_DIRECTION // 防止头文件重复包含
#define PATH_AND_DIRECTION // 定义头文件宏

#include "UsedTypes.h" // 包含自定义类型头文件

class Path; // 前向声明路径类

/** \brief 这个类是DescDirectionPath中使用的基本元素。它表示一对
	两个值：路径和与给定路径相关联的方向值。
*/
class PathAndDirection { // 路径和方向类
	public:
		PathAndDirection(); // 构造函数
		~PathAndDirection(); // 析构函数
		
		/** @return 存储路径的对象指针。
		*/
		Path* getPath() const; // 获取路径的方法
		/** @return 下降方向的值。
		*/
		FPType getDirectionValue() const; // 获取方向值的方法
		
		/** 设置存储路径的对象指针。
		*/
		void setPath(Path *path); // 设置路径的方法
		/** 设置下降方向值。
		*/
		void setDirection(FPType direction); // 设置方向的方法
		
	private:
		Path* path_; // 路径指针
		FPType directionVal_; // 方向值
};

#endif
