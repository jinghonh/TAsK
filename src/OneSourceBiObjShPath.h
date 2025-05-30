#ifndef ONE_SOURCE_BI_OBJ_SH_PATH
#define ONE_SOURCE_BI_OBJ_SH_PATH 

#include "BiObjLabelContainer.h" // 包含双目标标签容器

/** \brief 定义所有单源双目标最短路径算法接口的抽象基类。
*/
class OneSourceBiObjShPath { // 单源双目标最短路径抽象基类
	public:
		
		virtual ~OneSourceBiObjShPath() {}; // 虚析构函数
		/** 计算双目标最短路径。
			@param originIndex 起点索引。
		*/
		virtual void calculate(int originIndex) = 0; // 纯虚函数，计算最短路径
		/** @return 非支配标签容器的引用。
		*/
		const BiObjLabelContainer& getLabels() const { // 获取标签的常量方法
			return labels_; // 返回标签容器
		};

	protected:
		BiObjLabelContainer labels_; // 双目标标签容器
		/** @param nbNodes 网络中节点的总数。
		*/
		OneSourceBiObjShPath(int nbNodes) : labels_(nbNodes) {}; // 受保护的构造函数，初始化标签容器

};

#endif