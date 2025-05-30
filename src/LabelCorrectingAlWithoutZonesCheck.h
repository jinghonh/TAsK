#ifndef LABEL_CORRECTING_ALGO_WITHOUT_ZONES_CHECK  // 防止头文件重复包含
#define LABEL_CORRECTING_ALGO_WITHOUT_ZONES_CHECK 

#include "LabelCorrectingAl.h"  // 包含标签修正算法的基类头文件

/** \brief 实现常规的单源标签设置算法，允许最短路径经过区域节点
*/
class LabelCorrectingAlWithoutZonesCheck : public LabelCorrectingAl {  // 定义不检查区域的标签修正算法类，继承自LabelCorrectingAl
	public:
		LabelCorrectingAlWithoutZonesCheck(StarNetwork *netPointer);  // 构造函数，接收网络指针参数
		~LabelCorrectingAlWithoutZonesCheck();  // 析构函数

	protected:
		/** @return 如果curNode不为NULL则返回true，否则返回false
		*/
		virtual bool proceed(StarNode* curNode, int topNode) const;  // 虚函数，用于判断是否继续处理当前节点

};

#endif  // 结束头文件保护