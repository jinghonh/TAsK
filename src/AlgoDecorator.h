#ifndef _ALGO_DECOR_  // 防止头文件重复包含
#define _ALGO_DECOR_

#include "AlgoDecoratorBase.h"  // 包含基类头文件

#include <string>  // 包含字符串处理库

class AddHookStoreOut;  // 前向声明AddHookStoreOut类

/** \brief 使用AddHookStoreOut将之前存储的收敛数据写入文件来装饰EqAlgo。
*/
class AlgoDecorator : public AlgoDecoratorBase {  // 定义AlgoDecorator类，继承自AlgoDecoratorBase
	public:
		
		/** @param algo 需要被装饰的算法对象
			@param hook 包含存储数据的钩子对象
			@param fileName 收敛数据将被写入的文件名
			@param dir (默认值为空字符串) 可选参数，指定文件应该写入的目录
		*/
		AlgoDecorator(DecoratedEqAlgo *algo, AddHookStoreOut *hook, const std::string& fileName, 
			const std::string& dir = "");  // 构造函数声明
		~AlgoDecorator();  // 析构函数声明
		
		/** 调用component_的execute方法并将收敛数据写入文件。*/
		virtual int execute();  // 虚函数声明
		
		
	private:
		AddHookStoreOut *hook_;  // 存储输出的钩子对象指针
		const std::string fileName_;  // 输出文件名
		const std::string dir_;  // 输出目录
};

#endif  // 结束头文件保护
