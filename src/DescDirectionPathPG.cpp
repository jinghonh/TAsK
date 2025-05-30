#include "DescDirectionPathPG.h"  // 包含头文件

#include <math.h>      // 数学函数库
#include <limits>      // 数值限制库
#include <cassert>     // 断言库

// 构造函数，初始化delta值
DescDirectionPathPG::DescDirectionPathPG(FPType delta) : DescDirectionPath(delta) {

};

// 析构函数
DescDirectionPathPG::~DescDirectionPathPG(){

};

// 创建路径方向的方法
PathAndDirection** DescDirectionPathPG::createPathDirection(int &size, const std::list<Path*> &paths, 
      bool &isEquilibrated){	
  	  
  // 首先计算平均距离
  FPType avDist = 0.0;    // 平均距离
  FPType minDist = std::numeric_limits<FPType>::infinity( );  // 最小距离，初始化为无穷大
  FPType cost = 0.0;      // 当前路径成本
  FPType maxDist = 0.0;   // 最大距离
 
  // 遍历所有路径，计算总距离、最小距离和最大距离
  for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
  	cost = (*it)->getCurrCost();  // 获取当前路径成本
  	avDist += cost;               // 累加总距离
  	if (cost < minDist) {         // 更新最小距离
      minDist = cost;
    }
  	if (cost > maxDist) {         // 更新最大距离
      maxDist = cost;
    }
  }
  
  // 如果最大距离和最小距离的差值小于等于delta，则认为已达到平衡
  if (maxDist - minDist <= delta_) {
  	isEquilibrated = true;  // 设置平衡标志
  	size = 0;               // 设置大小为0
  	return NULL;            // 返回空指针
  }

  isEquilibrated = false;   // 设置不平衡标志
  size = paths.size();      // 设置大小为路径数量
  PathAndDirection **returnSet = new PathAndDirection*[size];  // 创建返回数组
  for (int i = 0; i < size; ++i) {
    returnSet[i] = new PathAndDirection();  // 初始化每个元素
  }

  avDist /= static_cast<FPType>(size);  // 计算平均距离
  int i = 0;                            // 数组索引
  FPType dirVal = 0.0;                  // 方向值
  Path* path = NULL;                    // 路径指针
  FPType dirSum = 0;                    // 方向值总和
  // 遍历所有路径，设置路径和方向
  for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
      path = *it;                       // 获取当前路径
    	(returnSet[i])->setPath(path);   // 设置路径
    	dirVal = avDist - path->getCurrCost();  // 计算方向值
    	(returnSet[i])->setDirection(dirVal);   // 设置方向
      if (i != size-1) dirSum += dirVal;       // 累加方向值（除最后一个外）
    	++i;                                    // 更新索引
  }
  (returnSet[size-1])->setDirection(-dirSum);  // 设置最后一个方向值为负的方向值总和，用于修复PG的收敛问题
 
  return returnSet;  // 返回结果数组
};
