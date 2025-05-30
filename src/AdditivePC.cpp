#include "AdditivePC.h"    // 包含AdditivePC类的头文件
#include "StarLink.h"      // 包含StarLink类的头文件
#include "Path.h"          // 包含Path类的头文件

AdditivePC::AdditivePC(){
    // 构造函数实现
};

AdditivePC::~AdditivePC(){
    // 析构函数实现
};

FPType AdditivePC::calculate(Path *path, int odIndex) const{
    FPType cost = 0.0;     // 初始化路径成本为0
    for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
        cost += (*it)->getTime();  // 累加路径上每个链接的时间成本
    }
    return cost;           // 返回总路径成本
};
