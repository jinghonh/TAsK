#ifndef _DESC_DIRECTION_PATH_ISP_  // 防止头文件重复包含
#define _DESC_DIRECTION_PATH_ISP_

#include "DescDirectionPath.h"  // 包含基类头文件

/** \brief 该类实现了改进的社会压力算法的下降方向
    \details 详细信息请参见 \cite Kumar2011
*/
class DescDirectionPathISP : public DescDirectionPath {  // 继承自DescDirectionPath类
    public:
        /** @param slope 路径导数的精度，参见 \cite Kumar2011
            @param scaleFact \f$\pi\f$的值，参见 \cite Kumar2011
            @param delta 下降方向容差
        */
        DescDirectionPathISP(FPType slope, FPType scaleFact, FPType delta);  // 构造函数
        ~DescDirectionPathISP();  // 析构函数
        
        PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
                                               bool &isEquilibrated);  // 创建路径方向的方法
        
    private:
        const FPType slope_;  // 路径导数精度
        const FPType scaleFact_;  // 缩放因子
        
        /** 计算算法中用于将路径集合分为两组的阈值
            详细信息请参见 \cite Kumar2011
        */
        FPType calculateThreshold(FPType minDist, FPType maxDist) const;  // 计算阈值的方法

        /** 计算"路径导数"。它基本上是路径所属的链路成本函数
            相对于链路流量的导数之和
        */
        FPType calculateDerivative(Path* path) const;  // 计算导数的方法
};

#endif  // 结束头文件保护
