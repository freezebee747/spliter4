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
	"subst", "filter", "sort", "word", "wordlist", "words",
	"firstword", "lastword", "dir", "notdir", "suffix",
	"basename", "addsuffix", "addprefix", "join", "wildcard"
};

bool IsVariable_func(const std::string& str);
bool IsFunction_func(const std::string& str);
std::string ExtractFunctionName(const std::string& function);
