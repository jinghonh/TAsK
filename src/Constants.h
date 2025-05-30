#ifndef CONSTANTS_
#define CONSTANTS_

// 本文件包含常用常量定义

// 路径算法类型枚举，表示已实现的基于路径的算法
enum PathAlgoType {Nothing, PE, GP, PG, ISP}; 
// 路径算法应用类型枚举，仅适用于基于路径的算法
enum PathApp {AppNothing, APP1, APP2, APP3}; 

// 起点算法类型枚举，表示已实现的基于起点的算法
enum OriginAlgoType {B, Bstep, LUCE, TAPAS}; 
// OD集合类型枚举，表示OD集合的可能类型
enum ODSetType {PEAPP3, WITH_STEP, GPAPP3 };

// 边函数类型枚举，表示已实现的路段费用函数类型
enum FncType {CONSTFNC, LINEARFNC, BPRFNC, 
	SPIESSFNC, SPIESSCOMBFNC, CONSTLINEARFNC, 
	SPIESSLINEARFNC, SPIESSCOMBLINEARFNC, FD12, FD22, ERROR}; 

#endif
