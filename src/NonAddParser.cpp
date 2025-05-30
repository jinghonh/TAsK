#include "NonAddParser.h" // 包含非加性解析器头文件

#include <cassert> // 包含断言功能

NonAddParser::NonAddParser(const std::string& fileWithNetwork) : 
		DefaultNetParser(fileWithNetwork), tolls_(NULL) { // 构造函数，接收网络文件路径，初始化通行费为NULL

};

NonAddParser::~NonAddParser() { // 析构函数

};

void NonAddParser::allocateTolls(int nbLinks){ // 为通行费数组分配内存
	assert(nbLinks > 0); // 断言链接数大于0
	tolls_ =  new TollContainerType(nbLinks); // 创建新的通行费容器
};

void NonAddParser::setToll(int linkIndex, TollType toll){ // 设置特定链接的通行费
	(*tolls_)[linkIndex] = toll; // 将通行费值赋给对应索引的链接
};	

TollContainerType* NonAddParser::getTolls(){ // 获取通行费容器
	return tolls_; // 返回通行费指针
};