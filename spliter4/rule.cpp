#include "rule.h"

Explicit_rule::Explicit_rule(RuleArg& ra) {
	AddTargets(ra.targets);
	AddPreqs(ra.prereq);
	AddRecipe(ra.recipes);
}

void Explicit_rule::AddTargets(const std::vector<std::string>& target) {
	for (const auto& i : target) {
		targets.push_back(std::make_unique<Target>(i));
	}
}
void Explicit_rule::AddPreqs(const std::vector<std::string>& preq) {
	for (const auto& i : preq) {
		preqs.push_back(std::make_unique<Prerequisite>(i));
	}
}
void Explicit_rule::AddRecipe(const std::vector<std::string>& recipe) {
	for (const auto& i : recipe) {
		recipes.push_back(std::make_unique<Recipe>(i));
	}
}

void Explicit_rule::print() {
	std::cout << "Targets : \n";
	for (const auto& i : targets) {
		i->print();
	}
	std::cout << "\n";
	std::cout << "Prerequisite : \n";
	for (const auto& i : preqs) {
		i->print();
	}
	std::cout << "\n";
	std::cout << "Recipe : \n";
	for (const auto& i : recipes) {
		i->print();
	}
	std::cout << "\n";
}


Static_pattern_Rule::Static_pattern_Rule(RuleArg& ra) {
	AddTarget(ra.targets);
	AddTargetPattern(ra.target_pattern);
	AddPrereqPattern(ra.prereq_pattern);
	AddRecipe(ra.recipes);
}
void Static_pattern_Rule::AddTarget(const std::vector<std::string>& target) {
	for (const auto& i : target) {
		targets.push_back(std::make_unique<Target>(i));
	}
}

void Static_pattern_Rule::AddTargetPattern(const std::string& tpattern) {
	tPattern = std::make_unique<Pattern>(tpattern);
}

void Static_pattern_Rule::AddPrereqPattern(const std::string& ppattern) {
	pPattern = std::make_unique<Pattern>(ppattern);
}

void Static_pattern_Rule::AddRecipe(const std::vector<std::string>& recipe) {
	for (const auto& i : recipe) {
		recipes.push_back(std::make_unique<Recipe>(i));
	}
}

void Static_pattern_Rule::print() {
	std::cout << "Targets : \n";
	for (const auto& i : targets) {
		i->print();
	}
	std::cout << "\n";
	std::cout << "Target pattern : \n";
	tPattern->print();
	std::cout << "\n";
	std::cout << "Target pattern : \n";
	pPattern->print();
	std::cout << "\n";
	std::cout << "Recipe : \n";
	for (const auto& i : recipes) {
		i->print();
	}
	std::cout << "\n";
}