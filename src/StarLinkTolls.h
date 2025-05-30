#ifndef STAR_LINK_TOLLS // 防止头文件重复包含
#define STAR_LINK_TOLLS 

#include "StarLink.h" // 包含星形链接头文件

/** \brief 实现一种特殊类型的链接，该链接返回常量收费而不是返回行程时间。
*/
class StarLinkTolls : public StarLink { // 带收费的星形链接类，继承自StarLink
	public:
		StarLinkTolls(StarLink* link, TollType toll); // 构造函数
		~StarLinkTolls(); // 析构函数
		/** @return 常量链接收费。
		*/
		virtual FPType getTime() const; // 重写获取时间方法
		/** @return 指向原始链接的指针，该链接计算正确的链接行程时间。
		*/
		virtual StarLink* getForwardLink(); // 重写获取前向链接方法
		
	private:
		TollType toll_; // 收费值
		StarLink* originalLink_; // 原始链接指针
};
#endif
