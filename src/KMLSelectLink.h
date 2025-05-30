#ifndef KML_SELECT_LINK  // 防止头文件重复包含
#define KML_SELECT_LINK 

#include "KMLNetOutput.h"  // 包含KML网络输出基类

class PathSet;  // 前向声明路径集合类
class Path;     // 前向声明路径类

/** \brief 实现用于选择链接分析的kml文件创建。

    高亮显示经过给定链接的路径。
*/
class KMLSelectLink : public KMLNetOutput {  // 继承自KML网络输出基类
	public:
		KMLSelectLink(StarNetwork* net, const PathSet& paths);  // 构造函数1
		KMLSelectLink(StarNetwork* net, const PathSet& paths,   // 构造函数2
			 			const std::string& logFileForMissingIDs);  // 带缺失ID日志文件的构造函数
		~KMLSelectLink();  // 析构函数

		/** 设置要显示所有路径的链接索引。
		*/
		void setLinkIndex(int linkIndex);  // 设置链接索引

	protected:
		/** @return 返回链接索引。
		*/
		virtual FPType calcLineWidth(StarLink* link);  // 计算线宽
		/** @return 根据索引返回链接的颜色。
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);  // 根据线宽创建颜色
		/** @return 如果链接为红色则返回"5"，否则返回"3"。
		*/
		virtual std::string createLineWidth(FPType lineWidth);  // 创建线宽
		/** @return 返回true。
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);  // 判断是否应创建地标
		virtual std::string createDescriptionString(StarLink* link);  // 创建描述字符串

	private:
		int currentLiknIndex_;  // 当前链接索引
		const PathSet& paths_;  // 路径集合引用
		std::vector<std::string> coloredLinks_;  // 已着色链接向量
		std::vector<int> linkDescriptions_;  // 链接描述向量

		void colorLinks();  // 为链接着色
		bool pathHasSelectedLink(Path* path) const;  // 检查路径是否包含选中的链接
		void colorLinksOfPath(Path* path, const std::string& decription);  // 为路径的链接着色
		void initialiseColoredLinks();  // 初始化已着色链接
};

#endif