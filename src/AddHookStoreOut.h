#ifndef ADD_HOOK_STORE_OUT
#define ADD_HOOK_STORE_OUT

#include "AddHookScreenOut.h"
#include "FileWriter.h"

#include <vector>
#include <sstream>
#include <iomanip>

/** \brief 这个类将收敛信息存储到内部数据结构中，并能够将其打印到文件中。
	有关文件格式，请参见printToFile()。
*/
class AddHookStoreOut : public AddHook { 
		public:
			AddHookStoreOut(){};
			virtual ~AddHookStoreOut(){};
			
			/** 将收敛数据存储到内部数据结构中。
				@param timePassed 自上次测量以来经过的时间。
				@param gap 当前收敛度量值。
			*/
			virtual void produceOut(FPType timePassed, FPType gap) {
				times_.push_back(timePassed);
				gaps_.push_back(gap);
			};
			
			/** 将存储的数据打印到指定的文件中。
				文件格式为：{CPU时间（秒）} {RGAP}\n
			*/
			void printToFile(const std::string& fileName) {
				std::cout << "Writing convergence to file: " << fileName << std::endl;
				FileWriter wfile(fileName);
				int size = gaps_.size();
				std::string line("");
				for (int i = 0; i < size; ++i){
					std::ostringstream strs;
					strs << std::setprecision(5)  << times_[i] << " " << std::setprecision(16) << gaps_[i] << "\n";
					line = strs.str();
					wfile.writeLine(line);
				}
			};
			
		private:
			std::vector<FPType> times_; /**< 存储的计算时间值。*/
			std::vector<FPType> gaps_; /**< 存储的收敛度量值。*/
};

#endif
