#ifndef PAS_SET
#define PAS_SET

#include "UsedTypes.h" // 包含自定义类型定义

#include <vector> // 包含向量容器
#include <list> // 包含链表容器
#include <unordered_set> // 包含无序集合容器

class ShortestPath; // 前向声明最短路径类
class DAGraphTapas; // 前向声明TAPAS有向无环图类
class PAS; // 前向声明PAS（备选路段对）类
class StarLink; // 前向声明星型链接类

typedef std::list<PAS*>::const_iterator CostPasIterator; // 定义PAS迭代器类型

/** \brief 该类是PAS的容器，实现了与其管理相关的主要操作。
*/
class PASManager { // PAS管理器类
	public:
		/** @param shPath 最短路径算法。
			@param dirTol 下降方向的容差。
			@param nbNodes 网络中的节点数。
			@param mu TAPAS特定常数，参见 \cite Hillel2010。
			@param v TAPAS特定常数，参见 \cite Hillel2010。
			@param zeroFlow 链接流量容差。
		*/
		explicit PASManager(const ShortestPath &shPath, FPType dirTol, int nbNodes,
							FPType mu, FPType v, FPType zeroFlow); // 显式构造函数
		virtual ~PASManager(); // 虚析构函数
		
		/** 检查具有指定昂贵链接 \b expLink 的PAS是否已存在。
			如果存在，方法将当前菊花添加到现有PAS的相关起点集合中，并检查当前PAS是否有效。
			如果PAS不存在，则创建新的PAS（如果可能），并检查它是否有效。
			如果创建的或现有的PAS无效，则创建另一个有效的PAS。
			\note 分支移动未实现。
		*/
		void createNewPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex); // 创建新PAS 

		/** 遍历所有PAS并检查它们是否未使用。如果PAS未使用，
			则将其从PAS集合中删除。否则，在该PAS内移动流量。
		*/
		void deleteUnusedPASAndMoveFlow(); // 删除未使用的PAS并移动流量
		
		/** @return 指向PAS容器开头的迭代器。
		*/
		CostPasIterator begin() const; // 获取开始迭代器
		/** @return 迭代器的结束。
		*/
		CostPasIterator end() const; // 获取结束迭代器
		/** @return PAS的总数。
		*/
		int getNbPAS() const; // 获取PAS数量
		/** 在屏幕上打印所有PAS。
		*/
		void print() const; // 打印所有PAS
		/** 遍历所有PAS并重新计算它们的路径成本。
		*/
		void recalculatePASCosts(); // 重新计算所有PAS成本
		
	protected:
		
		const FPType zeroFlow_; // 零流量阈值常量
		const FPType dirTol_; // 方向容差常量

		/** 分配适当类型的PAS并返回指向它的指针。
		*/
		virtual PAS* allocatePAS(); // 分配PAS（虚函数）

	private:
		const ShortestPath &shPath_; // 最短路径引用
		std::list<PAS*> listOfPas_; // PAS列表
		
		const int nbNodes_; // 节点数常量
		const FPType mu_; // mu常量
		const FPType v_; // v常量
		
		std::unordered_set<int> setOfNodes_; /**< 目前使用tr1无序集合，
													替代方案是std集合。
												\note 需要对更大的实例进行测试，以查看不同数据结构
												在性能上的差异。
												目前尚无差别。 */

		int nbIter_; // 迭代次数
		
		/** 检查其路段以合并链接结束的PAS是否存在。
			@returns 找到的PAS指针，否则为NULL。
		*/
		PAS* pasExist(StarLink* cheapLink, StarLink* expLink); // 检查PAS是否存在
		
		/** 检查索引为 \b nodeIndex 的节点是否属于当前最短路径树的当前最短路径。
		*/
		bool nodeBelongsToShPath(int nodeIndex); // 检查节点是否属于最短路径
		
		/** 从BFS树中回溯昂贵路段。
		*/
		void createExpSegment(PAS *pas, const std::vector<StarLink*> &checkedLinks, int divergeNode, 
								int mergingNodeIndex); // 创建昂贵路段
		/** 从合并索引 \b mergingNodeIndex 开始构建廉价路段，并在达到 \b divergeNode 或起点时停止。
		*/
		void createCheapSegment(PAS *pas, int divergeNode, int mergingNodeIndex); // 创建廉价路段

		/** 填充setOfNodes_以确保快速访问最短路径。
		*/
		inline void prepareShPathStructure(int mergingNodeIndex); // 准备最短路径结构
		
		/** @return 计算的降低成本。
		*/
		FPType calculateReducedCost(StarLink *expLink); // 计算降低成本
		
		/** @return 取决于当前迭代的阈值。它用于有效性检查。
		*/
		FPType calcThreshold(); // 计算阈值

		/** 创建PAS（包括有效PAS）：如果可能，创建廉价和昂贵路段，并将其添加到内部PAS容器中。
		*/
		PAS* createPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex, 
					bool (*checkPtr)(FPType originFlow, FPType thr)); // 创建PAS
		
		
};

#endif
