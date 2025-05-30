class StarLink; // 星形链接类声明
class StarNetwork; // 星形网络类声明

/** \brief 从给定网络创建反向网络的工厂类。

	反向网络中所有的链接都是反向的，即它们指向相反的方向。
	\warning 反向网络中的内部链接和节点索引与原始网络的内部索引不对应。

	\author Benoit Mulocher
*/
class ReverseNetwork { // 反向网络类

	public:
		ReverseNetwork(); // 构造函数
		virtual ~ReverseNetwork(); // 虚析构函数

		/** 从给定的前向星网络创建反向星网络。
		*/
		StarNetwork* createReverseStar (const StarNetwork& net); // 创建反向星网络的方法
		/** 辅助方法，检查两个网络中的节点索引是否相同。
			@return 如果所有索引都相同则返回true，否则返回false。
		*/
		bool checkNodeIndexes(const StarNetwork& net, StarNetwork* reversedNet); // 检查节点索引的方法

	protected:
		/** @return 分配的链接。
		*/
		virtual StarLink* createLink(StarLink* link); // 创建链接的虚方法
	
};