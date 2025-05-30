#ifndef PATH_
#define PATH_

#include "UsedTypes.h"  // 包含自定义类型定义

#include <list>         // 包含链表容器

class StarLink;          // 前向声明StarLink类

typedef std::list<StarLink*>::iterator StarLinkIterator;           // 定义链路指针链表的迭代器类型
typedef std::list<StarLink*>::const_iterator ConstStarLinkIterator; // 定义链路指针链表的常量迭代器类型

/** \brief 这个类表示一条路径。
*/
class Path {
	public:
		Path();          // 构造函数
		~Path();         // 析构函数
		
		/** 设置流量值。
		*/
		void setFlow(FPType flow);
		/** 存储路径流量的前一个值。这对于基于路径的算法是必需的。
		*/
		void setPrevFlow(FPType prevFlow); 
		/** 设置当前路径成本。
		*/
		void setCurrCost(FPType cost);
		
		/** 在存储所有链路的链表前端添加链路。
		*/
		void addLinkToPath(StarLink *link);
		/** 在存储所有链路的链表末尾添加链路。
		*/
		void pushBackLink(StarLink* link);
		
		/** @return 路径流量。
		*/
		FPType getFlow() const;
		/** @return 前一个路径流量。
		*/
		FPType getPrevFlow() const;
		/** @return 当前路径成本。
		*/
		FPType getCurrCost() const;
		/** 计算当前路径旅行时间。
			@return 计算出的当前路径旅行时间。
		*/
		FPType calcCurrTime() const;
		/** @return 指向属于此路径的链路容器开头的迭代器。
		*/
		StarLinkIterator begin();
		/** @return 容器的末尾。
		*/
		StarLinkIterator end();
		/** @return 指向属于此路径的链路容器开头的常量迭代器。
		*/
		ConstStarLinkIterator begin() const;
		/** @return 常量容器的末尾。
		*/
		ConstStarLinkIterator end() const;
		/** @return 如果路径没有任何链路则返回true，否则返回false。
		*/
		bool isEmpty() const;
		/** @return 属于此路径的链路数量。
		*/
		int size() const;
		/** 在屏幕上打印路径信息。
		*/
		void print();
		/** 从路径中移除所有链路。
			\note 此方法中不会释放每个链路对象占用的内存。
		*/
		void clear();
		
	private:
		std::list<StarLink*> links_;    // 存储路径中的链路列表
		FPType flow_;                   // 当前流量
		FPType prevFlow_;               // 前一步流量
		FPType currCost_;               // 当前路径成本
		
};

#endif
