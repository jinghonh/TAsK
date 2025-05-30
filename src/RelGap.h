#ifndef REL_GAP // 防止头文件重复包含
#define REL_GAP // 定义REL_GAP宏

#include "ConvMeasure.h" // 包含收敛测量类

class StarNetwork; // 前向声明星形网络类
class ODMatrix; // 前向声明OD矩阵类
class ShortestPath; // 前向声明最短路径类

/** \brief 此类实现相对差距计算。
	\details \f$ RGAP = rGap = 1 - \frac{\sum_{p \in Z}D_p \cdot C_{min}^p}{\sum_{a \in A} f_a \cdot c_a}\f$
	其中 \f$C_{min}^p = \min_{k \in K_p} C_k \f$ 是OD对 \f$ p \f$的最短路径。
*/
class RelGap : public ConvMeasure{ // 相对差距类，继承自收敛测量类
	
	public:

		/** @param precision 相对差距精度，用于决定何时停止计算。
			@param net 网络。
			@param mat OD矩阵。
			@param shPath 最短路径算法。
		*/
		RelGap(FPType precision, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath); // 构造函数
		~RelGap(); // 析构函数
		
	private:
		
		StarNetwork *net_; // 星形网络指针
		ODMatrix *mat_; // OD矩阵指针
		ShortestPath *shPath_; // 最短路径指针
		
		FPType getMinTravelTime(); // 获取最小旅行时间
		FPType getTotalTravelTime(); // 获取总旅行时间
		
		/** 如果总旅行时间接近零(\f$ \leq 10^{-25}\f$)，此方法返回无穷大。
			假设在这种情况下问题不可行。这种情况只在Frank-Wolf方法(见FWAlgo)
			的第一次迭代中出现，因为AON分配后链接流量尚未更新，但最小旅行时间已经计算。
		*/
		FPType calculateGapValue(FPType minTravelTime, FPType totalTravelTime); // 计算差距值

		FPType calculateGap(); // 计算差距
		FPType calculateGap(FPType minTravelTime); // 使用给定的最小旅行时间计算差距
		
};

#endif // 结束头文件保护