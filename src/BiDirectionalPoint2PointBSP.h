#ifndef BI_DIRECTIONAL_POINT_TO_POINT
#define BI_DIRECTIONAL_POINT_TO_POINT 

#include "LabelSettingPoint2PointBSP.h"  // 包含标签设置点对点BSP头文件

class PathsAdder;  // 前向声明路径添加器类

class BoundsCalculatorForBackwardNet;  // 前向声明反向网络边界计算器类

/** \brief 双向双目标点对点最短路径算法
	\author Benoit Mulocher, Olga Perederieieva
*/
class BiDirectionalPoint2PointBSP : public LabelSettingPoint2PointBSP {  // 继承自标签设置点对点BSP类
	public:

		/** @param net 网络
			@param tolls 包含路段收费的容器
			@param bounds 实现行程时间下界的对象
			@param pathAdder 用于向目标节点添加已知路径的对象
			@param backwardBounds 反向网络链接的行程时间边界
			@param dominanceRule 实现路径成本支配规则的对象
		*/
		BiDirectionalPoint2PointBSP(StarNetwork& net, const TollContainerType& tolls, 
							BoundsCalculatorForBSPBase& bounds, 
							const PathsAdder& pathAdder,
							BoundsCalculatorForBackwardNet& backwardBounds,
							DominationByPathCostBase& dominanceRule);

		~BiDirectionalPoint2PointBSP();  // 析构函数

		/** 从给定标签构建路径
			@param labelWithMinCost 用于恢复路径的标签
			@param path 方法调用后将添加链接的空路径对象
		*/
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path);

	protected:
		
		/** \note 必须先调用父类的initLabels()并返回父类的返回值
			此外，将目标标签添加到后向列表和后向标签中
			@param originLabel 包含在起点节点中的标签
			@param labels 所有存储标签的容器
			@return 指向用于检查停止条件的前向标签数据结构的指针
		*/
		virtual QueueType* initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels);
		
		/** 实现双向搜索的停止条件
			@param tmpLabels 包含所有未访问标签的列表
			@return 如果算法尚未完成则返回true，否则返回false
		*/
		virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;

		/** 改变搜索方向
		    @return 如果新方向是后向则返回后向列表的引用，
			否则调用父类方法并返回其返回值
		*/
		virtual QueueType* changeDirectionIfNeeded();

		/** 根据搜索方向改变标签集
			@param labels 前向标签集
			@return 如果方向是后向则返回后向标签的指针，如果方向不是前向，
			则返回输入的标签集
		*/
		virtual BiObjLabelContainer* changeLabelsIfNeeded(BiObjLabelContainer* labels);

		/** 根据方向返回前向网络或反向网络的节点*/
		virtual StarNode* beginNode(int owner) const;
		/** 根据方向返回前向网络或反向网络的链接*/
		virtual StarLink* beginLink() const;
		/** 根据方向返回前向网络或反向网络的链接*/
		virtual StarLink* getNextLink() const;

		/** @return 如果成本向量为[newTime, newToll]的新标签值得探索则返回true，
					否则返回false
		*/
		virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const;
		/** 组合属于同一路径的后向和前向标签
			@param newLabel 新生成的标签
		*/
		virtual void combine(BiObjLabel* newLabel);
		
		/** 将标签添加到标签集
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
				BiObjLabel* label);		
	private:
		
		QueueType backwardList_; /**< 未访问的后向标签列表*/
		QueueType* forwardList_; /**< 未访问的前向标签列表*/
		BiObjLabelContainer backwardLabels_; /**< 为每个节点存储的非支配后向标签*/
		BiObjLabelContainer* forwardLabels_; /**< 为每个节点存储的非支配前向标签*/
		StarNetwork* reversedNet_;  // 反向网络指针
		bool curDirectionIsBackward_; /**< 如果是后向则为true，否则为false*/
		int backwardOriginIndex_; /**< 起点节点的索引，反向和正向网络中的节点索引相同，
								但链接索引不同*/
		BoundsCalculatorForBackwardNet& backwardBounds_;  // 反向网络边界计算器引用

};

#endif
