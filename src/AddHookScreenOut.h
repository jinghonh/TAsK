#ifndef ADD_HOOK_SCREEN_OUT
#define ADD_HOOK_SCREEN_OUT

#include "AddHook.h"

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cassert>

/** \brief 这个类设计用于在每次算法迭代后在屏幕上打印收敛信息。
*/
class AddHookScreenOut : public AddHook {
	public:
		
		/** @param precision (默认值为10) 屏幕上打印的数字位数。
		*/
		AddHookScreenOut(int precision = 10) : precision_(precision) {
			assert(precision >= 1 && precision < 17);
		};

		virtual ~AddHookScreenOut() {};
		
		/** 在屏幕上打印收敛信息。
			@param timePassed 自上次测量以来经过的时间。
			@param gap 当前收敛度量值。
		*/
		virtual void produceOut(FPType timePassed, FPType gap){
			std::cout << std::setprecision(precision_) << timePassed << " " << gap << std::endl;
		};
		
	private:
		int precision_;

};

#endif
