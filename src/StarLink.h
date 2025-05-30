#ifndef STAR_LINK
#define STAR_LINK

#include "LinkFnc.h"

/** \brief 这个类是StarNetwork的基本元素。它表示图中的一个链接。
*/
class StarLink {
	public:
		/** @param nodeFrom 链接起始节点的ID。
			@param nodeTo 链接指向节点的ID。
			@param fnc 链接成本函数。
		*/
		StarLink(int nodeFrom, int nodeTo, LinkFnc *fnc);
		virtual ~StarLink();
		
		/** @return 链接起始节点的ID（ID来自网络数据文件）。
		*/
		int getNodeFrom() const;
		
		/** @return 链接指向节点的ID（ID来自网络数据文件）。
		*/
		int getNodeTo() const;
		
		/** @return 链接的当前行程时间。
			\note 当流量改变时，它不会自动更新
				（这样做的原因是某些算法知道何时更新行程时间）。
		*/
		virtual FPType getTime() const;

		/** @return 除非被派生类覆盖，否则返回this指针。
		*/
		virtual StarLink* getForwardLink();

		/** @return pointer to a link cost function - in some cases the derivative of the function 
			must be evaluated without physically changing link flow.
		*/
		LinkFnc* getLinkFnc() const;
		
		/** @return internal index (NOT ID) of a node where this link comes from.
		*/
		int getNodeFromIndex() const;
		/** @return internal index (NOT ID) of a node where this link leads to.
		*/
		int getNodeToIndex() const;
		
		/** Sets internal node index of a node where this link comes from.
		*/
		void setNodeToIndex(int index);
		/** Sets internal node index of a node where this link leads to.
		*/
		void setNodeFromIndex(int index);
		
		/** @return current link flow.
		*/
		FPType getFlow() const;
		/** Sets current link flow.
		*/
		void setFlow(FPType flow);
		/** Sets link flow to zero.
		*/
		void setFlowToZero();
		
		/** Adds a specified amount of flow \b flow to current link flow.
		*/
		void addFlow(FPType flow);
		/** @return internal link index.
		*/
		int getIndex() const;
		/** Sets internal link index.
		*/
		void setIndex(int index);
		
		/** Updates current link travel time value and the value of link cost function
			derivative.
		*/
		void updateTime();

		/** @return string representation of link.
		*/
		std::string toString() const;

		/** @return current link cost function derivative value.
		*/
		FPType getDerivative() const;
		
	private:
		int index_;
		const int nodeFrom_;
		const int nodeTo_;
		int nodeFromIndex_;
		int nodeToIndex_;
		FPType flow_;
		FPType time_;
		LinkFnc *fnc_;
		FPType der_;
};
#endif
