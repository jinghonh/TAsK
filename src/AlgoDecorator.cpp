// 包含必要的头文件
#include "AlgoDecorator.h"
#include "AddHookStoreOut.h"

// 构造函数实现
// @param algo 被装饰的算法对象
// @param hook 用于存储输出的钩子对象
// @param fileName 输出文件名
// @param dir 输出目录
AlgoDecorator::AlgoDecorator(DecoratedEqAlgo *algo, AddHookStoreOut *hook, const std::string& fileName,
					const std::string& dir) : AlgoDecoratorBase(algo),
					hook_(hook), fileName_(fileName), dir_(dir) {

};

// 析构函数实现
AlgoDecorator::~AlgoDecorator(){
};

// 执行算法并保存结果
// @return 返回算法执行的迭代次数
int AlgoDecorator::execute(){
	int nbIter = component_->execute();  // 执行被装饰的算法
	hook_->printToFile(dir_ + fileName_);  // 将结果保存到指定文件
	return nbIter;  // 返回迭代次数
};
