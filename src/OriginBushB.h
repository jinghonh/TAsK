#ifndef ORIGIN_BUSH_B
#define ORIGIN_BUSH_B

#include "OriginBush.h" // 包含起点菊花基类

#include <stdlib.h> // 包含标准库

class LineSearch; // 前向声明线搜索类

/** \brief 此类实现了DAGraphB和DAGraphBWithStep的创建。
*/
class OriginBushB : public OriginBush { // OriginBushB类，继承自OriginBush
	public:
		/** @param useMultiStep 如果需要应用多个牛顿步，则为true，否则为false。
			@param lineSearch (默认值为NULL) 如果未给出，则将创建DAGraphB，
			如果给出且不为NULL，则将创建DAGraphBWithStep。
		*/
		OriginBushB(bool useMultiStep, LineSearch* lineSearch = NULL); // 构造函数，带有多步骤标志和线搜索参数
		~OriginBushB(); // 析构函数
		
	private:
		static bool useMultiStep_; // 静态多步骤标志
		static LineSearch* lineSearch_; // 静态线搜索指针

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									  FPType zeroFlow, FPType dirTol); // 创建新DAG的虚函数实现
};

#endif
