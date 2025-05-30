#ifndef USED_TYPES // 防止头文件重复包含
#define USED_TYPES

#include <vector> // 包含向量库

/** 此类型定义所有浮点计算必须使用的精度（double或long double）。
	可以在Makefile中通过定义USE_EXTENDED_PRECISION（使用long double）
	或不定义（使用double）来更改。
*/
#ifdef USE_EXTENDED_PRECISION // 如果定义了使用扩展精度	
typedef long double FPType; // 定义FPType为long double类型
#else // 否则
typedef double FPType; // 定义FPType为double类型
#endif

typedef int TollType; /**< 收费值的类型。*/

typedef std::vector<TollType> TollContainerType; /**< 收费容器的类型。*/

#endif
