#ifndef DESC_DIRECTION_PATH_PE_SCALED_  // 防止头文件重复包含的宏定义
#define DESC_DIRECTION_PATH_PE_SCALED_

#include "DescDirectionPath.h"  // 包含基类头文件

/** \brief 该类实现了基于路径算法的缩放下降方向。
    \details 缩放方向意味着通常的下降方向除以目标函数相对于流量移动的二阶导数。
    缩放方向通常应用于方法2和3（这些方法在PE和GP算法中实现，参见PathAlgoCreator）。
*/
class DescDirectionPathScaled : public DescDirectionPath {  // 定义缩放下降方向类，继承自DescDirectionPath
	public:
		/** @param pathDir 实现下降方向计算的对象，该方向将被缩放。
            @param nbLinks 网络中的链路数量。
            @param delta 下降方向容差。
        */
		DescDirectionPathScaled(DescDirectionPath *pathDir, int nbLinks, FPType delta);  // 构造函数
		
		/** 该类负责销毁pathDir对象。 */
		~DescDirectionPathScaled();  // 析构函数
		
		/** \note 假设方向的正值始终是路径中的最后一个。
        */
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
							bool &isEquilibrated);  // 创建路径方向的方法

		
	private:
		DescDirectionPath *pathDir_;  // 下降方向计算对象指针
		const int nbLinks_;  // 网络中的链路数量
};

#endif  // 结束头文件保护宏
