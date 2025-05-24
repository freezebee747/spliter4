#include "elements.h"
#include "function.h"

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


std::string ExtractFunctionName(const std::string& function) {
	std::string result = "";
	if (IsFunction_func(function)) {
		result = safe_substr(function, 2, function.find(" ") - 2);
	}
	return result;
}

std::vector<std::string> ExtractFunctionArguments(const std::string& function) {
	std::vector<std::string> result;
	if (IsFunction_func(function)) {
		std::string args = safe_substr(function, function.find(" ") + 1, function.size() - function.find(" ") - 2);
		result = SplitComma(args);
	}
	return result;
}

std::string FunctionResult(const std::string& function_name, std::vector<std::string>& args, std::unordered_map<std::string, std::string>& variables) {
	for (auto& i : args) {
		if (IsFunction_func(i)) {
			std::string name = ExtractFunctionName(i);
			std::vector<std::string> sub_arg = ExtractFunctionArguments(i);
			std::string temp = FunctionResult(name, sub_arg, variables);
			i.swap(temp);
		}
		else if (IsVariable_func(i)) {
			auto temp = variables.find(i);
			if (temp != variables.end()) {
				i.swap(temp->second);
			}
		}
	}
	return Active_function(function_name, args);
}