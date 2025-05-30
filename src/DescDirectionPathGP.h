#ifndef _DESC_DIRECTION_PATH_GP_  // 防止头文件重复包含
#define _DESC_DIRECTION_PATH_GP_

#include "DescDirectionPath.h"  // 包含基类头文件

/** \brief 该类实现了梯度投影算法的下降方向
    \details 详细内容请参考 \cite Chen1998
*/
class DescDirectionPathGP : public DescDirectionPath {  // 定义梯度投影下降方向类，继承自DescDirectionPath
	public:
		DescDirectionPathGP(FPType delta);  // 构造函数，接收精度参数delta
		~DescDirectionPathGP();  // 析构函数

		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
											   bool &isEquilibrated);  // 创建路径方向的方法
};

#endif  // 结束头文件保护
