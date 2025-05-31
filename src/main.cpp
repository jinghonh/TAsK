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
 	    			仅适用于非加性交通
	    		    分配。如果尝试为加性
	   			交通分配选择此选项,则程序将因断言失败而终止。
 	    - LazySP - 惰性加性最短路径,参见\cite Chen1998。
		 	    仅适用于加性交通分配。
		 	    如果尝试为非加性
	   			交通分配选择此选项,则程序将因断言失败而终止。
	.

	\code{.cpp}
	<UseP2PShPathWithRandomReturn>: {} 
	\endcode
	如果指定任何非空值,则使用随机化流量更新策略,
	其中计算点对点最短路径的概率取决于迭代次数。
	此选项仅适用于基于路径的算法,如果使用其他组的算法则忽略。

	\code{.cpp}
	<FIXED_PROBABILITY>: {}
	\endcode
	计算点对点最短路径的概率。可能的值:区间(0,1]中的任何实数。
	如果字段<UseP2PShPathWithRandomReturn>为空,则忽略此选项。

	\code{.cpp}
	<CONV_MEASURE>: {MAX_DIFF} 
	\endcode
	收敛度量。
	可能的值:
		- RGAP - 相对间隙。此选项仅适用于加性交通分配。
		- MAX_DIFF - 最大-最小成本差异界,参见\cite Dial_2006。此选项仅适用于
			基于路径的算法(适用于加性和非加性问题)。

	\code{.cpp}
	<PRECISION>: {1e-5}
	\endcode
	算法精度。例如,如果<PRECISION>: {1e-5},则当
	收敛度量达到严格小于1e-5的任何值时算法将终止。可能的值:
	任何正数。

	\code{.cpp}
	<ZERO_FLOW>: {1e-10}
	\endcode
	链路流量容差。例如,如果<ZERO_FLOW>: {1e-10},则当
	链路流量值小于或等于1e-10时将被重置为零。可能的值:
	任何正数。

	\code{.cpp}
	<DIR_TOLERANCE>: {1e-10}
	\endcode
	基于路径和bush的算法的下降方向容差。
	例如,如果<DIR_TOLERANCE>: {1e-10},则当下降方向的分量
	值小于1e-10时将被设为零。可能的值:
	任何正数。

	\code{.cpp}
	<ALGORITHM>: {PE}
	\endcode
	算法。
	可能的值:
		- FW - Frank-Wolfe,参见\cite Frank1956。
		- CFW - 共轭Frank-Wolfe,参见\cite Mitradjieva2013。
		- BFW - 双共轭Frank-Wolfe,参见\cite Mitradjieva2013。
		- PE - 路径均衡,参见\cite Dafermos1968和\cite Florian1995。
		- GP - 梯度投影,参见\cite Jayakrishnan1994和\cite Chen1998。
		- PG - 投影梯度,参见\cite MichaelFlorian2009。
		- ISP - 改进的社会压力,参见\cite Kumar2011。
		- B - 基于牛顿步长的算法B,参见\cite Dial_2006和\cite Boyles2011。
		- Bstep - 基于线搜索的算法B,参见\cite Dial_2006和\cite Boyles2011。
		- LUCE - 线性用户成本均衡,参见\cite Gentile2014。
		- TAPAS - 基于牛顿步长的配对替代段交通分配,
				参见\cite Hillel2010。
		- TAPASstep - 基于线搜索的配对替代段交通分配,
				参见\cite Hillel2010。
	.
	\warning 对于非加性交通分配,仅PE和GP选项可用,
	且<APPROACH>必须设置为{APP3}。
	
	\code{.cpp}
 	<LINE_SEARCH>: {BISEC}.
 	\endcode
 	线搜索。此字段仅用于基于线搜索的算法。
 	如果指定了不同的算法,则忽略此字段。
 	可能的值:
 	   - BISEC - 二分法。
 	   - ARMIJO - Armijo类规则。
 	   - QUAD_APP - 二次近似。

 	\code{.cpp}
	<LS_PRECISION>: {1e-15}
	\endcode
	线搜索容差。此字段仅在使用线搜索方法时使用。

	\code{.cpp}
	<ARMIJO_DEC>: {2}
	\endcode
	Armijo类规则中的递减量。此选项仅当<LINE_SEARCH>: {ARMIJO}时适用。

	\code{.cpp}
	<EQUILIBRATION>: {EQI}
	\endcode
	均衡类型。可能的值:
		- EQI - 均衡I,参见\cite Perederieieva2015。
		- EQII - 均衡II,参见\cite Perederieieva2015。

	\code{.cpp}
	<MAX_ITER>: {10}
	\endcode
	均衡II的最大迭代次数。

	\code{.cpp}
	<APPROACH>: {APP3}
	\endcode
	基于路径的算法的下降方向类型。
	可能的值取决于算法:
		- 对于PE:
			+ APP1 - 基于线搜索的路径均衡。
			+ APP3 - 基于牛顿步长的路径均衡。

		- 对于GP:
			+ APP1 - 基于线搜索的梯度投影。
			+ APP2 - 基于带缩放下降方向的线搜索的梯度投影。
			+ APP3 - 无线搜索的梯度投影。

		- 对于PG:
			+ APP1 - 基于线搜索的投影梯度。

		- 对于ISP:
			+ APP1 - 基于线搜索的改进社会压力。
			.
		.

	\code{.cpp}
	<ALPHA>: {0.25}
	\endcode
	用于无线搜索梯度投影的常数,参见\cite Jayakrishnan1994。
	有效值:任何正实数。即使不使用梯度投影,
	此值也必须设置为正数。
	
	\code{.cpp}
	<SLOPE>: {1e-15}
	\endcode
	用于改进社会压力的链路成本函数导数的最小斜率。
	即使不使用改进社会压力,此值也必须设置为正数。
					
	\code{.cpp}
	<ISP_SCALE>: {0.15}
	\endcode
	改进社会压力算法中用于将所有路径
	分为两组的参数,详见\cite Kumar2011。
	即使不使用改进社会压力,此值也必须设置为正数。

	\code{.cpp}
	<NEWTON_STEPS>: {SINGLE}
	\endcode
	算法B和TAPAS的牛顿步长类型。
	可能的值:
		- SINGLE - 执行一个牛顿步长。
		- MULTI - 执行多个牛顿步长。

	\code{.cpp}
	<MU>: {0.5}
	\endcode
	TAPAS中用于确定成本有效%PAS的常数。

	\code{.cpp}
	<V>: {0.25}
	\endcode
	TAPAS中用于确定流量有效%PAS的常数。

	\code{.cpp}				
	<BIOBJ_SHPATH_P2P>: {BiLabelSetting} 
	\endcode
	双目标点对点最短路径算法。
	此字段仅用于非加性交通分配。
	可能的值:
		- BiLabelSetting - 双目标标签设置。
		- BiLabelSetting_bidirectional - 双向双目标标签设置。

	\code{.cpp}
	<BLS_BOUNDS> : {zeroFlow}
	\endcode
	旅行时间下界。此字段仅用于非加性交通分配。
	如果为空，则不使用边界，否则可能的值:
		- zeroFlow - 基于零流量的旅行时间边界。
		- currentFlow - 基于当前流量的旅行时间边界。

	\code{.cpp}
	<USE_EXISTING_PATHS> : {onePath}
	\endcode
	已知路径添加技术。
	此字段仅用于非加性交通分配。
	如果为空，则不添加路径，否则可能的值:
		- onePath - 仅添加最短路径。
		- currentPaths - 添加所有现有路径。

	\code{.cpp}
	<USE_PATH_COST_DOMINATION> : {yes}
	\endcode
	%路径成本支配规则。此字段仅用于非加性交通分配。
	如果为空，则使用普通支配规则，否则使用路径成本支配。

	\code{.cpp}
	<SHOW_CONVERGENCE>: {yes}
	\endcode
	如果此字段的值不为空，则在屏幕上打印收敛。

	\code{.cpp}
	<LINK_FLOWS>: {}
	\endcode
	%链路流量写入文件的路径。如果字段为空，则不创建文件。
	如果字段设置为AUTO，则使用自动生成的名称。

	\code{.cpp}
	<CONVERGENCE>: {}
	\endcode
	%收敛写入文件的路径。如果字段为空，则不创建文件。
	如果字段设置为AUTO，则使用自动生成的名称。
	
	\code{.cpp}
	<PATH_SET>: {}
	\endcode
	%路径集写入文件的路径。仅适用于基于路径的算法。
	如果字段为空，则不创建文件。
	如果字段设置为AUTO，则使用自动生成的名称。
	
	\code{.cpp}
	<LATEX_OUTPUT>: {net.tex}
	\endcode
	%LaTeX输出写入文件的路径。如果指定此字段，则必须同时指定<NODES>字段。
	如果字段为空，则不创建文件。
	如果字段设置为AUTO，则使用自动生成的名称。
	
	\code{.cpp}
	<KML_OUTPUT>: {}
	\endcode
	%指定KML网络输出文件路径。此KML文件根据流量/容量比例显示颜色编码的网络。
	如果指定此字段，则必须同时指定<NODES>字段。如果字段为空，则不创建KML文件。
	
	\code{.cpp}
	<KML_TOLLS_OUTPUT>: {}
	\endcode
	%指定KML收费可视化输出文件路径。此KML文件以红色高亮显示收费链接。
	如果指定此字段，则必须同时指定<NODES>和<INDIFF_CURVE>字段。如果字段为空，则不创建KML文件。
	
	\code{.cpp}
	<KML_SELECT_LINK>: {}
	\endcode
	%指定KML选择链接分析输出文件路径。此KML文件高亮显示经过所选链接的路径。
	如果指定此字段，则必须同时指定<NODES>和<SELECT_LINK_ID>字段。如果字段为空，则不创建KML文件。
	
	\code{.cpp}
	<SELECT_LINK_ID>: {}
	\endcode
	%选择链接分析中要分析的链接ID。如果指定了<KML_SELECT_LINK>，则此字段为必填项。
	
	\code{.cpp}
	<KML_DIFF_OUTPUT>: {}
	\endcode
	%指定KML流量差异可视化输出文件路径。此KML文件显示两个流量解之间的差异。
	如果指定此字段，则必须同时指定<NODES>和<FLOW_DIFF_FILE>字段。如果字段为空，则不创建KML文件。
	
	\code{.cpp}
	<FLOW_DIFF_FILE>: {}
	\endcode
	%包含流量差异数据的文件路径。此文件应每行包含一个流量差异值，按网络中链接的相同顺序排列。
	如果指定了<KML_DIFF_OUTPUT>，则此字段为必填项。
	
	\code{.cpp}
	<DIFF_TOLERANCE>: {1e-6}
	\endcode
	%流量差异分析的容差值。小于此值的流量差异将被视为零。默认值为1e-6。

	\note 对于所有KML输出，必须指定<NODES>参数提供节点坐标文件，
	所有坐标必须使用wgs84坐标系统。
		
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
#include "PathSetPrinterToFile.h"
#include "LatexNetOutput.h"
#include "KMLNetOutput.h"
#include "KMLSelectLink.h"
#include "KMLPlotTolls.h"
#include "DiffKMLNetOutput.h"

#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <vector>

#define NB_INPUT_PARAMS 2

/**
 * 从文件读取流量差异数据
 * @param filename 包含流量差异数据的文件路径
 * @param flowDiff 用于存储读取的流量差异数据的向量
 * @return 成功读取返回true，否则返回false
 */
bool readFlowDiffFromFile(const std::string& filename, std::vector<FPType>& flowDiff) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "无法打开流量差异文件: " << filename << std::endl;
        return false;
    }
    
    FPType value;
    while (inFile >> value) {
        flowDiff.push_back(value);
    }
    
    inFile.close();
    return true;
}

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
                                true, 8);
        }

        // KML输出
        if (!params.getParam("KML_OUTPUT").empty()) {
            std::cout << "正在写入KML文件到: " << params.getParam("KML_OUTPUT") << std::endl;
            
            // 创建基本的KML网络输出
            KMLNetOutput kmlOutput(manager.getNet());
            kmlOutput.createKML(params.getParam("NODES"), params.getParam("KML_OUTPUT"));
            
            // 如果设置了收费可视化输出
            if (!params.getParam("KML_TOLLS_OUTPUT").empty() && !params.getParam("INDIFF_CURVE").empty()) {
                KMLPlotTolls kmlTolls(manager.getNet(), *(manager.getTolls()));
                kmlTolls.createKML(params.getParam("NODES"), params.getParam("KML_TOLLS_OUTPUT"));
            }
            
            // 如果设置了选择链接分析
            if (!params.getParam("KML_SELECT_LINK").empty() && !params.getParam("SELECT_LINK_ID").empty()) {
                KMLSelectLink kmlSelectLink(manager.getNet(), *(manager.getPathSet()));
                int linkId = std::stoi(params.getParam("SELECT_LINK_ID"));
                kmlSelectLink.setLinkIndex(linkId);
                kmlSelectLink.createKML(params.getParam("NODES"), params.getParam("KML_SELECT_LINK"));
            }
            
            // 如果设置了流量差异分析
            if (!params.getParam("KML_DIFF_OUTPUT").empty() && !params.getParam("FLOW_DIFF_FILE").empty()) {
                // 读取流量差异数据
                std::vector<FPType> flowDiff;
                if(readFlowDiffFromFile(params.getParam("FLOW_DIFF_FILE"), flowDiff)) {
                    FPType tolerance = 1e-6; // 默认容差
                    if (!params.getParam("DIFF_TOLERANCE").empty()) {
                        tolerance = std::stod(params.getParam("DIFF_TOLERANCE"));
                    }
                    
                    DiffKMLNetOutput diffKml(manager.getNet(), flowDiff, tolerance);
                    diffKml.createKML(params.getParam("NODES"), params.getParam("KML_DIFF_OUTPUT"));
                } else {
                    std::cout << "无法生成流量差异KML文件，读取流量差异数据失败" << std::endl;
                }
            }
        }

    } catch (Error error) {
        // 错误处理
        std::cout << "执行过程中发生错误: " << error.getMessage() << std::endl;
        return 1;
    }

    return 0;
};
