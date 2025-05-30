#ifndef _DERIVATIVE_  // 防止头文件重复包含
#define _DERIVATIVE_

#include "UsedTypes.h"  // 包含类型定义头文件

class LinkFncContainer;  // 前向声明链路函数容器类
class LinkFnc;  // 前向声明链路函数类

/** \brief 该类实现了目标函数关于线搜索所需步长的方向导数的计算
*/
class Derivative {
	public:
		/** @param zeroFlow 链路流量容差
			@param fnc 链路成本函数容器
		*/
		Derivative(FPType zeroFlow, LinkFncContainer *fnc);  // 构造函数
		virtual ~Derivative();  // 虚析构函数
		
		/** 计算目标函数关于步长alpha的导数
			@param alpha 步长
			@return 计算得到的导数值
		*/
		virtual FPType calculate(FPType alpha) const;  // 计算导数的虚函数
		
		/** 设置计算导数所需的数据指针
			// 每次数据指针发生变化时，在调用calculate()之前必须调用此方法
			// @param size 数据数组的大小
			// @param x 包含当前链路流量的数组
			// @param y 包含当前下降方向(以链路流量表示)的数组
			// @param indexes 用于遍历数组的索引数组(x[indexes[i]])，
			// 这样数组x和y可以比输入中指定的大小更大
		*/
		void setDataPointers(int size, FPType *x, FPType *y, int *indexes);  // 设置数据指针

		/** @return 返回链路流量容差
		*/
		FPType getZeroFlow() const;  // 获取零流量阈值
		
		/** 此方法被QuadApp使用，因为它需要存储在此类中的相同信息，
			并且导数可以在那里与其他步长计算所需的数据并行计算。
			这就是为什么在QuadApp类中不直接调用calculate()方法的原因。*/
		int getSize() const;  // 获取数据大小
		
		/** 此方法被QuadApp使用
			@return 给定链路索引的x值 */
		FPType getX(int index) const;  // 获取指定索引的x值
		/** 此方法被QuadApp使用
			@return 给定链路索引的y值 */
		FPType getY(int index) const;  // 获取指定索引的y值
		/** 此方法被QuadApp使用
			@return 给定链路索引的链路成本函数 */
		LinkFnc* getLinkFnc(int index) const;  // 获取指定索引的链路函数
		
		int getLinkIndex(int index) const;  // 获取链路索引

		
	private:
		int size_;  // 数据大小
		FPType* x_;  // 链路流量数组指针
		FPType* y_;  // 下降方向数组指针
		int* indexes_;  // 索引数组指针
		const FPType zeroFlow_;  // 零流量阈值常量
		
		LinkFncContainer* fnc_;  // 链路函数容器指针

};

#endif  // 结束头文件保护
