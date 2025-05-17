#pragma once
#include "rule.h"

class Explicit_rule : public Rule {
private:
	std::vector<std::unique_ptr<Target>> targets;
	std::vector<std::unique_ptr<Prerequisite>> preqs;
	std::vector<std::unique_ptr<Recipe>> recipes;
public:
	Explicit_rule(RuleArg& ra);
	void variable_expend(std::unordered_map<std::string, std::string>&) override;
	void AddTargets(const std::vector<std::string>& target);
	void AddPreqs(const std::vector<std::string>& preq);
	void AddRecipe(const std::vector<std::string>& recipe);
	void PrereqExpend(std::unordered_map<std::string, std::string>& variables);
	void RecipeExpended(std::unordered_map<std::string, std::string>& variables);
	void print() override;
};