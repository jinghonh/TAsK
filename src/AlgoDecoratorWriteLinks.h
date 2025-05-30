#ifndef ALGO_DECOR_WRITE_LINKS  // 防止头文件重复包含
#define ALGO_DECOR_WRITE_LINKS

#include "AlgoDecoratorBase.h"  // 包含装饰器基类头文件

#include <string>  // 包含字符串处理库

class StarNetwork;  // 前向声明StarNetwork类

/** \brief 该类用于装饰EqAlgo，将链路流量写入文件。
*/
class AlgoDecoratorWriteLinks : public AlgoDecoratorBase {  // 定义AlgoDecoratorWriteLinks类，继承自AlgoDecoratorBase
	public:
		/** @param algo 需要被装饰的算法对象，在该类中被释放
			@param net 网络对象
			@param fileName 链路流量将被写入的文件名
			@param dir (默认值为空字符串) 可选参数，指定文件应该写入的目录
		*/
		AlgoDecoratorWriteLinks(DecoratedEqAlgo *algo, StarNetwork *net, const std::string &fileName,
			const std::string& dir = "");  // 构造函数声明
		~AlgoDecoratorWriteLinks();  // 析构函数声明
		
		/** 调用component_的execute方法并将链路流量写入文件。*/
		virtual int execute();  // 虚函数声明
		
		
	private:
		StarNetwork *net_;  // 网络对象指针
		const std::string fileName_;  // 输出文件名
		const std::string dir_;  // 输出目录
};

#endif  // 结束头文件保护
