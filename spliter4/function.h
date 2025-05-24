#pragma once
#include <iostream>
#include <functional>

#include "strcontrol.h"

std::string patsubs(const std::string& wildcard, const std::string& changer,  const std::string& target_str);
std::string Active_function(const std::string& function_name, const std::vector<std::string>& args);

std::string function_subst(const std::string& from, const std::string& to, const std::string& text);
std::string function_patsubst(const std::string& pattern, const std::string& replacement, const std::string& text);
std::string function_strip(const std::string& string);
std::string function_findstring(const std::string& find, const std::string& in);
std::string function_filter(const std::string& pattern, const std::string& text);
std::string function_sort(const std::string& list);
std::string function_word(const std::string& n, const std::string& text);