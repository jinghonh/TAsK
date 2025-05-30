// 包含必要的头文件
#include "DescDirectionPathScaled.h"
#include "Utils.h"

// 构造函数实现
DescDirectionPathScaled::DescDirectionPathScaled(DescDirectionPath *pathDir, int nbLinks,
											FPType delta): 
											DescDirectionPath(delta), pathDir_(pathDir),
											nbLinks_(nbLinks){

};

// 析构函数实现
DescDirectionPathScaled::~DescDirectionPathScaled(){
	delete pathDir_;  // 释放pathDir_指针指向的内存
};

// 创建路径方向的函数实现
PathAndDirection** DescDirectionPathScaled::createPathDirection(int &size, const std::list<Path*>
										&paths, bool &isEquilibrated){
	// 调用基类方法创建路径方向
	PathAndDirection** returnVal = pathDir_->createPathDirection(size, paths, isEquilibrated);
	if (isEquilibrated) {
		return NULL;  // 如果已平衡则返回空
	}
	// 假设方向的正值始终是最后一个！！！
	FPType positiveDir = 0.0;  // 初始化正方向值
	FPType currDirection = 0.0;  // 初始化当前方向值
	FPType currDer = 0.0;  // 初始化当前导数值
	Path *minPath = (returnVal[size-1])->getPath();  // 获取最小路径
	for (int i = 0; i < size-1; ++i) {
		currDer = Utils::calculatePathDerivative((returnVal[i])->getPath(), minPath, nbLinks_);  // 计算路径导数
		currDirection = (returnVal[i])->getDirectionValue() / currDer;  // 计算当前方向值
		(returnVal[i])->setDirection(currDirection);  // 设置当前方向
		positiveDir -= currDirection;  // 更新正方向值
	}
	(returnVal[size-1])->setDirection(positiveDir);  // 设置最后一个方向值
	return returnVal;  // 返回结果
};
