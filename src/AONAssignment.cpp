// 包含必要的头文件
#include "AONAssignment.h"  // 包含AONAssignment类的头文件
#include "ODMatrix.h"       // 包含ODMatrix类的头文件

// 构造函数实现
// @param mat OD矩阵对象
// @param shPath 最短路径算法对象
AONAssignment::AONAssignment(const ODMatrix &mat, ShortestPath* shPath) :
                shPath_(shPath), mat_(mat){  // 初始化成员变量
};

// 析构函数实现
AONAssignment::~AONAssignment(){
};

// 执行分配算法
// @param solution 初始解对象
// @return 返回最小旅行时间
FPType AONAssignment::execute(InitialiseSolution *solution){
    FPType minTravelTime = 0.0;  // 初始化最小旅行时间为0
    for (ConstOriginIterator it = mat_.begin(); it != mat_.end(); ++it){  // 遍历所有起点
        minTravelTime += performAssignmentForOrigin(solution, *it);  // 对每个起点执行分配并累加旅行时间
    }
    return minTravelTime;  // 返回总的最小旅行时间
};
