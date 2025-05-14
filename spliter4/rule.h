#pragma once
#include <vector>  
#include <fstream>  
#include <memory> 

#include "elements.h"  

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
};

class Explicit_rule : public Rule {
private:
	std::vector<std::unique_ptr<Target>> targets;
	std::vector<std::unique_ptr<Prerequisite>> preqs;
	std::vector<std::unique_ptr<Recipe>> recipes;
public:
	Explicit_rule(RuleArg& ra);
	void AddTargets(const std::vector<std::string>& target);
	void AddPreqs(const std::vector<std::string>& preq);
	void AddRecipe(const std::vector<std::string>& recipe);
	void print() override;
};

class Static_pattern_Rule : public Rule {
private:
	std::vector<std::unique_ptr<Target>> targets;
	std::unique_ptr<Pattern> tPattern;
	std::unique_ptr<Pattern> pPattern;
	std::vector<std::unique_ptr<Recipe>> recipes;
public:
	Static_pattern_Rule(RuleArg& ra);
	void AddTarget(const std::vector<std::string>& target);
	void AddTargetPattern(const std::string& tpattern);
	void AddPrereqPattern(const std::string& ppattern);
	void AddRecipe(const std::vector<std::string>& recipe);
	void print() override;
};