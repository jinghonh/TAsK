#ifndef ORIGIN_SET
#define ORIGIN_SET

#include "InitialiseSolution.h" // 包含初始化解决方案头文件
#include "AONBushBased.h" // 包含基于菊花的AON头文件

class OriginBush; // 前向声明起点菊花类
class ODMatrix; // 前向声明OD矩阵类
class StarNetwork; // 前向声明星型网络类
class PASManager; // 前向声明PAS管理器类
class LineSearch; // 前向声明线搜索类
class LabelCorrectingAl; // 前向声明标签修正算法类

#include <vector> // 包含向量容器

typedef std::vector<OriginBush*>::iterator OriginSetIterator; // 定义起点集合迭代器类型

/** \brief 该类表示OriginBush对象的容器。
	\details 它还负责实例化正确的对象。
*/
class OriginSet : public InitialiseSolution { // 起点集合类，继承自初始化解决方案类
	
	public:
		/** 必须使用此工厂方法来创建B算法。
		*/
		static OriginSet* createOriginSetB(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, bool useMultiStep, LabelCorrectingAl* LCShPath); // 创建B起点集合的静态工厂方法
		
		/** 必须使用此工厂方法来创建带线搜索的B算法。
		*/  
		static OriginSet* createOriginSetBWithStep(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, LineSearch* lineSearch, 
					LabelCorrectingAl* LCShPath); // 创建带步长的B起点集合的静态工厂方法

		/** 必须使用此工厂方法来创建LUCE算法。
		*/
		static OriginSet* createOriginSetLUCE(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, LineSearch* lineSearch, LabelCorrectingAl* LCShPath); // 创建LUCE起点集合的静态工厂方法
		
		/** 必须使用此工厂方法来创建TAPAS算法。
		*/
		static OriginSet* createOriginSetTAPAS(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, PASManager *pasManager, LabelCorrectingAl* LCShPath); // 创建TAPAS起点集合的静态工厂方法

		~OriginSet(); // 析构函数
		
		/** 通过使用AON程序初始化所有菊花。
		*/
		void initialise(); // 初始化方法
		

		/** @return 指向容器中第一个菊花的迭代器。
		*/
		OriginSetIterator begin(); // 获取开始迭代器的方法
		/** @return 容器的末尾。
		*/
		OriginSetIterator end(); // 获取结束迭代器的方法
		
		void initialiseItself(StarLink* link, PairOD *dest, int originIndex); // 初始化自身的方法，用于AON
		
		/** 在屏幕上打印菊花。
		*/
		void print(); // 打印方法
		
	private:
		StarNetwork *net_; // 星型网络指针
		const int nbOrigins_; // 起点数量
		std::vector<OriginBush*> bushes_; // 菊花对象向量
		std::vector<int> indexes_; // 索引映射向量
		AONBushBased aon_; // 基于菊花的AON对象
		
		OriginSet(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, FPType dirTol,
					PASManager *pasManager, 
					LabelCorrectingAl* LCShPath, bool useMultiStep, 
					LineSearch* lineSearch, OriginAlgoType algo); // 私有构造函数，用于工厂方法
			
};

#endif
