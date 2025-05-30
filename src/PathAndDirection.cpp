#include "PathAndDirection.h" // 包含路径和方向头文件

#include <stdlib.h> // 包含标准库头文件

PathAndDirection::PathAndDirection() : path_(NULL), directionVal_(0.0) { // 构造函数，初始化路径为空和方向值为0

};

PathAndDirection::~PathAndDirection(){ // 析构函数

};

Path* PathAndDirection::getPath() const{ // 获取路径的方法
	return path_; // 返回路径指针
};

FPType PathAndDirection::getDirectionValue() const{ // 获取方向值的方法
	return directionVal_; // 返回方向值
};
		
void PathAndDirection::setPath(Path *path){ // 设置路径的方法
	path_ = path; // 设置路径指针
};

void PathAndDirection::setDirection(FPType direction){ // 设置方向的方法
	directionVal_ = direction; // 设置方向值
};
