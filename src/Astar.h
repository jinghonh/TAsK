#ifndef _ASTAR_  // 防止头文件重复包含
#define _ASTAR_

#include <vector>    // 包含向量容器
#include <queue>     // 包含队列容器
#include <limits>    // 包含数值限制
#include <cassert>   // 包含断言

#include "ShortestPath.h"  // 包含最短路径基类

// 前向声明相关类
class StarNetwork;   // 网络类
class ODMatrix;      // OD矩阵类
class PathSet;       // 路径集合类
class StarNode;      // 节点类

/** \brief 该类实现了点对点情况下的A*最短路径算法，
    以及起点情况下的Dijkstra算法。
    \author Boshen Chen
*/
class Astar : public ShortestPath {
    private:
        // 定义优先队列中的元素类型
        typedef std::pair<FPType, int> PQPair;

        // 优先队列的比较器类
        class Prioritize{
            public:
                bool operator() ( const PQPair& p1, const PQPair& p2){
                    return p1.first > p2.first;  // 按第一个元素降序排列
                }
        };

        // 定义优先队列类型
        typedef std::priority_queue<PQPair, std::vector<PQPair>, Prioritize > PriorityQueue;

        PriorityQueue *Queue;  // 优先队列指针

        std::vector<FPType> *zeroFlowTimes;    // 零流量时间向量
        std::vector<FPType> *LabelVector;      // 标签向量
        std::vector<int> *Predecessors;        // 前驱节点向量

        int nbDests_;    // 目标节点数量
        int nbNodes_;    // 节点总数

        StarNetwork *netPointer_;  // 网络指针

        FPType bestKnownPathCost_;  // 已知最佳路径成本
        PathSet* pathSet_;          // 路径集合指针
        
        void initNodes();  // 初始化节点

        void updateBestKnownPathCost(int odPairIndex);  // 更新已知最佳路径成本

    public:
        /** @param netPointer 网络指针
            @param odMatrix OD矩阵指针
        */
        Astar(StarNetwork* netPointer, ODMatrix* odMatrix);
        virtual ~Astar();

        /** 计算零流量边界
            @param odMatrix OD矩阵指针
        */
        void initializeBounds(ODMatrix* odMatrix);

        void calculate(int O);  // 计算从起点O出发的最短路径
        void calculate(int O, int D, int odPairIndex);  // 计算从O到D的最短路径
        FPType getCost(int destIndex) const;  // 获取目标节点的成本
        StarLink* getInComeLink(int destIndex) const;  // 获取目标节点的入边

        /** 存储路径集合的指针
            \note 必须在对象创建后调用一次
        */
        void setPathSet(PathSet* pathSet);

    protected:
        /** @return 如果区域质心需要跳过返回true，否则返回false
        */
        virtual bool shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const;

};

#endif
