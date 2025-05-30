#ifndef _DESC_DIRECTION_  // 防止头文件重复包含
#define _DESC_DIRECTION_

#include "UsedTypes.h"  // 包含基本类型定义

class PathBasedFlowMove;  // 前向声明路径流移动类

/** \brief 这是一个基类，定义了在基于路径和基于链路的算法中
    当通过线搜索计算步长时使用的接口方法。
*/
class DescDirection {
	
	public:
		virtual ~DescDirection(){};  // 虚析构函数
		/** @return 返回一个指向下降方向数组的指针，该方向以链路流量表示
		*/
		virtual FPType* getProjectedDirection() = 0;  // 获取投影方向
		/** @return 返回线搜索的上界 */
		virtual FPType getUpperBound() = 0;  // 获取上界
		/** @return 返回指向当前链路流量数组的指针 */
		virtual FPType* getLinkFlows() = 0;  // 获取链路流量
		/** @return 返回索引数组的大小 */
		virtual int getSize() = 0;  // 获取大小
		/** @return 返回指向索引数组的指针。例如，参见Derivative类 */
		virtual int* getIndexes() = 0;  // 获取索引数组

		/** 此方法用于基于路径的算法
			@return 返回O-D对索引 */
		virtual int getOdIndex() const = 0;  // 获取O-D对索引
		/** 此方法用于基于路径的算法
			@return 返回实现路径流移动的对象的指针 */
		virtual PathBasedFlowMove* getPathBasedFlowMove() const = 0;  // 获取路径流移动对象
		
	protected:
		DescDirection(){};  // 保护构造函数
};

#endif
