#pragma once
#include "rule.h"

class Static_pattern_Rule : public Rule {
private:
	std::vector<std::unique_ptr<Target>> targets;
	std::unique_ptr<Pattern> tPattern;
	std::unique_ptr<Pattern> pPattern;
	std::vector<std::unique_ptr<Recipe>> recipes;
public:
	Static_pattern_Rule(RuleArg& ra);
	void variable_expend(std::unordered_map<std::string, std::string>&) override;
	void wildcard_expend(FileManagement& fm) override;
	void function_expend() override;

	void AddTarget(const std::vector<std::string>& target);
	void AddTargetPattern(const std::string& tpattern);
	void AddPrereqPattern(const std::string& ppattern);
	void AddRecipe(const std::vector<std::string>& recipe);
	void print() override;
};