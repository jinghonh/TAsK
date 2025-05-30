#include "DAGraphB.h" // 包含 DAGraphB 类的头文件
#include "DAGraphNode.h" // 包含 DAGraphNode 类的头文件
#include "Path.h" // 包含 Path 类的头文件

#include <cassert> // 引入断言库
#include <stdlib.h> // 引入标准库
#include <iostream> // 引入输入输出流库
#include <math.h> // 引入数学库
#include <limits> // 引入数值极限库

FPType DAGraphB::dirTol_ = 0.0; // 静态成员变量，方向容差，初始化为0.0
bool DAGraphB::useMultiStep_ = false; // 静态成员变量，是否使用多步牛顿法，初始化为false

// DAGraphB 类的构造函数
// net: 指向 StarNetwork 对象的指针
// mat: 指向 ODMatrix 对象的指针
// zeroFlow: 零流量阈值
// dirTol: 方向容差
// originIndex: 起点索引
// useMultiStep: 是否使用多步牛顿法
DAGraphB::DAGraphB(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
                   int originIndex, bool useMultiStep) : 
                   DAGraph(net, mat, zeroFlow, originIndex), nbIter(0) { // 调用基类构造函数，并初始化迭代次数nbIter为0
    dirTol_ = dirTol; // 设置方向容差
    useMultiStep_ = useMultiStep; // 设置是否使用多步牛顿法
}; // 构造函数结束

// DAGraphB 类的析构函数
DAGraphB::~DAGraphB(){

}; // 析构函数结束

// 执行流量移动的函数
// 返回true如果成功移动了流量，否则返回false
bool DAGraphB::moveFlow(){
    ++nbIter; // 迭代次数加1
    bool canMove = false; // 标记是否可以移动流量，初始化为false
    bool canMoveTmp = false; // 临时标记是否可以移动流量，初始化为false
    for (int i = beginDescPass(); i != -1; i = getNextDescPass()){ // 按拓扑降序遍历所有节点
        canMoveTmp = performFlowMove(i, getOriginIndex()); // 对当前节点执行流量移动操作
        if (canMoveTmp) { // 如果在当前节点成功移动了流量
            canMove = true; // 将总体标记设为true
        } // 条件判断结束
    } // 循环结束
    return canMove; // 返回是否成功移动了流量
}; // 函数结束

// 在指定节点上执行一次流量移动操作
// index: 当前节点索引
// origin: 起点索引 (当前实现中未使用此参数，但保留以兼容接口或未来扩展)
// 返回true如果成功移动了流量，否则返回false
bool DAGraphB::performFlowMove(int index, int origin){
    DAGraphNode * const node = getNode(index); // 获取当前节点的 DAGraphNode 对象
    if (node->maxDist - node->minDist  < dirTol_) { // 如果最大距离与最小距离之差小于方向容差
        return false; // 不执行移动，返回false
    } // 条件判断结束
    
    StarLink* minLink = node->minLink; // 获取到当前节点的最短路径上的最后一条路段
    StarLink* maxLink = node->maxLink; // 获取到当前节点的最长路径上的最后一条路段
    if (minLink == NULL || maxLink == NULL) { // 如果最短或最长路径不存在
        return false; // 不执行移动，返回false
    } // 条件判断结束
    
    std::list<Path*> paths; // 创建一个存储路径指针的列表 (当前未使用)
    Path minPath; // 创建最短路径对象
    Path maxPath; // 创建最长路径对象
    paths.push_back(&minPath); // 将最短路径对象指针添加到列表中 (当前未使用)
    paths.push_back(&maxPath); // 将最长路径对象指针添加到列表中 (当前未使用)
    
    FPType minDist = 0.0; // 初始化最短路径的成本为0.0
    FPType maxDist = 0.0; // 初始化最长路径的成本为0.0

    int minNode = minLink->getNodeFromIndex(); // 获取最短路径上一条路段的起点（即路径上的前一个节点）
    int maxNode = maxLink->getNodeFromIndex(); // 获取最长路径上一条路段的起点
    if (minNode != maxNode) { // 如果最短和最长路径在当前节点的前一个节点不同
        minPath.addLinkToPath(minLink); // 将当前最短路径的最后路段加入minPath
        minDist += minLink->getTime(); //累加路段成本
        maxPath.addLinkToPath(maxLink); // 将当前最长路径的最后路段加入maxPath
        maxDist += maxLink->getTime(); // 累加路段成本
        
    } else if (minLink != maxLink) { // 如果前一个节点相同，但路段不同（意味着在共同前驱节点后分叉）
        minPath.addLinkToPath(minLink); // 将路段加入minPath
        minDist += minLink->getTime(); // 累加成本
        maxPath.addLinkToPath(maxLink); // 将路段加入maxPath
        maxDist += maxLink->getTime(); // 累加成本
    } // 条件判断结束
    while (true) { // 循环回溯，直到找到最短和最长路径的共同祖先节点
        if (minNode == maxNode) { // 如果找到了共同祖先节点
            break; // 退出循环
        } else { // 如果还没找到共同祖先
            DAGraphNode * const nodeMin = getNode(minNode); // 获取最短路径当前回溯到的节点
            DAGraphNode * const nodeMax = getNode(maxNode); // 获取最长路径当前回溯到的节点
            if (nodeMin->post < nodeMax->post) { // 比较节点的拓扑序（post值越小，在拓扑序中越靠前）
                // minNode 在拓扑序中更靠前，需要回溯minNode
                minLink = nodeMin->minLink; // 获取minNode的最短路径上的前一条路段
                if (minLink != NULL) { // 如果存在前一条路段
                    minNode = minLink->getNodeFromIndex(); // 更新minNode为其前一个节点
                    minPath.addLinkToPath(minLink); // 将路段加入minPath
                    minDist += minLink->getTime(); // 累加成本
                } // 条件判断结束
            
            } else { // maxNode 在拓扑序中更靠前（或与minNode相同，但上面已处理），需要回溯maxNode
                maxLink = nodeMax->maxLink; // 获取maxNode的最长路径上的前一条路段
                if (maxLink != NULL) { // 如果存在前一条路段
                    maxNode = maxLink->getNodeFromIndex(); // 更新maxNode为其前一个节点
                    maxPath.addLinkToPath(maxLink); // 将路段加入maxPath
                    maxDist += maxLink->getTime(); // 累加成本
                } // 条件判断结束
            
            } // 条件判断结束
        } // 条件判断结束
        
    } // 循环结束
    
    bool canMoveFlow = false; // 标记是否可以移动流量，初始化为false
    if ( !minPath.isEmpty() && !maxPath.isEmpty() && ( maxDist - minDist > dirTol_) ) { // 如果最短路径和最长路径都非空，且成本差大于容差
        assert(minNode == maxNode); // 断言此时已找到共同祖先节点
        
        FPType dx = 0.0;    // 初始化流量移动量为0.0
        minPath.setCurrCost(minDist); // 设置最短路径的当前成本
        maxPath.setCurrCost(maxDist); // 设置最长路径的当前成本
        
        while (true) { // 循环执行流量移动（牛顿法迭代，如果useMultiStep_为true）
            dx = calcFlowStep(&minPath, &maxPath);    // 计算流量移动步长
            if (dx <= zeroFlow_) { // 如果步长小于等于零流量阈值
                break; // 停止移动，退出循环
            } else { // 如果步长有效
                canMoveFlow = true; // 标记可以移动流量
            } // 条件判断结束
            
            minDist = 0.0; // 重置最短路径成本累加器
            for(StarLinkIterator it = minPath.begin(); it != minPath.end(); ++it){ // 遍历最短路径上的所有路段
                StarLink* link = *it; // 获取当前路段
                addFlow(link, dx); // 给路段增加流量dx
                minDist += link->getTime(); // 重新累加路段更新后的时间成本
            } // 循环结束
            
            maxDist = 0.0; // 重置最长路径成本累加器
            for(StarLinkIterator it = maxPath.begin(); it != maxPath.end(); ++it){ // 遍历最长路径上的所有路段
                StarLink* link = *it; // 获取当前路段
                addFlow(link, -dx); // 给路段减少流量dx (注意是-dx)
                maxDist += link->getTime(); // 重新累加路段更新后的时间成本
            } // 循环结束
            if (useMultiStep_ == false) break; // 如果不使用多步牛顿法，则执行一次后退出循环

            if (maxDist <= minDist) { // 如果移动后最长路径成本不再大于最短路径成本
                break; // 停止移动，退出循环
            } // 条件判断结束
            
            minPath.setCurrCost(minDist); // 更新最短路径的当前成本
            maxPath.setCurrCost(maxDist); // 更新最长路径的当前成本
            
        } // 内部循环结束
    } // 条件判断结束
        
    return canMoveFlow; // 返回是否成功移动了流量
}; // 函数结束

// 打印指定节点最短路径上的起点流量（调试用）
// index: 节点索引
void DAGraphB::printMinPathOFlows(int index) const{
    StarLink *link = getNode(index)->minLink; // 获取到指定节点的最短路径的最后一条路段
    std::cout << "MIN PATH origin flows" << std::endl; // 打印标题
    while (link != NULL){ // 当还有上一条路段时
        std::cout << "[" << link->getNodeFromIndex() << ", " << // 打印路段起点
                link->getNodeToIndex() << "] " << // 打印路段终点
                getOriginFlow(link->getIndex()) << " "; // 打印该路段的起点流量
        link = getNode(link->getNodeFromIndex())->minLink; // 移动到更前一条路段
    } // 循环结束
    std::cout << std::endl; // 换行
}; // 函数结束

// 打印指定节点最长路径上的起点流量（调试用）
// index: 节点索引
void DAGraphB::printMaxPathOFlows(int index) const{
    StarLink *link = getNode(index)->maxLink; // 获取到指定节点的最长路径的最后一条路段
    std::cout << "MAX PATH origin flows" << std::endl; // 打印标题
    while (link != NULL){ // 当还有上一条路段时
        std::cout << "[" << link->getNodeFromIndex() << ", " << // 打印路段起点
                link->getNodeToIndex() << "] " << // 打印路段终点
                getOriginFlow(link->getIndex()) << " "; // 打印该路段的起点流量
        link = getNode(link->getNodeFromIndex())->maxLink; // 移动到更前一条路段
    } // 循环结束
    std::cout << std::endl;    // 换行
}; // 函数结束

// 给路段增加流量，并更新相关信息
// link: 指向 StarLink 对象的指针
// dFlow: 要增加的流量（可以为负，表示减少）
void DAGraphB::addFlow(StarLink *link, FPType dFlow){
    link->addFlow(dFlow); // 调用路段对象的addFlow方法，更新路段自身流量
    int linkIndex = link->getIndex(); // 获取路段索引
    addOriginFlow(linkIndex, dFlow); // 更新该路段的起点流量
    if (link->getFlow() <= zeroFlow_) { // 如果路段更新后的流量小于等于零流量阈值
        link->setFlow(0.0); // 将路段流量设为0
        setOriginFlowToZero(linkIndex); // 将该路段的起点流量也设为0
    } // 条件判断结束
    if (getOriginFlow(linkIndex) < 0.0) { // 如果起点流量变为负值 (理论上不应发生，除非dFlow很大且初始流量很小)
        setOriginFlowToZero(linkIndex); // 将起点流量强制设为0
    } // 条件判断结束
    link->updateTime(); // 更新路段的行驶时间（因为流量变化会导致时间变化）
    
}; // 函数结束

// 计算流量移动的步长（牛顿法）
// minPath: 指向最短路径对象的指针
// maxPath: 指向最长路径对象的指针
// 返回计算出的流量步长
FPType DAGraphB::calcFlowStep(Path* minPath, Path* maxPath) const{
    FPType g = maxPath->getCurrCost() - minPath->getCurrCost(); // 计算最长路径与最短路径的成本差 (g >= 0)
    assert(g == g && g >= 0.0); // 断言g是有效数字且非负 (g == g 用于检查NaN)
    if (g < dirTol_) return 0.0; // 如果成本差小于方向容差，则步长为0
    
    FPType h = 0.0; // 初始化h值 (二阶导数相关项)
    for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) { // 遍历最短路径上的所有路段
        h += (*it)->getDerivative(); // 累加路段成本函数对流量的导数
    } // 循环结束
    FPType minMove = std::numeric_limits<FPType>::infinity( ); // 初始化最小可移动流量为无穷大（用于限制步长不超过路径上的最小流量）
    FPType oFlow = 0.0; // 用于存储路段的起点流量
    for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){ // 遍历最长路径上的所有路段
        StarLink* link = *it; // 获取当前路段
        h += link->getDerivative(); // 累加路段成本函数对流量的导数
        oFlow = getOriginFlow(link->getIndex()); // 获取该路段的起点流量
        if (oFlow < minMove) minMove = oFlow; // 更新最小可移动流量（即最长路径上起点流量最小的那个路段的流量）
    } // 循环结束
    assert(h >= 0.0); // 断言h值非负 (通常BPR函数的导数非负)
    FPType dx = g / h; // 计算牛顿法步长 dx = g / h
    if (dx > minMove) return minMove; // 如果计算出的步长大于最小可移动流量，则限制步长为minMove
    return dx; // 返回计算出的步长
    
}; // 函数结束
