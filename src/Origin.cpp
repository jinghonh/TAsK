#include "Origin.h" // 包含Origin头文件
#include "PairOD.h" // 包含PairOD头文件

#include <cassert> // 包含断言功能
#include <stdlib.h> // 包含标准库

Origin::Origin(int index) : index_(index) { // 构造函数，初始化起点索引
};

Origin::~Origin(){ // 析构函数
	for (PairODIterator jt = begin(); jt != end(); ++jt) { // 遍历所有的目的地
		delete *jt; // 删除目的地对象
	}
};

void Origin::addDestination(PairOD *dest){ // 添加目的地的方法
	assert(dest != NULL); // 断言目的地指针不为空
	destList_.push_front(dest); // 将目的地添加到链表前端
};
	
PairODIterator Origin::begin(){ // 获取目的地链表的开始迭代器
	return destList_.begin(); // 返回链表的开始迭代器
};	

PairODIterator Origin::end(){ // 获取目的地链表的结束迭代器
	return destList_.end(); // 返回链表的结束迭代器
};

int Origin::getIndex() const{ // 获取起点索引的方法
	return index_; // 返回索引
};

bool Origin::isEmpty() const{ // 检查是否没有目的地的方法
	return destList_.empty(); // 返回链表是否为空
};

int Origin::getNbDest() const{ // 获取目的地数量的方法
	return destList_.size(); // 返回链表大小
};
