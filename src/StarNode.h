#ifndef STAR_NODE // 防止头文件重复包含
#define STAR_NODE

/** \brief 这个类是StarNetwork的基本元素。它表示一个带有出边的节点。
*/
class StarNode { // 星形节点类
	public:
		/** @param id 节点ID。
			@param isZone 如果节点是区域节点则为true，否则为false
		*/
		StarNode(int id, bool isZone); // 构造函数
		~StarNode(); // 析构函数
		
		/** @return 节点ID。
		*/
		int getID(); // 获取节点ID方法
		/** @return 如果节点是区域节点则为true，否则为false
		*/
		bool getIsZone(); // 获取节点是否为区域节点方法
		/** @return 节点内部索引。
		*/
		int getIndex(); // 获取节点索引方法
		/** 设置节点内部索引。
		*/
		void setIndex(int index); // 设置节点索引方法
		
	private:
		const int id_; // 节点ID
		int index_; // 节点索引
		const bool isZone_; // 节点是否为区域节点标志

};

#endif
