#ifndef LABEL_CORRECTING_ONE_SOURCE_BSP  // 防止头文件重复包含的宏定义
#define LABEL_CORRECTING_ONE_SOURCE_BSP 

#include "OneSourceBiObjShPath.h"  // 包含单源双目标最短路径算法的头文件
#include "LabelSettingBSPBase.h"   // 包含标签设置BSP基类的头文件

/** \brief 单源双目标标签设置算法的具体实现。
*/
class LabelSettingOneSourceBSP : public OneSourceBiObjShPath, private LabelSettingBSPBase {  // 定义标签设置单源BSP类，继承自OneSourceBiObjShPath和LabelSettingBSPBase
	public:
		LabelSettingOneSourceBSP(StarNetwork& net, const TollContainerType& tolls);  // 构造函数，接收网络和收费容器参数
		~LabelSettingOneSourceBSP();  // 析构函数

		virtual void calculate(int originIndex);  // 虚函数，计算从指定源点出发的最短路径

};
#endif  // 结束宏定义