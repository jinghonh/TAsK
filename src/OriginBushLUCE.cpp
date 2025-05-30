#include "OriginBushLUCE.h" // 包含LUCE起点菊花头文件
#include "DAGraphLUCE.h" // 包含LUCE有向无环图头文件

#include <stdlib.h> // 包含标准库

LineSearch* OriginBushLUCE::lineSearch_ = NULL; // 初始化静态线搜索指针为NULL

OriginBushLUCE::OriginBushLUCE(LineSearch* lineSearch) : 
				OriginBush(), dag_(NULL) { // 构造函数，初始化基类和dag_指针
	lineSearch_ = lineSearch; // 设置线搜索指针
}; 

OriginBushLUCE::~OriginBushLUCE() { // 析构函数

}; 

bool OriginBushLUCE::improve(){ // 重写改进方法
	bool wasImproved = OriginBush::improve(); // 调用基类的改进方法，并保存结果
	dag_->prepareData(); // 准备LUCE算法特定的数据
	return wasImproved; // 返回是否改进
};

DAGraph* OriginBushLUCE::createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
									  FPType dirTol){ // 创建新DAG的方法实现
	dag_ = new DAGraphLUCE(net, mat, zeroFlow, dirTol, index, lineSearch_); // 创建新的LUCE有向无环图
	return dag_; // 返回创建的图
};

