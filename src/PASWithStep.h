#ifndef PAS_WITH_STEP // 防止头文件重复包含
#define PAS_WITH_STEP // 定义头文件宏

#include "PAS.h" // 包含PAS（备选路段对）头文件

class LineSearch; // 前向声明线搜索类

/** \brief 这个类扩展了PAS类，增加了通过线搜索计算流量移动的可能性。
*/
class PASWithStep : public PAS { // PASWithStep类，继承自PAS
public:
	PASWithStep(FPType zeroFlow, FPType dirTol, LineSearch* lineSearch, int nbLinks); // 构造函数
	~PASWithStep(); // 析构函数

private:
	LineSearch* lineSearch_; // 线搜索指针
	const int nbLinks_; // 链接数量常量

	/** @return 通过线搜索计算的流量移动量。
	*/
	virtual FPType getFlowShift(); // 重写获取流量移动量的方法
};

#endif