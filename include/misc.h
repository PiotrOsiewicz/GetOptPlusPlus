#ifndef GetOptPlusPlus_Misc
#define GetOptPlusPlus_Misc
#include <vector>
#include <set>
#include <functional>
#include <stdexcept>

namespace GOpp{

template<typename T,typename X>
const std::set<T> BuildSymTable(std::vector<X> Source, std::function<std::vector<T>(const X)> ExtractVect)
{
	std::set<T> Result;
	for(const auto& a: Source){
		const std::vector<T> Elements = ExtractVect(a);
		for(const auto& b:Elements){
			if(Result.insert(b).second == false){//If element was already seen
				throw std::logic_error("Arguments have to be unique across the parameters");
			}
		}
	}
	return Result;
}
}
#endif
