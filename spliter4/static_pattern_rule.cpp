#include "static_pattern_rule.h"

Static_pattern_Rule::Static_pattern_Rule(RuleArg& ra) {
	AddTarget(ra.targets);
	AddTargetPattern(ra.target_pattern);
	AddPrereqPattern(ra.prereq_pattern);
	AddRecipe(ra.recipes);
}

void Static_pattern_Rule::variable_expend(std::unordered_map<std::string, std::string>& variable) {
	// 나중에 정의
	return;
}

void Static_pattern_Rule::wildcard_expend(FileManagement& fm){
	//나중에 정의
}

void Static_pattern_Rule::function_expend(){
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