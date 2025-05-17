#pragma once
#include "MakefileText.h"
#include "strcontrol.h"

std::vector<std::pair<unsigned, std::string>> ReadFileWithLineNumbers(const std::string& filename);
MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing);