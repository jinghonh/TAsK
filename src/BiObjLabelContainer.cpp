#include "BiObjLabelContainer.h"  // 包含双目标标签容器头文件

#include <algorithm>  // 包含算法库
#include <cassert>    // 包含断言库
#include <iostream>   // 包含输入输出流库

// 构造函数：初始化节点数量和标签容器
BiObjLabelContainer::BiObjLabelContainer(int nbNodes) : nbNodes_(nbNodes), labels_(nbNodes) {

};

// 析构函数：清理所有标签
BiObjLabelContainer::~BiObjLabelContainer(){
	deleteAllLabels();
};

// 获取指定节点标签容器的起始迭代器
LabelsIterator BiObjLabelContainer::begin(int nodeIndex)  const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);  // 确保节点索引有效
	return labels_[nodeIndex].begin();
};

// 获取指定节点标签容器的结束迭代器
LabelsIterator BiObjLabelContainer::end(int nodeIndex)  const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);  // 确保节点索引有效
	return labels_[nodeIndex].end();
};

// 删除指定节点中的指定标签
LabelsIterator BiObjLabelContainer::erase(int nodeIndex, LabelsIterator& it){
	return labels_[nodeIndex].erase(it);
};

// 向指定节点添加新标签
void BiObjLabelContainer::pushBackLabel(int nodeIndex, BiObjLabel* label) {
	labels_[nodeIndex].push_back(label);
};

// 删除所有节点的所有标签
void BiObjLabelContainer::deleteAllLabels(){
	for (int i = 0; i < nbNodes_; ++i){
		for (LabelsIterator labelIt = begin(i); labelIt != end(i); ++labelIt){
			delete *labelIt;  // 释放标签内存
		}
		clearNodeLabels(i);  // 清空节点标签容器
	}
};

// 清空指定节点的所有标签
void BiObjLabelContainer::clearNodeLabels(int nodeIndex){
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);  // 确保节点索引有效
	labels_[nodeIndex].clear();
};

// 获取指定节点的标签数量
int BiObjLabelContainer::getNbLabels(int nodeIndex) const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);  // 确保节点索引有效
	return labels_[nodeIndex].size();
};

// 检查指定节点是否没有标签
bool BiObjLabelContainer::empty(int nodeIndex) const {
	return labels_[nodeIndex].empty();
};

// 打印所有节点的所有标签信息
void BiObjLabelContainer::print() const {
	for (int i = 0; i < nbNodes_; ++i){
		std::cout << "node index: " << i << std::endl;  // 打印节点索引
		for (LabelsIterator labelIt = begin(i); labelIt != end(i); ++labelIt){
			std::cout << (*labelIt)->toString() << std::endl;  // 打印标签信息
		}
	}
};