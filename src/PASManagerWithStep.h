#ifndef PAS_MANAGER_WITH_STEP // 防止头文件重复包含
#define PAS_MANAGER_WITH_STEP value // 定义头文件宏

#include "PASManager.h" // 包含PAS管理器头文件

class LineSearch; // 前向声明线搜索类

/** \brief 这个类扩展了PASManager以允许使用线搜索计算流量移动。
	\details 这个类负责分配PASWithStep对象。
*/
class PASManagerWithStep : public PASManager { // PASManagerWithStep类，继承自PASManager
public:
	PASManagerWithStep(const ShortestPath &shPath, FPType dirTol, int nbNodes, 
						FPType mu, FPType v,
						int nbLinks, LineSearch* lineSearch, FPType zeroFlow); // 构造函数
	~PASManagerWithStep(); // 析构函数

private:
	LineSearch* lineSearch_; // 线搜索指针
	const int nbLinks_; // 链接数量常量

	virtual PAS* allocatePAS(); // 重写的分配PAS方法
};
#endif