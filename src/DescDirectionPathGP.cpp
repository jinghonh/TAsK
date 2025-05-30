#include "DescDirectionPathGP.h"  // 包含头文件

#include <cassert>  // 包含断言头文件

// 构造函数，初始化基类
DescDirectionPathGP::DescDirectionPathGP(FPType delta) : DescDirectionPath(delta) {

};

// 析构函数
DescDirectionPathGP::~DescDirectionPathGP(){

};

// 创建路径方向
// @param size 路径数量
// @param paths 路径列表
// @param isEquilibrated 是否达到平衡状态
// @return 返回路径方向数组
PathAndDirection** DescDirectionPathGP::createPathDirection(int &size, const std::list<Path*> &paths,
                                                            bool &isEquilibrated){
  	
  // 首先找到最小距离路径
  Path *minPath = findMinPath(paths);  // 获取最小成本路径
  FPType minDist = minPath->getCurrCost();  // 获取最小成本
  
  isEquilibrated = false;  // 初始化为非平衡状态
  size = paths.size();  // 设置路径数量
  PathAndDirection **returnSet = new PathAndDirection*[size];  // 创建返回数组
  for (int i = 0; i < size; ++i) {
    returnSet[i] = new PathAndDirection();  // 初始化每个元素
  }
  	
  int i = 0;  // 数组索引
  FPType minDir = 0.0;  // 最小方向值
  FPType dir = 0.0;  // 方向值
  FPType maxDir = 0.0;  // 最大方向值
  Path* path = NULL;  // 路径指针
  // 遍历所有路径
  for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
    path = *it;  // 获取当前路径
  	if (path != minPath) {  // 如果不是最小成本路径
  		(returnSet[i])->setPath(path);  // 设置路径
  		dir = minDist - path->getCurrCost();  // 计算方向值
  		assert(dir <= 0.0);  // 确保方向值非正
  		if (-dir > maxDir) maxDir = -dir;  // 更新最大方向值
  		(returnSet[i])->setDirection(dir);  // 设置方向
  		minDir -= dir;  // 累加最小方向值
  		++i;  // 更新索引
  	}
  }
  // 如果最大方向值小于等于阈值，认为达到平衡状态
  if (maxDir <= delta_){
  	isEquilibrated = true;  // 设置平衡状态
  	for (int i = 0; i < size; ++i) {
	 		delete returnSet[i];  // 释放内存
	 	}
	 	delete[] returnSet;  // 释放数组
  	size = 0;  // 重置大小
  	return NULL;  // 返回空指针
  }
  // 设置最小成本路径的方向
  (returnSet[i])->setPath(minPath);  // 设置最小成本路径
  (returnSet[i])->setDirection(minDir);  // 设置方向值
  return returnSet;  // 返回结果数组
};
