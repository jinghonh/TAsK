#ifndef STAR_LINK_WITH_TOLLS // 防止头文件重复包含
#define STAR_LINK_WITH_TOLLS 

#include "StarLinkWithReverseLink.h" // 包含带反向链接的星形链接头文件

/** \brief 实现一种特殊类型的链接，该链接返回常量收费而不是行程时间，
	但不重写getForwardLink()方法。
*/
class StarLinkWithTolls : public StarLinkWithReverseLink { // 带收费的星形链接类，继承自StarLinkWithReverseLink
	public:
		StarLinkWithTolls(StarLink* link, TollType toll); // 构造函数
		~StarLinkWithTolls(); // 析构函数

		virtual FPType getTime() const; // 重写获取时间方法

	private:
		TollType toll_; // 收费值

};
#endif