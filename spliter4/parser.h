#pragma once
#include "MakefileText.h"
#include "strcontrol.h"

enum class BlockType { variable, rule , none};
struct Block {
	BlockType type;
	std::pair<int, std::string> var_line;
	std::vector<std::pair<int, std::string>> _lines;
};

std::vector<std::pair<unsigned, std::string>> ReadFileWithLineNumbers(const std::string& filename);
MakefileText ParseMakefileTextFromLines(std::vector<Block>& blocks);
std::vector<std::string> ExpendPatternRule(const std::string& pattern, const std::unordered_set<std::string>& filenames);
std::vector<Block> SplitByBlock(std::vector<std::pair<unsigned, std::string>>& parsing);
void Immediate_Evaluation(std::vector<std::string>& targets);
void IE_variable(std::string& value, std::unordered_map<std::string, std::string>& variables);
std::vector<std::string> SplitValues(std::string& target);