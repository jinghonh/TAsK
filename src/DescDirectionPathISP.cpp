#include "DescDirectionPathISP.h"  // 包含头文件
#include "StarLink.h"  // 包含StarLink头文件

#include <cassert>  // 包含断言头文件

// 构造函数实现
DescDirectionPathISP::DescDirectionPathISP(FPType slope, FPType scaleFact, FPType delta):
                        DescDirectionPath(delta), 
												slope_(slope), scaleFact_(scaleFact) {
};

// 析构函数实现
DescDirectionPathISP::~DescDirectionPathISP(){

};
		
// 创建路径方向的方法实现
PathAndDirection** DescDirectionPathISP::createPathDirection(int &size, const std::list<Path*> &paths,
                                              bool &isEquilibrated){
	
  	// 寻找最短和最长的路径
  	Path *minPath = findMinPath(paths);  // 获取最短路径
  	FPType minDist = minPath->getCurrCost();  // 获取最短路径成本
  	Path *maxPath = findMaxPath(paths, minPath);  // 获取最长路径
  	FPType maxDist = maxPath->getCurrCost();  // 获取最长路径成本
  	
  	// 检查O-D是否达到平衡
  	if (maxDist - minDist <= delta_){
  		isEquilibrated = true;  // 设置平衡标志
  		size = 0;  // 设置大小为0
  		return NULL;  // 返回空指针
  	}
  	
    isEquilibrated = false;  // 设置不平衡标志
    size = paths.size();  // 设置大小为路径数量
    // 分配内存
    PathAndDirection **returnSet = new PathAndDirection*[size];  // 创建返回数组
    for (int i = 0; i < size; ++i) {
      returnSet[i] = new PathAndDirection();  // 初始化每个元素
    }

  	// 计算下降方向
  	FPType thr = calculateThreshold(minDist, maxDist);  // 计算阈值
  	FPType pathCost = 0.0;  // 路径成本
  	int count = 0;  // 计数器
  	FPType dir = 0.0;  // 方向
  	FPType totalDir = 0.0;  // 总方向
  	Path *curPath = NULL;  // 当前路径
  	int countCheap = 0;  // 廉价路径计数
  	int indexes[size];  // 索引数组
  	for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
  		curPath = *it;  // 获取当前路径
  		assert(curPath != NULL);  // 断言当前路径不为空
  		pathCost = curPath->getCurrCost();  // 获取当前路径成本
  		if (pathCost > thr) { // 成本较高的路径集合
  			dir = minDist - pathCost;  // 计算方向
  			totalDir -= dir;  // 更新总方向
  		} else {
  			dir = 0.0;  // 设置方向为0
  			indexes[countCheap] = count;  // 记录索引
  			++countCheap;  // 增加廉价路径计数
  		}
  		(returnSet[count])->setDirection(dir);  // 设置方向
  		(returnSet[count])->setPath(curPath);  // 设置路径
  		++count;  // 增加计数
  	}
  	
  	// 首先计算斜率和总斜率
  	FPType totalSlope = 0.0;  // 总斜率
  	FPType curSlope = 0.0;  // 当前斜率
  	FPType slopes[countCheap];  // 斜率数组
  	for (int i = 0; i < countCheap; ++i){
  		curSlope = calculateDerivative((returnSet[indexes[i]])->getPath());  // 计算导数
  		if (curSlope < slope_) curSlope = slope_;  // 确保斜率不小于最小斜率
  		slopes[i] = curSlope;  // 保存斜率
  		totalSlope += 1.0 / curSlope;  // 更新总斜率
  	}
  	// 计算廉价路径的方向
  	for (int i = 0; i < countCheap; ++i){
  		count = indexes[i];  // 获取索引
  		dir = totalDir / (slopes[i] * totalSlope);  // 计算方向
  		(returnSet[count])->setDirection(dir);  // 设置方向
  	}
	return returnSet;  // 返回结果集
};

// 计算阈值的方法实现
FPType DescDirectionPathISP::calculateThreshold(FPType minDist, FPType maxDist) const{
	return minDist + scaleFact_ * (maxDist - minDist);  // 返回阈值
};

// 计算导数的方法实现
FPType DescDirectionPathISP::calculateDerivative(Path* path) const{
	FPType der = 0.0;  // 导数
	FPType flow = 0.0;  // 流量
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
    StarLink* link = *it;  // 获取当前链接
		flow = link->getFlow();  // 获取流量
		if (flow < slope_) {
			flow = slope_;  // 确保流量不小于最小斜率
		} 
		der +=  link->getDerivative();  // 累加导数
	}
	return der;  // 返回总导数
};
