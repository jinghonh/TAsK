/**
 	@mainpage  交通分配框架 (TAsK)

 	TAsK软件实现了多种算法用于解决确定性静态交通分配问题(固定需求),参见\cite Sheffi1985,
 	以及非加性交通分配问题,参见\cite Larsson2004。

 	所有代码均使用C++编程语言实现。
 	这是一个基于MIT许可证的开源软件。

 	该软件仅在Ubuntu(12.10、13.10和14.04)操作系统上进行了测试。

 	\author <a href = http://www.des.auckland.ac.nz/people/o-perederieieva>Olga Perederieieva</a>
	\author <a href = https://github.com/Boshen>Boshen Chen</a> (A*最短路径算法)
	\author Benoit Mulocher (双向双目标标签设置算法)

	\b 贡献者

    - <a href = http://www.lancaster.ac.uk/lums/people/all/matthias-ehrgott/>Matthias Ehrgott</a>
    - <a href = https://unidirectory.auckland.ac.nz/profile/a-raith>Andrea Raith</a>
	- <a href = http://www.its.leeds.ac.uk/people/staff-profiles/judith-wang/>Judith Y.T. Wang</a>
	- <a href = http://www.rsm.nl/people/marie-schmidt/>Marie Schmidt</a>

	文档样式表来自 https://github.com/Velron/doxygen-bootstrapped。

	\version 1.0

	\copyright MIT许可证

 	## 如何引用
 	在论文中,请引用以下参考文献:

 	> Olga Perederieieva, Matthias Ehrgott, Andrea Raith, Judith Y.T. Wang, 
 	> A framework for and empirical study of algorithms for traffic assignment, 
 	> Computers & Operations Research, Volume 54, February 2015, Pages 90-107, 
 	> ISSN 0305-0548, http://dx.doi.org/10.1016/j.cor.2014.08.024.

 	在代码文档中,请包含对此网站的引用。
	
	## 功能特点
	- 11种加性交通分配算法,属于链路、路径和bush方法,参见\cite Perederieieva2015。
	- 2种非加性交通分配算法(路径均衡和梯度投影)。
	- 2种均衡策略,参见\cite Perederieieva2015。
	- 3种步长搜索算法(二分法、Armijo规则和二次近似)。
	- 多种下降方向选项。
	- 3种基于路径算法的流量更新策略。
	- 基于双目标标签设置的非加性最短路径算法,利用交通分配结构进行加速。
	- 计算结果的纯文本和KML输出。
 	
	## 安装说明

	要求:
	- C++编译器(软件使用gcc 4.8.2测试)。
	- Boost库,可从http://www.boost.org/下载。

	说明:
	- 下载源代码压缩包。
	- 解压。
	- 打开终端,进入源代码文件夹(src/)并输入:make。

	默认使用扩展浮点精度(long double类型)。
	要使用double类型,请在Makefile中注释掉
	CPPFLAGS += -DUSE_EXTENDED_PRECISION行,
	如有必要,在终端中输入:make clean; make重新编译代码。

	\note 如果使用double类型而不是long double,算法可能无法找到高精度解。
	
	## 文档
	API文档可在Docs/html/index.html中找到。
	
	## 使用方法

	要运行TAsK软件,首先需要创建一个包含各种运行参数的文本文件。

	### 示例数据

	源代码压缩包包含来自http://www.bgu.ac.il/~bargera/tntp/的SiouxFalls数据实例。
	该实例仅用于研究目的。

	### 如何运行
	
	编译源代码后,可以使用两个包含的参数文件之一运行。
	示例参数文件包括
	<a href="../../input.params">input.params</a>用于常规交通分配
	和<a href="../../inputNonAdd.params">inputNonAdd.params</a>
	用于非加性交通分配。请使用这些文件作为创建
	其他参数文件的模板。
	\note 如果某些字段缺失或使用了无效值,程序将
	终止并显示错误消息或断言失败。

	\warning 所有字段名称和可能的值都区分大小写。

	要使用<a href="../../input.params">input.params</a>运行代码:
	- 打开终端并进入源代码文件夹。
	- 在终端中输入:./ta ../input.params。

	文件<a href="../../input.params">input.params</a>允许使用基于二次近似的
	算法B(详情参见\cite Dial_2006)解决SiouxFalls实例的
	加性交通分配问题。

	文件<a href="../../inputNonAdd.params">inputNonAdd.params</a>允许使用基于牛顿步的
	路径均衡算法(详情参见\cite Florian1995)
	解决SiouxFalls实例的
	非加性交通分配问题。

	### 参数文件

	参数文件必须包含以下字段(支持\b/\b*多行注释,
	\b/\b/单行注释):

	\code{.cpp}	<NETWORK>: {Data/SiouxFalls_tolls.txt}
	\endcode
	%网络数据文件路径
	
	\code{.cpp}
	<OD_MATRIX>: {Data/SiouxFalls_trips.txt}
	\endcode
	%O-D矩阵文件路径。
		
	\code{.cpp}
	<WRITE_RND_TOLLS_TO_FILE>: {}
	\endcode
	此字段用于非加性交通分配模型。
	如果此字段不为空,则将使用随机收费而不是网络文件中指定的收费。
    如果解决加性交通分配问题,则忽略此字段。

    \code{.cpp}
	<RND_TOLLS_PROBABILITY>: {}
	\endcode
	为链路分配收费的概率。
	如果选项<WRITE_RND_TOLLS_TO_FILE>的值为空,则忽略此字段。

	\code{.cpp}
	<RND_TOLLS_MAX_TOLL>: {}
	\endcode
	随机生成收费的最大值。
	如果选项<WRITE_RND_TOLLS_TO_FILE>的值为空,则忽略此字段。

	\code{.cpp}
	<INDIFF_CURVE_TYPE>: {PiecewiseLinear}
	\endcode
	标量化函数类型。
	此字段用于非加性交通分配模型。
    如果选项<INDIFF_CURVE>的值为空,则忽略此字段。

    可能的值:
     - PiecewiseLinear。
     - Linear。
     - Convex。
	 - Concave。

	\code{.cpp}
	<INDIFF_CURVE>: {Curves/SiouxFalls_tolls.curves} 
	\endcode
	%标量化函数文件路径。
	此字段用于非加性交通分配模型。
	如果此字段为空,则认为问题是加性的。
	如果此字段设置为RND_GEN,则将随机生成分段线性标量化函数。
	
	\code{.cpp}
	<MAX_NB_POINTS_PER_CURVE>: {3}
	\endcode
	每个分段线性函数的最大断点数。
	如果选项<INDIFF_CURVE>的值不是{RND_GEN},则忽略此字段。

	\code{.cpp}
	<NODES>: {}
	\endcode
	%节点坐标文件路径。如果需要LaTeX输出,请指定此字段。

	\code{.cpp}
	<TIME_LIMIT> : {20000}
	\endcode
	算法执行的时间限制(秒)。

	\code{.cpp}
	<ShPathAlgo> : {LC}
	\endcode
	最短路径算法。
    可能的值:
    	- LC - 单源标签设置算法。如果选择此选项
	   			 与基于路径的算法一起使用,则单源标签设置
	   			 将用于点对点最短路径计算,
	   			 这比A*慢得多。
	   			 仅适用于加性交通
	   			 分配。如果尝试为非加性
	   			 交通分配选择此选项,则程序将因断言失败而终止。
	    - %Astar - A*点对点算法。如果指定此选项,
	   			A*将用于基于路径的算法,单源
	     			Dijkstra算法用于收敛度量。
	     			如果指定此选项与基于链路或bush的算法一起使用,
	     			则所有单源最短路径计算都将使用Dijkstra算法。
	     			仅适用于加性交通
	    		    分配。如果尝试为非加性
	   			交通分配选择此选项,则程序将因断言失败而终止。
	   			\warning %Astar选项不能与TAPAS一起使用。永远不要一起使用它们。
	    - NonAdd - 非加性最短路径。
	    			仅适用于非加性交通
	    		    分配。如果尝试为加性
	   			交通分配选择此选项,则程序将因断言失败而终止。
 	    - LazyNonAdd - 惰性非加性最短路径,参见\cite Chen1998。
	Shortest path algorithm.
    Possible values:
    	- LC - single-source label-setting algorithm. If this option is chosen
	   			 along with a path-based algorithm, then single-source label-setting
	   			 will be used for point-to-point shortest path calculations
	   			 which is much slower than A*. 
	   			 Works only for additive traffic
	   			 assignment. If one tries to choose this option for non-additive 
	   			 traffic assignment, then the program will terminate with failed assertion.
	    - %Astar - A* point-to-point algorithm. If this option is specified,
	   			A* will be used for path-based algorithms, and one-source
	     			Dijkstra's algorithm for convergence measure. 
	     			If this option is specified along with a link- or bush-based algorithm,
	     			then for all one-source shortest path calculations Dijkstra's algorithm
	     			will be used.
	     			Works only for additive traffic
	    		    assignment. If one tries to choose this option for non-additive 
	   			traffic assignment, then the program will terminate with failed assertion.
	   			\warning %Astar option does not work with TAPAS. Never use them together.
	    - NonAdd - non-additive shortest path.
	    			Works only for non-additive traffic
	    		    assignment. If one tries to choose this option for additive 
	   			traffic assignment, then the program will terminate with failed assertion.
 	    - LazyNonAdd - lazy non-additive shortest path, see \cite Chen1998.
 	    			Works only for non-additive traffic
	    		    assignment. If one tries to choose this option for additive 
	   			traffic assignment, then the program will terminate with failed assertion.
 	    - LazySP - lazy additive shortest path, see \cite Chen1998. 
		 	    Works only for additive traffic assignment.
		 	    If one tries to choose this option for non-additive 
	   			traffic assignment, then the program will terminate with failed assertion.
	.

	\code{.cpp}
	<UseP2PShPathWithRandomReturn>: {} 
	\endcode
	If any non-empty value is specified, then the randomised flow update strategy is used 
	where probability of calculating a point-to-point shortest path depends on the iteration number.
	This option works only with path-based algorithms and is ignored if an algorithm 
	from a different group is used.

	\code{.cpp}
	<FIXED_PROBABILITY>: {}
	\endcode
	Probability of calculating a point-to-point shortest path. Possible values: any real number in the
	interval (0, 1]. This option is ignored if field <UseP2PShPathWithRandomReturn> has
	empty value.

	\code{.cpp}
	<CONV_MEASURE>: {MAX_DIFF} 
	\endcode
	Convergence measure. 
	Possible values: 
		- RGAP - relative gap. This option works for additive traffic assignment only.
		- MAX_DIFF - max-min cost difference bound, see \cite Dial_2006. This option works for
			path-based algorithms only (for both additive and non-additive problems).

	\code{.cpp}
	<PRECISION>: {1e-5}
	\endcode
	Algorithm precision. For example, if <PRECISION>: {1e-5}, then the algorithm will terminate when
	convergence measure reaches any value strictly smaller than 1e-5. Possible values:
	any positive number.

	\code{.cpp}
	<ZERO_FLOW>: {1e-10}
	\endcode
	Link flow tolerance. For example, if <ZERO_FLOW>: {1e-10}, then link flow will be reset to zero if
	its value is less than or equal to 1e-10. Possible values:
	any positive number.

	\code{.cpp}
	<DIR_TOLERANCE>: {1e-10}
	\endcode
	Tolerance of descent direction of path- and bush-based algorithms.
	For example, if <DIR_TOLERANCE>: {1e-10}, then a component 
	of the direction of descent is set to zero if its value is less than 1e-10. Possible values:
	any positive number.

	\code{.cpp}
	<ALGORITHM>: {PE}
	\endcode
	Algorithm.
	Possible values:
		- FW - Frank-Wolfe, see \cite Frank1956.
		- CFW - conjugate Frank-Wolfe, see \cite Mitradjieva2013.
		- BFW - bi-conjugate Frank-Wolfe, see \cite Mitradjieva2013.
		- PE - path equilibration, see \cite Dafermos1968 and \cite Florian1995.
		- GP - gradient projection, see \cite Jayakrishnan1994 and \cite Chen1998.
		- PG - projected gradient, see \cite MichaelFlorian2009.
		- ISP - improved social pressure, see \cite Kumar2011.
		- B - Algorithm B based on Newton step, see \cite Dial_2006 and \cite Boyles2011.
		- Bstep - Algorithm B based on a line search, see \cite Dial_2006 and \cite Boyles2011.
		- LUCE - linear user cost equilibrium, see \cite Gentile2014.
		- TAPAS - traffic assignment by paired alternative segments based on Newton step,
					see \cite Hillel2010.
		- TAPASstep - traffic assignment by paired alternative segments 
					based on  a line search, see \cite Hillel2010.
	.
	\warning In the case of non-additive traffic assignment only the PE and GP options are 
	available with <APPROACH> set to {APP3}.
	
	\code{.cpp}
 	<LINE_SEARCH>: {BISEC}.
 	\endcode
 	Line search. This field is used only for algorithms based on line search.
 	If a different algorithm is specified, this field is ignored.
 	Possible values: 
 	   - BISEC - bisection.
 	   - ARMIJO - Armijo-like rule.
 	   - QUAD_APP - quadratic approximation.

 	\code{.cpp}
	<LS_PRECISION>: {1e-15}
	\endcode
	Line search tolerance. This field is used only if one of the line search methods is used.

	\code{.cpp}
	<ARMIJO_DEC>: {2}
	\endcode
	Decrement in Armijo-like rule. This option is applicable only if <LINE_SEARCH>: {ARMIJO}.

	\code{.cpp}
	<EQUILIBRATION>: {EQI}
	\endcode
	Type of equilibration. Possible values: 
		- EQI - Equilibration I, see \cite Perederieieva2015.
		- EQII - Equilibration II, see \cite Perederieieva2015.

	\code{.cpp}
	<MAX_ITER>: {10}
	\endcode
	Maximum number of iterations for Equilibration II.

	\code{.cpp}
	<APPROACH>: {APP3}
	\endcode
	Type of the direction of descent for path-based algorithms. 
	Possible values depend on the algorithm.
		- For PE: 
			+ APP1 - path equilibration based on a line search.
			+ APP3 - path equilibration based on Newton step.

		- For GP:
			+ APP1 - gradient projection based on a line search.
			+ APP2 - gradient projection based on a line search with scaled direction of descent.
			+ APP3 - gradient projection without a line search.

		- For PG:
			+ APP1 - projected gradient based on a line search.

		- For ISP:
			+ APP1 - improved social pressure based on a line search.
			.
		.

	\code{.cpp}
	<ALPHA>: {0.25}
	\endcode
	Constant that is used in gradient projection without a line search, see \cite Jayakrishnan1994.
	Valid values: any positive real number. This value must be set to a positive number even if
	gradient projection is not used.
	
	\code{.cpp}
	<SLOPE>: {1e-15}
	\endcode
	Minimum slope of the derivative of link cost function used in improved social pressure.
	This value must be set to a positive number even if improved social pressure is not used.
					
	\code{.cpp}
	<ISP_SCALE>: {0.15}
	\endcode
	The parameter of the improved social pressure algorithm that is used to divide all paths
	into two sets, for details see \cite Kumar2011.
	This value must be set to a positive number even if improved social pressure is not used.

	\code{.cpp}
	<NEWTON_STEPS>: {SINGLE}
	\endcode
	Type of Newton step for Algorithm B and TAPAS.
	Possible values: 
		- SINGLE - performs one Newton step.
		- MULTI - performs several Newton steps.

	\code{.cpp}
	<MU>: {0.5}
	\endcode
	Constant used in TAPAS for determining cost-effective %PAS.

	\code{.cpp}
	<V>: {0.25}
	\endcode
	Constant used in TAPAS for determining flow-effective %PAS.

	\code{.cpp}				
	<BIOBJ_SHPATH_P2P>: {BiLabelSetting} 
	\endcode
	Bi-objective point-to-point shortest path algorithm.
	This field is used on for non-additive traffic assignment.
	Possible values: 
		- BiLabelSetting - bi-objective label-setting. 
		- BiLabelSetting_bidirectional - bi-directional bi-objective label-setting.

	\code{.cpp}
	<BLS_BOUNDS> : {zeroFlow}
	\endcode
	Travel time lower bound. This field is used only for non-additive traffic assignment.
	If empty, then no bounds are used, otherwise possible values: 
		- zeroFlow - travel time bounds based on zero flows.
		- currentFlow - travel time bounds based on current flows.

	\code{.cpp}
	<USE_EXISTING_PATHS> : {onePath}
	\endcode
	Addition of known paths technique. 
	This field is used only for non-additive traffic assignment.
	If empty no paths are added, otherwise possible values are:
		- onePath - only shortest path is added.
		- currentPaths - all existing paths are added.

	\code{.cpp}
	<USE_PATH_COST_DOMINATION> : {yes}
	\endcode
	%Path cost domination rule. This field is used only for non-additive traffic assignment.
	If empty usual domination rule is used, otherwise domination by path cost is used.

	\code{.cpp}
	<SHOW_CONVERGENCE>: {yes}
	\endcode
	If the value of this field is not empty, prints convergence on the screen.

	\code{.cpp}
	<LINK_FLOWS>: {}
	\endcode
	%Path to file where link flows must be written. If the field is empty, no file will be created.
	If the field is set to AUTO, an automatically generated name will be used.

	\code{.cpp}
	<CONVERGENCE>: {}
	\endcode
	%Path to file where convergence must be written.  If the field is empty, no file will be created.
	If the field is set to AUTO, an automatically generated name will be used.
	
	\code{.cpp}
	<PATH_SET>: {}
	\endcode
	%Path to file where path sets must be written. Applicable only to path-based algorithms.
	 If the field is empty, no file will be created.
	If the field is set to AUTO, an automatically generated name will be used.
	
	\code{.cpp}
	<LATEX_OUTPUT>: {net.tex}
	\endcode

	%Path to file where latex output must be written. If this field is specified, then
	field <NODES> must be specified too.  If the field is empty, no file will be created.
	If the field is set to AUTO, an automatically generated name will be used.
		
	\note For KML output one has to modify main.cpp and use classes
	KMLNetOutput, KMLSelectLink, KMLPlotTolls and DiffKMLNetOutput.

	## Existing problems of the current implementation
	 - Excessive usage of pointers where references are more appropriate.
	 - In a few places arrays are used where vectors would be better.
	 - ObjectManager is responsible for creation of ALL objects.
	 - StarNetwork and PathBasedFlowMove iterator-like methods are not safe to use in nested loops.
	
 */

#include "Error.h"
#include "DecoratedEqAlgo.h"
#include "Timer.h"
#include "ParseParams.h"
#include "ObjectManager.h"
#include "Utils.h"
#include "FileWriter.h"
#include "PathSetPrinterToFile.h"
#include "LatexNetOutput.h"

#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <cstdlib>

#define NB_INPUT_PARAMS 2

int main(int argc, char *argv[]) {
    try {
        // 创建计时器对象用于记录总CPU时间
        Timer timer; // 对象创建时开始计时

        // 检查输入参数数量是否正确
        if (argc < NB_INPUT_PARAMS) {
            Error er("输入参数数量不正确。");
            throw er;
        }
        
        // 初始化随机数生成器
        srand(time(nullptr));

        // 获取参数文件路径
        std::string fileParams = argv[1];
        
        // 声明各种输出目录变量
        std::string dirConv;      // 收敛性输出目录
        std::string dirFlows;     // 流量输出目录
        std::string dirCounters;  // 计数器输出目录
        std::string dirPathSet;   // 路径集输出目录
        
        // 如果参数数量大于等于5，获取相应的输出目录
        if (argc >= 5) {
            dirConv = argv[2];
            dirFlows = argv[3];
            dirCounters = argv[4];
        }
        // 如果参数数量等于6，获取路径集输出目录
        if (argc == 6) {
            dirPathSet = argv[5];
        }
        
        // 创建参数对象和解析器
        Params params;
        ParseParams parser;
        parser.execute(fileParams, &params);
        
        // 创建对象管理器并记录创建时间
        Timer objCreationTimer;
        ObjectManager manager(&params);

        // 获取算法对象
        DecoratedEqAlgo *algo = manager.getEqAlgo(dirConv, dirFlows);
        FPType objCreationTime = objCreationTimer.getTimePassed();
        
        // 执行算法并记录执行时间
        Timer exeTimer;
        int nbIter = algo->execute();
        FPType executionTime = exeTimer.getTimePassed();
        
        // 检查网络可行性
        FPType maxDiffTest = Utils::checkFeasibility(manager.getNet(), manager.getODMatrix());

        // 获取总运行时间并输出统计信息
        FPType timePassed = timer.getTimePassed(); 
        std::stringstream ss;
        ss << "<总时间> : {" << timePassed <<  "}; <对象创建时间> : {" << objCreationTime << 
                "}; <初始化时间> : {" << algo->getInitialisationTime() <<
                "}; <执行时间> : {" << executionTime << 
                "}; <收敛时间> : {" << algo->getTotalConvTime() <<
                "}; <纯迭代时间> : {" << algo->getTotalPureIterTime() <<
                "}; <节点流量可行性最大变化> : {" << maxDiffTest << 
                "}; <迭代次数> : {" << nbIter << "};\n";
        std::cout << ss.str();

        // 如果指定了路径集输出，则写入文件
        if (!params.getParam("PATH_SET").empty()) {
            std::string filename = params.getParam("PATH_SET");
            if (filename == "AUTO") 
                filename = dirPathSet + params.getAutoFileName() + ".paths";
            PathSetPrinterToFile writePathSet(manager.getODMatrix());
            writePathSet.writePathSetToFile(manager.getPathSet(), filename);
        }

        // 如果指定了LaTeX输出，则生成LaTeX文件
        if (!params.getParam("LATEX_OUTPUT").empty()) {
            std::cout << "正在写入LaTeX文件到: " << params.getParam("LATEX_OUTPUT") <<
                std::endl;
            
            LatexNetOutput latex(manager.getNet());
            latex.printToLaTeX(params.getParam("NODES"), params.getParam("LATEX_OUTPUT"), 
                                true, 0.3);
        }

    } catch (Error error) {
        // 错误处理
        std::cout << "执行过程中发生错误: " << error.getMessage() << std::endl;
        return 1;
    }

    return 0;
};
