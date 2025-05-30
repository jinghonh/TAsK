#ifndef OD_MATRIX
#define OD_MATRIX

#include "UsedTypes.h" // 包含使用的类型定义

#include <string> // 包含字符串库
#include <unordered_map> // 包含无序映射容器
#include <vector> // 包含向量容器

class Origin; // 前向声明Origin类

typedef std::vector<Origin*>::iterator OriginIterator; // 定义Origin指针向量的迭代器类型
typedef std::vector<Origin*>::const_iterator ConstOriginIterator; // 定义Origin指针向量的常量迭代器类型

/** \brief 此类实现起始地-目的地矩阵。
*/
class ODMatrix { // 定义ODMatrix类
	public:
		
		ODMatrix(int nbNodes, int nbOrigins); // 构造函数，接收节点数和起点数
		~ODMatrix(); // 析构函数
		
		/** 此方法用于创建矩阵。
			向O-D矩阵添加Origin对象。
		*/
		void addOrigin(Origin *origin); // 添加起点对象到矩阵
		
		/** @return 指向容器开始的迭代器。
		*/
		OriginIterator begin(); // 获取起点容器的开始迭代器

		/** @return 容器的结束。
		*/
		OriginIterator end(); // 获取起点容器的结束迭代器

		ConstOriginIterator begin() const; // 获取起点容器的常量开始迭代器
		ConstOriginIterator end() const; // 获取起点容器的常量结束迭代器
		
		/** 此方法为所有O-D对创建O-D索引并填充数据结构，
			用于支持getDemandByIndex()方法。 
			\note 必须在所有起点都添加到类后调用一次。
		*/
		void setIndexes(); // 设置所有OD对的索引
		
		/** 在屏幕上打印O-D矩阵。
		*/
		void print(); // 打印矩阵内容到屏幕
		
		/** @return O-D对的总数。
		*/
		int getNbODPairs() const; // 获取OD对的总数
		/** @return 起点的总数。
		*/
		int getNbOrigins() const; // 获取起点的总数
		
		/** 此方法返回从起点originIndex到目的地destIndex的需求量。
			\note 平均情况下的复杂度：常数；最坏情况：容器大小的线性时间。
			详细信息请参见 http://www.cplusplus.com/reference/unordered_map/unordered_map/find/。
		*/
		FPType getDemandByIndex(int originIndex, int destIndex) const; // 根据索引获取需求量

		/** @return 给定起点索引的Origin对象指针。
		*/
		Origin* getOriginByIndex(int index) const; // 根据索引获取起点对象
		
	private:
	
		const int nbNodes_; // 节点总数
		int nbODPairs_; // OD对总数
		std::vector<Origin*> odMatrix_; /**< O-D矩阵的内部表示 */
		std::unordered_map<std::string, FPType> demandByIndex_; /**< 特殊结构，允许 */
																	/**< 给定起点和目的地索引 */
																	/**< 获取节点的需求量 */	
		inline std::string createKey(int originIndex, int destIndex) const; // 创建键的内联方法
};

#endif
