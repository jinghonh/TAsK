#include "StarNode.h" // 包含星形节点头文件

StarNode::StarNode(int id, bool isZone) : id_(id), index_(-1), isZone_(isZone){ // 构造函数，初始化成员变量

};

StarNode::~StarNode(){ // 析构函数

};

int StarNode::getID(){ // 获取节点ID的方法
	return id_; // 返回节点ID
};

bool StarNode::getIsZone(){ // 获取节点是否为区域节点的方法
	return isZone_; // 返回节点是否为区域节点
};

void StarNode::setIndex(int index){ // 设置节点索引的方法
	index_ = index; // 设置节点索引
};

int StarNode::getIndex(){ // 获取节点索引的方法
	return index_; // 返回节点索引
};
