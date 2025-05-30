#include <stdlib.h> // 包含标准库
#include <iostream> // 包含输入输出流

#include "Params.h" // 包含参数类
#include "StepSizeAdd.h" // 包含加性步长类
#include "ObjectManager.h" // 包含对象管理器类
#include "Error.h" // 包含错误处理类
#include "FWAlgo.h" // 包含Frank-Wolfe算法类
#include "RelGap.h" // 包含相对间隙类
#include "Bisection.h" // 包含二分法类
#include "Armijo.h" // 包含Armijo线搜索类
#include "QuadApp.h" // 包含二次近似类
#include "PathBasedAlgo.h" // 包含基于路径的算法类
#include "AdditivePC.h" // 包含加性路径成本类
#include "NonAdditivePC.h" // 包含非加性路径成本类
#include "LinkFlowsCFW.h" // 包含CFW链路流量类
#include "LinkFlowsBFW.h" // 包含BFW链路流量类
#include "OriginBasedAlgo.h" // 包含基于起点的算法类
#include "OriginBasedAlgoTapas.h" // 包含TAPAS基于起点的算法类
#include "OriginBasedAlgoEQII.h" // 包含平衡II基于起点的算法类
#include "Astar.h" // 包含A*算法类
#include "AddHookScreenAndStore.h" // 包含屏幕显示和存储钩子类
#include "AlgoDecorator.h" // 包含算法装饰器类
#include "AlgoDecoratorWriteLinks.h" // 包含写入链路算法装饰器类
#include "PathBasedFlowMovePE.h" // 包含PE基于路径的流量移动类
#include "PASManagerWithStep.h" // 包含带步长的PAS管理器类
#include "PathBasedAlgoEqII.h" // 包含平衡II基于路径的算法类
#include "DescDirectionPathScaled.h" // 包含缩放的下降方向路径类
#include "DescDirectionPathPE.h" // 包含PE下降方向路径类
#include "DescDirectionPathGP.h" // 包含GP下降方向路径类
#include "DescDirectionPathPG.h" // 包含PG下降方向路径类
#include "DescDirectionPathISP.h" // 包含ISP下降方向路径类
#include "OriginBasedAlgoTapasEqII.h" // 包含TAPAS平衡II基于起点的算法类
#include "AONNonAdditive.h" // 包含非加性全有或全无类
#include "MaxDiffConvMeasure.h" // 包含最大差异收敛度量类
#include "NonAddParser.h" // 包含非加性解析器类
#include "TollsManagement.h" // 包含收费管理类
#include "DefaultNetParser.h" // 包含默认网络解析器类
#include "DefaultODMatrixParser.h" // 包含默认OD矩阵解析器类
#include "LabelSettingOneSourceBSP.h" // 包含标签设置单源BSP类
#include "LabelSettingPoint2PointBSP.h" // 包含标签设置点对点BSP类
#include "BoundsCalculatorWithTimeUpdate.h" // 包含带时间更新的边界计算器类
#include "BiDirectionalPoint2PointBSP.h" // 包含双向点对点BSP类
#include "ParseIndiffCurves.h" // 包含解析无差异曲线类
#include "NoPathsAdder.h" // 包含无路径添加器类
#include "CurrentPathsAdder.h" // 包含当前路径添加器类
#include "ShortestPathAdder.h" // 包含最短路径添加器类
#include "Astar.h" // 包含A*算法类
#include "ParseSpecificIndiffCurves.h" // 包含解析特定无差异曲线类
#include "IndiffCurveContainerLinearCurve.h" // 包含线性无差异曲线容器类
#include "IndiffCurveContainerConvexCurve.h" // 包含凸无差异曲线容器类
#include "IndiffCurveContainerConcaveCurve.h" // 包含凹无差异曲线容器类
#include "BoundsCalculatorForBackwardNet.h" // 包含反向网络边界计算器类
#include "DominationByPathCostBase.h" // 包含基于路径成本支配基类
#include "DominationByPathCost.h" // 包含基于路径成本支配类
#include "NonAddShortestPathWithLazyBSP.h" // 包含带惰性BSP的非加性最短路径类
#include "ShortestPathWithRandomReturn.h" // 包含带随机返回的最短路径类
#include "NonAddShortestPathForAON.h" // 包含用于AON的非加性最短路径类
#include "LazyShortestPath.h" // 包含惰性最短路径类
#include "AstarWithoutZoneSkip.h" // 包含不跳过区域的A*类
#include "ShortestPathWithRandomReturnWithFixedProbability.h" // 包含带固定概率随机返回的最短路径类
#include "OriginSet.h" // 包含起点集合类
#include "PathBasedFlowMoveGP.h" // 包含GP基于路径的流量移动类
#include "PathBasedFlowMoveWithStep.h" // 包含带步长的基于路径的流量移动类
#include "PathSet.h" // 包含路径集合类
#include "LinkFncContainer.h" // 包含链路函数容器类
#include "Derivative.h" // 包含导数类
#include "ODMatrix.h" // 包含OD矩阵类

ObjectManager::ObjectManager(Params *params): params_(params) { // 构造函数，初始化参数
	
	isAdditive_ = -1; // 初始化加性标志为-1（未确定）
	
	net_ = NULL; // 初始化网络指针为空
	odMatrix_ = NULL; // 初始化OD矩阵指针为空
	algo_ = NULL; // 初始化算法指针为空
	linkFnc_ = NULL; // 初始化链路函数指针为空
	linkFlows_ = NULL; // 初始化链路流量指针为空
	lineSearch_ = NULL; // 初始化线搜索指针为空
	convMeasure_ = NULL; // 初始化收敛度量指针为空
	shPath_ = NULL; // 初始化最短路径指针为空
	der_ = NULL; // 初始化导数指针为空
	pathSet_ = NULL; // 初始化路径集合指针为空
	pathCost_ = NULL; // 初始化路径成本指针为空
	algoType_ = Nothing; // 初始化算法类型为Nothing
	algoApp_ = AppNothing; // 初始化算法应用为AppNothing
	originSet_ = NULL; // 初始化起点集合指针为空
	addHook_ = NULL; // 初始化钩子指针为空
	addHookStore_ = NULL; // 初始化钩子存储指针为空
	pasManager_ = NULL; // 初始化PAS管理器指针为空
	descDirPath_ = NULL; // 初始化下降方向路径指针为空
	flowMove_ = NULL; // 初始化流量移动指针为空
	stepSize_ = NULL; // 初始化步长指针为空
	stepSizeAdd_ = NULL; // 初始化加性步长指针为空
	flowMoveWithStep_ = NULL; // 初始化带步长的流量移动指针为空
	flowMoveGP_ = NULL; // 初始化GP流量移动指针为空
	LCShortestPath_ = NULL; // 初始化标签修正最短路径指针为空
	nonAddShPath_ = NULL; // 初始化非加性最短路径指针为空
	ndPathCost_ = NULL; // 初始化非加性路径成本指针为空
	indiffCurveCont_ = NULL; // 初始化无差异曲线容器指针为空
	tolls_ = NULL; // 初始化收费指针为空
	oneSourceBSP_ = NULL; // 初始化单源BSP指针为空
	point2pointBSP_ = NULL; // 初始化点对点BSP指针为空
	boundsCalculator_ = NULL; // 初始化边界计算器指针为空
	pathsAdder_ = NULL; // 初始化路径添加器指针为空
	currPathsAdder_ = NULL; // 初始化当前路径添加器指针为空
	aStar_ = NULL; // 初始化A*指针为空
	backwardBounds_ = NULL; // 初始化反向边界指针为空
	dominationRule_ = NULL; // 初始化支配规则指针为空
	aStarForNonAdd_ = NULL; // 初始化用于非加性的A*指针为空
	boundsWithTimeUpdate_ = NULL; // 初始化带时间更新的边界指针为空
};

ObjectManager::~ObjectManager(){ // 析构函数
	
	
	delete net_; // 删除网络
	delete odMatrix_; // 删除OD矩阵
	delete algo_; // 删除算法
	delete linkFnc_; // 删除链路函数
	delete linkFlows_; // 删除链路流量
	delete lineSearch_; // 删除线搜索
	delete convMeasure_; // 删除收敛度量
	if (shPath_ == LCShortestPath_ || shPath_ == nonAddShPath_ || shPath_ == aStar_) { // 如果最短路径与其他指针相同
		shPath_ = NULL; // 设置最短路径指针为空，避免重复删除
	} else {
		delete shPath_; // 否则删除最短路径
	}
	delete der_; // 删除导数
	delete pathSet_; // 删除路径集合
	if (pathCost_ == ndPathCost_) { // 如果路径成本与非加性路径成本相同
		pathCost_ = NULL; // 设置路径成本指针为空，避免重复删除
	} else {
		delete pathCost_; // 否则删除路径成本
	}
	delete originSet_; // 删除起点集合
	delete addHook_; // 删除钩子
	delete pasManager_; // 删除PAS管理器
	delete descDirPath_; // 删除下降方向路径
	delete flowMove_; // 删除流量移动
	delete stepSize_; // 删除步长

	delete LCShortestPath_; // 删除标签修正最短路径
	delete nonAddShPath_; // 删除非加性最短路径
	delete ndPathCost_; // 删除非加性路径成本
	delete indiffCurveCont_; // 删除无差异曲线容器
	delete tolls_; // 删除收费
	delete oneSourceBSP_; // 删除单源BSP
	delete point2pointBSP_; // 删除点对点BSP
	delete boundsCalculator_; // 删除边界计算器
	delete pathsAdder_; // 删除路径添加器
	delete aStar_; // 删除A*
	delete backwardBounds_; // 删除反向边界
	delete dominationRule_; // 删除支配规则
};

PathCost* ObjectManager::getPathCost(){ // 获取路径成本方法
	if (pathCost_ == NULL){ // 如果路径成本为空
		if (getIfAdditive()) { // 如果是加性模型
			std::cout << "Creating additive path cost" << std::endl; // 输出创建加性路径成本
			pathCost_ = new AdditivePC(); // 创建新的加性路径成本
		} else { // 如果是非加性模型
			std::cout << "Creating non-additive path cost" << std::endl; // 输出创建非加性路径成本
			getNonAdditivePC(); // 调用获取非加性路径成本方法
		}
	}
	return pathCost_; // 返回路径成本
};

NonAdditivePC* ObjectManager::getNonAdditivePC(){ // 获取非加性路径成本方法
	assert(getIfAdditive() == false); // 断言模型为非加性
	if (ndPathCost_ == NULL) { // 如果非加性路径成本为空
		std::cout << "Creating non-additive PC" << std::endl; // 输出创建非加性PC
		assert(pathCost_ == NULL); // 断言路径成本为空
		ndPathCost_ = new NonAdditivePC(*getIndiffCurveContainer(), *getTolls()); // 创建新的非加性路径成本
		pathCost_ = ndPathCost_; // 将路径成本设置为非加性路径成本
		std::cout << "Non-additive PC created" << std::endl; // 输出非加性PC已创建
	}
	return ndPathCost_; // 返回非加性路径成本
};

IndiffCurveContainer* createProperIndiffCurveContainer(const std::string& curveType, 
			int nbOdPairs){ // 创建适当的无差异曲线容器函数
	if (curveType == "PiecewiseLinear") { // 如果曲线类型为分段线性
		return new IndiffCurveContainer(nbOdPairs); // 返回新的无差异曲线容器
	} else if (curveType == "Linear") { // 如果曲线类型为线性
		return new IndiffCurveContainerLinearCurve(nbOdPairs); // 返回新的线性无差异曲线容器
	} else if (curveType == "Convex") { // 如果曲线类型为凸
		return new IndiffCurveContainerConvexCurve(nbOdPairs); // 返回新的凸无差异曲线容器
	} else if (curveType == "Concave") { // 如果曲线类型为凹
		return new IndiffCurveContainerConcaveCurve(nbOdPairs); // 返回新的凹无差异曲线容器
	} else { // 如果曲线类型无效
		throw Error("Unexpected value of parameter <INDIFF_CURVE_TYPE>"); // 抛出错误
	}
	return NULL; // 返回空指针（永远不会执行到这里）
};

IndiffCurveContainer* ObjectManager::getIndiffCurveContainer(){ // 获取无差异曲线容器方法
	assert(getIfAdditive() == false); // 断言模型为非加性
	if (indiffCurveCont_ == NULL) { // 如果无差异曲线容器为空
		std::cout << "Creating indiffCurveCont" << std::endl; // 输出创建无差异曲线容器
		std::string curves =  params_->getParam("INDIFF_CURVE"); // 获取无差异曲线参数
		std::string curveType =  params_->getParamWoSpaces("INDIFF_CURVE_TYPE"); // 获取无差异曲线类型参数
		indiffCurveCont_ = createProperIndiffCurveContainer(curveType,
				getODMatrix()->getNbODPairs()); // 创建适当的无差异曲线容器
		if (curves == "RND_GEN") { // 如果曲线为随机生成
			indiffCurveCont_->generateRandomCurvesWithBSP(static_cast<int>(
				getFloatValue("MAX_NB_POINTS_PER_CURVE")), // 使用BSP生成随机曲线
				*getOneSourceBSP(),
				*getODMatrix()	);
			indiffCurveCont_->writeToFile(getNet()->getNetName() + ".curves", 
				*getODMatrix()  ); // 将曲线写入文件
		} else { // 如果曲线不是随机生成
			
			if (curveType == "PiecewiseLinear") { // 如果曲线类型为分段线性
				ParseIndiffCurves curvesParser; // 创建无差异曲线解析器
				curvesParser.parse(curves, *indiffCurveCont_); // 解析曲线
			} else if (curveType == "Linear" || curveType == "Convex" 
				|| curveType == "Concave") { // 如果曲线类型为线性、凸或凹
				ParseSpecificIndiffCurves curvesParser; // 创建特定无差异曲线解析器
				curvesParser.parse(curves, *indiffCurveCont_); // 解析曲线
			} else { // 如果曲线类型无效
				throw Error("Unexpected value of parameter <INDIFF_CURVE_TYPE>"); // 抛出错误
			}
		}
		std::cout << "IndiffCurveCont created" << std::endl; // 输出无差异曲线容器已创建
	}
	return indiffCurveCont_; // 返回无差异曲线容器
};

TollContainerType* ObjectManager::getTolls(){ // 获取收费容器方法
	assert(getIfAdditive() == false); // 断言模型为非加性
	if (tolls_ == NULL) { // 如果收费容器为空
		std::cout << "Creating tolls" << std::endl; // 输出创建收费
		getNet(); // 调用获取网络方法
		std::cout << "Tolls created" << std::endl; // 输出收费已创建
	}
	return tolls_; // 返回收费容器
};

bool ObjectManager::getIfAdditive(){ // 获取是否为加性模型方法
	if ( isAdditive_ == -1) { // 如果加性标志未确定
		std::string tmp =  params_->getParamWoSpaces("INDIFF_CURVE"); // 获取无差异曲线参数
		 isAdditive_ = tmp.empty(); // 如果参数为空，则为加性模型
	}
	return  isAdditive_; // 返回是否为加性模型
};

FPType ObjectManager::getFloatValue(std::string paramName){ // 获取浮点值方法
	if (paramName == "") return 0.0; // 如果参数名为空，返回0.0
	FPType value = 0.0; // 初始化值为0.0
	try { // 尝试获取参数值
		std::string tmp =  params_->getParamWoSpaces(paramName); // 获取参数值
		if (tmp.empty()) { // 如果参数值为空
			throw Error("Parameter <" + paramName + "> was not initialised"); // 抛出错误
		}
		value = atof(tmp.c_str()); // 将字符串转换为浮点数
	} catch (...) { // 捕获所有异常
		throw Error("Unexpected value of parameter <" + paramName + ">"); // 抛出错误
	}
	return value; // 返回浮点值
};

void ObjectManager::generateRndTollsAndWriteToFile(const std::string& rndTollsFile) { // 生成随机收费并写入文件方法
	
	LabelCorrectingAl shPath(net_); // 创建标签修正最短路径算法
	TollsManagement::generateRandomTollsOnShortestPath(*tolls_, // 在最短路径上生成随机收费
		getFloatValue("RND_TOLLS_PROBABILITY"), // 获取随机收费概率
		static_cast<TollType>(getFloatValue("RND_TOLLS_MAX_TOLL")), // 获取最大收费值
		getODMatrix(), &shPath);
	int nbTolledLinks = 0; // 初始化收费链接数量为0
	for (int i = 0; i < net_->getNbLinks(); ++i) // 遍历所有链接
		if ((*tolls_)[i] != 0.0) ++nbTolledLinks; // 如果链接有收费，增加计数
	std::cout << "nbTolledLinks = " << nbTolledLinks << std::endl; // 输出收费链接数量
	TollsManagement::writeTollsToFile(*tolls_,  params_->getParam("NETWORK"), 
			rndTollsFile, getNet()); // 将收费写入文件
};

StarNetwork* ObjectManager::getNet(){ // 获取网络方法
	if (net_ == NULL) { // 如果网络为空
		std::cout << "Creating network" << std::endl; // 输出创建网络
		if (getIfAdditive()) { // 如果是加性模型
			DefaultNetParser parser( params_->getParam("NETWORK")); // 创建默认网络解析器
			net_ = parser.parseNetwork(); // 解析网络
			net_->linkNodes(); // 链接节点
			std::cout << "DefaultNetParser and link nodes finished" << std::endl; // 输出默认网络解析器和链接节点已完成
		} else { // 如果是非加性模型
		
			NonAddParser parser( params_->getParam("NETWORK")); // 创建非加性解析器
			
			net_ = parser.parseNetwork(); // 解析网络
			net_->linkNodes(); // 链接节点
			tolls_ = parser.getTolls(); // 获取收费
			
			std::string rndTollsFile =  params_->getParamWoSpaces("WRITE_RND_TOLLS_TO_FILE"); // 获取随机收费文件参数
			if (!rndTollsFile.empty()) { // 如果随机收费文件参数不为空
				generateRndTollsAndWriteToFile(rndTollsFile); // 生成随机收费并写入文件
			}
			
			assert(tolls_ != NULL); // 断言收费不为空
		}
		std::cout << "Network created " << std::endl; // 输出网络已创建
	}
	return net_; // 返回网络
};

ODMatrix* ObjectManager::getODMatrix(){ // 获取OD矩阵方法
	if (odMatrix_ == NULL) { // 如果OD矩阵为空
		std::cout << "Creating OD matrix" << std::endl; // 输出创建OD矩阵
		std::string file =  params_->getParam("OD_MATRIX"); // 获取OD矩阵文件参数
		DefaultODMatrixParser parser(file); // 创建默认OD矩阵解析器
		odMatrix_ = parser.parseODMatrix(getNet()); // 解析OD矩阵
		std::cout << "OD matrix created" << std::endl; // 输出OD矩阵已创建
	}
	return odMatrix_; // 返回OD矩阵
};

ShortestPath* ObjectManager::getShPath(){ // 获取最短路径方法
	if (shPath_ == NULL) { // 如果最短路径为空
		std::cout << "Creating shortest path" << std::endl; // 输出创建最短路径
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo"); // 获取最短路径算法参数
	    if( algo == "LC"){ // 如果算法为标签修正
	    	assert(getIfAdditive() == true); // 断言模型为加性
	       	LCShortestPath_ = new LabelCorrectingAl(getNet()); // 创建标签修正算法
	       	shPath_ = LCShortestPath_; // 设置最短路径为标签修正算法
	    } else if( algo == "Astar"){ // 如果算法为A*
	    	assert(getIfAdditive() == true); // 断言模型为加性
        	aStar_ = new Astar(getNet(), getODMatrix()); // 创建A*算法
        	aStar_->initializeBounds(getODMatrix()); // 初始化边界
        	shPath_ = aStar_; // 设置最短路径为A*算法
        } else if (algo == "NonAdd" || algo == "LazyNonAdd" || algo == "NonAddWithAstar") { // 如果算法为非加性或惰性非加性或带A*的非加性
        	assert(getIfAdditive() == false); // 断言模型为非加性
        	nonAddShPath_ = getNonAddShPath(); // 获取非加性最短路径
        } else if (algo == "LazySP") { // 如果算法为惰性最短路径
        	assert(getIfAdditive() == true); // 断言模型为加性
        	shPath_ = new LazyShortestPath(new LabelCorrectingAl(getNet())); // 创建惰性最短路径
        } else { // 如果算法无效
        	throw Error("Unexpected parameter for <ShPathAlgo>. Possible values are LC, Astar."); // 抛出错误
        }
       
	    if (nonAddShPath_ == NULL && ! params_->getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) { // 如果非加性最短路径为空且使用带随机返回的点对点最短路径
			std::cout << " params_->getParamWoSpaces(UseP2PShPathWithRandomReturn) = " << // 输出参数值
       	 	 params_->getParamWoSpaces("UseP2PShPathWithRandomReturn") << std::endl;
       	 	if (! params_->getParamWoSpaces("FIXED_PROBABILITY").empty()) { // 如果固定概率参数不为空
       	 		FPType probability = getFloatValue("FIXED_PROBABILITY"); // 获取概率值
       	 		if (probability <= 0 || probability > 1) throw Error("Unexpected value of parameter <FIXED_PROBABILITY>"); // 如果概率无效，抛出错误
       	 		shPath_ = new ShortestPathWithRandomReturnWithFixedProbability(shPath_, probability); // 创建带固定概率随机返回的最短路径
       	 	} else { // 如果固定概率参数为空
				shPath_ = new ShortestPathWithRandomReturn(shPath_); // 创建带随机返回的最短路径
			}
		}
	    std::cout << "Shortest path created" << std::endl; // 输出最短路径已创建
	}
	return shPath_; // 返回最短路径
};

Derivative* ObjectManager::getDerivative(){ // 获取导数方法
	if (der_ == NULL){ // 如果导数为空
		std::cout << "Creating derivative" << std::endl; // 输出创建导数
		if (getIfAdditive() == true) { // 如果是加性模型
			der_ = new Derivative(getFloatValue("ZERO_FLOW"), getLinkFncCont()); // 创建新的导数
		} else { // 如果是非加性模型
			throw Error("Derivative is not implemented for the non-additive case"); // 抛出错误
		}
		std::cout << "Derivative created" << std::endl; // 输出导数已创建
	}	
	return der_; // 返回导数
};

LineSearch* ObjectManager::getLineSearch(){ // 获取线搜索方法
	if (lineSearch_ == NULL) { // 如果线搜索为空
		if (getIfAdditive() == false) { // 如果是非加性模型
			throw Error("Line search is not implemented for the non-additive case"); // 抛出错误
		}
		// 线搜索，可能的值：BISEC, ARMIJO, QUAD_APP
		std::cout << "Creating line search" << std::endl; // 输出创建线搜索
		std::string tmp =  params_->getParamWoSpaces("LINE_SEARCH"); // 获取线搜索参数
		if (tmp == "BISEC") { // 如果线搜索为二分法
			FPType pr = getFloatValue("LS_PRECISION"); // 获取精度值
			if (pr <= 0.0) throw Error("Line search precision must be positive"); // 如果精度不正，抛出错误
			lineSearch_ = new Bisection(pr, getDerivative()); // 创建二分法线搜索
		} else if (tmp == "ARMIJO") { // 如果线搜索为Armijo
			FPType dec = getFloatValue("ARMIJO_DEC"); // 获取减少值
			if (dec <= 0.0) throw Error("Decrement for Armijo search must be positive"); // 如果减少值不正，抛出错误
			lineSearch_ = new Armijo(dec, getDerivative()); // 创建Armijo线搜索
		} else if (tmp == "QUAD_APP") { // 如果线搜索为二次近似
			lineSearch_ = new QuadApp(getDerivative(), getNet()); // 创建二次近似线搜索
		} else { // 如果线搜索无效
			throw Error("Unexpected value of parameter <LINE_SEARCH>"); // 抛出错误
		}
		std::cout << "Line search created" << std::endl; // 输出线搜索已创建
		
	}
	return lineSearch_; // 返回线搜索
};

ConvMeasure* ObjectManager::getConvMeasure(){ // 获取收敛度量方法
	if (convMeasure_ == NULL) { // 如果收敛度量为空
		std::string conv =  params_->getParamWoSpaces("CONV_MEASURE"); // 获取收敛度量参数
		if (conv == "RGAP") { // 如果收敛度量为相对间隙
			assert(getIfAdditive() == true); // 断言模型为加性
			std::cout << "Creating relative gap" << std::endl; // 输出创建相对间隙
			convMeasure_ = new RelGap(getFloatValue("PRECISION"), getNet(), getODMatrix(), getLabelCorrectingAlgo()); // 创建相对间隙
			std::cout << "RGAP created" << std::endl; // 输出相对间隙已创建
		} else if (conv == "MAX_DIFF") { // 如果收敛度量为最大差异
			assert(getPathAlgoType() != Nothing); // 断言路径算法类型不为空
			convMeasure_ = new MaxDiffConvMeasure(getFloatValue("PRECISION"), getShPath(),
							getODMatrix(), getPathSet()); // 创建最大差异收敛度量
		} else { // 如果收敛度量无效
			throw Error("Unexpected convergence measure"); // 抛出错误
		}
	}
	return convMeasure_; // 返回收敛度量
};

PathAlgoType ObjectManager::getPathAlgoType(){ // 获取路径算法类型方法
	if ( algoType_ == Nothing) { // 如果算法类型为空
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM"); // 获取算法参数
		if (tmp == "PE") { // 如果算法为PE
			 algoType_ = PE; // 设置算法类型为PE
		} else if (tmp == "GP") { // 如果算法为GP
			 algoType_ = GP; // 设置算法类型为GP
		} else if (tmp == "PG") { // 如果算法为PG
			 algoType_ = PG; // 设置算法类型为PG
		} else if (tmp == "ISP"){ // 如果算法为ISP
			 algoType_ = ISP; // 设置算法类型为ISP
		}
	}
	return  algoType_; // 返回算法类型
};

PathApp ObjectManager::getPathAlgoApp(){ // 获取路径算法应用方法
	if (algoApp_ == AppNothing) { // 如果算法应用为空
		std::string tmp =  params_->getParamWoSpaces("APPROACH"); // 获取应用参数
		if (tmp == "APP1") { // 如果应用为APP1
			algoApp_ = APP1; // 设置算法应用为APP1
		} else if (tmp == "APP2") { // 如果应用为APP2
			algoApp_ = APP2; // 设置算法应用为APP2
		} else if (tmp == "APP3") { // 如果应用为APP3
			algoApp_ = APP3; // 设置算法应用为APP3
		} else if (tmp == "") { // 如果应用参数为空
			algoApp_ = APP1; // 默认设置算法应用为APP1
		} else { // 如果应用参数无效
			throw Error("Unexpected approach type"); // 抛出错误
		}
	}
	return algoApp_; // 返回算法应用
};

DescDirectionPath* ObjectManager::getDescDirectionPath(){ // 获取下降方向路径方法
	if (descDirPath_ == NULL) { // 如果下降方向路径为空
		std::cout << "Creating Desc direction" << std::endl; // 输出创建下降方向
		descDirPath_ = allocateDescDirectionPath(getPathAlgoType(), getPathAlgoApp(), 
						getNet()->getNbLinks(), getFloatValue("DIR_TOLERANCE"), 
						getFloatValue("SLOPE"), 
						getFloatValue("ISP_SCALE")); // 分配下降方向路径
		std::cout << "descDirection created" << std::endl; // 输出下降方向已创建
	}
	return descDirPath_; // 返回下降方向路径
};

PathSet* ObjectManager::getPathSet(){
	if (pathSet_ == NULL) {
		std::cout << "Creating path set" << std::endl;
		PathApp app = getPathAlgoApp();
		
		AONAssignment* aon = NULL;
		if (getIfAdditive()) {
			aon = new AONUsual(*getODMatrix(), getLabelCorrectingAlgo());
		} else {
			aon = new AONNonAdditive(*getODMatrix(), new NonAddShortestPathForAON(getNonAdditivePC(), 
							getNet()->getNbNodes(),
            				getOneSourceBSP(), getODMatrix()));
		}

		if (app == APP3) {
			PathAlgoType algo = getPathAlgoType();
			if (algo == PE ) {
				pathSet_ = PathSet::createSetPEAPP3(getNet(), getODMatrix(), getShPath(), 
							getPathCost(), 
							getFloatValue("ZERO_FLOW"), getPathBasedFlowMove(), 
							aon);
			} else if (algo == GP) {
				pathSet_ = PathSet::createSetGPAPP3(getNet(), getODMatrix(), getShPath(), 
							getPathCost(), 
							getFloatValue("ZERO_FLOW"), getPathBasedFlowMoveGP(), 
							aon);
			} else {
				throw Error("Approach 3 is not implemented for this algorithm");
			}
		} else {
			pathSet_ = PathSet::createSetWithStep(getNet(), getODMatrix(), getShPath(), 
						getPathCost(), 
						getFloatValue("ZERO_FLOW"), getPathBasedFlowMoveWithStep(), 
						aon);
		}
		std::cout << "Path set created" << std::endl;
	}
	return pathSet_;
};
		
DecoratedEqAlgo* ObjectManager::getEqAlgo(const std::string& dirConv, 
							const std::string& dirFlows){
	if (algo_ == NULL) {
		std::cout << "Creating algo" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		if ((tmp == "FW") || (tmp == "CFW") || (tmp == "BFW")) {
			assert(getIfAdditive() == true);
			if (linkFlows_ == NULL) {
				if (tmp == "CFW") {
					linkFlows_ = new LinkFlowsCFW(getNet(), getODMatrix(), getShPath(), getLineSearch(), 
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				} else if (tmp == "BFW") {
					linkFlows_ = new LinkFlowsBFW(getNet(), getODMatrix(), getShPath(), getLineSearch(),
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				} else {
					linkFlows_ = new LinkFlows(getNet(), getODMatrix(), getShPath(), getLineSearch(),
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				}
	
			}
			algo_ = new FWAlgo(linkFlows_, getAddHook(), getNet(), getConvMeasure(), 
					getFloatValue("TIME_LIMIT")); 
		} else if (getPathAlgoType() != Nothing){
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;

			if (tmp == "EQI") {
				
				algo_ = new PathBasedAlgo(getPathSet(), getAddHook(), getConvMeasure(), getNet(),
					getFloatValue("TIME_LIMIT"), getODMatrix()); 
			} else if (tmp == "EQII") {
				algo_ = new PathBasedAlgoEqII(getPathSet(), getAddHook(), getConvMeasure(), getNet(),
					getFloatValue("TIME_LIMIT"), 
					static_cast<int>(getFloatValue("MAX_ITER")), getODMatrix());
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
			if (currPathsAdder_ != NULL) currPathsAdder_->setPathSet(getPathSet());
			if (aStar_ != NULL) aStar_->setPathSet(getPathSet());
			if (aStarForNonAdd_ != NULL) aStarForNonAdd_->setPathSet(getPathSet());

		} else if (tmp == "B" || tmp == "Bstep") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgo(getOriginSet(), getNet(), getAddHook(),  
							getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoEQII(getOriginSet(), getNet(), getAddHook(),
						getConvMeasure(), getFloatValue("TIME_LIMIT"), 
						static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
		} else if (tmp == "LUCE") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgo(getOriginSet(), getNet(), getAddHook(),
					getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoEQII(getOriginSet(), getNet(), getAddHook(),
						getConvMeasure(), getFloatValue("TIME_LIMIT"), 
						static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
		} else if (tmp == "TAPAS" || tmp == "TAPASstep") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgoTapas(getOriginSet(), getNet(), getPASManager(), 
					getAddHook(), getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoTapasEqII(getOriginSet(), getNet(), getPASManager(), 
					getAddHook(), getConvMeasure(), getFloatValue("TIME_LIMIT"),
					static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}

		} else {
			throw Error("Unexpected value of parameter <ALGORITHM>");
		}
		
		std::cout << "Adding decorations" << std::endl;
		tmp =  params_->getParamWoSpaces("CONVERGENCE");
		if (tmp != "") {
			getAddHook();
			assert(addHookStore_ != NULL);
			if (tmp == "AUTO") {
				tmp =  params_->getAutoFileName() + ".conv";
			}
			algo_ = new AlgoDecorator(algo_, addHookStore_, tmp, dirConv);
		}
		tmp =  params_->getParamWoSpaces("LINK_FLOWS");
		if (tmp != "") {
			if (tmp == "AUTO") {
				tmp =  params_->getAutoFileName() + ".flows";
			}
			algo_ = new AlgoDecoratorWriteLinks(algo_, getNet(), tmp, dirFlows);
		}
		std::cout << "Algo created" << std::endl;
	}
	return algo_;
};

LinkFncContainer* ObjectManager::getLinkFncCont(){
	if (linkFnc_ == NULL) {
		std::cout << "Creating linkFncContainer" << std::endl;
		linkFnc_ = new LinkFncContainer(getNet());
		std::cout << "linkFncContainer created" << std::endl;
	}
	return linkFnc_;
};

OriginSet* ObjectManager::getOriginSet(){
	assert(getIfAdditive() == true);
	if (originSet_ == NULL) {
		std::cout << "Creating origin set" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		
		if (tmp == "B") {

			std::string steps =  params_->getParamWoSpaces("NEWTON_STEPS");
			bool useMulti = true;
			if (steps == "SINGLE") {
				useMulti = false;
			} else if (steps == "MULTI") {
				useMulti = true;
			} else {
				throw Error("Unexpected value of parameter <NEWTON_STEPS>");
			} 

			originSet_ = OriginSet::createOriginSetB(getODMatrix(), getNet(), 
					getFloatValue("ZERO_FLOW"), getFloatValue("DIR_TOLERANCE"), useMulti, 
					getLabelCorrectingAlgo());
		} else if (tmp == "Bstep"){
			originSet_ = OriginSet::createOriginSetBWithStep(getODMatrix(), getNet(), 
					getFloatValue("ZERO_FLOW"), getFloatValue("DIR_TOLERANCE"), 
					getLineSearch(), getLabelCorrectingAlgo());
		} else if (tmp == "LUCE") {
			originSet_ = OriginSet::createOriginSetLUCE(getODMatrix(), getNet(), getFloatValue("ZERO_FLOW"), 
						getFloatValue("DIR_TOLERANCE"), getLineSearch(), getLabelCorrectingAlgo());
		} else if (tmp == "TAPAS" || tmp == "TAPASstep") {
			originSet_ = OriginSet::createOriginSetTAPAS(getODMatrix(), getNet(), getFloatValue("ZERO_FLOW"), 
						getFloatValue("DIR_TOLERANCE"), getPASManager(), getLabelCorrectingAlgo());
		} else {
			throw Error("Unexpected algorithm value");
		}
		std::cout << "origin set created" << std::endl;
	}
	return originSet_;
};

AddHook* ObjectManager::getAddHook(){
	if (addHook_ == NULL) {
		std::cout << "Creating AddHook" << std::endl;
		std::string onScreen =  params_->getParamWoSpaces("SHOW_CONVERGENCE");
		std::string toFile =  params_->getParamWoSpaces("CONVERGENCE");
		if (onScreen != "") {
			if (toFile != "") {
				addHookStore_ = new AddHookScreenAndStore();
				addHook_ = addHookStore_;
			} else {
				addHook_ = new AddHookScreenOut();
			}
		} else if (toFile != "") {
			addHookStore_ = new AddHookStoreOut();
			addHook_ = addHookStore_;
		} else {
			addHook_ = new AddHook();
		}
		std::cout << "AddHook created" << std::endl;	
	}
	return addHook_;
};


PASManager* ObjectManager::getPASManager(){
	assert(getIfAdditive() == true);
	if (pasManager_ == NULL) {
		std::cout << "Creating PASManager" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		if (tmp == "TAPAS") {
			pasManager_ = new PASManager(*getShPath(), getFloatValue("DIR_TOLERANCE"), getNet()->getNbLinks(), 
						getFloatValue("MU"), getFloatValue("V"), 
						getFloatValue("ZERO_FLOW"));
		} else if (tmp == "TAPASstep") {
			pasManager_ = new PASManagerWithStep(*getShPath(), getFloatValue("DIR_TOLERANCE"), getNet()->getNbLinks(), 
						getFloatValue("MU"), getFloatValue("V"), getNet()->getNbLinks(),
						getLineSearch(), getFloatValue("ZERO_FLOW"));
		} else {
			throw Error("Unexpected algorithm type. Possible values are TAPAS or TAPASwithStep");
		}
		std::cout << "PASManager created" << std::endl;
	}
	return pasManager_;
};

PathBasedFlowMove* ObjectManager::getPathBasedFlowMove(){
	if (flowMove_ == NULL) {
		std::cout << "Creating PathBasedFlowMove" << std::endl;
		PathAlgoType algo = getPathAlgoType();
		assert(algo != Nothing);
		PathApp app = getPathAlgoApp();
		if (app == APP3) {
			if (algo == PE) {
				flowMove_ = new PathBasedFlowMovePE(getDescDirectionPath());
			} else if (algo == GP) {
				flowMoveGP_ = new PathBasedFlowMoveGP(getFloatValue("ALPHA"), getDescDirectionPath()); 
				flowMove_ = flowMoveGP_;
			} else {
				throw Error("Approach 3 is not implemented for this algorithm");
			}
		} else {
			flowMoveWithStep_ = new PathBasedFlowMoveWithStep(getStepSize(), getDescDirectionPath(), 
				getFloatValue("ZERO_FLOW"));
			flowMove_ = flowMoveWithStep_;
		}
		std::cout << "PathBasedFlowMove created" << std::endl;
	}	
	return flowMove_;
};

StepSize* ObjectManager::getStepSize(){
	if (stepSize_ == NULL) {
		std::cout << "Creating StepSize" << std::endl;
		if (getIfAdditive() == true) {
			std::string tmp =  params_->getParamWoSpaces("LINE_SEARCH");
			stepSizeAdd_ = new StepSizeAdd(getLineSearch(), getDerivative()); // so far only additive step size 
			stepSize_ = stepSizeAdd_;
		} else {
			throw Error("Step size is not implemented for the non-additive case");
		}
		std::cout << "StepSize created" << std::endl;
	} 
	return stepSize_;
};

StepSizeAdd* ObjectManager::getStepSizeAdd(){
	assert(getIfAdditive() == true);
	if (stepSizeAdd_ == NULL) {
		std::cout << "Creating StepSizeAdd" << std::endl;
		stepSizeAdd_ = new StepSizeAdd(getLineSearch(), getDerivative()); // so far only additive step size 
		stepSize_ = stepSizeAdd_;
		std::cout << "StepSizeAdd created" << std::endl;
	} 
	return stepSizeAdd_;
};

PathBasedFlowMoveWithStep* ObjectManager::getPathBasedFlowMoveWithStep(){
	assert(getIfAdditive() == true);
	if (flowMoveWithStep_ == NULL) {
		std::cout << "Creating PathBasedFlowMoveWithStep" << std::endl;
		flowMoveWithStep_ = new PathBasedFlowMoveWithStep(getStepSize(), getDescDirectionPath(), 
			getFloatValue("ZERO_FLOW"));
		flowMove_ = flowMoveWithStep_;
		std::cout << "PathBasedFlowMoveWithStep created" << std::endl;
	}
	return flowMoveWithStep_;
};

PathBasedFlowMoveGP* ObjectManager::getPathBasedFlowMoveGP(){
	if (flowMoveGP_ == NULL) {
		std::cout << "Creating PathBasedFlowMoveGP" << std::endl;
		flowMoveGP_ = new PathBasedFlowMoveGP(getFloatValue("ALPHA"), getDescDirectionPath()); 
		flowMove_ = flowMoveGP_;
		std::cout << "PathBasedFlowMoveGP created" << std::endl;
	}
	return flowMoveGP_;
};


LabelCorrectingAl* ObjectManager::getLabelCorrectingAlgo(){
	assert(getIfAdditive() == true);
	if (LCShortestPath_ == NULL) {
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo");
		LCShortestPath_ = new LabelCorrectingAl(getNet());
		if (shPath_ == NULL && algo == "LC") {
			shPath_ = LCShortestPath_;
		} 
	}
	return LCShortestPath_;
};

NonAddShortestPath* ObjectManager::getNonAddShPath(){
	assert(getIfAdditive() == false);
	if (nonAddShPath_ == NULL) {
		std::cout << "Creating non-additive shortest path" << std::endl;
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo");
		if (algo == "LazyNonAdd") {
			nonAddShPath_ = new NonAddShortestPathWithLazyBSP(getNonAdditivePC(), getNet()->getNbNodes(),
            				getOneSourceBSP(), getODMatrix()); 
		} else if (algo == "NonAdd") {
			nonAddShPath_ = new NonAddShortestPath(getNonAdditivePC(), getNet()->getNbNodes(),
            				getOneSourceBSP(), getPoint2PointBSP(), getODMatrix());
			
		} else {
			throw Error("Unexpected value of parameter <ShPathAlgo>");
		}
		if (shPath_ == NULL && (algo == "NonAdd" || algo == "LazyNonAdd" || algo == "NonAddWithAstar")) {
			shPath_ = nonAddShPath_;
		} 
		if (! params_->getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) {
			std::cout << "******************************************" << std::endl;
			if (! params_->getParamWoSpaces("FIXED_PROBABILITY").empty()) {
	       	 	FPType probability = getFloatValue("FIXED_PROBABILITY");
	       	 	if (probability <= 0 || probability > 1) throw Error("Unexpected value of parameter <FIXED_PROBABILITY>");
	       	 		shPath_ = new ShortestPathWithRandomReturnWithFixedProbability(shPath_, probability);
	       	 	} else {
					shPath_ = new ShortestPathWithRandomReturn(shPath_);
				}

			std::cout << "******************************************: " << 
			(shPath_ == nonAddShPath_) << std::endl;
		}
		std::cout << "Non-additive shortest path created" << std::endl;
	}
	return nonAddShPath_;
};

DescDirectionPath* ObjectManager::allocateDescDirectionPath(PathAlgoType algo, PathApp app, int nbLinks, 
				FPType delta, FPType slope, FPType scaleFact){
	assert(delta > 0.0);
	assert(slope > 0.0);
	assert(scaleFact > 0.0);
	if (app == APP3) {
		if (algo == PE) {
			return new DescDirectionPathScaled(new DescDirectionPathPE(delta), nbLinks, delta);
		} else if (algo == GP){
			return new DescDirectionPathScaled(new DescDirectionPathGP(delta), nbLinks, delta);
		} else {
			throw Error("Approach 3 is not defined for this algorithm type");
		}
	} else if (app == APP2){
		if (algo == GP) {
			return new DescDirectionPathScaled(new DescDirectionPathGP(delta), nbLinks, delta);
		} else if (algo == PE) {
			return new DescDirectionPathScaled(new DescDirectionPathPE(delta), nbLinks, delta);
		} else {
			throw Error("Approach 2 is not defined for this algorithm type");
		}
	} else if (app == APP1) {
		if (algo == PE) {
			return new DescDirectionPathPE(delta);
		} else if (algo == GP) {
			return new DescDirectionPathGP(delta);
		} else if (algo == PG) {
			return new DescDirectionPathPG(delta);
		} else if (algo == ISP) {
			return new DescDirectionPathISP(slope, scaleFact, delta);
		} else {
			throw Error("Path-based algorithm is not defined");
		}
	} else {
		throw Error("Path-based algorithm is not defined");
	}
	return NULL;
};

OneSourceBiObjShPath* ObjectManager::getOneSourceBSP() {
	assert(getIfAdditive() == false);
	if (oneSourceBSP_ == NULL) {
		std::cout << "Creating OneSourceBSP" << std::endl;
		oneSourceBSP_ = new LabelSettingOneSourceBSP(*getNet(), *getTolls()); 
		std::cout << "OneSourceBSP created" << std::endl;
	}
	return oneSourceBSP_;
};

Point2PointBiObjShPath* ObjectManager::getPoint2PointBSP(){
	assert(getIfAdditive() == false);
	if (point2pointBSP_ == NULL) {
		std::cout << "Creating p2pBSP" << std::endl;
		std::string bsp =  params_->getParamWoSpaces("BIOBJ_SHPATH_P2P");
		if (bsp == "BiLabelSetting") {
			point2pointBSP_ = new LabelSettingPoint2PointBSP(*getNet(), *getTolls(), *getBoundsCaclulator(),
				*getPathsAdder(), *getDominationByPathCost());
		
		} else if (bsp == "BiLabelSetting_bidirectional"){
			point2pointBSP_ = new BiDirectionalPoint2PointBSP(*getNet(), *getTolls(),
						*getBoundsCaclulator(),
						*getPathsAdder(), *getBackwardBound(), *getDominationByPathCost());
		} else {
			throw Error("Unexpected value of parameter <BIOBJ_SHPATH_P2P>.");
		}
		std::cout << "P2PBSP created" << std::endl;
	}
	return point2pointBSP_;
};

BoundsCalculatorForBSPBase* ObjectManager::getBoundsCaclulator() {
	assert(getIfAdditive() == false);
	if (boundsCalculator_ == NULL) {
		std::cout << "Creating BoundsCalculatorForBSP" << std::endl;
		std::string bsp =  params_->getParamWoSpaces("BLS_BOUNDS");
		if (bsp.empty()) {
			boundsCalculator_ = new BoundsCalculatorForBSPBase;
		} else if (bsp == "zeroFlow") {
			boundsCalculator_ = new BoundsCalculatorForBSP(*getNet(), *getTolls());
		} else if (bsp == "currentFlow") {
			boundsWithTimeUpdate_ = new BoundsCalculatorWithTimeUpdate(*getNet(), *getTolls());
			boundsCalculator_ = boundsWithTimeUpdate_;
		} else {
			throw Error("Unexpected value of parameter <BIOBJ_SHPATH_P2P>.");
		}
		std::cout << "BoundsCalculatorForBSP created" << std::endl;
		boundsCalculator_->initializeBounds();
	}
	return boundsCalculator_;
};

PathsAdder* ObjectManager::getPathsAdder() {
	assert(getIfAdditive() == false);
	if (pathsAdder_ == NULL) {
		std::cout << "Creating PathAdder" << std::endl;
		std::string usePaths =  params_->getParamWoSpaces("USE_EXISTING_PATHS");
		if (usePaths.empty()) {
			pathsAdder_ = new NoPathsAdder();
		} else if (usePaths == "onePath") {
			currPathsAdder_ = new ShortestPathAdder(*getTolls());
			pathsAdder_ = currPathsAdder_;
		} else if (usePaths == "currentPaths") {
			currPathsAdder_ = new CurrentPathsAdder(*getTolls());
			pathsAdder_ = currPathsAdder_;
		} else {
			throw Error("Unexpected value of parameter <USE_EXISTING_PATHS>.");
		}
		std::cout << "PathAdder created" << std::endl;
	}
	return pathsAdder_;
};

BoundsCalculatorForBackwardNet* ObjectManager::getBackwardBound() {
	if (backwardBounds_ == NULL) {
		std::cout << "creating backward bounds *********************" << std::endl;
		backwardBounds_ = new BoundsCalculatorForBackwardNet(*getNet(), *getTolls());

		backwardBounds_->initializeBounds();
	}
	return backwardBounds_;

};

DominationByPathCostBase* ObjectManager::getDominationByPathCost() {
	if (dominationRule_ == NULL) {
		std::string usePathCost =  params_->getParam("USE_PATH_COST_DOMINATION");
		if (usePathCost.empty()) {
			dominationRule_ = new DominationByPathCostBase;
		} else {
			std::cout << "creating path cost domination rule" << std::endl;
			dominationRule_ = new  DominationByPathCost(*getNonAdditivePC());
			std::cout << "path cost domination rule created" << std::endl;
		}
	}
	return dominationRule_;
};