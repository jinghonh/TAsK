#include "StarLinkWithTolls.h" // 包含带收费的星形链接头文件

StarLinkWithTolls::StarLinkWithTolls(StarLink* link, TollType toll) : 
	StarLinkWithReverseLink(link), toll_(toll) { // 构造函数，初始化基类和收费值
};

StarLinkWithTolls::~StarLinkWithTolls() { // 析构函数

};

FPType StarLinkWithTolls::getTime() const { // 重写获取时间方法
	return toll_; // 返回收费值而非时间
};
