#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "target.h"
#include "prerequisite.h"
#include "recipe.h"
#include "pattern.h"
#include "file.h"

inline const std::unordered_set<std::string> functions = {
	"let", "foreach", "file", "call", "value", "eval",
	"origin", "flaver", "shell", "guile", "patsubst",
	"subst", "filter", "sort", "word"
};

bool IsVariable_func(const std::string& str);
bool IsFunction_func(const std::string& str);
std::string ExtractFunctionName(const std::string& function);
std::vector<std::string> ExtractFunctionArguments(const std::string& function);
std::string FunctionResult(const std::string& function_name, std::vector<std::string>& args, std::unordered_map<std::string, std::string>& variables);