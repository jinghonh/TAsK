
#ifndef DESC_DIRECTION_PATH  // 防止头文件重复包含
#define DESC_DIRECTION_PATH

#include "PathAndDirection.h"  // 包含路径和方向类
#include "Path.h"  // 包含路径类

#include <stdlib.h>  // 标准库
#include <iostream>  // 输入输出流
#include <list>  // 链表容器
#include <limits>  // 数值限制
#include <cassert>  // 断言

/** \brief 这是一个基类，用于实现基于路径算法中的不同下降方向
    \details 下降方向在路径流空间中。
    预期只会分配和使用派生类的一个实例。
    DescDirectionPathScaled是友元类，因为它调用传递给它的指针上的createPathDirection()
*/
class DescDirectionPath {
    friend class DescDirectionPathScaled;  // 声明友元类
    public:
        virtual ~DescDirectionPath(){  // 虚析构函数
            for (int i = 0; i < size_; ++i) {  // 遍历释放内存
                delete pathDirection_[i];
            }
            delete[] pathDirection_;  // 释放数组内存
        };
        /** 释放之前计算的下降方向(由PathAndDirection类表示)使用的内存，
            创建新的并返回它。
            @param size 调用此函数后将包含分配的基于路径的下降方向数组的大小
            @param paths 当前存储在给定O-D集中的路径列表
            @param isEquilibrated 调用此函数后将包含当前O-D集是否平衡的信息
            (平衡的概念取决于算法，但基本上意味着在给定O-D集内转移流量要么无用要么不可能)
        */
        PathAndDirection** calculatePathDirection(int &size, const std::list<Path*> &paths,
                        bool &isEquilibrated){
            for (int i = 0; i < size_; ++i) {  // 释放旧的方向数组
                delete pathDirection_[i];
            }
            delete[] pathDirection_;  // 释放数组
            pathDirection_ = createPathDirection(size_, paths, isEquilibrated);  // 创建新方向
            size = size_;  // 更新大小
            return pathDirection_;  // 返回新方向
        };
        
    protected:
        const FPType delta_;  // 存储下降方向的精度

        DescDirectionPath(FPType delta) :  delta_(delta), pathDirection_(NULL), size_(0) {};  // 构造函数
        
        /** 在给定路径集合中找到成本最小的路径*/
        Path* findMinPath(const std::list<Path*> &paths) const{
            FPType minDist = std::numeric_limits<FPType>::infinity();  // 初始化最小距离为无穷大
            Path *minPath = NULL;  // 初始化最小路径指针
            FPType cost = 0.0;  // 初始化成本
            Path* path = NULL;  // 初始化路径指针
            for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {  // 遍历所有路径
                path = *it;
                cost = path->getCurrCost();  // 获取当前路径成本
                if (cost < minDist) {  // 如果找到更小的成本
                    minDist = cost;  // 更新最小距离
                    minPath = path;  // 更新最小路径
                }
            }
            assert(minPath != NULL);  // 确保找到了最小路径
            return minPath;  // 返回最小路径
        };
        
        /** @return 在给定路径集合中找到成本最大的路径
            同时确保它不与成本最小的路径重合
        */
        Path* findMaxPath(const std::list<Path*> &paths, Path *minPath) const{
            Path *maxPath = NULL;  // 初始化最大路径指针
            FPType maxDist = 0.0;  // 初始化最大距离
            FPType cost = 0.0;  // 初始化成本
            Path* path = NULL;  // 初始化路径指针
            for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {  // 遍历所有路径
                path = *it;
                if (path != minPath) {  // 如果不是最小路径
                    cost = path->getCurrCost();  // 获取当前路径成本
                    if (cost > maxDist) {  // 如果找到更大的成本
                        maxDist = cost;  // 更新最大距离
                        maxPath = path;  // 更新最大路径
                    }
                }
            }
            assert(maxPath != NULL);  // 确保找到了最大路径
            return maxPath;  // 返回最大路径
        };
        
        /** 此方法负责创建下降方向。它取决于特定的基于路径的算法*/
        virtual PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths, 
                        bool &isEquilibrated) = 0;  // 纯虚函数
        
    private:
        PathAndDirection** pathDirection_;  // 路径方向数组指针
        int size_;  // 数组大小
};

#endif
