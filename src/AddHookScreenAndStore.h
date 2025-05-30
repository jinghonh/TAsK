#ifndef ADD_HOOK_SCREEN_AND_STORE  // 防止头文件重复包含
#define ADD_HOOK_SCREEN_AND_STORE

#include "AddHookStoreOut.h"      // 包含存储输出钩子头文件
#include "AddHookScreenOut.h"     // 包含屏幕输出钩子头文件

/** \brief 这个类设计用于在屏幕上显示收敛信息并存储它，
    以便在算法成功执行后打印到文件中。
*/ 

class AddHookScreenAndStore : public AddHookStoreOut, public AddHookScreenOut {  // 定义屏幕和存储钩子类，继承自存储输出和屏幕输出钩子
	public:
		AddHookScreenAndStore(){};  // 构造函数
		~AddHookScreenAndStore(){}; // 析构函数
		
		/** 在屏幕上打印收敛数据并存储以供后续使用。
			@param timePassed 自上次测量以来经过的时间。
			@param gap 当前收敛度量值。
		*/
		virtual void produceOut(FPType timePassed, FPType gap) {  // 重写输出方法
			AddHookScreenOut::produceOut(timePassed, gap);        // 调用屏幕输出
			AddHookStoreOut::produceOut(timePassed, gap);         // 调用存储输出
		};
};

#endif
