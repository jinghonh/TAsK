#ifndef UTILS_FOR_DOMINATION
#define UTILS_FOR_DOMINATION 

#include "UsedTypes.h" // 包含自定义类型定义

#include <list> // 包含标准库链表

class BiObjLabelContainer; // 前向声明BiObjLabelContainer类
class BiObjLabel; // 前向声明BiObjLabel类
class StarLink; // 前向声明StarLink类

/** \brief 为双目标最短路径算法定义有用的支配检查函数。
*/
namespace UtilsForDomination {

	extern const FPType tolerance_; // 定义支配关系的容差值
	/** @return 如果具有成本向量[newTime, newToll]的标签被目标节点destIndex中
			的至少一个标签支配，则返回true，否则返回false。
	*/
	bool isDominatedByLabelInDestNode(const BiObjLabelContainer& labels, int destIndex, 
			FPType newTime, TollType newToll); // 检查是否被目标节点中的标签支配
	/** @return 如果标签first被标签second支配，则返回true，否则返回false。
	*/
	bool isDominated(BiObjLabel* first, BiObjLabel* second); // 检查一个标签是否被另一个标签支配
	/** @return 如果具有成本向量[newTime, newToll]的标签被标签second支配，
			则返回true，否则返回false。
	*/
	bool isDominated(FPType newTime, TollType newToll, BiObjLabel* second); // 检查成本值是否被标签支配
	/** 从destLabel回溯路径。路径的链接被添加到 \b path。
	*/
	void createPathFromLabel(BiObjLabel* destLabel, std::list<StarLink*> &path); // 从标签创建路径
};

#endif // UTILS_FOR_DOMINATION头文件结束