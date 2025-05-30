#include "OriginSet.h" // 包含起点集合头文件
#include "OriginBushLUCE.h" // 包含LUCE起点菊花头文件
#include "OriginBushB.h" // 包含B起点菊花头文件
#include "OriginBushTapas.h" // 包含TAPAS起点菊花头文件
#include "Error.h" // 包含错误处理头文件
#include "Origin.h" // 包含起点头文件
#include "ODMatrix.h" // 包含OD矩阵头文件
#include "StarNetwork.h" // 包含星型网络头文件
#include "LabelCorrectingAl.h" // 包含标签修正算法头文件

OriginSet::OriginSet(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, PASManager *pasManager, 
					LabelCorrectingAl* LCShPath, bool useMultiStep,
					LineSearch* lineSearch, OriginAlgoType algo) : // 构造函数，初始化成员变量 
					net_(net), nbOrigins_(mat->getNbOrigins()),
					bushes_(mat->getNbOrigins()), indexes_(net->getNbNodes(), -1),
					aon_(*mat, LCShPath, net->getNbNodes()){

	int i = 0; // 初始化索引
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it){ // 遍历所有起点
		Origin* origin = *it; // 获取当前起点
		if (algo == B) { // 如果算法为B
			bushes_[i] =  new OriginBushB(useMultiStep); // 创建B起点菊花
		} else if (algo == Bstep) { // 如果算法为Bstep
			bushes_[i] = new OriginBushB(useMultiStep, lineSearch); // 创建带步长的B起点菊花
		} else if (algo == LUCE) { // 如果算法为LUCE
			bushes_[i] = new OriginBushLUCE(lineSearch); // 创建LUCE起点菊花
		} else if (algo == TAPAS) { // 如果算法为TAPAS
			bushes_[i] = new OriginBushTapas(LCShPath, pasManager); // 创建TAPAS起点菊花
		} else { // 如果算法无效
			throw Error("It is required to create an origin-based algorithm that is not implemented."); // 抛出错误
		}
		bushes_[i]->allocateDAG(origin->getIndex(), net, mat, zeroFlow, dirTol); // 分配DAG
		indexes_[origin->getIndex()] = i; // 设置索引映射
		++i; // 增加索引
	}

};

OriginSet* OriginSet::createOriginSetB(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, bool useMultiStep, LabelCorrectingAl* LCShPath){ // 静态工厂方法，创建B起点集合
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, useMultiStep, NULL, B); // 返回新的起点集合
};
		
OriginSet* OriginSet::createOriginSetBWithStep(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, LineSearch* lineSearch, 
			LabelCorrectingAl* LCShPath){ // 静态工厂方法，创建带步长的B起点集合
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, false, lineSearch, Bstep); // 返回新的起点集合
};

OriginSet* OriginSet::createOriginSetLUCE(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, LineSearch* lineSearch, LabelCorrectingAl* LCShPath){ // 静态工厂方法，创建LUCE起点集合
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, false, lineSearch, LUCE); // 返回新的起点集合
};
		
OriginSet* OriginSet::createOriginSetTAPAS(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, PASManager *pasManager, LabelCorrectingAl* LCShPath){ // 静态工厂方法，创建TAPAS起点集合
	return new OriginSet(mat, net, zeroFlow, dirTol, pasManager, LCShPath, false, NULL, TAPAS); // 返回新的起点集合
};

OriginSet::~OriginSet(){ // 析构函数
	for(int i = 0; i < nbOrigins_; ++i){ // 遍历所有起点菊花
		delete bushes_[i]; // 删除起点菊花
	}
};
		
void OriginSet::initialise(){ // 初始化方法
	aon_.execute(this); // 执行全有或全无分配
	net_->calculateLinkCosts(); // 计算链路成本
	
};

OriginSetIterator OriginSet::begin(){ // 获取起点集合开始迭代器的方法
	return bushes_.begin(); // 返回容器开始迭代器
};

OriginSetIterator OriginSet::end(){ // 获取起点集合结束迭代器的方法
	return bushes_.end(); // 返回容器结束迭代器
};

void OriginSet::initialiseItself(StarLink* link, PairOD *dest, int originIndex){ // 初始化自身的方法，用于AON
	bushes_[indexes_[originIndex]]->addLinkAndUpdateFlow(link, dest); // 添加链接并更新流量

};

void OriginSet::print(){ // 打印方法
	for (OriginSetIterator bushIt = begin(); bushIt != end(); ++bushIt){ // 遍历所有起点菊花
		(*bushIt)->print(); // 打印当前起点菊花
	}
};
