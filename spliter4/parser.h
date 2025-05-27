#pragma once
#include "MakefileText.h"
#include "strcontrol.h"

enum class BlockType { variable, rule , none};
struct Block {
	BlockType type;
	std::string variable_line;
	std::vector<std::string> lines;
};

std::vector<std::pair<unsigned, std::string>> ReadFileWithLineNumbers(const std::string& filename);
void Immediate_Evaluation(std::unordered_map<std::string, std::string>& ie, std::vector<std::string>& targets);
MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing);
std::vector<Block> SplitByBlock(std::vector<std::pair<unsigned, std::string>>& parsing);
MakefileText ParseMakefileTextFromLines_redefine(std::vector<Block>& blocks);
std::vector<std::string> SplitValues(std::string& target);