#include "rule.h"

// $^ 처리
void Rule::HandleCaretToken(std::string& token, std::vector<std::unique_ptr<Prerequisite>> preq) {
	std::string preq_str;
	for (const auto& i : preq) {
		preq_str = preq_str + ' ' + i->GetPreqs();
	} 
	token.swap(preq_str);
}
//$@ 처리
void Rule::HandleAtToken(std::string& token, std::vector<std::unique_ptr<Target>> target) {
	std::string target_str;
	for (const auto& i : target) {
		target_str = target_str + ' ' + i->GetTarget();
	}
	token.swap(target_str);
}
// $(VAR) 또는 함수 치환 처리
void Rule::HandleFunctionToken(std::string& token, std::unordered_map<std::string, std::string>& variables) {
	std::string variable = token.substr(2, token.size() - 3);
	auto test = variables.find(variable);
	if (test != variables.end()) {
		std::string temp = test->second;
		token.swap(temp);
	}

}

Explicit_rule::Explicit_rule(RuleArg& ra) {
	AddTargets(ra.targets);
	AddPreqs(ra.prereq);
	AddRecipe(ra.recipes);
}

void Explicit_rule::variable_expend(std::unordered_map<std::string, std::string>& variable) {
	PrereqExpend(variable);
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
void Explicit_rule::PrereqExpend(std::unordered_map<std::string, std::string>& variables) {
	for (const auto& preq : preqs) {
		if (preq->IsVariable()) {
			std::string temp = preq->GetVariable();
			auto i = variables.find(temp);
			if (i != variables.end()) {
				preq->SetExpended(i->second);
			}
		}
	}
}



void Explicit_rule::RecipeExpended(std::unordered_map<std::string, std::string>& variables) {

	for (auto& recipe : recipes) {
		std::vector<std::string> extend = recipe->SplitRecipe();
		for (auto& token : extend) {
			if (token.size() < 2 || token[0] != '$') continue;

			char symbol = token[1];

			switch (symbol) {
			case '^':
				HandleCaretToken(token, preqs);
				break;
			case '@':
				HandleAtToken(token, targets);
				break;
			case '(':
				if (token.back() == ')') {
					HandleFunctionToken(token, variables);
				}
				break;
			default:
				// 다른 $ 기호 처리 필요 시 여기에 추가
				break;
			}
		}
		recipe->SetExpended(extend);
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

void Static_pattern_Rule::variable_expend(std::unordered_map<std::string, std::string>& variable) {
	// 나중에 정의
	return;
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