#ifndef _LINK_FNC_CONTAINER_  // 防止头文件重复包含
#define _LINK_FNC_CONTAINER_

class LinkFnc;  // 前向声明链路成本函数类
class StarNetwork;  // 前向声明星形网络类

#include <vector>  // 包含向量容器库

typedef std::vector<LinkFnc*>::iterator LinkIterator;  // 定义链路迭代器类型

/** \brief 该类是链路成本函数的容器
    \details 在Derivative中使用以实现更快的访问。另外，所有链路成本函数也存储在StarLink对象中
*/
class LinkFncContainer {  // 定义链路函数容器类
	public:
		LinkFncContainer(StarNetwork *net);  // 构造函数，接收星形网络指针
		~LinkFncContainer();  // 析构函数
		
		/** @return 返回容器第一个元素的迭代器 */
		LinkIterator begin();  // 获取容器起始迭代器
		/** @return 返回容器末尾 */
		LinkIterator end();  // 获取容器结束迭代器
		
		/** @return 返回索引为index的链路成本函数指针 */
		LinkFnc* getLinkFnc(int index);  // 获取指定索引的链路函数
		
	private:
		std::vector<LinkFnc*> links_;  // 存储链路函数指针的向量
};

#endif  // 结束头文件保护
