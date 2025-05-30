#ifndef LABEL_CORRECTING_BSP_BASE  // 防止头文件重复包含
#define LABEL_CORRECTING_BSP_BASE

#include "BiObjLabelContainer.h"  // 包含双目标标签容器头文件
#include "StarNetwork.h"          // 包含星形网络头文件
#include "LexOptComparison.h"     // 包含字典序优化比较头文件

#include <set>                    // 包含集合容器头文件

typedef std::multiset<BiObjLabel*, LexOptComparison > QueueType;  // 定义队列类型为多重集合

/** \brief 实现单源双目标标签设置算法主要步骤的抽象基类
    \note 与单目标情况不同，允许有效路径穿过区域
*/
class LabelSettingBSPBase {
    public:
        virtual ~LabelSettingBSPBase() = 0;  // 纯虚析构函数

    protected:
        StarNetwork& net_;                    // 网络引用
        const TollContainerType& tolls_;      // 通行费容器常量引用

        /** @param net 网络
            @param tolls 包含链路通行费的容器
        */
        LabelSettingBSPBase(StarNetwork& net, const TollContainerType& tolls);

        /** 实现单源双目标标签设置算法的通用结构
            用于计算从originIndex开始的非支配标签
        */
        void calculateBSP(int originIndex, BiObjLabelContainer& labels); 

        /** @return 如果停止条件尚未满足返回true，否则返回false
        */
        virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;

        /** 查找下一个必须访问的标签并将其从标签列表中移除
            @return 指向下一个必须访问的标签的指针
        */
        virtual BiObjLabel* getNextLabelAndRemoveItFromList(QueueType& tmpLabels);

        /** 子类的钩子方法
            @return true
        */
        virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const {
            return true;
        };

        /** 初始化每个节点的潜在有效标签集和非支配标签集
        */
        virtual QueueType* initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels);

        /** 不做任何操作。此方法在双向搜索中重新定义
        */
        virtual QueueType* changeDirectionIfNeeded();
        /** 不做任何操作。此方法在双向搜索中重新定义
        */
        virtual BiObjLabelContainer* changeLabelsIfNeeded(BiObjLabelContainer* labels);
        /** @return 指向索引为owner的节点的指针
        */
        virtual StarNode* beginNode(int owner) const;
        /** @return 指向从beginNode返回的节点出发的链路的指针
        */
        virtual StarLink* beginLink() const;
        /** @return 指向从beginNode返回的节点出发的下一个链路的指针
        */
        virtual StarLink* getNextLink() const;

        /** 将标签添加到属于nodeIndex的标签集中
        */
        virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, BiObjLabel* label);
        /** 子类的钩子方法。默认不做任何操作
        */
        virtual void uploadPaths();
        /** 子类的钩子方法。默认不做任何操作
        */
        virtual void combine(BiObjLabel* newLabel);

        /** @return 如果由newTime和newToll指定的标签不被属于节点linkHead的任何标签支配则返回true，否则返回false
        */
        bool isNonDominated(FPType newTime, TollType newToll, const BiObjLabelContainer& labels,
                int linkHead) const;

        /** 遍历属于节点linkHead的所有标签，移除被label支配的任何标签
            将label添加到属于节点linkHead的标签集中
        */
        void mergeLabels(BiObjLabel* label, BiObjLabelContainer& labels, int linkHead,
                QueueType& tmpLabels);

    private:
        QueueType tmpLabels_;                 // 临时标签队列
        std::vector<StarLink*> seenLinks_;    // 已访问链路向量

        /** 为新标签分配内存
            @return 新创建的标签
        */
        BiObjLabel* createLabel(BiObjLabel* currentLabel, StarLink* link) const;
        /** 如果labelInNode在tmpLabels中则删除它。实现较复杂，
            因为所有标签都按字典序排序，但我们需要删除确切的labelInNode，
            而不是恰好具有相同旅行时间值的其他标签
        */
        void deleteFromTmpLabels(QueueType& tmpLabels, BiObjLabel* labelInNode);
};
#endif
