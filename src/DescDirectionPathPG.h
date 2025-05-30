#ifndef _DESC_DIRECTION_PATH_PG_  // 防止头文件重复包含的宏定义开始
#define _DESC_DIRECTION_PATH_PG_  // 定义宏

#include "DescDirectionPath.h"    // 包含基类头文件

/** \brief 该类实现了投影梯度算法的下降方向
    \details 详细信息请参见 \note MichaelFlorian2009
    \note 该实现添加了特殊技术以保持下降方向与流量守恒约束的一致性。
          这使得算法能够达到高精度。
*/
class DescDirectionPathPG : public DescDirectionPath {  // 定义投影梯度下降方向类，继承自DescDirectionPath
    public:
        DescDirectionPathPG(FPType delta);  // 构造函数，参数为delta值
        ~DescDirectionPathPG();             // 析构函数
        PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
            bool &isEquilibrated);          // 创建路径方向的函数，返回路径和方向的指针数组
        
};

#endif  // 防止头文件重复包含的宏定义结束
