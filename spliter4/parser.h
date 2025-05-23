#pragma once
#include "MakefileText.h"
#include "strcontrol.h"


std::vector<std::pair<unsigned, std::string>> ReadFileWithLineNumbers(const std::string& filename);
void Immediate_Evaluation(std::unordered_map<std::string, std::string>& ie, std::vector<std::string>& targets);
MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing);
std::vector<std::string> SplitValues(std::string& target);