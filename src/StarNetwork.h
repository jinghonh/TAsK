#ifndef STAR_NETWORK
#define STAR_NETWORK

#include "UsedTypes.h"

#include <string>
#include <unordered_map>
#include <limits>
#include <list>

class StarNode;
class StarLink;

/** \brief 此类实现前向星图表示法。
	\details 详细信息请参见 \cite Sheffi1985。 */
class StarNetwork {
	public:
		StarNetwork(int nbNodes, int nbLinks, std::string &netName);
		~StarNetwork();
		
		/** 将节点 \b node 添加到内部数据结构中。
			\note 假设节点有出边，必须在节点添加后使用addLink()方法添加这些出边。
			此方法可能会抛出错误。
		*/
		void addNode(StarNode *node);

		/** 将链接 \b link 添加到内部数据结构中。链接与最后添加的节点相关联。
			此方法可能会抛出错误。
		*/
		void addLink(StarLink *link);
		
		/** 此方法必须在所有链接和节点添加到网络后调用一次。
			它为所有节点创建内部索引（所有内部索引遵循约定：对于节点和链接索引，
			分别是0到节点数量或链接数量之间的数字），并适当设置它们。
		*/
		void linkNodes();
		
		/** @return 网络的名称。
		*/
		std::string getNetName() const;
		/** @return 网络中节点的总数。
		*/
		int getNbNodes() const;
		/** @return 网络中链接的总数。
		*/
		int getNbLinks() const;
		
		/** @return 指向第一个节点的指针。必须与getNextNode()方法一起使用。
			例如，可以按以下方式遍历整个前向星网络：
			@code for (StarNode* node = beginNode(); node != NULL; node = getNextNode()) {
			    for (StarLink* link = beginLink(); link != NULL; link = getNextLink()) {
					//对链接和/或节点进行操作
				}
			} @endcode 
			\warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarNode* beginNode() const;
		/** 从输入中指定索引 \b index 的节点开始迭代过程。必须与getNextNode()方法一起使用。
			\warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarNode* beginNode(int index) const;
		/** \warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarNode* getNextNode() const;
		/** 此方法必须与getNextLink()方法一起使用。
			\warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarLink* beginLink() const;
		/** \warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarLink* getNextLink() const;

		/** 此方法用于遍历网络的所有链接。必须与getNextOnlyLink()方法一起使用。
			\warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarLink* beginOnlyLink() const;
		/** \warning 当在同一对象上多次调用此方法时，在嵌套循环中使用此方法是不安全的。
		*/
		StarLink* getNextOnlyLink() const;
		
		/** @return 指向由索引 \b linkIndex 指定的链接的指针。
		*/
		StarLink* getLink(int linkIndex) const;
		
		/** @return 指向由索引 \b index 指定的节点的StarNode*类型指针。
			但是，只有当节点有出边时才会创建此类型的节点。这就是为什么
			此方法只能返回这种节点。如果尝试使用此方法获取另一个节点，
			则返回NULL。
		*/
		StarNode* getNodeWithLinks(int index);
		
		/** @return 由其ID指定的节点的内部索引
		 	\note 尽量避免调用此函数。
		 	它的平均情况复杂度：常数，
		 	最坏情况复杂度：与容器大小成线性关系。
		*/
		int getNodeIndex(int id);
		
		/** 在屏幕上打印网络。
		*/
		void print();

		/** 将链接流量写入文件 \b fileName，格式如下：
			nodeFrom nodeTo linkFlow travelTime 
			linkFlow和travelTime的默认打印位数为16。
		*/
		void printToFile(const std::string& fileName, int precision = 16);
		
		/** 从文件 \b fileName 加载链接流量（重新计算链接行程时间）。
			文件格式为：nodeFrom nodeTo linkFlow travelTime
			\note 链接流量必须与创建的网络对象相对应，即
			与参数文件中指定的一致。
		*/
		void loadFromFile(const std::string& fileName);
		
		/** 遍历所有链接并评估所有链接成本函数。
		*/
		void calculateLinkCosts();
		
		/** @return 给定内部索引的节点ID。
			复杂度与容器大小（节点总数）成线性关系。
		*/
		int findID(int index) const;

	private:

		const std::string netName_;
		const int nbNodes_;
		const int nbLinks_;
		std::vector<StarNode*> nodes_;
		std::vector<StarLink*> links_;
		std::vector<int> pointers_;
		int size_;
		int sizeLinks_;
		mutable int curNode_;
		mutable int curLink_;
		mutable int curOnlyLink_;
		std::unordered_map<int, int> idMap_;
		bool linkAdded_;
	
		/** 创建节点ID和节点索引之间的映射表。
		*/
		void createIndexes();
		/** 此方法在loadFromFile()中使用。它尝试将链接流量 \b flow 加载到
			有节点 \b nodeFrom 和 \b nodeTo 的链接上。如果这样的链接不存在，则返回false。
		*/
		bool assignLinkFlow(int nodeFrom, int nodeTo, FPType flow) const;
};

#endif
