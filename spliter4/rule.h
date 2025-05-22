#pragma once
#include <vector>  
#include <fstream>  
#include <memory> 
#include <unordered_map>

#include "elements.h"  
#include "function.h"

enum class SelectRule {
	none, explicit_rule, static_pattern_rule
};

struct RuleArg {
	std::vector<std::string> targets;
	std::vector<std::string> prereq;
	std::vector<std::string> recipes;
	std::string target_pattern;
	std::string prereq_pattern;
	SelectRule sr = SelectRule::none;
};

class Rule {
public:
	virtual void print() = 0;
	virtual void variable_expend(std::unordered_map<std::string, std::string>&) = 0;
	virtual void wildcard_expend(FileManagement& fm) = 0;
	virtual void function_expend() = 0;
	void HandleCaretToken(std::string& token, std::vector<std::unique_ptr<Prerequisite>>& preq);
	void HandleAtToken(std::string& token, std::vector<std::unique_ptr<Target>>& target);
	void HandleFunctionToken(std::string& token, std::unordered_map<std::string, std::string>& variables);
	void HandleRBracketToken(std::string& token, std::vector<std::unique_ptr<Prerequisite>>& preq);
};
