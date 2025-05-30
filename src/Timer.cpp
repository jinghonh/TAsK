#include "Timer.h" // 包含计时器头文件

Timer::Timer() : start_(clock()) { // 构造函数，初始化开始时间为当前时钟值

}; 

Timer::~Timer() { // 析构函数

}; 

void Timer::start() { // 开始计时方法
	start_ = clock(); // 将开始时间设置为当前时钟值
}; 

FPType Timer::getTimePassed() const { // 获取经过时间的方法
	return (static_cast<FPType>(clock() - start_))/ (CLOCKS_PER_SEC); // 计算并返回经过的秒数
}; 

