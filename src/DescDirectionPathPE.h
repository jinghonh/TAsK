#ifndef _DESC_DIRECTION_PATH_PE_APPONE_  // 防止头文件重复包含的宏定义开始
#define _DESC_DIRECTION_PATH_PE_APPONE_  // 定义宏

#include "DescDirectionPath.h"  // 包含基类头文件

/** \brief 该类实现了路径均衡算法的下降方向
	\details 详细内容请参考 \cite Florian1995
*/
class DescDirectionPathPE : public DescDirectionPath {  // 定义路径均衡下降方向类，继承自DescDirectionPath
	public:
		DescDirectionPathPE(FPType delta);  // 构造函数，参数为delta值
		~DescDirectionPathPE();  // 析构函数
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
											   bool &isEquilibrated);  // 创建路径方向的函数，返回路径和方向数组
		
};

#endif  // 防止头文件重复包含的宏定义结束
