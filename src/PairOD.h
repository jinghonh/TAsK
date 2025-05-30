#ifndef OD_PAIR_
#define OD_PAIR_

#include "UsedTypes.h" // 包含自定义类型头文件

/** \brief 此类实现了一个目的地。它是Origin类的基本元素。
*/
class PairOD { // OD对类
	
	public:
		/** @param index 目的地索引。
			@param demand OD需求。
		*/
		PairOD(int index, FPType demand); // 构造函数，接收索引和需求参数
		~PairOD(); // 析构函数
		
		/** 在屏幕上打印所有相关信息。
		*/
		void print(); // 打印方法
		
		/** @return 需求。
		*/
		FPType getDemand() const; // 获取需求的常量方法
		/** @return 目的地索引。
		*/
		int getIndex() const; // 获取目的地索引的常量方法
		/** @return OD对索引。
		*/
		int getODIndex() const; // 获取OD对索引的常量方法
		/** 将OD对索引设置为index。
		*/
		void setODIndex(int index); // 设置OD对索引的方法
	
	private:
	
		const int index_; // 目的地索引
		const FPType demand_; // OD需求
		int odIndex_; // OD对索引
};

#endif
