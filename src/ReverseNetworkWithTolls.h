#ifndef REVERSED_NETWORK_WITH_TOLLS // 防止头文件重复包含
#define REVERSED_NETWORK_WITH_TOLLS 

#include "ReverseNetwork.h" // 包含反向网络头文件
#include "UsedTypes.h" // 包含类型定义头文件

/** \brief 用于创建反向网络的工厂类，其中每个链接成本是
	常数收费。
*/
class ReverseNetworkWithTolls : public ReverseNetwork { // 带收费的反向网络类，继承自ReverseNetwork
	public:
		ReverseNetworkWithTolls(const TollContainerType& tolls); // 构造函数，接收收费容器
		~ReverseNetworkWithTolls(); // 析构函数


	protected:
		virtual StarLink* createLink(StarLink* link); // 重写创建链接的虚方法

	private:
		const TollContainerType& tolls_; // 存储收费的常量引用
};
#endif