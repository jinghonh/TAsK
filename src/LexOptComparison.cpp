#include "LexOptComparison.h"  // 包含LexOptComparison类的头文件
#include "BiObjLabel.h"        // 包含BiObjLabel类的头文件

bool LexOptComparison::operator() (BiObjLabel* lhs, BiObjLabel* rhs) const {
    // 对于标签设置算法，标签必须按字典序排序，否则算法将不会收敛。
    // 实际上，我们需要保证从列表中取出的标签是非支配的
    // （如果标签是字典序最小的，这一点就能得到保证）
    return  (lhs->getTime() < rhs->getTime() ||                    // 如果lhs的时间小于rhs的时间
            (lhs->getTime() == rhs->getTime() && lhs->getToll() < rhs->getToll()) ); // 或者时间相等但lhs的收费小于rhs的收费
    // return (lhs->getTime() + lhs->getToll() < rhs->getTime() +  rhs->getToll()); // 使用总和比较在这里不起作用
    // 在我们的情况下，根据论文，成本必须严格为正才能保证算法的正确性
    // 但在我们的问题中并不满足这个条件
};