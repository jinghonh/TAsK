#ifndef TIMER_
#define TIMER_

#include "UsedTypes.h" // 包含自定义类型定义

#include <ctime> // 包含C时间库

/** \brief 这个类表示一个用于测量CPU时间的计时器。

    这个类简化了测量经过时间的方法。
*/
class Timer {
	public:
		Timer(); // 构造函数
		~Timer(); // 析构函数
		
		/** 启动计时器。可以根据需要多次调用以重启计时器。
            如果从未调用此方法，则计时起点将是Timer对象的创建时间。
		*/
		void start(); // 开始/重启计时器
		
		/** @return 返回自对象创建或上次调用start()方法以来经过的时间（以秒为单位）。
		*/
		FPType getTimePassed() const; // 获取经过的时间（秒）
		
	private:
		clock_t start_; // 存储开始时间点
		
};

#endif // TIMER_头文件结束
