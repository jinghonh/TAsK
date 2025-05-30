// 包含必要的头文件
#include "AlgoDecoratorWriteLinks.h"  // 包含AlgoDecoratorWriteLinks类的头文件
#include "StarNetwork.h"              // 包含StarNetwork类的头文件

#include <iostream>                   // 包含输入输出流库

// 构造函数实现
// @param algo 被装饰的算法对象
// @param net 网络对象
// @param fileName 输出文件名
// @param dir 输出目录
AlgoDecoratorWriteLinks::AlgoDecoratorWriteLinks(DecoratedEqAlgo *algo, StarNetwork *net, 
                const std::string &fileName, const std::string& dir) : 
                AlgoDecoratorBase(algo), net_(net), fileName_(fileName), dir_(dir) {

}; 

// 析构函数实现
AlgoDecoratorWriteLinks::~AlgoDecoratorWriteLinks() {
}; 

// 执行算法并保存结果
// @return 返回算法执行的迭代次数
int AlgoDecoratorWriteLinks::execute(){
    int nbIter = component_->execute();  // 执行被装饰的算法
    std::cout << "正在将链路流量写入文件: " << fileName_ << std::endl;  // 输出提示信息
    net_->printToFile(dir_ + fileName_);  // 将网络数据保存到指定文件
    return nbIter;  // 返回迭代次数
};
