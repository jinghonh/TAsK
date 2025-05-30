#ifndef STAR_LINK_WITH_REVERSE_LINK // 防止头文件重复包含
#define STAR_LINK_WITH_REVERSE_LINK 

#include "StarLink.h" // 包含星形链接头文件

/** \brief 实现一种特殊类型的链接，该链接存储指向原始链接的指针。
*/
class StarLinkWithReverseLink : public StarLink { // 带反向链接的星形链接类，继承自StarLink
	public:
		StarLinkWithReverseLink(StarLink* link); // 构造函数
		~StarLinkWithReverseLink(); // 析构函数

		virtual FPType getTime() const; // 重写获取时间方法

		virtual StarLink* getForwardLink(); // 重写获取前向链接方法

	private:
		StarLink* originalLink_; // 原始链接指针

};

#endif
