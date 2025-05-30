#ifndef BI_OBJ_LABEL_CONT_PATH  // 防止头文件重复包含
#define BI_OBJ_LABEL_CONT_PATH

#include "BiObjLabel.h"  // 包含双目标标签类的头文件

#include <list>    // 包含列表容器
#include <vector>  // 包含向量容器

class BiObjLabel;  // 前向声明双目标标签类

typedef std::list<BiObjLabel*>  NodeContainerType;  // 定义节点容器类型为BiObjLabel指针的列表
typedef NodeContainerType::iterator LabelsIterator;  // 定义标签迭代器类型

/** \brief 双目标标签的容器类
*/
class BiObjLabelContainer {
	public:
		
		/** @param nbNodes 网络中的节点总数
		*/
		BiObjLabelContainer(int nbNodes);  // 构造函数
		~BiObjLabelContainer();  // 析构函数

		/** @param nodeIndex 要遍历其标签的节点索引
			@return 返回属于nodeIndex的标签的迭代器
		*/
		LabelsIterator begin(int nodeIndex) const;  // 获取指定节点标签容器的起始迭代器
		
		/** @return 返回属于nodeIndex的标签容器的结束迭代器
		*/
		LabelsIterator end(int nodeIndex) const;  // 获取指定节点标签容器的结束迭代器

		/** 从nodeIndex的标签列表中删除迭代器it指向的标签
			@return 返回删除元素后的迭代器
		*/
		LabelsIterator erase(int nodeIndex, LabelsIterator& it);  // 删除指定节点的指定标签

		/** @return 返回属于索引为nodeIndex的节点的标签数量
		*/
		int getNbLabels(int nodeIndex) const;  // 获取指定节点的标签数量

		/** @return 如果属于节点nodeIndex的标签集合为空则返回true，否则返回false
		*/
		bool empty(int nodeIndex) const;  // 检查指定节点的标签容器是否为空
		
		/** 将标签添加到节点nodeIndex的标签容器末尾
		*/
		void pushBackLabel(int nodeIndex, BiObjLabel* label);  // 向指定节点添加标签

		/** 删除属于节点nodeIndex的所有标签
		*/
		void clearNodeLabels(int nodeIndex);  // 清空指定节点的所有标签

		/** 在屏幕上打印每个节点的所有可用标签
		*/
		void print() const;  // 打印所有节点的标签信息

		/** 删除所有标签
			\warning 如果在某个时刻向容器添加了两个相同的BiObjLabel指针，此方法将无法正常工作
		*/
		void deleteAllLabels();  // 删除所有标签

		/** @return 返回网络中的节点数量
		*/
		int getNbNodes() const {  // 获取节点总数
			return nbNodes_;
		};

	private:
		const int nbNodes_;  // 网络中的节点总数
		mutable std::vector<NodeContainerType> labels_;  // 存储每个节点的标签容器的向量

};

#endif