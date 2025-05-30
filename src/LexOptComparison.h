// 前向声明BiObjLabel类
class BiObjLabel;

/** \brief 实现比较运算符，用于对双目标标签进行字典序排序
    其中旅行时间作为第一目标，通行费作为第二目标
*/
class LexOptComparison {
	public:
		// 重载()运算符，用于比较两个BiObjLabel指针
		// lhs: 左操作数
		// rhs: 右操作数
		// const: 表示该成员函数不会修改类的成员变量
		bool operator() (BiObjLabel* lhs, BiObjLabel* rhs) const;
};
