#ifndef NON_ADD_PARSER
#define NON_ADD_PARSER

#include "DefaultNetParser.h"

/** \brief 用于解析文本文件中指定网络的解析器，
    通行费在与网络相同的文件中描述。

	文件格式参见 <a href="http://www.bgu.ac.il/~bargera/tntp/"> 这里 </a>。
*/
class NonAddParser : public DefaultNetParser {
	public:
		NonAddParser(const std::string& fileWithNetwork);
		~NonAddParser();

		/** @return 已分配通行费数组的指针。
		*/
		TollContainerType* getTolls();

	protected:
		/** 为链接通行费数组分配内存。
		*/
		virtual void allocateTolls(int nbLinks);
		/** 为索引为linkIndex的链接设置通行费toll。
		*/
		virtual void setToll(int linkIndex, TollType toll);

	private:
		TollContainerType* tolls_;

};
#endif