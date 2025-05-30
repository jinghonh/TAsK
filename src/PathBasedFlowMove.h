#ifndef PATH_BASED_FLOW_MOVE
#define PATH_BASED_FLOW_MOVE

#include <list>          // 包含链表容器

class PathAndDirection;   // 前向声明路径和方向类
class DescDirectionPath;  // 前向声明下降方向路径类
class Path;              // 前向声明路径类

/** \brief 这个类负责在当前OD集合内执行流量移动。
	\note 仅改变路径流量（链路流量保持不变）。
*/
class PathBasedFlowMove {
	public:
		virtual ~PathBasedFlowMove(){}; // 虚析构函数
		
		/** 计算路径下降方向。
			@return 如果无法移动流量则返回true（仅在PathBasedFlowMoveWithStep中计算的步长为零时发生），
			否则返回false。
		*/
		bool calculatePathDirection(const std::list<Path*> &paths);
		
		/** 这个方法负责根据指定算法移动路径流量。
		*/
		virtual bool executeFlowMove() = 0;
		
		/** @return 指向列表中第一个PathAndDirection对象的指针。
			\warning 在同一对象上多次调用此方法的嵌套循环中使用此方法是不安全的。
		*/
		PathAndDirection* beginPathDirection() const;
		/** @return 指向列表中下一个PathAndDirection对象的指针，如果到达列表末尾则返回NULL。
			\warning 在多次调用此方法的嵌套循环中使用此方法是不安全的。
		*/
		PathAndDirection* getNextPathDirection() const;

	protected:
		
		PathBasedFlowMove(DescDirectionPath* pathDirectionAlgo); // 构造函数
		
		PathAndDirection** pathDirection_; // 路径方向数组指针
		int size_;                        // 路径方向数组大小
		mutable int currDirection_;       // 当前方向索引(可变的)
		
	private:
		
		DescDirectionPath* pathDirectionAlgo_; // 下降方向路径算法指针
};

#endif
