#ifndef LINK_FLOWS_BFW  // 防止头文件重复包含
#define LINK_FLOWS_BFW

#include "LinkFlowsCFW.h"  // 包含双共轭Frank-Wolfe算法的基类头文件

/** \brief 该类实现了双共轭Frank-Wolfe算法
    \details 详细信息请参见 \cite Mitradjieva2012
    \note 系数的计算是通过实验得出的，目的是使算法收敛。论文中并未明确说明应该如何计算。
    \warning 当精度较高时，该算法会出现收敛问题
*/
class LinkFlowsBFW : public LinkFlowsCFW {  // 定义双共轭Frank-Wolfe算法类，继承自LinkFlowsCFW

    public:
        // 构造函数，初始化网络、OD矩阵、最短路径等参数
        LinkFlowsBFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, FPType zeroFlow,
                LabelCorrectingAl* LCShPath);
        virtual ~LinkFlowsBFW();  // 虚析构函数
        
    private:
        FPType *linkFlowsBFW_; /**< BFW算法的链路流量数组 */
        
        /** 计算BFW算法的系数
        */
        void calcCoeff(FPType &beta0, FPType &beta1, FPType &beta2);  // 计算三个系数beta0、beta1和beta2
        virtual void calculateDirection();  // 计算搜索方向的虚函数

};

#endif  // 结束头文件保护
