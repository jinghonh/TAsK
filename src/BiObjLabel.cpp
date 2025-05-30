// 包含必要的头文件
#include "BiObjLabel.h"
#include "StarLink.h"

#include <stdlib.h>
#include <sstream>
#include <cassert>

#include <iomanip>  

// 构造函数：初始化双目标标签对象
BiObjLabel::BiObjLabel(FPType time, TollType toll, int owner, BiObjLabel* prevLabel,
						StarLink* prevLink) : 
						time_(time), toll_(toll), owner_(owner),
						prevLabel_(prevLabel), prevLink_(prevLink) {

};

// 析构函数
BiObjLabel::~BiObjLabel(){

};

// 获取时间值
FPType BiObjLabel::getTime() const {
	return time_;
};

// 获取收费值
TollType BiObjLabel::getToll() const {
	return toll_;
};

// 获取前一个标签指针
BiObjLabel* BiObjLabel::getPrevLabel() const {
	return prevLabel_;
};

// 获取所有者ID
int BiObjLabel::getOwner() const {
	return owner_;
};

// 获取前一个链接指针
StarLink* BiObjLabel::getPrevLink() const {
	return prevLink_;
};

// 将标签信息转换为字符串
std::string BiObjLabel::toString() const{
	std::stringstream ss;
	ss << std::setprecision(20) << "time: " << getTime() << " toll: " << getToll() << ";" <<
		" owner: " << owner_ ;
		if (getPrevLink() != NULL) ss << " preLink: " << getPrevLink()->toString();
		 if (getPrevLabel() != NULL) {
		 	ss << " prevLabel: owner = " << getPrevLabel()->getOwner() << 
		 	"[" << getPrevLabel()->getTime() << ", " << getPrevLabel()->getToll() << "] link: ";
		 }
	return ss.str();
};

// 设置时间值
void BiObjLabel::setTime(FPType timeVal) {
	time_ = timeVal;
};