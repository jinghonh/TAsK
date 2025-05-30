#include "PASManagerWithStep.h" // 包含带步长的PAS管理器头文件
#include "PASWithStep.h" // 包含带步长的PAS头文件

PASManagerWithStep::PASManagerWithStep(const ShortestPath &shPath, FPType dirTol, int nbNodes, FPType mu, 
					FPType v, int nbLinks, LineSearch* lineSearch, 
					FPType zeroFlow) : 
					PASManager(shPath, dirTol, nbNodes, mu, v, zeroFlow), 
					lineSearch_(lineSearch), 
					nbLinks_(nbLinks) { // 构造函数，初始化基类和成员变量

};

PASManagerWithStep::~PASManagerWithStep(){ // 析构函数

};

PAS* PASManagerWithStep::allocatePAS(){ // 重写分配PAS的方法
	return new PASWithStep(zeroFlow_, dirTol_, lineSearch_, nbLinks_); // 创建并返回带步长的PAS
};