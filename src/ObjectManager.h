#ifndef OBJECT_MANAGER
#define OBJECT_MANAGER

#include "UsedTypes.h" // 包含自定义类型定义
#include "Constants.h" // 包含常量定义

class DescDirectionPath; // 前向声明下降方向路径类
class PathBasedFlowMove; // 前向声明基于路径的流量移动类
class StepSize; // 前向声明步长类
class PathBasedFlowMoveWithStep; // 前向声明带步长的基于路径的流量移动类
class NonAdditivePC; // 前向声明非加性路径成本类
class Params; // 前向声明参数类
class ShortestPath; // 前向声明最短路径类
class StarNetwork; // 前向声明星型网络类
class ODMatrix; // 前向声明起始地-目的地矩阵类
class DecoratedEqAlgo; // 前向声明装饰均衡算法类
class LinkFncContainer; // 前向声明链接函数容器类
class LinkFlows; // 前向声明链接流量类
class LineSearch; // 前向声明线搜索类
class ConvMeasure; // 前向声明收敛测量类
class Derivative; // 前向声明导数类
class PathSet; // 前向声明路径集合类
class PathCost; // 前向声明路径成本类
class OriginSet; // 前向声明起点集合类
class AddHook; // 前向声明添加钩子类
class AddHookStoreOut; // 前向声明存储输出的添加钩子类
class PASManager; // 前向声明PAS管理器类
class StepSizeAdd; // 前向声明加性步长类
class PathBasedFlowMoveGP; // 前向声明基于路径的梯度投影流量移动类
class LabelCorrectingAl; // 前向声明标签修正算法类
class NonAddShortestPath; // 前向声明非加性最短路径类
class IndiffCurveContainer; // 前向声明无差异曲线容器类
class OneSourceBiObjShPath; // 前向声明单源双目标最短路径类
class Point2PointBiObjShPath; // 前向声明点到点双目标最短路径类
class BoundsCalculatorForBSPBase; // 前向声明BSP边界计算基类
class PathsAdder; // 前向声明路径添加器类
class CurrentPathsAdder; // 前向声明当前路径添加器类
class Astar; // 前向声明A*算法类
class BoundsCalculatorForBackwardNet; // 前向声明反向网络边界计算器类
class DominationByPathCostBase; // 前向声明基于路径成本支配的基类
class BoundsCalculatorWithTimeUpdate; // 前向声明带时间更新的边界计算器类

/** \brief 此类负责创建所有对象。 
	\details 建议在需要某个对象时使用它。
	它还确保每个对象只创建一个实例（由一个ObjectManager实例）。
*/
class ObjectManager { // 对象管理器类
	public:
		/** @param params 包含创建适当对象所需的所有参数信息的对象。
		*/
		ObjectManager(Params *params); // 构造函数，接收参数对象指针
		~ObjectManager(); // 析构函数
		
		/** @return 网络。
		*/
		StarNetwork* getNet(); // 获取网络
		/** @return 起始地-目的地矩阵。
		*/
		ODMatrix* getODMatrix(); // 获取OD矩阵
		/** @param dirConv 存储收敛文件的目录。如果未指定，
				将使用参数中的值。
			@param dirFlows 存储链接流量文件的目录。如果未指定，
				将使用参数中的值。
			@return 参数对象中指定的交通分配算法。
		*/
		DecoratedEqAlgo* getEqAlgo(const std::string& dirConv = "",
			const std::string& dirFlows = ""); // 获取均衡算法
		/** @return 链接成本函数的容器。
		*/
		LinkFncContainer* getLinkFncCont(); // 获取链接函数容器
		/** @return 线搜索算法。
		*/
		LineSearch* getLineSearch(); // 获取线搜索算法
		/** @return 收敛测量。
		*/
		ConvMeasure* getConvMeasure(); // 获取收敛测量
		/** @return 单目标最短路径算法。
		*/
		ShortestPath* getShPath(); // 获取最短路径算法
		/** @return 导数对象。
		*/
		Derivative* getDerivative(); // 获取导数对象
		/** @return 每个OD对之间的路径集合。
		*/
		PathSet* getPathSet(); // 获取路径集合
		/** @return 负责路径成本计算的对象。
		*/
		PathCost* getPathCost(); // 获取路径成本
		/** @return 所有菊花的集合。
		*/
		OriginSet* getOriginSet(); // 获取起点集合
		/** @return PAS的管理器，参见PASManager类。
		*/
		PASManager* getPASManager(); // 获取PAS管理器
		
		/** @return 所有标量化函数的容器。
		*/
		IndiffCurveContainer* getIndiffCurveContainer(); // 获取无差异曲线容器
		/** @return 链接通行费的容器。
		*/
		TollContainerType* getTolls(); // 获取通行费

		/** @returns 如果配置文件中存在，则返回配置文件中参数的FPType值。
			如果指定参数不存在，则抛出错误。
		*/
		FPType getFloatValue(std::string paramName); // 获取浮点参数值

	private:
		Params *params_; // 参数对象指针
		
		int isAdditive_; // 是否为加性问题的标志
		PathAlgoType algoType_; // 路径算法类型
		PathApp algoApp_; // 路径算法应用
		
		StarNetwork *net_; // 星型网络指针
		ODMatrix *odMatrix_; // OD矩阵指针
		DecoratedEqAlgo *algo_; // 装饰均衡算法指针
		LinkFncContainer *linkFnc_; // 链接函数容器指针
		LinkFlows *linkFlows_; // 链接流量指针
		LineSearch *lineSearch_; // 线搜索指针
		ConvMeasure *convMeasure_; // 收敛测量指针
		ShortestPath *shPath_; // 最短路径指针
		Derivative *der_; // 导数指针
		PathSet *pathSet_; // 路径集合指针
		PathCost *pathCost_; // 路径成本指针
		OriginSet *originSet_; // 起点集合指针
		AddHook *addHook_; // 添加钩子指针
		AddHookStoreOut *addHookStore_; /**< 只是addHook_的副本，但指定了
										明确的类型 */
		PASManager* pasManager_; // PAS管理器指针
		DescDirectionPath* descDirPath_; // 下降方向路径指针
		PathBasedFlowMove* flowMove_; // 基于路径的流量移动指针
		StepSize* stepSize_; // 步长指针
		StepSizeAdd* stepSizeAdd_; /**< 只是addHook_的副本，但指定了明确类型 */
		PathBasedFlowMoveWithStep* flowMoveWithStep_; /**< 只是addHook_的副本，
									但指定了明确类型 */
		PathBasedFlowMoveGP* flowMoveGP_; /**< 只是addHook_的副本，但指定了
										明确类型 */
		
		LabelCorrectingAl *LCShortestPath_; // 标签修正算法指针
		NonAddShortestPath* nonAddShPath_; // 非加性最短路径指针
		NonAdditivePC* ndPathCost_; // 非加性路径成本指针
		IndiffCurveContainer* indiffCurveCont_; // 无差异曲线容器指针
		TollContainerType* tolls_; // 通行费容器指针
		OneSourceBiObjShPath* oneSourceBSP_; // 单源双目标最短路径指针
		Point2PointBiObjShPath* point2pointBSP_; // 点到点双目标最短路径指针
		BoundsCalculatorForBSPBase* boundsCalculator_; // BSP边界计算器指针
		PathsAdder* pathsAdder_; // 路径添加器指针
		CurrentPathsAdder* currPathsAdder_; // 当前路径添加器指针
		Astar* aStar_; // A*算法指针
		BoundsCalculatorForBackwardNet* backwardBounds_; // 反向网络边界计算器指针
		DominationByPathCostBase* dominationRule_; // 基于路径成本支配规则指针
		Astar* aStarForNonAdd_; // 用于非加性的A*算法指针
		BoundsCalculatorWithTimeUpdate* boundsWithTimeUpdate_; // 带时间更新的边界计算器指针
		
		/** @return 如果使用加性交通分配则返回true，否则返回false。
		*/
		bool getIfAdditive(); // 检查是否为加性问题
		/** @return 基于路径算法的类型，可能的值见Constants.h。
		*/
		PathAlgoType getPathAlgoType(); // 获取路径算法类型
		/** @return 基于路径算法的方法类型，可能的值见Constants.h。
		*/
		PathApp getPathAlgoApp(); // 获取路径算法应用类型
		/** @return 指向负责计算基于路径算法下降方向的对象的指针。
		*/
		DescDirectionPath* getDescDirectionPath(); // 获取下降方向路径
		/** @return 指向负责算法输出额外操作的对象的指针
				（例如，显示在屏幕上，写入文件）。
		*/
		AddHook* getAddHook(); // 获取添加钩子
		/** @return 指向负责在基于路径的算法中在路径之间移动流量的对象的指针。
		*/
		PathBasedFlowMove* getPathBasedFlowMove(); // 获取基于路径的流量移动
		/** @return 指向负责在使用线搜索计算步长的基于路径的算法中
				在路径之间移动流量的对象的指针。
		*/
		PathBasedFlowMoveWithStep* getPathBasedFlowMoveWithStep(); // 获取带步长的基于路径的流量移动
		/** @return 指向负责在梯度投影算法中在路径之间移动流量的对象的指针。
		*/
		PathBasedFlowMoveGP* getPathBasedFlowMoveGP(); // 获取基于路径的梯度投影流量移动
		/** @return 指向负责通过线搜索计算适当步长的对象的指针。
		*/
		StepSize* getStepSize(); // 获取步长
		/** @return 与getStepSize()相同，但返回的对象仅适用于加性交通分配。
		*/
		StepSizeAdd* getStepSizeAdd(); // 获取加性步长
		/** @return 指向实现非加性最短路径算法的对象的指针。
		*/
		NonAddShortestPath* getNonAddShPath(); // 获取非加性最短路径

		/** @return 指向实现单目标标签修正算法的对象的指针。
		*/
		LabelCorrectingAl* getLabelCorrectingAlgo(); // 获取标签修正算法
		/** 分配适当的DescDirectionPath对象并返回指向它的指针。 
		*/
		DescDirectionPath* allocateDescDirectionPath(PathAlgoType algo, PathApp app, 
													 int nbLinks, FPType delta, 
													 FPType slope, FPType scaleFact); // 分配下降方向路径

		/** @return 指向实现点到点双目标最短路径算法的对象的指针。
		*/
		Point2PointBiObjShPath* getPoint2PointBSP(); // 获取点到点双目标最短路径
		/** @return 指向负责旅行时间和通行费下限计算的对象的指针。
		*/
		BoundsCalculatorForBSPBase* getBoundsCaclulator(); // 获取边界计算器
		/** @return 指向负责向目标节点添加已知路径的对象的指针。
		*/
		PathsAdder* getPathsAdder(); // 获取路径添加器
		/** @return 指向实现非加性路径成本计算的对象的指针。
		*/
		NonAdditivePC* getNonAdditivePC(); // 获取非加性路径成本
		/** @return 指向实现单源双目标最短路径算法的对象的指针。
		*/
		OneSourceBiObjShPath* getOneSourceBSP(); // 获取单源双目标最短路径
		/** 生成最短路径上的随机通行费并将其写入文件。
		*/
		void generateRndTollsAndWriteToFile(const std::string& rndTollsFile); // 生成并写入随机通行费
		/** @return 指向负责反向网络的旅行时间和通行费下限计算的对象的指针。
		*/
		BoundsCalculatorForBackwardNet* getBackwardBound(); // 获取反向边界
		/** @return 指向实现路径成本支配规则的对象的指针。
		*/
		DominationByPathCostBase* getDominationByPathCost(); // 获取基于路径成本的支配
};

#endif
