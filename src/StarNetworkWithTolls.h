#ifndef NETWORK_WITH_TOLLS // 防止头文件重复包含
#define NETWORK_WITH_TOLLS 

#include "UsedTypes.h" // 包含自定义类型定义

class StarNetwork; // 前向声明星形网络类
class StarLink; // 前向声明星形链接类

/** \brief 从给定网络创建所有链接都是StarLinkTolls类型的网络对象的工厂类。
*/
class StarNetworkWithTolls { // 带收费的星形网络类
	public:
		StarNetworkWithTolls(const TollContainerType& tolls); // 构造函数
		~StarNetworkWithTolls(); // 析构函数

		StarNetwork* createNetWithTolls(const StarNetwork & net); // 创建带收费的网络方法

	protected:
		virtual StarLink* createLink(StarLink* link); // 创建链接的虚方法
		

	private:
		const TollContainerType& tolls_; // 收费容器的常量引用
};
#endif
