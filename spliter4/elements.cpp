#include "elements.h"

bool IsVariable_func(const std::string& str)
{
	if (IsFunction_func(str)) return false;
	if (str[0] == '$' && str[1] == '(' && str.back() == ')') {
		return true;
	}
	return false;
}

bool IsFunction_func(const std::string& str){
	if (str[0] == '$' && str[1] == '(' && str.back() == ')') {
		std::string temp = str.substr(2, str.size() - 3);
		temp = SplitSpace(temp)[0];
		auto i = functions.find(temp);
		if (i != functions.end()) {
			return true;
		}
	}
	return false;
}


