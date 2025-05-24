#pragma once
#include "rule.h"

class Explicit_rule : public Rule {
private:
	std::vector<std::unique_ptr<Target>> targets;
	std::vector<std::unique_ptr<Prerequisite>> preqs;
	std::vector<std::unique_ptr<Recipe>> recipes;
public:
	Explicit_rule(RuleArg& ra);
	void variable_expend(std::unordered_map<std::string, std::string>& variables) override;
	void wildcard_expend(FileManagement& fm) override;
	void function_expend(std::unordered_map<std::string, std::string>& variable) override;

	void AddTargets(const std::vector<std::string>& target);
	void AddPreqs(const std::vector<std::string>& preq);
	void AddRecipe(const std::vector<std::string>& recipe);
	void TargetVariableExpend(std::unordered_map<std::string, std::string>& variables);
	void PrereqVariableExpend(std::unordered_map<std::string, std::string>& variables);
	void RecipeVariableExpend(std::unordered_map<std::string, std::string>& variables);
	void TargetFunctionExpend(std::unordered_map<std::string, std::string>& variables);
	void PrereaFunctionExpend(std::unordered_map<std::string, std::string>& variables);
	void print() override;
};