#ifndef ORIGIN_H
#define ORIGIN_H

#include <list> // 包含链表容器

class PairOD; // 前向声明PairOD类

typedef std::list<PairOD*>::iterator PairODIterator; // 定义PairOD指针链表的迭代器类型

/** \brief 此类用于表示对应一个起点的目的地节点容器。
 */
class Origin { // 起点类
	
	public:
		
		/** @param index 起点索引。
		*/
		explicit Origin(int index); // 显式构造函数，接收起点索引

		/** 此类负责在被销毁时删除所有目的地。
		*/
		~Origin(); // 析构函数
		
		/** 将目的地dest添加到属于此起点的目的地集合中。
		*/
		void addDestination(PairOD *dest); // 添加目的地的方法
		
		/** @return 指向属于此起点的第一个目的地的迭代器。
		*/
		PairODIterator begin(); // 获取开始迭代器
		/** @return 容器的末尾。
		*/
		PairODIterator end(); // 获取结束迭代器
		
		/**	@return 起点索引。
		*/
		int getIndex() const; // 获取索引
		/** @return 如果没有属于此起点的目的地则返回true，否则返回false。
		*/
		bool isEmpty() const; // 检查是否为空
		/**	@return 属于此起点的目的地节点总数。
		*/
		int getNbDest() const; // 获取目的地数量
	
	private:
	
		const int index_; // 起点索引
		std::list<PairOD*> destList_; // 目的地链表
};

#endif
